import qbs 1.0
import qbs.File
import qbs.FileInfo
Product {
   name: "resources"
   type: ["copied upgrademgr_master resources"]
   Group {
      name: "assets"
      files : [
           "upgrademgr_master.service",
       ]
      fileTags: ["asset files"]
      qbs.install: true
      qbs.installDir: project.resourcesInstallDir
      qbs.installSourceBase: "."
   }
   Rule {
      inputs: ["asset files"]
      Artifact {
         filePath: FileInfo.joinPaths(project.buildDirectory, project.resourcesInstallDir
                                     , FileInfo.relativePath(product.sourceDirectory, input.filePath))
         fileTags: ["copied upgrademgr master resources"]
      }
      prepare: {
         var cmd = new JavaScriptCommand();
         cmd.description = "Copying resource " + input.fileName + " to build directory.";
         cmd.highlight = "codegen";
         cmd.sourceCode = function() { File.copy(input.filePath, output.filePath); }
         return cmd;
      }
   }
}
