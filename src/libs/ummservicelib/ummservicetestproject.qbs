import qbs
Project
{
   name: "corelibtestproject"
   minimumQbsVersion: "1.4.2"
   qbsSearchPaths:["tests/qbs-resources"]
   autotestBuild : true
   references: [
      "../corelib/corelib.qbs",
      "../ummlib/ummlib.qbs",
      "../ummservicelib/ummservicelib.qbs",
      "tests/tests.qbs"
   ]
}
