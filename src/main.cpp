#include "rtaudio/RtAudio.h"
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <vector>


float gFrequency = 220.0;
float gAmplitude = 0.6;
float gDuration = 5.0;
int gNumSamples = 0;
std::vector<float> gSampleData;


void calculate_sine(float *buffer, int numSamples, float sampleRate) {
    for (int n = 0; n < numSamples; n++) {
        float out = gAmplitude * sin(2.0 * M_PI * n * gFrequency / sampleRate);
        buffer[n] = out;
    }
}


int render(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
        double streamTime, RtAudioStreamStatus status, void *userData) {

    unsigned int i, j;
    double *buffer = (double *)outputBuffer;

    int idx = *(int *)userData;
    for (i = 0; i < nBufferFrames; i++) {
        for (j = 0; j < 2; j++) {
            *buffer++ = gSampleData[idx];
        }
        idx++;
        if (idx >= gSampleData.size())
            idx = 0;
    }

    *(int *)userData = idx;

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
    int tableIndex;


    // pre-calc sine data - enough for one cycle
    gNumSamples = static_cast<int>(sampleRate / gFrequency);
    gSampleData.resize(gNumSamples);
    calculate_sine(gSampleData.data(), gNumSamples, sampleRate);


    try {
        dac.openStream(&params, NULL, RTAUDIO_FLOAT64, sampleRate, &bufferFrames, &render, (void*)&tableIndex);
        dac.startStream();
    } catch (RtAudioError &e) {
        e.printMessage();
        exit(0);
    }

    char input;
    std::cout << "\nPlaying ... press <enter> to quit.\n";
    std::cin.get(input);

    try {
        // stop the stream
        dac.stopStream();
    } catch (RtAudioError &e) {
        e.printMessage();
    }

    if (dac.isStreamOpen()) dac.closeStream();

    return 0;
}
