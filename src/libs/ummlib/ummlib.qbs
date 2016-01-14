import qbs 1.0
Product
{
   type: "dynamiclibrary"
   name : "ummlib"
   targetName : "umm"
   Depends { 
      name: "Qt"; 
      submodules: ["core", "network","websockets"]
   }
   Depends { name:"corelib"}
   Depends { name:"cpp"}
   destinationDirectory: "lib"
   cpp.defines: {
      var defines = [];
      if(product.type == "staticlibrary"){
         defines.push("UM_MASTER_STATIC_LIB");
      }else{
         defines.push("UM_MASTER_LIBRARY");
      }
      defines = defines.concat([
                                  'UMM_LIB_VERSION="'+ version+'"'
                               ]);
      return defines;
   }
   cpp.visibility: "minimal"
   cpp.cxxLanguageVersion: "c++14"
   cpp.includePaths:[".","../ummlib/", "../"]
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
      name: "global"
      prefix: name+"/"
      files: [
           "const.h",
           "error_code.h",
           "global.h",
       ]
   }
   
   Group {
      name:"network"
      prefix: name+"/"
      files:[
           "multi_thread_server.cpp",
           "multi_thread_server.h",
           "web_service_server.cpp",
           "web_service_server.h",
       ]
   }
   
   Group {
      name: "kernel"
      prefix: name+"/"
      files: [
         "stddir.cpp",
         "stddir.h",
      ]
   }
   
   Group {
      name:"meta"
      prefix:name+"/"
      files: [
           "webserver/webserver_env_list.cpp",
           "webserver/webserver_env_list.h",
       ]
   }
}
