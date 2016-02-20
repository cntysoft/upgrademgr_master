#include <QString>
#include <QJsonDocument>
#include <QByteArray>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>

#include "server_info.h"
#include "io/filesystem.h"
#include "ummlib/kernel/stddir.h"

namespace ummservice{
namespace serverstatus{

using sn::corelib::Filesystem;
using ummlib::kernel::StdDir;

QString Info::getAddressMetaFilename()const
{
   return StdDir::getMetaDir() + '/' + "ServerAddress.json";
}

Info& Info::setServiceServerAddressMeta(const QVariant &data)
{
   QJsonDocument doc = QJsonDocument::fromVariant(data);
   Filesystem::filePutContents(getAddressMetaFilename(), doc.toJson());
   return *this;
}

QMap<QString, QMap<QString, QString>> Info::getServiceServerAddressMeta()
{
   QString filename = getAddressMetaFilename();
   QByteArray json("[]");
   if(Filesystem::fileExist(filename)){
      json = Filesystem::fileGetContents(filename);
   }
   QJsonParseError parserError;
   QJsonDocument doc = QJsonDocument::fromJson(json, &parserError);
   QMap<QString, QMap<QString, QString>> ret;
   if(parserError.error == QJsonParseError::NoError){
      QJsonArray array = doc.array();
      QJsonArray::const_iterator ait = array.constBegin();
      QJsonArray::const_iterator aendmarker = array.constEnd();
      while(ait != aendmarker){
         QJsonValue v = *ait;
         if(v.isObject()){
            QJsonObject item = v.toObject();
            QJsonObject::const_iterator oit = item.constBegin();
            QJsonObject::const_iterator oendmarker = item.constEnd();
            QMap<QString, QString> dataItem;
            while(oit != oendmarker){
               dataItem.insert(oit.key(), oit.value().toString());
               oit++;
            }
            ret.insert(dataItem.value("key"), dataItem);
         }
         ait++;
      }
   }
   return ret;
}

QMap<QString, QString> Info::getServiceServerAddressMetaByKey(const QString &key)
{
   QMap<QString, QMap<QString, QString>> pool = getServiceServerAddressMeta();
   if(!pool.contains(key)){
      return QMap<QString, QString>();
   }else{
      return pool.value(key);
   }
}

}//serverstatus
}//ummservice
