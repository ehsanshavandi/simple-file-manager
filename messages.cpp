#include "messages.h"

UserMessages::UserMessages(QObject* parent, QString configPath) : QObject(parent)
{
  init(configPath);
}

UserMessages* UserMessages::instance(QObject* parent)
{
  if (m_instance == nullptr)
  {
    m_instance = new UserMessages(parent);
  }

  return m_instance;
}

UserMessages* UserMessages::instance(QObject* parent, QString configPath)
{
  if (m_instance == nullptr)
  {
    m_instance = new UserMessages(parent, configPath);
  }

  return m_instance;
}

QVariant UserMessages::readData(QString data)
{
  QJsonValue value = m_config.value(QString(data));
  return value.toVariant();
}

void UserMessages::init(QString fileName)
{
  QString valueFromFile;
  QFile file;
  file.setFileName(fileName);

  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    qCritical() << file.errorString();
    return;
  }

  valueFromFile = file.readAll();
  file.close();

  QJsonDocument documentJson = QJsonDocument::fromJson(valueFromFile.toUtf8());
  m_config = documentJson.object();
}


// Initialize static member of class DatabaseManager
UserMessages* UserMessages::m_instance = nullptr;
