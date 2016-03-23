import qbs 1.0
Product
{
   type: "dynamiclibrary"
   name : "ummservicelib"
   targetName : "ummservice"
   Depends { 
      name: "Qt"; 
      submodules: ["core", "network","websockets"]
   }
   Depends { name:"corelib"}
   Depends { name:"ummlib"}
   Depends { name:"cpp"}
   destinationDirectory: "lib"
   cpp.defines: {
      var defines = [];
      if(product.type == "staticlibrary"){
         defines.push("UMM_SERVICE_STATIC_LIB");
      }else{
         defines.push("UMM_SERVICE_LIBRARY");
      }
      defines = defines.concat([
                                  'UMM_SERVICE_LIB_VERSION="'+ version+'"',
                                  'UMM_VERSION="' + project.upgrademgrMasterVersion+'"'
                               ]);
      return defines;
   }
   cpp.visibility: "minimal"
   cpp.cxxLanguageVersion: "c++14"
   cpp.includePaths:[".","../ummlib/", "../ummservicelib/", "../"]
   Export {
      Depends { name: "cpp" }
      Depends { name: "Qt"; submodules: ["core"] }
      cpp.rpaths: ["$ORIGIN/../lib"]
      cpp.includePaths: [product.sourceDirectory+"../"]
   }
   Group {
      fileTagsFilter: product.type.concat("dynamiclibrary_symlink")
      qbs.install: true
      qbs.installDir: "lib"
   }
   
   Group {
      name : "common"
      prefix : name + "/"
      files : [
         "download_server.h",
         "download_server_warpper.cpp",
         "filesystem.h",
         "filesystem_wrapper.cpp",
         "uploader_wrapper.cpp",
         "uploader.h"
      ]
   }

   Group {
      name : "repo"
      prefix : name + "/"
      files : [
         "repo_info.cpp",
         "repo_info.h"
      ]
   }
   
   Group {
      name : "serverstatus"
      prefix : name + "/"
      files : [
           "deploy_system_runtime.h",
           "deploy_system_runtime_wrapper.cpp",
           "server_info.cpp",
           "server_info_wrapper.cpp",
           "server_info.h",
           "upgrademgr_slave_server_info.h",
           "upgrademgr_slave_server_info_wrapper.cpp",
       ]
   }
   
   Group {
      name : "upgrader"
      prefix : name + "/"
      files : [
         "upgrade_cloudcontroller.h",
         "upgrade_cloudcontroller_wrapper.cpp",
         "upgrade_luoxi.h",
         "upgrade_luoxi_wrapper.cpp",
         "upgrade_metaserver.h",
         "upgrade_metaserver_wrapper.cpp",
         "upgrade_upgrademgr_master.h",
         "upgrade_upgrademgr_master_wrapper.cpp",
         "upgrade_upgrademgr_slave.h",
         "upgrade_upgrademgr_slave_wrapper.cpp",
      ]
   }

   files:[
        "global_defs.h",
        "service_error_code.h",
        "service_repo.h",
        "macros.h"
    ]
}
