#ifndef UPGRADEMGR_MASTER_COMMAND_WEBSERVER_ENV_SOFTWARE_COMANND_H
#define UPGRADEMGR_MASTER_COMMAND_WEBSERVER_ENV_SOFTWARE_COMANND_H

#include <QFile>
#include <QJsonDocument>
#include <QString>

#include "corelib/command/abstract_command.h"

namespace upgrademgr{
namespace master{
namespace command{

using sn::corelib::AbstractCommand;
using sn::corelib::AbstractCommandRunner;
using sn::corelib::CommandMeta;

class AbstractWebServerSoftwareCommand : public AbstractCommand
{
public:
   AbstractWebServerSoftwareCommand(AbstractCommandRunner& runner, const CommandMeta& invokeMeta, const QString &metaFilename);
protected:
   void loadToCached();
   void writeBackToFile();
protected:
   QJsonDocument m_repo;
   QFile m_metaFile;
};

class SetWebServerSoftwareItemCommand : public AbstractWebServerSoftwareCommand
{
public:
   SetWebServerSoftwareItemCommand(AbstractCommandRunner& runner, const CommandMeta& invokeMeta);
public:
   virtual void exec();
};

class DeleteWebServerSoftwareItemCommand : public AbstractWebServerSoftwareCommand
{
public:
   DeleteWebServerSoftwareItemCommand(AbstractCommandRunner& runner, const CommandMeta& invokeMeta);
public:
   virtual void exec();
};

class ListWebServerSoftwareItemCommand : public AbstractWebServerSoftwareCommand
{
public:
   ListWebServerSoftwareItemCommand(AbstractCommandRunner& runner, const CommandMeta& invokeMeta);
public:
   virtual void exec();
};

}//command
}//master
}//upgrademgr

#endif // UPGRADEMGR_MASTER_COMMAND_WEBSERVER_ENV_SOFTWARE_COMANND_H

