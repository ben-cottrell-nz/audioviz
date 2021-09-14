//
// Created by ben on 13/09/21.
//

#ifndef AUDIOVIZ__ABOUTDIALOG_H
#define AUDIOVIZ__ABOUTDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class AboutDialog; }
QT_END_NAMESPACE

class AboutDialog : public QDialog {
Q_OBJECT

public:
  explicit AboutDialog(QWidget *parent = nullptr);
  ~AboutDialog() override;

private:
  Ui::AboutDialog *ui;
};

#endif //AUDIOVIZ__ABOUTDIALOG_H
