﻿#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <signal.h>
#include <vlbi.h>
#include <fitsio2.h>
#include <json.h>
#include <base64.h>
#include "vlbi_client_json.h"

JSONClient::JSONClient()
{
}

JSONClient::~JSONClient()
{
}

int JSONClient::Init(int argc, char** argv)
{
	return VLBI::Client::Init(argc, argv);
}

void JSONClient::Parse(FILE* f)
{
    char *str = nullptr;
    json_value *v;
    char *n;
    size_t len = 0;
    getdelim(&str, &len, (int)'\n', f);
    json_value *value = json_parse(str, len);
    for(int x = 0; x < value->u.object.length; x++) {
        n = value->u.object.values[x].name;
        v = value->u.object.values[x].value;
        if(!strcmp(n, "context")) {
            AddContext(v->u.string.ptr);
            SetContext(v->u.string.ptr);
        }
        if(!strcmp(n, "location")) {
            if(v->u.object.length == 3) {
                double lat = 0, lon = 0, el = 0;
                int i = 0;
                for(int y = 0; y < 3; y ++) {
                    if(!strcmp(v->u.object.values[y].name, "latitude")) {
                        lat = v->u.object.values[y].value->u.dbl;
                        i++;
                    }
                    if(!strcmp(v->u.object.values[y].name, "longitude")) {
                        lon = v->u.object.values[y].value->u.dbl;
                        i++;
                    }
                    if(!strcmp(v->u.object.values[y].name, "elevation")) {
                        el = v->u.object.values[y].value->u.dbl;
                        i++;
                    }
                }
                if(i == 3) {
                    relative = true;
                    vlbi_set_location(GetContext(), lat, lon, el);
                }
            }
        }
        int i = 0;
        if(!strcmp(n, "node")) {
            if(v->u.object.length == 5) {
                timespec_t starttime;
                char *name = nullptr;
                char *buf = nullptr;
                char *locations = nullptr;
                int buflen = 0;
                int locationslen = 0;
                for(int y = 0; y < 5; y ++) {
                    if(!strcmp(v->u.object.values[y].name, "name")) {
                        name = v->u.object.values[y].value->u.string.ptr;
                        i++;
                    }
                    if(!strcmp(v->u.object.values[y].name, "starttime")) {
                        starttime = vlbi_time_string_to_utc(v->u.object.values[y].value->u.string.ptr);
                        i++;
                    }
                    if(!strcmp(v->u.object.values[y].name, "bitspersample")) {
                        Bps = v->u.object.values[y].value->u.dbl;
                        i++;
                    }
                    if(!strcmp(v->u.object.values[y].name, "buffer")) {
                        char *base64 = v->u.object.values[y].value->u.string.ptr;
                        int base64len = v->u.string.length;
                        buflen = from64tobits_fast(buf, base64, base64len);
                        i++;
                    }
                    if(!strcmp(v->u.object.values[y].name, "locations")) {
                        char *base64 = v->u.object.values[y].value->u.string.ptr;
                        int base64len = v->u.object.values[y].value->u.string.length;
                        locationslen = from64tobits_fast(locations, base64, base64len);
                        i++;
                    }
                }
                if(name && buf && locations && Bps > 0 && starttime.tv_sec > 0) {
                    AddNode(name, (dsp_location*)locations, buf, buflen, starttime, relative);
                }
            }
        }
        if(!strcmp(n, "plot")) {
            if(v->u.object.length == 6) {
                int type = 0;
                for(int y = 0; y < 6; y ++) {
                    if(!strcmp(v->u.object.values[y].name, "target")) {
                        for(int z = 0; z < 2; z ++) {
                            if(!strcmp(v->u.object.values[y].value->u.object.values[z].name, "ra")) {
                                Ra = v->u.object.values[y].value->u.object.values[z].value->u.dbl;
                            }
                            if(!strcmp(v->u.object.values[y].value->u.object.values[z].name, "dec")) {
                                Dec = v->u.object.values[y].value->u.object.values[z].value->u.dbl;
                            }
                        }
                        i++;
                    }
                    if(!strcmp(v->u.object.values[y].name, "frequency")) {
                        Freq = v->u.object.values[y].value->u.dbl;
                        i++;
                    }
                    if(!strcmp(v->u.object.values[y].name, "samplerate")) {
                        SampleRate = v->u.object.values[y].value->u.dbl;
                        i++;
                    }
                    if(!strcmp(v->u.object.values[y].name, "resolution")) {
                        for(int z = 0; z < 2; z ++) {
                            if(!strcmp(v->u.object.values[y].value->u.object.values[z].name, "width")) {
                                w = (int)v->u.object.values[y].value->u.object.values[z].value->u.integer;
                            }
                            if(!strcmp(v->u.object.values[y].value->u.object.values[z].name, "height")) {
                                h = (int)v->u.object.values[y].value->u.object.values[z].value->u.integer;
                            }
                        }
                        i++;
                    }
                    if(!strcmp(v->u.object.values[y].name, "calculate_delays")) {
                        nodelay = (int)~v->u.object.values[y].value->u.boolean;
                        i++;
                    }
                    if(!strcmp(v->u.object.values[y].name, "type")) {
                        if(!strcmp(v->u.string.ptr, "synthesis_raw_idft"))
                            type = APERTURE_SYNTHESIS|UV_IDFT;
                        if(!strcmp(v->u.string.ptr, "synthesis_coverage_idft"))
                            type = APERTURE_SYNTHESIS|UV_COVERAGE|UV_IDFT;
                        if(!strcmp(v->u.string.ptr, "synthesis_raw"))
                            type = APERTURE_SYNTHESIS;
                        if(!strcmp(v->u.string.ptr, "synthesis_coverage"))
                            type = APERTURE_SYNTHESIS|UV_COVERAGE;
                        if(!strcmp(v->u.string.ptr, "movingbase_raw_idft"))
                            type = UV_IDFT;
                        if(!strcmp(v->u.string.ptr, "movingbase_coverage_idft"))
                            type = UV_COVERAGE|UV_IDFT;
                        if(!strcmp(v->u.string.ptr, "movingbase_raw"))
                            type = 0;
                        if(!strcmp(v->u.object.values[y].value->u.string.ptr, "movingbase_coverage"))
                            type = UV_COVERAGE;
                        i++;
                    }
                }
                if(i == 6) {
                    dsp_stream_p plot = GetPlot(w, h, type, nodelay);
                    if(type & UV_IDFT) {
                        dsp_stream_p idft = vlbi_get_ifft_estimate(plot);
                        dsp_stream_free_buffer(plot);
                        dsp_stream_free(plot);
                        plot = idft;
                    }
                    unsigned char *base64 = (unsigned char *)malloc(sizeof(dsp_t)*plot->len * 4/3+4);
                    to64frombits(base64, (unsigned char*)plot->buf, plot->len*sizeof(double));
                    fprintf(stdout, "{ \"context\": \"%s\", \"type\": \"%s\", \"plot\": \"%s\" }");
                }
            }
        }
    }
    json_value_free(value);
}

