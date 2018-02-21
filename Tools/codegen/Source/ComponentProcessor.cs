using System;
using System.IO;

namespace CodeGenerator
{
	class ComponentProcessor
	{
		public static void Process(Component comp)
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

			const string SERIALIZE_FORMAT = "archive.Serialize<{0}>(&{1});";
			StringWriter sourceWriter = new StringWriter();

			foreach(Property prop in comp.Properties)
			{
				if (Array.IndexOf(prop.MetaData, "Serialize") == -1)
					continue;

				sourceWriter.WriteLine(SERIALIZE_FORMAT, prop.Parameter.Type, prop.Parameter.Name);
			}

			serializeFunction.Source = sourceWriter.ToString();

			comp.GeneratedFunctions = new Function[] { serializeFunction };
		}
	}
}