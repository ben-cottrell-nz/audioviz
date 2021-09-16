//
// Created by ben on 13/09/21.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SettingsDialog.h" resolved

#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "audioengine.h"
#include "configmanager.h"
#include <QColorDialog>
#include <QComboBox>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::SettingsDialog) {
  ui->setupUi(this);
  QPalette pal = ui->frameFGColor->palette();
  pal.setColor(QPalette::Window, ConfigManagerInstance()->getPropColor("foreground-color"));
  ui->frameFGColor->setPalette(pal);
  pal = ui->frameBGColor->palette();
  pal.setColor(QPalette::Window, ConfigManagerInstance()->getPropColor("background-color"));
  ui->frameBGColor->setPalette(pal);
  ui->labelAPI->setText(AudioEngineInstance()->audioAPIName);
  ui->comboInputDevices->addItems(AudioEngineInstance()->availableInputDevices);
  {
    const int index = ui->comboInputDevices->currentIndex();
    ui->comboNumChannels->clear();
    ui->comboSamplingRates->clear();
    ui->comboNumChannels->addItems(AudioEngineInstance()->inputDevicesInfo[index].availableChannels);
    ui->comboSamplingRates->addItems(AudioEngineInstance()->inputDevicesInfo[index].availableSamplingRates);
    ui->comboSamplingRates->setCurrentIndex(AudioEngineInstance()->inputDevicesInfo[index].currentSamplingRateIndex);
  }
  //fgcolorselect
  connect(ui->buttonFGColorSelect,
          &QPushButton::pressed,
          this,
          [this]() {
            QColorDialog dialog(this);
            if (dialog.exec() == QDialog::Accepted) {
              QPalette pal = ui->frameFGColor->palette();
              QColor selCol = dialog.selectedColor();
              pal.setColor(QPalette::Window, selCol);
              ui->frameFGColor->setPalette(pal);
            }
          });
  //bgcolorselect
  connect(ui->buttonBGColorSelect,
          &QPushButton::pressed,
          this,
          [this]() {
            QColorDialog dialog;
            if (dialog.exec() == QDialog::Accepted) {
              QPalette pal = ui->frameBGColor->palette();
              QColor selCol = dialog.selectedColor();
              pal.setColor(QPalette::Window, selCol);
              ui->frameBGColor->setPalette(pal);
              dialog.close();
            }
          });
  connect(ui->comboInputDevices,
          &QComboBox::currentTextChanged,
          this,
          [&](QString text) {
            const int index = ui->comboInputDevices->currentIndex();
            ui->comboNumChannels->clear();
            ui->comboSamplingRates->clear();
            ui->comboNumChannels->addItems(AudioEngineInstance()->inputDevicesInfo[index].availableChannels);
            ui->comboSamplingRates->addItems(AudioEngineInstance()->inputDevicesInfo[index].availableSamplingRates);
          });
  connect(ui->buttonBox,
          &QDialogButtonBox::accepted,
          this,
          [this]() {
            ConfigManagerInstance()->setPropColor("foreground-color",
                                                  ui->frameFGColor->palette().color(QPalette::Window));
            ConfigManagerInstance()->setPropColor("background-color",
                                                  ui->frameBGColor->palette().color(QPalette::Window));
            ConfigManagerInstance()->setPropString("input-device",
                                                   ui->comboInputDevices->currentText());
            ConfigManagerInstance()->setPropString("sampling-rate",
                                                   ui->comboSamplingRates->currentText());
            //ConfigManagerInstance()->setPropColor("foreground-color",)
            AudioEngineInstance()->restart();
            this->accept();
          });
  connect(ui->buttonBox,
          &QDialogButtonBox::rejected,
          this,
          &SettingsDialog::reject);
}

SettingsDialog::~SettingsDialog() {
  delete ui;
}

