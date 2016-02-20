#include <QObject>
#include <QtTest/QtTest>
#include <QDebug>
#include "test_serverstatus.h"

namespace ummservertest{
namespace serverstatus{

TestInfo::TestInfo()
   : m_info(new Info)
{
   
}

void TestInfo::initTestCase()
{
   
}

void TestInfo::testInfo()
{
   auto ret = m_info->getServiceServerAddressMeta();
   qDebug() << ret;
}

}//serverstatus
}//ummservertest
QTEST_MAIN(ummservertest::serverstatus::TestInfo)