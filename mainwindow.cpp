#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QComboBox>
#include "settingsdialog.h"
#include "aboutdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), mPlotWidget(this) {
  ui->setupUi(this);
  ui->verticalLayout->addWidget(&mPlotWidget, 1);
  connect(ui->settingsButton,
          &QPushButton::clicked,
          this,
          [this]() {
            SettingsDialog dialog;
            dialog.setModal(true);
            dialog.setWindowFlag(Qt::WindowType::Dialog, true);
            dialog.exec();
          });
  connect(ui->aboutButton,
          &QPushButton::clicked,
          this,
          [this]() {
            AboutDialog dialog;
            dialog.exec();
          });
  connect(ui->displayModeComboBox,
          &QComboBox::currentTextChanged,
          this,
          [this](const QString &text) {
            if (text == "Oscilloscope") {
              mPlotWidget.setDisplayMode(PlotWidget::DisplayMode::OSCILLOSCOPE);
            } else if (text == "Spectrograph") {
              mPlotWidget.setDisplayMode(PlotWidget::DisplayMode::SPECTROGRAPH);
            } else if (text == "Waterfall") {
              mPlotWidget.setDisplayMode(PlotWidget::DisplayMode::WATERFALL);
            }
          });
//    setCentralWidget(&mPlotWidget);
}

MainWindow::~MainWindow() {
  delete ui;
}

