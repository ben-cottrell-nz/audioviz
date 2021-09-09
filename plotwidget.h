#ifndef PLOTWIDGET_H
#define PLOTWIDGET_H

#include <QObject>
#include <QWidget>
#include <QPainterPath>

using sample_t = signed short;

class PlotWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PlotWidget(QWidget *parent = nullptr);
    ~PlotWidget();
protected:
    void paintEvent(QPaintEvent*);
    void resizeEvent(QResizeEvent*);
private:
    const sample_t* audioBuffer;
    int audioBufferLength;
    QPolygonF plotPath;
    double barWidth;
};

#endif // PLOTWIDGET_H
