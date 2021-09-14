//
// Created by ben on 13/09/21.
//

// You may need to build the project (run Qt uic code generator) to get "ui_AboutDialog.h" resolved

#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::AboutDialog) {
  ui->setupUi(this);
  connect(ui->buttonBox,
          &QDialogButtonBox::accepted,
          this,
          &AboutDialog::accept);
}

AboutDialog::~AboutDialog() {
  delete ui;
}

