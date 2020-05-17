#include "rtaudio/RtAudio.h"
#include <cstdlib>
#include <iostream>


int saw(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
        double streamTime, RtAudioStreamStatus status, void *userData) {

    // TODO:....

    return 0;
}


int main() {
    RtAudio dac;
    if (dac.getDeviceCount() < 1) {
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
        dac.openStream(&params, NULL, RTAUDIO_FLOAT64, sampleRate, &bufferFrames, &saw, (void*)&data);
        dac.startStream();
    } catch (RtAudioError &e) {
        e.printMessage();
        exit(0);
    }








    if (dac.isStreamOpen()) dac.closeStream();

    return 0;
}
