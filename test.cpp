#include "RtAudio.h"
#include <iostream>
#include <cstdlib>
#include <cstring>

int record(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
           double streamTime, RtAudioStreamStatus status, void *userData) {

    if (status)
        std::cout << "Stream overflow detected!" << std::endl;
    double* buffer = (double*)inputBuffer;
    for (int i=0; i < nBufferFrames; i++) {
        if (buffer[i] > 0.00001f)
            std::cout << buffer[i] << '\n';
    }
    // Do something with the data in the "inputBuffer" buffer.
    return 0;
}

int main() {
    RtAudio adc;
    if (adc.getDeviceCount() < 1) {
        std::cout << "\nNo audio devices found!\n";
        exit(0);
    }
    RtAudio::StreamParameters parameters;
    parameters.deviceId = adc.getDefaultInputDevice();
    parameters.nChannels = 2;
    parameters.firstChannel = 0;
    unsigned int sampleRate = 44100;
    unsigned int bufferFrames = 256; // 256 sample frames
    try {
        adc.openStream(NULL, &parameters, RTAUDIO_SINT16,
                       sampleRate, &bufferFrames, &record);
        adc.startStream();
    }
    catch (RtAudioError &e) {
        e.printMessage();
        exit(0);
    }

    char input;
    std::cout << "\nRecording ... press <enter> to quit.\n";
    std::cin.get(input);
    try {
        // Stop the stream
        adc.stopStream();
    }
    catch (RtAudioError &e) {
        e.printMessage();
    }
    if (adc.isStreamOpen()) adc.closeStream();
    return 0;
}


