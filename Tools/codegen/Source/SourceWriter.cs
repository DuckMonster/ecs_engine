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
		const string FUNC = "{0}\r\n{{\r\n\t// This is a generated function! Hello!\r\n{1}\r\n}}\r\n";
		SourceFile file;

		public SourceWriter(SourceFile file, Stream stream) : base(stream)
		{
			this.file = file;

			WriteInclude("CorePCH.h");
			WriteInclude(file.FullPath);
		}

		public void WriteInclude(string file)
		{
			WriteLine(INCLUDE, file);
		}

		public void WriteNamespace(string file)
		{
			WriteLine(NAMESPACE, file);
		}

		public void WriteFunction(Function func, string className, string source)
		{
			source = IndentSource(source, 1);

			if (className.Length > 0)
				WriteLine(FUNC, func.Signature(className), source);
			else
				WriteLine(FUNC, func.Signature(), source);
		}

		public void WriteDatabase(TypeDatabase db, CodeGenerationManifest manifest)
		{
			const string REG_SIGNATURE = "{0}( \"{1}\", {2} );";

			StringWriter writer = new StringWriter();

			for(int i=0; i<manifest.Components.Count; ++i)
			{
				Component comp = manifest.Components[i];
				writer.WriteLine(REG_SIGNATURE, "Register", comp.Name, i);
			}
			Function initFunc = new Function();
			initFunc.Name = db.InitFuncName;
			initFunc.ReturnType = "void";

			WriteFunction(initFunc, db.ClassName, writer.ToString());
		}

		public void WriteComponent(Component component)
		{
			if (component.Functions != null)
			{
				foreach (Function func in component.Functions)
					WriteFunction(func, component.Name, "");
			}
			if (component.GeneratedFunctions != null)
			{
				foreach (Function func in component.GeneratedFunctions)
					WriteFunction(func, component.Name, func.Source);
			}
		}

		static string IndentSource(string src, int indent)
		{
			string indentString = "";

			for (int i = 0; i < indent; i++)
				indentString += "\t";

			src = src.Insert(0, indentString);

			return src.Replace("\n", "\n" + indentString);
		}
	}
}
