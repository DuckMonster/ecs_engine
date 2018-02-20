using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;

namespace CodeGenerator
{
	struct SourceFile
	{
		public string FileName;
		public string FullPath;
		public string Source;

		public string StripExtension()
		{
			return Path.GetFileNameWithoutExtension(FileName);
		}

		public string GenFileName()
		{
			string name = StripExtension();
			return name + ".gen.cpp";
		}
	}

	struct Parameter
	{
		public string Type;
		public string Name;

		public override string ToString() { return string.Format("[{0}] [{1}]", Type, Name); }
	}

	struct Function
	{
		public string Name;
		public string ReturnType;
		public Parameter[] Arguments;
		public string[] MetaData;

		public string Signature()
		{
			string result = ReturnType + " " + Name + "(";
			string[] argStrings = new string[Arguments.Length];

			for(int i=0; i<Arguments.Length; ++i)
			{
				argStrings[i] = Arguments[i].ToString();
			}

			result += string.Join(", ", argStrings) + ")";

			return result;
		}

		public string Signature(string className)
		{
			string result = ReturnType + " " + className + "::" + Name + "(";
			string[] argStrings = new string[Arguments.Length];

			for(int i=0; i<Arguments.Length; ++i)
			{
				argStrings[i] = Arguments[i].ToString();
			}

			result += string.Join(", ", argStrings) + ")";

			return result;
		}
	}

	struct Property
	{
		public Parameter Parameter;
		public string[] MetaData;
	}

	struct Component
	{
		public SourceFile File;
		public string Name;
		public string Parent;
		public string[] MetaData;
		public string[] Namespaces;
		public Function[] Functions;
		public Property[] Properties;
	}

	struct TypeDatabase
	{
		public SourceFile File;
		public string ClassName;
		public string InitFuncName;
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
			Utils.Print("--- MANIFEST ---");
			Utils.Print("Components [{0}]", Components.Count);
			foreach (Component c in Components)
			{
				Utils.Print("- {0}", c.Name);
				Utils.Print("File: {0}", c.File.FileName);
				Utils.Print("Parent: {0}", c.Parent);
				Utils.Print("Metadata: {0}", string.Join(", ", c.MetaData));
				Utils.Print("Namespaces: {0}", string.Join(", ", c.Namespaces));

				if (c.Functions != null)
				{
					Utils.Print("Functions: [{0}]", c.Functions.Length);
					foreach (Function func in c.Functions)
					{
						Utils.Print("Metadata: {0}", string.Join(", ", func.MetaData));
						Utils.Print(func.Signature(c.Name));
					}
				}

				if (c.Properties != null)
				{
					Utils.Print("Properties [{0}]: ", c.Properties.Length);
					foreach (Property prop in c.Properties)
					{
						Utils.Print("Metadata: {0}", string.Join(", ", prop.MetaData));
						Utils.Print(prop.Parameter.ToString());
					}
				}
			}

			Utils.Print("\n\n");
			Utils.Print("- Database");
			Utils.Print("Class Name: {0}", Database.ClassName);
			Utils.Print("File: {0}", Database.File.FileName);
			Utils.Print("InitFunction: {0}", Database.InitFuncName);
		}
	}
}