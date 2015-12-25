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
   files: [
        "application.cpp",
        "application.h",
        "command/command_repo.h",
        "command/global_version_command.cpp",
        "command/global_version_command.h",
        "command_runner.cpp",
        "command_runner.h",
        "const.h",
        "main.cpp",
    ]
   Group {
      name: "utils"
      prefix: name+"/"
      files:[
           "common_funcs.cpp",
           "common_funcs.h",
           "default_cfg_initializer.cpp",
       ]
   }
}