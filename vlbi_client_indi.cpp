#include "vlbi_client_indi.h"

INDIClient::INDIClient()
	: INDI::BaseClient()
{
}

int INDIClient::Init(int argc, char** argv)
{
    if(argc > 1) {
        Address = argv[1];
        Port = strtol(argv[2], NULL, 10);
        setServer(Address, Port);
        connectServer();
    } else {
        fprintf(stderr, "usage: %s hostname port\n", argv[0]);
        return -22;
    }
    return 0;
}

INDIClient::~INDIClient()
{
    disconnectServer();
    if(GetContext() != NULL) {
        vlbi_exit(GetContext());
    }
}

void INDIClient::SetCapture(double seconds)
{
    if(!isServerConnected())
        return;
    std::vector<INDI::BaseDevice*> devices;
    getDevices(devices, INDI::BaseDevice::DETECTOR_INTERFACE);
    for(INDI::BaseDevice* dev : devices) {
        dev->setMediator(this);
        this->sendNewNumber(dev->getDeviceName(), "DETECTOR_CAPTURE", "DETECTOR_CAPTURE_VALUE", seconds);
    }
}

void INDIClient::AbortCapture()
{
    if(!isServerConnected())
        return;
    std::vector<INDI::BaseDevice*> devices;
    getDevices(devices, INDI::BaseDevice::DETECTOR_INTERFACE);
    for(INDI::BaseDevice* dev : devices) {
        dev->setMediator(this);
        this->sendNewSwitch(dev->getDeviceName(), "ABORT", "DETECTOR_ABORT_CAPTURE");
    }
}

void INDIClient::SetExposure(double seconds)
{
    if(!isServerConnected())
        return;
    std::vector<INDI::BaseDevice*> devices;
    getDevices(devices, INDI::BaseDevice::DETECTOR_INTERFACE);
    for(INDI::BaseDevice* dev : devices) {
        dev->setMediator(this);
        this->sendNewNumber(dev->getDeviceName(), "CCD_EXPOSURE", "CCD_EXPOSURE_VALUE", seconds);
    }
}

void INDIClient::AbortExposure()
{
    if(!isServerConnected())
        return;
    std::vector<INDI::BaseDevice*> devices;
    getDevices(devices, INDI::BaseDevice::CCD_INTERFACE);
    for(INDI::BaseDevice* dev : devices) {
        dev->setMediator(this);
        this->sendNewSwitch(dev->getDeviceName(), "ABORT", "CCD_ABORT_EXPOSURE");
    }
}

void INDIClient::SetFrequency(double centerfrequency)
{
    if(!isServerConnected())
        return;
    std::vector<INDI::BaseDevice*> devices;
    getDevices(devices, INDI::BaseDevice::DETECTOR_INTERFACE);
    for(INDI::BaseDevice* dev : devices) {
        dev->setMediator(this);
        this->sendNewNumber(dev->getDeviceName(), "DETECTOR_SETTINGS", "DETECTOR_FREQUENCY", centerfrequency);
    }
}

void INDIClient::SetSampleRate(double samplingfrequency)
{
    if(!isServerConnected())
        return;
    std::vector<INDI::BaseDevice*> devices;
    getDevices(devices, INDI::BaseDevice::DETECTOR_INTERFACE);
    for(INDI::BaseDevice* dev : devices) {
        dev->setMediator(this);
        this->sendNewNumber(dev->getDeviceName(), "DETECTOR_SETTINGS", "DETECTOR_SAMPLERATE", samplingfrequency);
    }
}

void INDIClient::SetBadwidth(double bandwidth)
{
    if(!isServerConnected())
        return;
    std::vector<INDI::BaseDevice*> devices;
    getDevices(devices, INDI::BaseDevice::DETECTOR_INTERFACE);
    for(INDI::BaseDevice* dev : devices) {
        dev->setMediator(this);
        this->sendNewNumber(dev->getDeviceName(), "DETECTOR_SETTINGS", "DETECTOR_BANDWIDTH", bandwidth);
    }
}

void INDIClient::SetGain(double gain)
{
    if(!isServerConnected())
        return;
    std::vector<INDI::BaseDevice*> devices;
    getDevices(devices, INDI::BaseDevice::DETECTOR_INTERFACE);
    for(INDI::BaseDevice* dev : devices) {
        dev->setMediator(this);
        this->sendNewNumber(dev->getDeviceName(), "DETECTOR_SETTINGS", "DETECTOR_GAIN", gain);
    }
}

void INDIClient::SetBps(int Bps)
{
    if(!isServerConnected())
        return;
    std::vector<INDI::BaseDevice*> devices;
    getDevices(devices, INDI::BaseDevice::DETECTOR_INTERFACE);
    for(INDI::BaseDevice* dev : devices) {
        dev->setMediator(this);
        this->sendNewNumber(dev->getDeviceName(), "DETECTOR_SETTINGS", "DETECTOR_Bps", Bps);
    }
}

