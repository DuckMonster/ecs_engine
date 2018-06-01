using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;

namespace CodeGenerator
{
	class SourceFile
	{
		public string FileName;
		public string FullPath;
		public string Source;
		public bool IsDirty;

		public string CacheFileName
		{ get { return StripExtension() + ".cache"; } }
		public string GenSrcFileName
		{ get { return StripExtension() + ".gen.cpp"; } }
		public string GenHeaderFileName
		{ get { return StripExtension() + ".gen.h"; } }
		public string GenSourcePath
		{ get { return Path.Combine(CodeGen.GenData.TargetPath, GenSrcFileName); } }
		public string GenHeaderPath
		{ get { return Path.Combine(CodeGen.GenData.TargetPath, GenHeaderFileName); } }

		public string StripExtension()
		{
			return Path.GetFileNameWithoutExtension(FileName);
		}
	}

	struct Parameter
	{
		public string Type;
		public string Name;

		public override string ToString() { return string.Format("{0} {1}", Type, Name); }
		public static string ParamaterListToString(Parameter[] paramList)
		{
			if (paramList == null)
				return "";

			string[] paramStrings = new string[paramList.Length];
			for(int i=0; i<paramList.Length; ++i)
			{
				paramStrings[i] = paramList[i].ToString();
			}

			return string.Join(", ", paramStrings);
		}
	}

	class Function
	{
		public string Name;
		public string ReturnType;
		public Parameter[] Arguments;
		public string[] MetaData;
		public string Source;
		public string PreSpecifiers;
		public string PostSpecifiers;
		public string AccessSpecifier;

		public string Signature()
		{
			string result = string.Format("{0}: {1} {2} {3}( {4} ) {5}", AccessSpecifier, PreSpecifiers, ReturnType, Name, Parameter.ParamaterListToString(Arguments), PostSpecifiers);
			return result;
		}

		public string Implemenatation(string className = "")
		{
			if (className == "")
				return string.Format("{0} {1}( {2} )", ReturnType, Name, Parameter.ParamaterListToString(Arguments));
			else
				return string.Format("{0} {1}::{2}( {3} )", ReturnType, className, Name, Parameter.ParamaterListToString(Arguments));
		}
	}

	class Property
	{
		public Parameter Parameter;
		public string CleanName;
		public string[] MetaData;
	}

	class Component
	{
		public SourceFile File;
		public string Name;
		public string Parent;
		public string[] MetaData;
		public string[] Namespaces;
		public Function[] Functions;
		public Function[] GeneratedFunctions;
		public Property[] Properties;
		public int Id;
		public bool HasUserSerialize;

		public bool HasFunction(string funcName)
		{
			foreach (Function func in Functions)
				if (func.Name == funcName)
					return true;

			return false;
		}
	}

	class TypeDatabase
	{
		public SourceFile File;
		public string ClassName;
		public Function InitFunction;
		public Function RegisterFunction;
	}

	class CodeGenerationManifest
	{
		//public string SourcePath;
		//public string TargetPath;
		//public bool CanGenerate;

		public List<Component> Components;
		public TypeDatabase Database;

		public void Print()
		{
			Utils.PrintVerbal("--- MANIFEST ---");
			Utils.PrintVerbal("Components [{0}]", Components.Count);
			foreach (Component c in Components)
			{
				Utils.PrintVerbal("- {0}", c.Name);
				Utils.PrintVerbal("File: {0}", c.File.FileName);
				Utils.PrintVerbal("Parent: {0}", c.Parent);
				Utils.PrintVerbal("Metadata: {0}", string.Join(", ", c.MetaData));
				Utils.PrintVerbal("Namespaces: {0}", string.Join(", ", c.Namespaces));

				if (c.Functions != null)
				{
					Utils.PrintVerbal("Functions: [{0}]", c.Functions.Length);
					foreach (Function func in c.Functions)
					{
						Utils.PrintVerbal("Metadata: {0}", string.Join(", ", func.MetaData));
						Utils.PrintVerbal(func.Signature());
					}
				}

				if (c.Properties != null)
				{
					Utils.PrintVerbal("Properties [{0}]: ", c.Properties.Length);
					foreach (Property prop in c.Properties)
					{
						Utils.PrintVerbal("Metadata: {0}", string.Join(", ", prop.MetaData));
						Utils.PrintVerbal(prop.Parameter.ToString());
					}
				}
			}

			Utils.PrintVerbal("\n\n");
			Utils.PrintVerbal("- Database");
			Utils.PrintVerbal("Class Name: {0}", Database.ClassName);
			Utils.PrintVerbal("File: {0}", Database.File.FileName);
			Utils.PrintVerbal("InitFunction: {0}", Database.InitFunction.Name);
			Utils.PrintVerbal("RegisterFunction: {0}", Database.RegisterFunction.Name);
		}
	}
}