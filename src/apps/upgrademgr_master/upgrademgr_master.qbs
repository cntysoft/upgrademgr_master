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
   cpp.includePaths: base.concat([
                                    ".","../../libs"
                                 ])
   cpp.defines: base.concat([
                               'UPGRADEMGR_MASTER_VERSION="' + project.upgrademgrMasterversion + '"'
                            ])
   files: [
      "application.cpp",
      "application.h",
      "command_runner.cpp",
      "command_runner.h",
      "const.h",
      "main.cpp",
   ]
   
   Group {
      name: "command"
      prefix: name+"/"
      files:[
         "command_repo.h",
         "global_version_command.cpp",
         "global_version_command.h",
         "start_server_command.cpp",
         "start_server_command.h",
      ]
   }
   
   Group {
      name: "utils"
      prefix: name+"/"
      files:[
         "common_funcs.cpp",
         "common_funcs.h",
         "default_cfg_initializer.cpp",
      ]
   }
   
   Group {
      name: "api"
      prefix: name+"/"
      files:[
         
      ]
   }
}