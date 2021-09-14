//
// Created by ben on 14/09/21.
//

#include "configmanager.h"
#include <QApplication>
#include <QFile>
#include <QColor>
#include <QJsonObject>
#include <QJsonArray>
ConfigManager *ConfigManagerInstance() {
  static ConfigManager singleton;
  return &singleton;
}
ConfigManager::ConfigManager() {
  QFile configFile("config.json");
  if (!QFile::exists("config.json")) {
    configFile.open(QIODevice::OpenModeFlag::WriteOnly);
    document = QJsonDocument::fromJson(R"(
{
  "foreground-color" : [0,255,0],
  "background-color" : [0,0,0]
}
)");
    configFile.write(document.toJson());
    configFile.close();
  } else {
    configFile.open(QIODevice::OpenModeFlag::ReadOnly);
    document = QJsonDocument::fromJson(configFile.readAll());
  }
}
void ConfigManager::setProp(const QString prop, const QColor &color) {
  qDebug() << __PRETTY_FUNCTION__ << " prop " << prop << " color " << color;
  QJsonObject jsonObj = document.object();
  jsonObj[prop] = QJsonValue({color.red(), color.green(), color.blue()});
  document.setObject(jsonObj);
  QColor fgColor = jsonValueToColor(document.object()["foreground-color"]);
  QColor bgColor = jsonValueToColor(document.object()["background-color"]);
  emit colorsChanged(fgColor,bgColor);
}
void ConfigManager::save() {
  QFile configFile("config.json");
  configFile.open(QIODevice::OpenModeFlag::WriteOnly);
  configFile.write(document.toJson());
}
QColor ConfigManager::getProp(const QString prop) {
  return jsonValueToColor(document.object()[prop]);
}
ConfigManager::~ConfigManager() {
  save();
}
QColor ConfigManager::jsonValueToColor(const QJsonValue &val) {
  QJsonArray array = val.toArray();
  return QColor{array[0].toInt(),
                array[1].toInt(),
                array[2].toInt()};
}