#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include <QObject>
#include <QtMultimedia/QAudioInput>
#include <QBuffer>
#include <QThread>
#include <functional>
#include "RtAudio.h"

using sample_t = signed short;
const int BUFFER_FRAMES = 512;

/*!
 * Used by AudioEngine. Please don't use directly.
 */
class AudioEngineWorker : public QObject {
Q_OBJECT
public:
  void run();
  void stop();
  enum {
    RUNNING,
    STOPPED,
    BROKEN
  } state;
  double samplingRate();

  /*!
   * Called from record
   */
  void recordCallbackBufferReady(const sample_t *, const uint);
signals:
  void bufferReady(const sample_t *, const uint);
public:
  RtAudio rtAdc;
  RtAudio::StreamParameters rtParameters;

};

/*!
 * This listens for the default input audio device,
 * delivering a filled buffer with the bufferReady signal.
 */
class AudioEngine : public QObject {
Q_OBJECT

public:
  explicit AudioEngine(QObject *parent = nullptr);
  ~AudioEngine();
  void startListening();
  void stopListening();
signals:
  /*!
   * Triggered when the buffer is filled by the worker thread
   * @param buffer A buffer of double*
   * @param length Number of samples
   */
  void bufferReady(const sample_t *buffer, const int length);
private:
  AudioEngineWorker *worker;
  QThread workerThread;
};

AudioEngine *AudioEngineInstance();

#endif // AUDIOENGINE_H
