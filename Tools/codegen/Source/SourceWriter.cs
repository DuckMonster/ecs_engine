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
			if (source == null)
				source = "";

			source = IndentSource(source, 1);

			if (className.Length > 0)
				WriteLine(FUNC, func.Signature(className), source);
			else
				WriteLine(FUNC, func.Signature(), source);
		}

		public void WriteDatabase(TypeDatabase db, CodeGenerationManifest manifest)
		{
			// Include all component files
			foreach(Component comp in manifest.Components)
				WriteInclude(comp.File.FullPath);

			WriteFunction(db.InitFunction, db.ClassName, db.InitFunction.Source);
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

	class HeaderWriter : StreamWriter
	{
		const string INCLUDE = "#include \"{0}\"";
		const string DEFINE_CURR = "#ifdef CURRENT_FILE\r\n#undef CURRENT_FILE\r\n#endif\r\n#define CURRENT_FILE {0}\r\n\r\n";
		const string DEFINE_GEN_HEADER = "#define GEN_BODY_{0}(...) \\\r\n{1}\r\n";
		const string FUNC = "{0}\r\n{{\r\n\t// This is a generated function! Hello!\r\n{1}\r\n}}\r\n";
		const string FUNC_DECL = "protected: {0};\r\n";
		SourceFile file;

		string CURRENT_FILE;
		StringWriter CompleteSource;

		public HeaderWriter(SourceFile file, Stream stream) : base(stream)
		{
			this.file = file;
			CURRENT_FILE = file.StripExtension();

			WriteInclude("Core/Serialize/Archive.h");
			WriteLine(DEFINE_CURR, CURRENT_FILE);

			CompleteSource = new StringWriter();
		}

		public override void Flush()
		{
			string source = NewLineSource(CompleteSource.ToString());
			WriteLine(DEFINE_GEN_HEADER, CURRENT_FILE, source);

			base.Flush();
		}

		public void WriteInclude(string file)
		{
			WriteLine(INCLUDE, file);
		}

		public void WriteFunction(Function func, string className, string source)
		{
			if (source == null)
				source = "";

			source = IndentSource(source, 1);

			if (className.Length > 0)
				CompleteSource.WriteLine(FUNC, func.Signature(className), source);
			else
				CompleteSource.WriteLine(FUNC, func.Signature(), source);
		}

		public void WriteFunctionDeclaration(Function func)
		{
			CompleteSource.WriteLine(FUNC_DECL, func.Signature());
		}

		public void WriteComponent(Component component)
		{
			if (component.GeneratedFunctions != null)
			{
				foreach (Function func in component.GeneratedFunctions)
					WriteFunctionDeclaration(func);
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

		static string NewLineSource(string src)
		{
			return src.Replace("\r\n", "\\\r\n");
		}
	}
}
