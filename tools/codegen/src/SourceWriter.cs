using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace CodeGenerator
{
	class SourceWriter : StreamWriter
	{
		const string INCLUDE = "#include \"{0}\"";
		const string NAMESPACE = "using namespace {0}";
		const string FUNC = "{0} {1}::{2}( {3} )\r\n{{\r\n\tprintf(\"This is a generated function! Hello!\\n\");\r\n}}\r\n";
		SourceFile file;

		public SourceWriter(SourceFile file, Stream stream) : base(stream)
		{
            this.file = file;

			WriteInclude(file.FileName);
			//foreach (string ns in file.Component.Namespaces)
				//WriteNamespace(ns);
		}

		public void WriteInclude(string file)
		{
			WriteLine(INCLUDE, file);
		}

		public void WriteNamespace(string file)
		{
			WriteLine(NAMESPACE, file);
		}

		public void WriteFunction(Function func)
		{
			//WriteLine(FUNC, func.ReturnType, file.Component.Name, func.Name, func.Args);
		}

		public void WriteDatabase(TypeDatabase db, CodeGenerationManifest manifest)
		{

		}

		static string IndentSource(string src, int indent)
		{
			string indentString = "";
			for (int i = 0; i < indent; i++)
				indentString += "\t";
			indentString += "\n";

			return src.Replace("\n", indentString);
		}
	}
}
