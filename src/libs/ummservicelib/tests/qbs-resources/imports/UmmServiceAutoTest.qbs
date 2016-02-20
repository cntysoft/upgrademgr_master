import qbs
QtApplication {
   type: ["application", "autotest"]
   consoleApplication: true
   property string testName
   name: "ummservertest_" + testName
   Depends { name: "Qt.test" }
   Depends {
      name: "corelib"
   }
   Depends {
      name: "ummlib"
   }
   Depends {
      name: "ummservicelib"
   }
   cpp.includePaths:[".", "../../../ummlib/", "../../../ummservicelib/","../../../corelib/", "../../../"]
   cpp.cxxLanguageVersion: "c++14"
   destinationDirectory: "bin"
}
