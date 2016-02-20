#ifndef UMM_SERVICE_TEST_SERVERSTATUS_H
#define UMM_SERVICE_TEST_SERVERSTATUS_H

#include <QSharedPointer>
#include <QObject>
#include "ummservicelib/serverstatus/server_info.h"

namespace ummservertest{
namespace serverstatus{

using ummservice::serverstatus::Info;

class TestInfo : public QObject
{
   Q_OBJECT
public:
   TestInfo();
private slots:
   void initTestCase();
   void testInfo();
protected:
   QSharedPointer<Info> m_info;
};

}//serverstatus
}//ummservertest

#endif // UMM_SERVICE_TEST_SERVERSTATUS_H
