//
// Created by ben on 13/09/21.
//

#ifndef AUDIOVIZ__SETTINGSDIALOG_H
#define AUDIOVIZ__SETTINGSDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class SettingsDialog; }
QT_END_NAMESPACE

class SettingsDialog : public QDialog {
Q_OBJECT

public:
  explicit SettingsDialog(QWidget *parent = nullptr);
  ~SettingsDialog() override;
signals:
  void colorsChanged(QColor, QColor);

private:
  Ui::SettingsDialog *ui;
};

#endif //AUDIOVIZ__SETTINGSDIALOG_H
