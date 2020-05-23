#include "rtaudio/RtAudio.h"
#include <cstdlib>
#include <cmath>


const int gWavetableLength = 512;
float gWavetable[gWavetableLength];
float gReadPointer = 0.0;

float gAmplitude = 0.333;
float gFrequency = 220.0;


void create_tri_wavetable(float *wavetable, int length) {
    // first half of wavetable: -1 to 1
    for (unsigned int n = 0; n < length/2; n++) {
        wavetable[n] = -1.0 + 4.0 * (float)n / (float)length;
    }
    // end of wavetable: 1 to -1
    for (unsigned int n = length/2; n < length; n++) {
        wavetable[n] = 1.0 - 4.0 * (float)(n - length/2) / (float)length;
    }
}


int render(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
        double streamTime, RtAudioStreamStatus status, void *userData) {

    unsigned int i, j;
    double *buffer = (double *)outputBuffer;

    for (i = 0; i < nBufferFrames; i++) {
        float out = gAmplitude * gWavetable[(int)gReadPointer];

        // increment read pointer according to frequency
        gReadPointer += gWavetableLength * gFrequency / 44100.0;
        while (gReadPointer >= gWavetableLength)
            gReadPointer -= gWavetableLength;

        for (j = 0; j < 2; j++) {
            *buffer++ = out;
        }
    }

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

    // generate wavetable
    create_tri_wavetable(gWavetable, gWavetableLength);

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
