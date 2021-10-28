#include "plotwidget.h"
#include <QPainter>
#include "audioengine.h"
#include <QDebug>
#include <QDateTime>
#include <cmath>
#include "configmanager.h"

void hamming(double *in, int num_samples) {
  // https://github.com/audacity/audacity/blob/c5ebc396eb06857b4509101fdd2b0620dc0658b3/src/FFT.cpp#L344
  const double multiplier = 2 * M_PI / num_samples;
  static const double coeff0 = 0.54, coeff1 = -0.46;
  for (int ii = 0; ii < num_samples; ++ii) {
    in[ii] *= coeff0 + coeff1 * cos(ii * multiplier);
  }
}

const double ONE_OVER_SAMPLE_T = 1.f / std::numeric_limits<sample_t>::max();

PlotWidget::PlotWidget(QWidget *parent) : QWidget(parent) {
  plotPath.resize(BUFFER_FRAMES);
  repaintTimer.setInterval(16);
  displayMode = DisplayMode::OSCILLOSCOPE;
  fftRealInput.resize(BUFFER_FRAMES);
  connect(&repaintTimer,
          &QTimer::timeout,
          this,
          [this]() {
            repaint();
          });
  connect(ConfigManagerInstance(),
          &ConfigManager::settingsChanged,
          this,
          [this]() {
            updateColors();
          });
  fgColor = ConfigManagerInstance()->getPropColor("foreground-color");
  bgColor = ConfigManagerInstance()->getPropColor("background-color");
  repaintTimer.start();
  fftOutput = (fftw_complex *) fftw_malloc(sizeof(fftw_complex) * BUFFER_FRAMES);
  fftwPlan = fftw_plan_dft_r2c_1d(BUFFER_FRAMES,
                                  fftRealInput.data(),
                                  fftOutput,
                                  FFTW_ESTIMATE);
  connect(AudioEngineInstance(),
          &AudioEngine::bufferReady,
          this,
          [this](const sample_t *buffer, const int length) {
            audioBuffer = buffer;
            audioBufferLength = length;
            if (displayMode == DisplayMode::SPECTROGRAPH) {
              //convert to double
              for (int i = 0; i < BUFFER_FRAMES; i++) {
                fftRealInput[i] = audioBuffer[i] * ONE_OVER_SAMPLE_T;
              }
              hamming(fftRealInput.data(), BUFFER_FRAMES);
              fftw_execute(fftwPlan);
            }
          });
  AudioEngineInstance()->startListening();
}

void PlotWidget::paintEvent(QPaintEvent *) {
  QPainter painter(this);
  barWidth = (double) width() / BUFFER_FRAMES;
  painter.fillRect(rect(), bgColor);
  painter.setPen(QPen(fgColor));
  if (displayMode == DisplayMode::OSCILLOSCOPE) {
    for (int i = 0; i < BUFFER_FRAMES; i++) {
      plotPath[i] = QPointF(i * barWidth,
                            (0.5 * height()) - (double) audioBuffer[i] * ONE_OVER_SAMPLE_T * (double) height());
    }
    painter.drawPolyline(plotPath);
  } else if (displayMode == DisplayMode::SPECTROGRAPH) {
//    double freqBinSize = (double) 44100 / BUFFER_FRAMES;
    double freq = 0;
    double freqBinSize = (double) 44100 / BUFFER_FRAMES;
    double freqBin = 0;
    barWidth *= 2;
    const int max_frames = BUFFER_FRAMES / 2;
    const double ONE_OVER_HS = 1.0 / (44100 / 2);
    QRect lastTextRect;
    int nSkipBins = 0;
    QRectF labelRect;
    for (int i = 0; i < max_frames; i++) {
      freq = sqrt(pow(fftOutput[i][0], 2) + pow(fftOutput[i][1], 2));
      painter.fillRect(
          QRectF(i * barWidth, height() * 0.5, barWidth, -freq * height() * 0.5),
          fgColor);
      //make sure there's enough room to draw text
      if (nSkipBins == 30) {
        QString freqBinText = QString::asprintf("%.2f", freqBin);
        labelRect = painter.fontMetrics().boundingRect(freqBinText);
        painter.drawLine(i * barWidth, height() * 0.5, i * barWidth, height() * 0.5 + 15);
        //lastTextRect = painter.fontMetrics().boundingRect(freqBinText);
        painter.drawText(i * barWidth - labelRect.width() * 0.5, height() * 0.5 + 30, freqBinText);
        nSkipBins = 0;
      } else {
        nSkipBins++;
      }
      freqBin += freqBinSize;
    }
    QFont font = painter.font();
    font.setBold(true);
    painter.setFont(font);
    painter.drawText(width() * 0.5 - painter.fontMetrics().horizontalAdvance("Frequency (Hz)") * 0.5,
                     height() * 0.5 + 60,
                     "Frequency (Hz)");
  } else if (displayMode == DisplayMode::WATERFALL) {

  }

}

PlotWidget::~PlotWidget() {
  AudioEngineInstance()->stopListening();
  fftw_destroy_plan(fftwPlan);
  fftw_free(fftOutput);
}
void PlotWidget::resizeEvent(QResizeEvent *e) {
  barWidth = width() / BUFFER_FRAMES;
  QWidget::resizeEvent(e);
}
void PlotWidget::setDisplayMode(int index) {
  displayMode = index;
}
void PlotWidget::updateColors() {
  QColor fg, bg;
  fg = ConfigManagerInstance()->getPropColor("foreground-color");
  bg = ConfigManagerInstance()->getPropColor("background-color");
  qDebug() << "in " << __PRETTY_FUNCTION__ << " fg:" << fg;
  fgColor = fg;
  bgColor = bg;
}
