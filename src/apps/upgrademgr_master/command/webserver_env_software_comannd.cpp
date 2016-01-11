#include <QJsonParseError>

#include "webserver_env_software_comannd.h"
#include "corelib/io/filesystem.h"
#include "corelib/kernel/errorinfo.h"

#include "ummlib/kernel/stddir.h"

namespace upgrademgr{
namespace master{
namespace command{

using ummlib::kernel::StdDir;
using sn::corelib::Filesystem;
using sn::corelib::ErrorInfo;


AbstractWebServerSoftwareCommand::AbstractWebServerSoftwareCommand(AbstractCommandRunner &runner, const CommandMeta &invokeMeta, const QString &metaFilename)
   : AbstractCommand(runner, invokeMeta),
     m_metaFile(metaFilename)
{
   if(Filesystem::fileExist(metaFilename)){
      loadToCached();
   }
}

void AbstractWebServerSoftwareCommand::loadToCached()
{
   m_metaFile.open(QIODevice::ReadOnly);
   if(m_metaFile.size() > 0){
      QJsonParseError error;
      m_repo = QJsonDocument::fromJson(m_metaFile.readAll(), &error);
      if(error.error != QJsonParseError::ParseError::NoError){
         throw ErrorInfo(error.errorString());
      }
      m_metaFile.close();
   }
}

void AbstractWebServerSoftwareCommand::writeBackToFile()
{
   m_metaFile.open(QIODevice::WriteOnly|QIODevice::Truncate);
   m_metaFile.write(m_repo.toJson());
   m_metaFile.close();
}

SetWebServerSoftwareItemCommand::SetWebServerSoftwareItemCommand(AbstractCommandRunner &runner, const CommandMeta &invokeMeta)
   : AbstractWebServerSoftwareCommand(runner, invokeMeta, StdDir::getMetaDir()+"/repo.json")
{
}

void SetWebServerSoftwareItemCommand::exec()
{
}

DeleteWebServerSoftwareItemCommand::DeleteWebServerSoftwareItemCommand(AbstractCommandRunner &runner, const CommandMeta &invokeMeta)
   : AbstractWebServerSoftwareCommand(runner, invokeMeta, StdDir::getMetaDir()+"/repo.json")
{
}

void DeleteWebServerSoftwareItemCommand::exec()
{}

ListWebServerSoftwareItemCommand::ListWebServerSoftwareItemCommand(AbstractCommandRunner &runner, const CommandMeta &invokeMeta)
   : AbstractWebServerSoftwareCommand(runner, invokeMeta, StdDir::getMetaDir()+"/repo.json")
{
}

void ListWebServerSoftwareItemCommand::exec()
{}

}//command
}//master
}//upgrademgr