void INDIClient::GoTo(double Ra, double Dec)
{
    if(!isServerConnected())
        return;
    std::vector<INDI::BaseDevice*> devices;
    getDevices(devices, INDI::BaseDevice::TELESCOPE_INTERFACE | INDI::BaseDevice::DOME_INTERFACE);
    for(INDI::BaseDevice* dev : devices) {
        dev->setMediator(this);
        this->sendNewNumber(dev->getDeviceName(), "EQUATORIAL_EOD_COORD", "RA", Ra);
        this->sendNewNumber(dev->getDeviceName(), "EQUATORIAL_EOD_COORD", "DEC", Dec);
    }
}

void INDIClient::Connect()
{
    if(!isServerConnected())
        return;
    std::vector<INDI::BaseDevice*> devices;
    getDevices(devices, INDI::BaseDevice::TELESCOPE_INTERFACE | INDI::BaseDevice::DOME_INTERFACE | INDI::BaseDevice::DETECTOR_INTERFACE | INDI::BaseDevice::CCD_INTERFACE | INDI::BaseDevice::GUIDER_INTERFACE);
    for(INDI::BaseDevice* dev : devices) {
        dev->setMediator(this);
        this->sendNewSwitch(dev->getDeviceName(), "CONNECTION", "CONNECT");
    }
}

void INDIClient::Disconnect()
{
    if(!isServerConnected())
        return;
    std::vector<INDI::BaseDevice*> devices;
    getDevices(devices, INDI::BaseDevice::TELESCOPE_INTERFACE | INDI::BaseDevice::DOME_INTERFACE | INDI::BaseDevice::DETECTOR_INTERFACE | INDI::BaseDevice::CCD_INTERFACE | INDI::BaseDevice::GUIDER_INTERFACE);
    for(INDI::BaseDevice* dev : devices) {
        dev->setMediator(this);
        this->sendNewSwitch(dev->getDeviceName(), "CONNECTION", "DISCONNECT");
    }
}

void INDIClient::Tracking(bool on)
{
    if(!isServerConnected())
        return;
    std::vector<INDI::BaseDevice*> devices;
    getDevices(devices, INDI::BaseDevice::TELESCOPE_INTERFACE | INDI::BaseDevice::DOME_INTERFACE);
    for(INDI::BaseDevice* dev : devices) {
        dev->setMediator(this);
        if(dev->getDriverInterface() & (INDI::BaseDevice::TELESCOPE_INTERFACE | INDI::BaseDevice::DOME_INTERFACE)) {
            if(on)
                this->sendNewSwitch(dev->getDeviceName(), "TELESCOPE_TRACK_STATE", "TRACK_ON");
            else
                this->sendNewSwitch(dev->getDeviceName(), "TELESCOPE_TRACK_STATE", "TRACK_OFF");
        }
    }
}

void INDIClient::Park()
{
    if(!isServerConnected())
        return;
    std::vector<INDI::BaseDevice*> devices;
    getDevices(devices, INDI::BaseDevice::TELESCOPE_INTERFACE | INDI::BaseDevice::DOME_INTERFACE);
    for(INDI::BaseDevice* dev : devices) {
        dev->setMediator(this);
        if(dev->getDriverInterface() & (INDI::BaseDevice::TELESCOPE_INTERFACE | INDI::BaseDevice::DOME_INTERFACE))
            this->sendNewSwitch(dev->getDeviceName(), "TELESCOPE_PARK", "PARK");
    }
}

void INDIClient::Unpark()
{
    if(!isServerConnected())
        return;
    std::vector<INDI::BaseDevice*> devices;
    getDevices(devices, INDI::BaseDevice::TELESCOPE_INTERFACE | INDI::BaseDevice::DOME_INTERFACE);
    for(INDI::BaseDevice* dev : devices) {
        dev->setMediator(this);
        if(dev->getDriverInterface() & (INDI::BaseDevice::TELESCOPE_INTERFACE | INDI::BaseDevice::DOME_INTERFACE))
            this->sendNewSwitch(dev->getDeviceName(), "TELESCOPE_PARK", "UNPARK");
    }
}

void INDIClient::newDevice(INDI::BaseDevice *dev) {
    INDI_UNUSED(dev);
}

void INDIClient::removeDevice(INDI::BaseDevice *dp) {
    INDI_UNUSED(dp);
}

void INDIClient::newProperty(INDI::Property *property) {
    this->setBLOBMode(B_ONLY, property->getDeviceName());
    INDI_UNUSED(property);
}

void INDIClient::removeProperty(INDI::Property *property) {
    INDI_UNUSED(property);
}

