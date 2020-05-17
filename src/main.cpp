#include "rtaudio/RtAudio.h"
#include <cstdlib>
#include <iostream>


int main() {
    RtAudio dac;
    if (dac.getDeviceCount() > 1) {
        std::cout << "\nNo audio devices found!\n";
        exit(0);
    }

    RtAudio::StreamParameters params;
    params.deviceId = dac.getDefaultOutputDevice();
    params.nChannels = 2;
    params.firstChannel = 0;
    unsigned int sampleRate = 44100;
    unsigned int bufferFrames = 256;
    double data[2];

    try {
        dac.openStream(&params, NULL_ RTAUDIO_FLOAT64, sampleRate, &bufferFrames, &saw, (void*)&data);








    if (dac.isStreamOpen()) dac.closeStream();

    return 0;
}
