//
// Created by ben on 14/09/21.
//

#include "configmanager.h"
#include <QApplication>
#include <QFile>
#include <QColor>
#include <QJsonObject>
#include <QJsonArray>
#include <QStandardPaths>
#include <QDir>
static const QString config_file_parent_dir = QStandardPaths::writableLocation(
    QStandardPaths::StandardLocation::AppConfigLocation) +
        "/audioviz";
static const QString config_file_path = config_file_parent_dir + "/config.json";
ConfigManager* ConfigManagerInstance() {
  static ConfigManager singleton;
  return &singleton;
}
ConfigManager::ConfigManager() {
#ifdef QT_DEBUG
  qDebug() << "opening " + config_file_path;
#endif
  QFile configFile(config_file_path);
  if (!QDir(config_file_parent_dir).exists()) {
    QDir().mkdir(config_file_parent_dir);
  }
  if (!QFile::exists(config_file_path)) {
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
    configFile.open(QIODevice::OpenModeFlag::ReadWrite);
    document = QJsonDocument::fromJson(configFile.readAll());
  }
}
void ConfigManager::save() {
  QFile configFile(config_file_path);
  configFile.open(QIODevice::OpenModeFlag::WriteOnly);
  configFile.write(document.toJson());
}
ConfigManager::~ConfigManager() {
  save();
}
QVariant ConfigManager::getProp(const QString& key) {
  return document.object()[key].toVariant();
}
QColor ConfigManager::getPropColor(const QString& key) {
  QJsonArray array = getProp(key).toJsonArray();
#ifdef QT_DEBUG
  qDebug() << "prop color " << key << ": " << document.object()[key].toVariant().toList();
#endif
  return {(uint8_t)array[0].toInt(),
          (uint8_t)array[1].toInt(),
          (uint8_t)array[2].toInt()};
}
QString ConfigManager::getPropString(const QString& key) {
  return getProp(key).toString();
}
void ConfigManager::setProp(const QString &key, const QVariant &value) {
  QJsonObject jsonObj = document.object();
  jsonObj[key] = value.toJsonValue();
  document.setObject(jsonObj);
  emit settingsChanged();
}
void ConfigManager::setPropColor(const QString &key, const QColor &value) {
  QList<QVariant> arrayValue = {value.red(),value.green(),value.blue()};
  setProp(key, QVariant::fromValue(arrayValue));
}
