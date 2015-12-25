import qbs 1.0
UpgraderApplication
{
   name : "upgrademgr_master"
   Depends { 
      name: "Qt"; 
      submodules: [
         "sql"
      ]
   }
   Depends { name: "corelib"}
   cpp.includePaths: base.concat([
                                    ".","../../libs"
                                 ])
   cpp.defines: base.concat([
                               'UPGRADEMGR_MASTER_VERSION="' + project.upgrademgrMasterversion + '"'
                            ])
}