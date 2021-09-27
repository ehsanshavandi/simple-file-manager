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

QVariant UserMessages::readData(const QString& data)
{
  QJsonValue value = m_config.value(QString(data));
  return value.toVariant();
}

QVariant UserMessages::readCmpxData(const QString& data)
{
  QStringList textList = data.split('.');
  QString lastText = textList.last();

  if (textList.size() == 1)
    return readData(lastText);

  QVector<QJsonObject> jsonObjs;

  for (int i = 0; i <= textList.size() - 2; ++i)
  {
    if (i == 0)
    {
      QJsonValue value = m_config.value(textList[i]);
      qInfo() << "value in first iterate: " << value;
      QJsonObject objValue = value.toObject();
      jsonObjs.push_back(objValue);
      qInfo() << "vector at first: " << jsonObjs;
      continue;
    }

    QJsonObject prevObj = jsonObjs.takeLast();
    qInfo() << "prev obj: " << prevObj;
    QJsonObject currObj = prevObj[textList[i]].toObject();
    qInfo() << "curr obj: " << currObj;
    jsonObjs.push_back(currObj);
  }

  QJsonObject lastObj = jsonObjs.takeLast();
  qInfo() << "last obj: " << lastObj;
  QJsonValue fineValue = lastObj[lastText];
  qInfo() << "fine value: " << fineValue;
  return fineValue.toVariant();
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