void INDIClient::newBLOB(IBLOB *bp) {
    if(!strcmp(bp->label, "CONTINUUM")) {
        if(GetContext() != NULL) {
            int status = 0;
            while (status == 0) {
                size_t len = (bp->bloblen-4) * 3/4;
                unsigned char* buf = (unsigned char*)malloc(len);
                from64tobits_fast((char*)(buf), (char*)bp->blob, bp->bloblen);
                fitsfile* f;
                fits_create_memfile(&f, (void**)&buf, &len, 1, NULL, &status);
                char starttime[32], ra[32], dec[32], lat[32], lon[32], el[32], wordsize[32];
                int Y, M, D, H, m;
                double s;
                long long offset = f->Fptr->datastart;
                fits_read_key(f, 0, (char*)"OBJCTRA", ra, (char*)"Target Ra", &status);
                fits_read_key(f, 0, (char*)"OBJCTDEC", dec, (char*)"Target Dec", &status);
                fits_read_key(f, 0, (char*)"LATITUDE", lat, (char*)"Node Latutude", &status);
                fits_read_key(f, 0, (char*)"LONGITUDE", lon, (char*)"Node Longitude", &status);
                fits_read_key(f, 0, (char*)"ELEVATION", el, (char*)"Node Elevation", &status);
                fits_read_key(f, 0, (char*)"DATE-OBS", starttime, (char*)"Observation Date", &status);
		fits_read_key(f, 0, (char*)"SIZE-T", wordsize, (char*)"", &status);
                fits_str2time(starttime, &Y, &M, &D, &H, &m, &s, &status);
		buf = &buf[len-offset];
		len = (len-offset)*abs(strtol(wordsize, NULL, 10)) / 8;
		double* dbuf = (double*)malloc(len*sizeof(double));
		dsp_buffer_copy(buf, dbuf, len);
		AddNode(atof(lat), atof(lon), atof(el), dbuf, len, vlbi_time_mktimespec(Y, M, D, H, m, floor(s), (s - floor(s))*1000000000.0));
                return;
            }
            char errstr[120];
            fits_get_errstatus(status, errstr);
            fprintf(stdout, "error adding stream: %s.\n", errstr);
        } else {
            fprintf(stdout, "context null.\n");
            INDI_UNUSED(bp);
        }
    }
}

void INDIClient::newSwitch(ISwitchVectorProperty *svp) {
    if(!strcmp(svp->name, "CONNECTION")) {
        if(svp->sp[0].s == ISS_ON) {
            fprintf(stdout, "Device %s connected.\n", svp->device);
        }
        if(svp->sp[0].s == ISS_OFF) {
            fprintf(stdout, "Device %s disconnected.\n", svp->device);
        }
    }
}

void INDIClient::newNumber(INumberVectorProperty *nvp) {
    if(!strcmp(nvp->name, "DETECTOR_CAPTURE")) {
        fprintf(stdout, "Capture left: %lf.\n", nvp->np[0].value);
        if(nvp->np[0].value < 1.0) {
            fprintf(stdout, "Capture complete.\n");
        }
    }
    if(!strcmp(nvp->name, "DETECTOR_EXPOSURE")) {
        fprintf(stdout, "Exposure left: %lf.\n", nvp->np[0].value);
        if(nvp->np[0].value < 1.0) {
            fprintf(stdout, "Exposure complete.\n");
        }
    }
    if(!strcmp(nvp->name, "EQUATORIAL_EOD_COORDS")) {
        fprintf(stdout, "Current coordinates: RA:%lf DEC:%lf.\n", nvp->np[0].value, nvp->np[1].value);
    }
}

void INDIClient::newText(ITextVectorProperty *tvp) {
    INDI_UNUSED(tvp);
}

void INDIClient::newLight(ILightVectorProperty *lvp) {
    INDI_UNUSED(lvp);
}

void INDIClient::newMessage(INDI::BaseDevice *dp, int messageID) {
    INDI_UNUSED(dp);
    INDI_UNUSED(messageID);
}

void INDIClient::serverConnected() {
    fprintf(stdout, "Connected to server\n");
}

void INDIClient::serverDisconnected(int exit_code) {
    fprintf(stdout, "Disconnected from server\n");
    INDI_UNUSED(exit_code);
}

void INDIClient::Parse(char* cmd, char* arg, char* value)
{
    VLBI::Client::Parse(cmd, arg, value);
    if(!strcmp(cmd, "set")) {
            if(!strcmp(arg, "connection")) {
                if(!strcmp(value, "on")) {
                    Connect();
                }
                if(!strcmp(value, "off")) {
                    Disconnect();
                }
            }
            else if(!strcmp(arg, "tracking")) {
                if(!strcmp(value, "on")) {
                    Tracking(true);
                }
                if(!strcmp(value, "off")) {
                    Tracking(false);
                }
            }
            else if(!strcmp(arg, "parking")) {
                if(!strcmp(value, "on")) {
                    Park();
                }
                if(!strcmp(value, "off")) {
                    Unpark();
                }
            }
            else if(!strcmp(arg, "target")) {
                GoTo(Ra, Dec);
            }
            else if(!strcmp(arg, "frequency")) {
                SetFrequency(Freq);
            }
            else if(!strcmp(arg, "samplerate")) {
                SetSampleRate(SampleRate);
            }
            else if(!strcmp(arg, "bitspersample")) {
                SetBps(Bps);
            }
            else if(!strcmp(arg, "gain")) {
                SetGain(Gain);
            }
            else if(!strcmp(arg, "bandwidth")) {
                SetBadwidth(Bandwidth);
            }
            else if(!strcmp(arg, "capture")) {
                SetCapture(duration);
            }
        }
}

INDIClient* client = new INDIClient();
