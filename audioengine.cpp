#include "audioengine.h"
#include "configmanager.h"
#include <QDebug>

AudioEngine::AudioEngine(QObject *parent) : QObject(parent) {

}

void AudioEngine::startListening() {
  worker = nullptr;
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
            {
              const int numDevices = rt->getDeviceCount();
              RtAudio::DeviceInfo devInfo;
              availableInputDevices.clear();
              inputDevicesInfo.clear();
              for (int dev = 0; dev < numDevices; dev++) {
                devInfo = rt->getDeviceInfo(dev);
                if (devInfo.inputChannels > 0) {
                  InputDeviceInfo aeDevInfo;
                  aeDevInfo.availableChannels << QString::number(devInfo.inputChannels);

                  for (auto r = devInfo.sampleRates.begin(); r != devInfo.sampleRates.end(); r++) {
                    if (*r == devInfo.preferredSampleRate) {
                      aeDevInfo.currentSamplingRateIndex = (r - devInfo.sampleRates.begin());
                    }
                    aeDevInfo.availableSamplingRates << QString::number(*r);
                  }
                  availableInputDevices += devInfo.name.data();
                  inputDevicesInfo += aeDevInfo;
                }
              }
            }
            rt->getDeviceCount();
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
void AudioEngine::restart() {
  stopListening();
  startListening();
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
  //if we don't have an input-device property string set,
  //set rtParameters to the default input settings and
  //save them to config for next time
  QString inputDeviceNameProp = ConfigManagerInstance()->getPropString("input-device");
  unsigned int sampleRate;
  unsigned int bufferFrames;
  RtAudio::DeviceInfo inputDeviceInfo;
  //input device
  if (inputDeviceNameProp.isEmpty()) {
    rtParameters.deviceId = rtAdc.getDefaultInputDevice();
    inputDeviceInfo = rtAdc.getDeviceInfo(rtParameters.deviceId);
    ConfigManagerInstance()->setPropString("input-device", inputDeviceInfo.name.data());

  } else {
    //find the matching device id by string
    bool found = false;
    int d;
    for (d = 0; d < rtAdc.getDeviceCount(); d++) {
      inputDeviceInfo = rtAdc.getDeviceInfo(rtParameters.deviceId);
      if (inputDeviceNameProp.toStdString() == inputDeviceInfo.name) {
        found = true;
        break;
      }
    }
    if (found) {
      rtParameters.deviceId = d;
    } else {
      //correct the incompatible input device
      rtParameters.deviceId = rtAdc.getDefaultInputDevice();
      inputDeviceInfo = rtAdc.getDeviceInfo(rtParameters.deviceId);
      ConfigManagerInstance()->setPropString("input-device", QString(inputDeviceInfo.name.data()));
    }
  }
  //sampling rate
  QString sampleRateProp = ConfigManagerInstance()->getPropString("sampling-rate");
  if (sampleRateProp.isEmpty()) {
    sampleRate = inputDeviceInfo.preferredSampleRate;
    ConfigManagerInstance()->setPropString("sampling-rate", QString::number(sampleRate));
  } else {
    bool found = false;
    for (auto &r:inputDeviceInfo.sampleRates) {
      if (r == sampleRateProp.toInt()) {
        found = true;
        break;
      }
    }
    if (found) {
      sampleRate = sampleRateProp.toInt();
    } else {
      //correct the incompatible sample rate
      sampleRate = inputDeviceInfo.preferredSampleRate;
      ConfigManagerInstance()->setPropString("sampling-rate", QString::number(sampleRate));
    }
  }
  bufferFrames = 512;
  rtParameters.nChannels = 2;
  rtParameters.firstChannel = 0;
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