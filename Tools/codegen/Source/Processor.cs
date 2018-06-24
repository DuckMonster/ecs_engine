using System;
using System.Collections.Generic;
using System.IO;

namespace CodeGenerator
{
	class Process
	{
		static Parameter ConstructParameter(string type, string name)
		{
			Parameter result = new Parameter();
			result.Type = type;
			result.Name = name;

			return result;
		}

		static Function ConstructFunction(string accessSpecifier, string returnType, string name, string preSpecifiers, string postSpecifiers, string source, params Parameter[] args)
		{
			Function func = new Function();
			func.ReturnType = returnType;
			func.Name = name;
			func.PreSpecifiers = preSpecifiers;
			func.PostSpecifiers = postSpecifiers;
			func.Source = source;
			func.Arguments = args;
			func.AccessSpecifier = accessSpecifier;

			return func;
		}

		public static void ProcessComponent(Component comp, CodeGenerationManifest manifest)
		{
			string serializeSource = "",
				initializeSource = "",
				staticTypeSource = "";

			List<Function> generatedFunctions = new List<Function>();

			//---------- Type binder
			{
				StringWriter sourceWriter = new StringWriter();
				sourceWriter.WriteLine("namespace {");
				sourceWriter.WriteLine("ComponentType::Bind<{0}> {0}_BIND( \"{0}\", {1} );", comp.Name, comp.Id);
				sourceWriter.WriteLine("}");
				comp.GeneratedTypeBinder = sourceWriter.ToString();
			}

			//---------- Serialize Function
			if (!comp.HasUserSerialize)
			{
				const string SERIALIZE_FORMAT = "archive.Serialize<{0}>(\"{1}\", {2});";
				StringWriter sourceWriter = new StringWriter();

				sourceWriter.WriteLine("{0}::Serialize( archive );", comp.Parent);

				foreach (Property prop in comp.Properties)
				{
					if (Array.IndexOf(prop.MetaData, "Serialize") == -1)
						continue;

					sourceWriter.WriteLine(SERIALIZE_FORMAT, prop.Parameter.Type, prop.CleanName, prop.Parameter.Name);
				}

				sourceWriter.WriteLine("OnSerialized();");

				serializeSource = sourceWriter.ToString();
				generatedFunctions.Add(ConstructFunction("public", "void", "Serialize", "", "override", serializeSource, ConstructParameter("NamedArchive&", "archive")));
			}

			//---------- Initialize function
			{
				StringWriter sourceWriter = new StringWriter();
				sourceWriter.WriteLine("{0}::InitializeInternal();", comp.Parent, comp.Id);
				sourceWriter.WriteLine("m_Type = StaticType();");

				foreach (Property prop in comp.Properties)
					sourceWriter.WriteLine("RegisterProperty<{0}>( \"{1}\", &{2} );", prop.Parameter.Type, prop.CleanName, prop.Parameter.Name);

				initializeSource = sourceWriter.ToString();
				generatedFunctions.Add(ConstructFunction("public", "void", "InitializeInternal", "", "override", initializeSource));
			}

			//---------- Static Type function
			{
				StringWriter sourceWriter = new StringWriter();
				sourceWriter.WriteLine("return {0}_BIND.ToType();", comp.Name);

				staticTypeSource = sourceWriter.ToString();
				generatedFunctions.Add(ConstructFunction("public", "const ComponentType&", "StaticType", "static", "", staticTypeSource));
			}

			comp.GeneratedFunctions = generatedFunctions.ToArray();
		}
	}
}