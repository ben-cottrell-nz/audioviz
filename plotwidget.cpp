#include "plotwidget.h"
#include <QPainter>
#include "audioengine.h"
#include <QDebug>
#include <QDateTime>

const double ONE_OVER_SAMPLE_T = 1.f / std::numeric_limits<sample_t>::max();

PlotWidget::PlotWidget(QWidget *parent) : QWidget(parent) {
  plotPath.resize(BUFFER_FRAMES);
  QObject::connect(AudioEngineInstance(),
                   &AudioEngine::bufferReady,
                   this,
                   [this](const sample_t *buffer, const int length) {
                     audioBuffer = buffer;
                     audioBufferLength = length;
                     repaint();
                     //qInfo() << "recieved buffer: " << length << '\n';
                   });
  AudioEngineInstance()->startListening();
}

void PlotWidget::paintEvent(QPaintEvent *) {
  QPainter painter(this);
  barWidth = (double)width() / BUFFER_FRAMES;
  painter.fillRect(rect(), QColor(0,0,0));
  painter.setPen(QPen(QColor(0,255,0),2));
//  plotPath[0] = QPointF(0,0.5*height());
//  plotPath[plotPath.length()-1] = QPointF(width(),0.5*height());
  for (int i = 0; i < BUFFER_FRAMES; i++) {
    plotPath[i] = QPointF(i * barWidth,
                          (0.5*height())-((double)audioBuffer[i] * ONE_OVER_SAMPLE_T * (double)height()));
//    painter.fillRect(
//        QRectF(i * barWidth, height(), barWidth,
//              -((double)audioBuffer[i] * ONE_OVER_SAMPLE_T * height())),
//        QColor(0, 255, 0));
  }
painter.drawPolyline(plotPath);
//  painter.drawLines(plotPath);

}

PlotWidget::~PlotWidget() {
  AudioEngineInstance()->stopListening();
}
void PlotWidget::resizeEvent(QResizeEvent *e) {
  barWidth = width() / BUFFER_FRAMES;
  QWidget::resizeEvent(e);
}
