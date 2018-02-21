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
		public bool Verbal;
	}

	class CodeGen
	{
		public static GenerationData GenData;

		static int Main(string[] args)
		{
			if (args.Length < 2)
			{
				Utils.Print("Usage: CodeGen source target [/G] [/V]\n  /G - Allow generating new files. Otherwise will error if new files have to be created.\n  /V - Verbal, will display additional debug information.");
				Utils.Print("\nSupplied {0} arguments:", args.Length);
				foreach (string arg in args)
					Utils.Print("\t{0}", arg);

				return 1;
			}

			GenData = new GenerationData();
			GenData.SourcePath = Path.GetFullPath(args[0]);
			GenData.TargetPath = Path.GetFullPath(args[1]);
			GenData.CanGenerate = args.Contains("/G");
			GenData.Verbal = args.Contains("/V");

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

			Utils.Print("-- GENERATING FILES --\n\"{0}\" => \"{1}\"", GenData.SourcePath, GenData.TargetPath, GenData.CanGenerate);
			Utils.PrintVerbal("CanGenerate: {2}", GenData.SourcePath, GenData.TargetPath, GenData.CanGenerate);

			if (!Directory.Exists(GenData.TargetPath))
			{
				Directory.CreateDirectory(GenData.TargetPath);
				Utils.PrintVerbal("Created directory \"{0}\"", GenData.TargetPath);
			}

			// Collect manifest
			CodeGenerationManifest manifest = new CodeGenerationManifest();
			manifest.Components = new List<Component>();

			CollectDirectory(GenData.SourcePath, manifest);

			manifest.Print();

			// Process
			foreach(Component comp in manifest.Components)
			{
				Process.ProcessComponent(comp, manifest);
			}

			Process.ProcessDatabase(manifest.Database, manifest);

			// Generate
			foreach(Component comp in manifest.Components)
			{
				// Source
				{
					string genPath = Path.Combine(GenData.TargetPath, comp.File.GenSrcFileName());

					using (FileStream fileStream = new FileStream(genPath, FileMode.Create))
					{
						SourceWriter writer = new SourceWriter(comp.File, fileStream);

						writer.WriteComponent(comp);

						writer.Flush();
						writer.Close();

						Utils.Print(comp.File.GenSrcFileName());
					}
				}

				// Header
				{
					string genPath = Path.Combine(GenData.TargetPath, comp.File.GenHeaderFileName());

					using (FileStream fileStream = new FileStream(genPath, FileMode.Create))
					{
						HeaderWriter writer = new HeaderWriter(comp.File, fileStream);

						writer.WriteComponent(comp);

						writer.Flush();
						writer.Close();

						Utils.Print(comp.File.GenHeaderFileName());
					}
				}
			}

			{
				string genPath = Path.Combine(GenData.TargetPath, manifest.Database.File.GenSrcFileName());

				using (FileStream fileStream = new FileStream(genPath, FileMode.Create))
				{
					SourceWriter writer = new SourceWriter(manifest.Database.File, fileStream);

					writer.WriteDatabase(manifest.Database, manifest);

					writer.Flush();
					writer.Close();

					Utils.Print(manifest.Database.File.GenSrcFileName());
				}
			}

			Utils.Print("-- GENERATION DONE --");

			return 0;
		}

		static void CollectDirectory(string dir, CodeGenerationManifest manifest)
		{
			dir = Path.GetFullPath(dir);

			if (!Directory.Exists(dir))
				Error("Directory \"{0}\" doesn't exist", dir);

			// Get all header files
			string[] files = Directory.GetFiles(dir);
			foreach (string file in files)
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
