using System;
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

				serializeSource = sourceWriter.ToString();
			}

			//---------- Initialize function
			{
				StringWriter sourceWriter = new StringWriter();
				sourceWriter.WriteLine("{0}::Initialize();", comp.Parent, comp.Id);
				sourceWriter.WriteLine("m_Type = {0}::FromId( {1} );", manifest.Database.ClassName, comp.Id);

				foreach (Property prop in comp.Properties)
					sourceWriter.WriteLine("RegisterProperty<{0}>( \"{1}\", &{2} );", prop.Parameter.Type, prop.CleanName, prop.Parameter.Name);

				initializeSource = sourceWriter.ToString();
			}

			//---------- Static Type function
			{
				StringWriter sourceWriter = new StringWriter();
				sourceWriter.WriteLine("static ComponentType STATIC_TYPE( \"{0}\", {1} );", comp.Name, comp.Id);
				sourceWriter.WriteLine("return STATIC_TYPE;");

				staticTypeSource = sourceWriter.ToString();
			}

			comp.GeneratedFunctions = new Function[] 
			{
				ConstructFunction("public", "void", "Serialize", "", "override", serializeSource, ConstructParameter("NamedArchive&", "archive")),
				ConstructFunction("public", "void", "Initialize", "", "override", initializeSource),
				ConstructFunction("public", "const ComponentType&", "StaticType", "static", "", staticTypeSource)
			};
		}

		public static void ProcessDatabase(TypeDatabase database, CodeGenerationManifest manifest)
		{
			const string REGISTER_FORMAT = "{0}<{1}>( \"{1}\", {2} );";
			StringWriter sourceWriter = new StringWriter();

			foreach(Component comp in manifest.Components)
			{
				sourceWriter.WriteLine(REGISTER_FORMAT, database.RegisterFunction.Name, comp.Name, comp.Id);
			}

			database.InitFunction.Source = sourceWriter.ToString();
		}
	}
}