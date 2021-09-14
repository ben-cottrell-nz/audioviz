#include "audioengine.h"
#include <QDebug>

AudioEngine::AudioEngine(QObject *parent) : QObject(parent) {

}

void AudioEngine::startListening() {
  worker = new AudioEngineWorker;
  worker->moveToThread(&workerThread);
  connect(worker,
          &AudioEngineWorker::ready,
          this,
          [this](RtAudio *rt) {
            static const QString RTAUDIO_API_NAMES[] = {
                "Unspecified",
                "ALSA",
                "PulseAudio",
                "OSS",
                "JACK",
                "Core Audio",
                "WASAPI",
                "ASIO",
                "DUMMY"
            };
            audioAPIName = RTAUDIO_API_NAMES[rt->getCurrentApi()];
          });
  connect(&workerThread,
          &QThread::finished,
          worker,
          &QObject::deleteLater);
  connect(worker,
          &AudioEngineWorker::bufferReady,
          this,
          &AudioEngine::bufferReady);
  workerThread.start();
  worker->run();
}

void AudioEngine::stopListening() {
  worker->stop();
  workerThread.quit();
  workerThread.wait();
}

AudioEngine::~AudioEngine() {
  qInfo() << __PRETTY_FUNCTION__;
}

AudioEngine *AudioEngineInstance() {
  static AudioEngine singleton;
  return &singleton;
}

int record(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
           double streamTime, RtAudioStreamStatus status, void *userData) {

  if (status)
    std::cout << "Stream overflow detected!" << std::endl;
  auto *buffer = (sample_t *) inputBuffer;
  ((AudioEngineWorker *) userData)->recordCallbackBufferReady(buffer, nBufferFrames);
  return 0;
}

void AudioEngineWorker::run() {
  if (rtAdc.getDeviceCount() < 1) {
    std::cout << "\nNo audio devices found!\n";
    exit(0);
  }
  rtParameters.deviceId = rtAdc.getDefaultInputDevice();
  rtParameters.nChannels = 2;
  rtParameters.firstChannel = 0;
//    RtAudio::DeviceInfo inputDeviceInfo = rtAdc.getDeviceInfo(rtParameters.deviceId);
  unsigned int sampleRate = 44100;
  unsigned int bufferFrames = 512; // 256 sample frames
  try {
    rtAdc.openStream(nullptr, &rtParameters, RTAUDIO_SINT16,
                     sampleRate, &bufferFrames, &record, this);
    rtAdc.startStream();
    emit ready(&rtAdc);
  }
  catch (RtAudioError &e) {
    e.printMessage();
    exit(0);
  }
  //while (state == RUNNING);
}

void AudioEngineWorker::stop() {
  state = STOPPED;
  rtAdc.stopStream();
  rtAdc.closeStream();
}

void AudioEngineWorker::recordCallbackBufferReady(const sample_t *buffer, const uint length) {
  emit bufferReady(buffer, length);
}