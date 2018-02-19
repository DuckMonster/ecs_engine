using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Text.RegularExpressions;

namespace CodeGenerator
{
	struct FileParser
	{
		// ----------- STATIC FUNCTIONS
		static Regex COMP_RGX = new Regex(@"\sCOMPONENT\(\s*([\w\s,]*?)\s*\)\n?\s*class (.+) : *(?:public )? *(.+)\n((\s*){(?:.*\n)*?\5})");
		static Regex FUNC_RGX = new Regex(@"\sFUNCTION\s*\(\s*([\w\s,]*)\s*\)\W*(.+?)\s+(\w+)\s*\(\s*([\w\s,&*:]*)\s*\)");
		static Regex PROP_RGX = new Regex(@"\sPROPERTY\s*\(\s*([\w\s,]*)\s*\)\W*(.*?);");

		static Regex TYPE_DB_RGX = new Regex(@"\s_TYPE_DATABASE\(\s*([\w\s,]*?)\s*\)\n?\s*class (.+).*\n((\s*){(?:.*\n)*?\4})");
		static Regex TYPE_DB_INIT_RGX = new Regex(@"\s_DATABASE_INIT_FUNC\s*\(\s*\)\W*.+\s+(\w+)\s*\(\s*[\w\s,&*:]*\s*\)");

		public static void Parse(string sourcePath, CodeGenerationManifest manifest)
		{
			if (!File.Exists(sourcePath))
			{
				CodeGen.Error("File \"{0}\" does not exist.", sourcePath);
				return;
			}

			SourceFile file;

			file.FileName = Path.GetFileName(sourcePath);
			file.FullPath = sourcePath;
			file.Source = File.ReadAllText(sourcePath).Replace("\r", "");

			Utils.Print("\n-------------- Parsing \"{0}\" ------------------\n", file.FileName);

			// ----------------------------- Collect Components
			MatchCollection match = COMP_RGX.Matches(file.Source);
			if (match.Count > 0)
			{
				string[] namespaces = Utils.FindNamespaces(file.Source, match[0].Index);
				string[] metadata = ParseMetadata(match[0].Groups[1].Value);

				Component comp = new Component();
				comp.File = file;
				comp.Name = match[0].Groups[2].Value;
				comp.Parent = match[0].Groups[3].Value;
				comp.MetaData = metadata;
				comp.Namespaces = namespaces;

				// Parse functions
				{
					MatchCollection funcMatch = FUNC_RGX.Matches(match[0].Value);
					Function[] compFunctions = new Function[funcMatch.Count];

					for (int f = 0; f < funcMatch.Count; ++f)
					{
						string[] funcMetaData = ParseMetadata(funcMatch[f].Groups[1].Value);

						Function func = new Function();
						func.Name = funcMatch[f].Groups[3].Value;
						func.ReturnType = funcMatch[f].Groups[2].Value;
						func.Arguments = ParseParameters(funcMatch[f].Groups[4].Value);
						func.MetaData = funcMetaData;

						compFunctions[f] = func;
					}

					comp.Functions = compFunctions;
				}

				// Parse properties
				{
					MatchCollection propMatch = PROP_RGX.Matches(match[0].Value);
					Property[] compProperties = new Property[propMatch.Count];

					for (int p = 0; p < propMatch.Count; ++p)
					{
						string[] propMetaData = ParseMetadata(propMatch[p].Groups[1].Value);

						Property prop = new Property();
						prop.Parameter = ParseParameters(propMatch[p].Groups[2].Value)[0];
						prop.MetaData = propMetaData;

						compProperties[p] = prop;
					}

					comp.Properties = compProperties;
				}

				Utils.Print("Parsed component \"{0}\"...", comp.Name);
				manifest.Components.Add(comp);
			}

			// ----------------------------- Collect Database
			match = TYPE_DB_RGX.Matches(file.Source);
			if (match.Count > 0)
			{
				TypeDatabase database = new TypeDatabase();
				database.File = file;
				database.ClassName = match[0].Groups[2].Value;

				if (manifest.Database.ClassName != null)
					CodeGen.Error("Multiple databases found: \"{0}\" and \"{1}\"", manifest.Database.ClassName, database.ClassName);

				Match initMatch = TYPE_DB_INIT_RGX.Match(match[0].Groups[3].Value);
				if (!initMatch.Success)
					CodeGen.Error("Type database \"{0}\" (\"{1}\") without an init function", database.ClassName, file.FileName);

				database.InitFuncName = initMatch.Groups[1].Value;

				Utils.Print("Parsed database \"{0}\"...", database.ClassName);
				manifest.Database = database;
			}
		}

		public static Parameter[] ParseParameters(string src)
		{
			string[] commaSplit = src.Split(new char[] { ',' }, StringSplitOptions.RemoveEmptyEntries);
			Parameter[] result = new Parameter[commaSplit.Length];

			for(int i=0; i<result.Length; ++i)
			{
				string[] paramSplit = commaSplit[i].Split(new char[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);
				if (paramSplit.Length == 1)
					CodeGen.Error("\"{0}\" is an invalid parameter!", src);

				for (int j = 0; j < paramSplit.Length; j++)
					paramSplit[j] = paramSplit[j].Trim();

				Parameter param = new Parameter();
				param.Type = string.Join(" ", paramSplit, 0, paramSplit.Length - 1);
				param.Name = paramSplit[paramSplit.Length - 1];

				result[i] = param;
			}

			return result;
		}

		public static string[] ParseMetadata(string src)
		{
			string[] metadata = src.Split(new char[] { ',' }, StringSplitOptions.RemoveEmptyEntries);

			for (int j = 0; j < metadata.Length; j++)
				metadata[j] = metadata[j].Trim();

			return metadata;
		}

		public static void Generate(CodeGenerationManifest manifest, string path)
		{
			// Generate database
			{
				TypeDatabase db = manifest.Database;
				string fullpath = path + db.File.GenFileName();

				if (!File.Exists(fullpath) && !CodeGen.GenData.CanGenerate)
					CodeGen.Error("File \"{0}\" does not exists in update mode", fullpath);

				FileStream stream = File.Open(fullpath, FileMode.Create);
				SourceWriter writer = new SourceWriter(db.File, stream);


				//foreach (Function func in file.Component.Functions)
				//writer.WriteFunction(func);

				writer.Flush();
				stream.Close();

				Utils.Print("Generated \"{0}\"...", fullpath);
			}
		}
	}
}
