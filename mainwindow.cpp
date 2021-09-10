#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QComboBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), mPlotWidget(this) {
  ui->setupUi(this);
  ui->verticalLayout->addWidget(&mPlotWidget, 1);
  QObject::connect(ui->displayModeComboBox,
                   &QComboBox::currentTextChanged,
                   this,
                   [this](const QString& text) {
                     if (text == "Oscilloscope") {
                       mPlotWidget.setDisplayMode(PlotWidget::DisplayMode::OSCILLOSCOPE);
                     } else if (text == "Spectrograph") {
                       mPlotWidget.setDisplayMode(PlotWidget::DisplayMode::SPECTROGRAPH);
                     }
                   });
//    setCentralWidget(&mPlotWidget);
}

MainWindow::~MainWindow() {
  delete ui;
}

