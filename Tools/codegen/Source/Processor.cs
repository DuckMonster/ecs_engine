using System;
using System.IO;

namespace CodeGenerator
{
	class Process
	{
		public static void ProcessComponent(Component comp, CodeGenerationManifest manifest)
		{
			//---------- Serialize Function
			if (comp.HasFunction("Serialize"))
				return;

			Function serializeFunction = new Function();
			serializeFunction.Name = "Serialize";
			serializeFunction.ReturnType = "void";

			Parameter archiveParameter = new Parameter();
			archiveParameter.Name = "archive";
			archiveParameter.Type = "IArchive*";

			serializeFunction.Arguments = new Parameter[] { archiveParameter };

			{
				const string SERIALIZE_FORMAT = "archive.Serialize<{0}>(&{1});";
				StringWriter sourceWriter = new StringWriter();

				foreach (Property prop in comp.Properties)
				{
					if (Array.IndexOf(prop.MetaData, "Serialize") == -1)
						continue;

					sourceWriter.WriteLine(SERIALIZE_FORMAT, prop.Parameter.Type, prop.Parameter.Name);
				}

				serializeFunction.Source = sourceWriter.ToString();
			}

			//---------- Initialize function
			Function initializeFunction = new Function();
			initializeFunction.Name = "Initialize";
			initializeFunction.ReturnType = "void";

			{
				StringWriter sourceWriter = new StringWriter();
				sourceWriter.WriteLine("m_Type = {0}::FromId( {1} );", manifest.Database.ClassName, comp.Id);

				initializeFunction.Source = sourceWriter.ToString();
			}

			comp.GeneratedFunctions = new Function[] { serializeFunction, initializeFunction };
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