#ifndef MESSAGES_H
#define MESSAGES_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QIODevice>

class UserMessages : public QObject {
  Q_OBJECT
 public:
  // delete copy constructor and assingment operator
  UserMessages(UserMessages const&) = delete;
  void operator=(UserMessages const&) = delete;

  static UserMessages* instance(QObject* parent);
  static UserMessages* instance(QObject* parent, QString configPath);

  QVariant readData(const QString& data);
  QVariant readCmpxData(const QString& data);
 signals:

 private:
  explicit UserMessages(QObject* parent = nullptr, QString configPath = "messages.json");
  static UserMessages* m_instance;
  void init(QString fileName);
  QJsonObject m_config;
};

#endif // MESSAGES_H
