import qbs 1.0
UpgraderApplication
{
   name : "upgrademgr_master"
   Depends { 
      name: "Qt"; 
      submodules: [
         "sql",
         "websockets"
      ]
   }
   cpp.includePaths: base.concat([
                                    ".","../../libs"
                                 ])
   cpp.defines: {
      var defines = [];
      defines.push('UMM_VERSION="' + project.upgrademgrMasterVersion + '"');
      if(!project.releaseBuild){
         defines.push("UMM_DEBUG_BUILD")
      }
      return defines;
   }
   files: [
        "application.cpp",
        "application.h",
        "command_runner.cpp",
        "command_runner.h",
        "main.cpp",
    ]
   
   Group {
      name: "command"
      prefix: name+"/"
      files:[
           "command_repo.h",
           "global_help_command.cpp",
           "global_help_command.h",
           "global_version_command.cpp",
           "global_version_command.h",
           "start_server_command.cpp",
           "start_server_command.h",
           "webserver_env_software_comannd.cpp",
           "webserver_env_software_comannd.h",
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
      name: "initializers"
      prefix: name+"/"
      files:[
         "global_initializer_and_cleanup.cpp",
         "init_service_provider.cpp",
         "initializer_cleanup_funcs.h",
      ]
   }
}