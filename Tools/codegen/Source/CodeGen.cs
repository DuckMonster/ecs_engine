using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;

namespace CodeGenerator
{
	struct GenerationData
	{
		public string SourcePath;
		public string TargetPath;
		public bool CanGenerate;
	}

	class CodeGen
	{
		public static GenerationData GenData;

		static int Main(string[] args)
		{
			if (args.Length < 2)
			{
				Utils.Print("Usage: CodeGen source target [/G]\n  /G - Allow generating new files. Otherwise will error if new files have to be created.");
                Utils.Print("\nSupplied {0} arguments:", args.Length);
                foreach(string arg in args)
                    Utils.Print("\t{0}", arg);

                return 1;
			}

			GenData = new GenerationData();
			GenData.SourcePath = Path.GetFullPath(args[0]);
			GenData.TargetPath = Path.GetFullPath(args[1]);
			GenData.CanGenerate = args.Contains("/G");

			if (Path.HasExtension(GenData.SourcePath) || Path.HasExtension(GenData.TargetPath))
			{
				Utils.Print("Using a file as source/target is invalid, only directories are supported");
				return 1;
			}

			if (!Directory.Exists(GenData.SourcePath))
			{
				Utils.Print("Couldn't find source directory \"{0}\"", GenData.SourcePath);
				return 1;
			}

			Utils.Print("-- GENERATING FILES --\n\"{0}\" => \"{1}\"\nCanGenerate: {2}", GenData.SourcePath, GenData.TargetPath, GenData.CanGenerate);

			if (!Directory.Exists(GenData.TargetPath))
			{
				Directory.CreateDirectory(GenData.TargetPath);
				Utils.Print("Created directory \"{0}\"", GenData.TargetPath);
			}

			// Start generating
			CodeGenerationManifest manifest = new CodeGenerationManifest();
			manifest.Components = new List<Component>();

			CollectDirectory(GenData.SourcePath, manifest);

			manifest.Print();

			return 0;

			//List<Component> components = new List<Component>();

			//// File
			//if (Path.HasExtension(path))
			//{
			//	if (!File.Exists(path))
			//		Error("Couldn't find file \"{0}\"", path);

			//	Utils.Print("Generating code for file \"{0}\"...", path);
			//	Component[] parsedComponents = Component.Parse(path);
			//		components.AddRange(parsedComponents);

			//	path = Path.GetDirectoryName(path);
			//}

			//// Directory
			//else
			//{
			//	if (!Directory.Exists(path))
			//		Error("Couldn't open directory \"{0}\"", path);

			//	Utils.Print("Generating code for files in \"{0}\"...", path);

			//	string[] files = Directory.GetFiles(path);

			//	foreach (string file in files)
			//	{
			//		Component[] parsedComponents = Component.Parse(file);
			//		components.AddRange(parsedComponents);
			//	}
			//}

			//Utils.Print("\n---- PARSE FINISHED ----\n");
			//Utils.Print("Cleaning directory...");

			//string genDir = Path.Combine(path, ".gen\\");

			//if (!Directory.Exists(genDir))
			//	Directory.CreateDirectory(genDir);

			//DirectoryInfo info = new DirectoryInfo(genDir);

			//// Don't clean if folder contains subdirectories! This probably means bad path...
			//if (info.GetDirectories().Count() > 0)
			//	Error("Directory \"{0}\" contains sub-directories, aboring for safety...", genDir);

			//// Delete em all
			//foreach (FileInfo file in info.GetFiles())
			//	file.Delete();

			//// Generate
			//foreach (Component component in components)
			//	component.Generate(genDir);

			//Utils.Print("Done!");

			//return 0;
		}

		static void CollectDirectory(string dir, CodeGenerationManifest manifest)
		{
			dir = Path.GetFullPath(dir);

			if (!Directory.Exists(dir))
				Error("Directory \"{0}\" doesn't exist", dir);

			// Get all header files
			string[] files = Directory.GetFiles(dir);
			foreach(string file in files)
			{
				if (Path.GetExtension(file) != ".h")
					continue;

				FileParser.Parse(file, manifest);
			}

			// Continue recursively
			string[] childDirs = Directory.GetDirectories(dir);
			foreach (string child in childDirs)
				CollectDirectory(child, manifest);
		}

		public static void Error(string error, params string[] args)
		{
			Console.ForegroundColor = ConsoleColor.Red;
			Utils.Print("ERROR: {0}", string.Format(error, args));
			Debugger.Break();

			Environment.Exit(0);
		}
	}
}
