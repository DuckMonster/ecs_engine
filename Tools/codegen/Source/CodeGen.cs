using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Security.Cryptography;

namespace CodeGenerator
{
	struct GenerationData
	{
		public string SourcePath;
		public string TargetPath;
		public string CachePath { get { return Path.Combine(TargetPath, "cache"); } }
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
				Utils.Print("Created directory \"{0}\"", GenData.TargetPath);
			}

			if (!Directory.Exists(GenData.CachePath))
			{
				Directory.CreateDirectory(GenData.CachePath);
				Utils.Print("Created directory \"{0}\"", GenData.CachePath);
			}

			// Collect manifest
			CodeGenerationManifest manifest = new CodeGenerationManifest();
			manifest.Components = new List<Component>();

			CollectDirectory(GenData.SourcePath, manifest);

			manifest.Print();

			Utils.PrintVerbal("\n- Generating...");

			// Process
			foreach(Component comp in manifest.Components)
			{
				Process.ProcessComponent(comp, manifest);
			}

			Process.ProcessDatabase(manifest.Database, manifest);

			// Generate
			if (GenData.CanGenerate)
			{
				// If generate is enabled, clean the directory first
				string[] files = Directory.GetFiles(GenData.TargetPath);
				foreach(string file in files)
				{
					File.Delete(file);
				}
			}

			foreach (Component comp in manifest.Components)
			{
				if (!comp.File.IsDirty)
					continue;

				// Source
				using (FileStream fileStream = OpenSource(comp.File))
				{
					SourceWriter writer = new SourceWriter(comp.File, fileStream);

					writer.WriteComponent(comp);

					writer.Flush();
					writer.Close();

					Utils.Print(comp.File.GenSrcFileName);
				}

				// Header
				using (FileStream fileStream = OpenHeader(comp.File))
				{
					HeaderWriter writer = new HeaderWriter(comp.File, fileStream);

					writer.WriteComponent(comp);

					writer.Flush();
					writer.Close();

					Utils.Print(comp.File.GenHeaderFileName);
				}
			}

			using (FileStream fileStream = OpenSource(manifest.Database.File))
			{
				SourceWriter writer = new SourceWriter(manifest.Database.File, fileStream);

				writer.WriteDatabase(manifest.Database, manifest);

				writer.Flush();
				writer.Close();

				Utils.Print(manifest.Database.File.GenSrcFileName);
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

		public static void Error(string file, string error, params string[] args)
		{
			ConsoleColor prevColor = Console.ForegroundColor;
			Console.ForegroundColor = ConsoleColor.Red;

			if (file.Length > 0)
				Utils.Print("ERROR ({0}): {1}", file, string.Format(error, args));
			else
				Utils.Print("ERROR: {0}", string.Format(error, args));

			Console.ForegroundColor = prevColor;
			Environment.Exit(10);
		}

		static FileStream OpenSource(SourceFile file)
		{
			string genPath = Path.Combine(GenData.TargetPath, file.GenSrcFileName);

			if (!GenData.CanGenerate && !File.Exists(genPath))
				Error("", "File \"{0}\" doesn't exist, and CanGenerate is false. Remake the project.", file.GenSrcFileName);

			return new FileStream(genPath, FileMode.Create);
		}

		static FileStream OpenHeader(SourceFile file)
		{
			string genPath = Path.Combine(GenData.TargetPath, file.GenHeaderFileName);

			if (!GenData.CanGenerate && !File.Exists(genPath))
				Error("", "File \"{0}\" doesn't exist, and CanGenerate is false. Remake the project.", file.GenHeaderFileName);

			return new FileStream(genPath, FileMode.Create);
		}
	}
}
