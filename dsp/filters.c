/*
 *   libDSPAU - a digital signal processing library for astronomy usage
 *   Copyright (C) 2017  Ilia Platone <info@iliaplatone.com>
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "dsp.h"

void dsp_filter_squarelaw(dsp_stream_p stream)
{
    dsp_t* in = stream->buf;
    dsp_t *out = (dsp_t*)malloc(sizeof(dsp_t) * stream->len);
    int len = stream->len;
    dsp_t mean = dsp_stats_mean(stream->buf, stream->len);
    int val = 0;
    int i;
    for(i = 0; i < len; i++) {
        val = in [i] - mean;
        out [i] = (abs (val) + mean);
    }
    memcpy(stream->buf, out, stream->len * sizeof(dsp_t));
    free(out);
}

void dsp_filter_lowpass(dsp_stream_p stream, double Frequency)
{
    int d, x;
    double radius = 0.0;
    for(d = 0; d < stream->dims; d++) {
        radius += pow(stream->sizes[d]/2.0, 2);
    }
    radius = sqrt(radius);
    dsp_stream_p matrix = dsp_stream_copy(stream);
    dsp_buffer_set(matrix->magnitude->buf, matrix->len, 0);
    for(x = 0; x < matrix->len; x++) {
        int* pos = dsp_stream_get_position(matrix, x);
        double dist = 0.0;
        for(d = 0; d < matrix->dims; d++) {
            dist += pow(matrix->sizes[d]/2.0-pos[d], 2);
        }
        free(pos);
        dist = sqrt(dist);
        dist *= M_PI/radius;
        if(dist<Frequency)
            matrix->magnitude->buf[x] = 1.0;
    }
    dsp_fourier_idft(matrix);
    dsp_convolution_convolution(stream, matrix);
    dsp_stream_free_buffer(matrix);
    dsp_stream_free(matrix);
}

void dsp_filter_highpass(dsp_stream_p stream, double Frequency)
{
    int d, x;
    double radius = 0.0;
    for(d = 0; d < stream->dims; d++) {
        radius += pow(stream->sizes[d]/2.0, 2);
    }
    radius = sqrt(radius);
    dsp_stream_p matrix = dsp_stream_copy(stream);
    dsp_buffer_set(matrix->magnitude->buf, matrix->len, 0);
    for(x = 0; x < matrix->len; x++) {
        int* pos = dsp_stream_get_position(matrix, x);
        double dist = 0.0;
        for(d = 0; d < matrix->dims; d++) {
            dist += pow(matrix->sizes[d]/2.0-pos[d], 2);
        }
        free(pos);
        dist = sqrt(dist);
        dist *= M_PI/radius;
        if(dist>Frequency)
            matrix->magnitude->buf[x] = 1.0;
    }
    dsp_fourier_idft(matrix);
    dsp_convolution_convolution(stream, matrix);
    dsp_stream_free_buffer(matrix);
    dsp_stream_free(matrix);
}

void dsp_filter_bandreject(dsp_stream_p stream, double LowFrequency, double HighFrequency)
{
    int d, x;
    double radius = 0.0;
    for(d = 0; d < stream->dims; d++) {
        radius += pow(stream->sizes[d]/2.0, 2);
    }
    radius = sqrt(radius);
    dsp_stream_p matrix = dsp_stream_copy(stream);
    dsp_buffer_set(matrix->magnitude->buf, matrix->len, 0);
    for(x = 0; x < matrix->len; x++) {
        int* pos = dsp_stream_get_position(matrix, x);
        double dist = 0.0;
        for(d = 0; d < matrix->dims; d++) {
            dist += pow(matrix->sizes[d]/2.0-pos[d], 2);
        }
        free(pos);
        dist = sqrt(dist);
        dist *= M_PI/radius;
        if(dist>HighFrequency||dist<LowFrequency)
            matrix->magnitude->buf[x] = 1.0;
    }
    dsp_fourier_idft(matrix);
    dsp_convolution_convolution(stream, matrix);
    dsp_stream_free_buffer(matrix);
    dsp_stream_free(matrix);
}

void dsp_filter_bandpass(dsp_stream_p stream, double LowFrequency, double HighFrequency)
{
    int d, x;
    double radius = 0.0;
    for(d = 0; d < stream->dims; d++) {
        radius += pow(stream->sizes[d]/2.0, 2);
    }
    radius = sqrt(radius);
    dsp_stream_p matrix = dsp_stream_copy(stream);
    dsp_buffer_set(matrix->magnitude->buf, matrix->len, 0);
    for(x = 0; x < matrix->len; x++) {
        int* pos = dsp_stream_get_position(matrix, x);
        double dist = 0.0;
        for(d = 0; d < matrix->dims; d++) {
            dist += pow(matrix->sizes[d]/2.0-pos[d], 2);
        }
        free(pos);
        dist = sqrt(dist);
        dist *= M_PI/radius;
        if(dist<HighFrequency&&dist>LowFrequency)
            matrix->magnitude->buf[x] = 1.0;
    }
    dsp_fourier_idft(matrix);
    dsp_convolution_convolution(stream, matrix);
    dsp_stream_free_buffer(matrix);
    dsp_stream_free(matrix);
}
