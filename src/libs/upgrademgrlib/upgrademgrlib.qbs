import qbs 1.0
Product
{
   type: "dynamiclibrary"
   name : "ummlib"
   targetName : "umm"
   Depends { 
      name: "Qt"; 
      submodules: ["core", "network"]
   }
   Depends { name:"cpp" }
   Depends { name:"corelib" }
   destinationDirectory: "lib"
   cpp.defines: base.concat(type == "staticlibrary" ? ["SE_STATIC_LIB"] : ["SE_LIBRARY"])
   cpp.visibility: "minimal"
   cpp.cxxLanguageVersion: "c++14"
   version : "0.1.1"
   cpp.includePaths: base.concat(["../", "."])
   Group {
      fileTagsFilter: product.type.concat("dynamiclibrary_symlink")
      qbs.install: true
      qbs.installDir: "lib"
   }
}