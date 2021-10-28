//
// Created by ben on 14/09/21.
//

#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H
#include <QObject>
#include <QJsonDocument>

class ConfigManager : public QObject {
  Q_OBJECT
public:
  ConfigManager();
  ~ConfigManager();
  void setProp(const QString&, const QVariant&);
  void setPropColor(const QString&, const QColor&);
  QVariant getProp(const QString&);
  QString getPropString(const QString&);
  QColor getPropColor(const QString&);
  void save();
signals:
  void settingsChanged();
private:
  QJsonDocument document;
};

ConfigManager* ConfigManagerInstance();

#endif //CONFIGMANAGER_H