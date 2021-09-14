//
// Created by ben on 13/09/21.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SettingsDialog.h" resolved

#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "configmanager.h"
#include <QColorDialog>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::SettingsDialog) {
  ui->setupUi(this);
  QPalette pal = ui->frameFGColor->palette();
  pal.setColor(QPalette::Window, ConfigManagerInstance()->getProp("foreground-color"));
  ui->frameFGColor->setPalette(pal);
  pal = ui->frameBGColor->palette();
  pal.setColor(QPalette::Window, ConfigManagerInstance()->getProp("background-color"));
  ui->frameBGColor->setPalette(pal);
  //fgcolorselect
  connect(ui->buttonFGColorSelect,
          &QPushButton::clicked,
          this,
          [this](){
    QColorDialog dialog;
    if (dialog.exec() == QDialog::Accepted) {
      QPalette pal = ui->frameFGColor->palette();
      QColor selCol = dialog.selectedColor();
      pal.setColor(QPalette::Window, selCol);
      ui->frameFGColor->setPalette(pal);
      ConfigManagerInstance()->setProp("foreground-color", selCol);
    }
  });
  //bgcolorselect
  connect(ui->buttonFGColorSelect,
          &QPushButton::clicked,
          this,
          [this](){
    QColorDialog dialog;
    if (dialog.exec() == QDialog::Accepted) {
      QPalette pal = ui->frameFGColor->palette();
      QColor selCol = dialog.selectedColor();
      pal.setColor(QPalette::Window, selCol);
      ui->frameFGColor->setPalette(pal);

    }
  });
  connect(ui->buttonBox,
          &QDialogButtonBox::accepted,
          this,
          [this](){
    ConfigManagerInstance()->setProp("foreground-color", ui->frameFGColor->palette().color(QPalette::Window));
    ConfigManagerInstance()->setProp("background-color", ui->frameBGColor->palette().color(QPalette::Window));
    //ConfigManagerInstance()->setProp("foreground-color",)
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
