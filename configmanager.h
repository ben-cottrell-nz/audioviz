//
// Created by ben on 14/09/21.
//

#ifndef AUDIOVIZ__CONFIGMANAGER_H
#define AUDIOVIZ__CONFIGMANAGER_H
#include <QObject>
#include <QJsonDocument>

class ConfigManager : public QObject {
Q_OBJECT
public:
  ConfigManager();
  ~ConfigManager();
  void setProp(const QString, const QColor &);
  QColor getProp(const QString);
  void save();
signals:
  void colorsChanged(QColor, QColor);
private:
  QColor jsonValueToColor(const QJsonValue &val);
  QJsonDocument document;
};

ConfigManager *ConfigManagerInstance();

#endif //AUDIOVIZ__CONFIGMANAGER_H
