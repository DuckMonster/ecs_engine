﻿using System;
using System.Collections.Generic;
using System.IO;
using System.Security.Cryptography;
using System.Text;
using System.Text.RegularExpressions;

namespace CodeGenerator
{
	struct FileParser
	{
		static SourceFile CurrentFile;

		/* Component regex
		 * Group 1: MetaData
		 * Group 2: Component Name (class name)
		 * Group 3: Parent Name
		 * Group 4: Full body of component
		 * Group 5: (group used for brace matching)
		 */
		static Regex COMP_RGX = new Regex(@"\sCOMPONENT\(\s*([\w\s,]*?)\s*\)\n?\s*class (.+) : *(?:public )? *(.+)\n((\s*){(?:.*\n)*?\5})");

		/* Function regex
		 * Group 1: MetaData
		 * Group 2: Return Type
		 * Group 3: Function Name
		 * Group 4: Arguments
		 */
		static Regex FUNC_RGX = new Regex(@"\sFUNCTION\s*\(\s*([\w\s,]*)\s*\)\W*(.+?)\s+(\w+)\s*\(\s*([\w\s,&*:]*)\s*\)");

		/* Serialize regex
		 * No capture groups, this is just to check if we should generate a serialize function for you :)
		 */
		static Regex SERIALIZE_RGX = new Regex(@"Serialize\s*\(\s*[\w\s,&*:]*\s*\)\s*override");

		/* Property regex
		 * Group 1: MetaData
		 * Group 2: Type
		 * Group 3: Name
		 */
		static Regex PROP_RGX = new Regex(@"\sPROPERTY\s*\(\s*([\w\s,]*)\s*\)\W*(.*)\s(.*?)\s[=;]");

		/* Type database regex
		 * Group 1: MetaData
		 * Group 2: Class Name
		 * Group 3: Full body of class
		 */
		static Regex TYPE_DB_RGX = new Regex(@"\s_TYPE_DATABASE\(\s*([\w\s,]*?)\s*\)\n?\s*class (.+).*\n((\s*){(?:.*\n)*?\4})");

		public static void Parse(string sourcePath, CodeGenerationManifest manifest)
		{
			if (!File.Exists(sourcePath))
			{
				CodeGen.Error("File \"{0}\" does not exist.", sourcePath);
				return;
			}

			SourceFile file = new SourceFile();

			file.FileName = Path.GetFileName(sourcePath);
			file.FullPath = sourcePath;
			file.Source = File.ReadAllText(sourcePath).Replace("\r", "");
			file.IsDirty = false;

			CurrentFile = file;

			bool fileShouldBeGenerated = false;

			// ----------------------------- Collect Components
			MatchCollection match = COMP_RGX.Matches(file.Source);
			if (match.Count > 0)
			{
				fileShouldBeGenerated = true;

				string[] namespaces = Utils.FindNamespaces(file.Source, match[0].Index);
				string[] metadata = ParseMetadata(match[0].Groups[1].Value);

				Component comp = new Component();
				comp.File = file;
				comp.Name = match[0].Groups[2].Value;
				comp.Parent = match[0].Groups[3].Value;
				comp.MetaData = metadata;
				comp.Namespaces = namespaces;
				comp.Id = manifest.Components.Count;
				comp.HasUserSerialize = SERIALIZE_RGX.Match(match[0].Groups[4].Value).Success;

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
						prop.Parameter.Type = propMatch[p].Groups[2].Value;
						prop.Parameter.Name = propMatch[p].Groups[3].Value;
						prop.CleanName = CleanPropertyName(prop.Parameter.Name);

						prop.MetaData = propMetaData;

						compProperties[p] = prop;
					}

					comp.Properties = compProperties;
				}

				Utils.PrintVerbal("({0}) Parsed component \"{1}\"...", file.FileName, comp.Name);
				manifest.Components.Add(comp);
			}

			if (fileShouldBeGenerated)
				file.IsDirty = GetFileIsDirty(file);
		}

		public static Parameter[] ParseParameters(string src)
		{
			string[] commaSplit = src.Split(new char[] { ',' }, StringSplitOptions.RemoveEmptyEntries);
			Parameter[] result = new Parameter[commaSplit.Length];

			for(int i=0; i<result.Length; ++i)
			{
				string[] paramSplit = commaSplit[i].Split(new char[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);
				if (paramSplit.Length == 1)
					CodeGen.Error(CurrentFile.FileName, "\"{0}\" is an invalid parameter!", src);

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

		public static string CleanPropertyName(string name)
		{
			if (name.StartsWith("m_"))
				return name.Substring(2);
			else
				return name;
		}

		static bool GetFileIsDirty(SourceFile file)
		{
			if (CodeGen.GenData.CanGenerate)
				return true;

			if (File.Exists(file.GenSourcePath) && File.Exists(file.GenHeaderPath))
			{
				// Check time modified
				DateTime SourceDate = File.GetLastWriteTime(file.FullPath),
					TargetSourceDate = File.GetLastWriteTime(file.GenSourcePath),
					TargetHeaderDate = File.GetLastWriteTime(file.GenHeaderPath);

				return (SourceDate > TargetSourceDate || SourceDate > TargetHeaderDate);
			}
			// The generated files dont exist, it's dirty
			else return true;
		}
	}
}
