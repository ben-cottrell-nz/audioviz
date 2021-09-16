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
  void setPropColor(const QString, const QColor &);
  void setPropString(const QString, const QString &);
  QColor getPropColor(const QString);
  QString getPropString(const QString);
  void save();
signals:
  /*!
   * We emit this signal for PlotWidget to listen to
   * because we assume that getting values out of the
   * document is expensive.
   *
   * We don't need this for the AudioEngine, because it
   * only retrieves string properties during a restart
   */
  void colorsChanged(QColor, QColor);
private:
  QColor jsonValueToColor(const QJsonValue &val);
  QJsonDocument document;
};

ConfigManager *ConfigManagerInstance();

#endif //AUDIOVIZ__CONFIGMANAGER_H
