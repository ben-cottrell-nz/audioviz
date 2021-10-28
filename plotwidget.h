#ifndef PLOTWIDGET_H
#define PLOTWIDGET_H

#include <QObject>
#include <QWidget>
#include <QPainterPath>
#include <QTimer>
#include <fftw3.h>

using sample_t = signed short;

class PlotWidget : public QWidget {
Q_OBJECT
public:
  explicit PlotWidget(QWidget *parent = nullptr);
  ~PlotWidget();
  void setDisplayMode(int);
  enum DisplayMode {
    OSCILLOSCOPE,
    SPECTROGRAPH,
    WATERFALL
  };
  void updateColors();
protected:
  void paintEvent(QPaintEvent *);
  void resizeEvent(QResizeEvent *);
private:
  const sample_t *audioBuffer;
  int audioBufferLength;
  QColor fgColor,bgColor;
  QPolygonF plotPath;
  double barWidth;
  QTimer repaintTimer;
  int displayMode;
  fftw_plan fftwPlan;
  fftw_complex *fftOutput;
  QVector<double> fftRealInput;
};

#endif // PLOTWIDGET_H
