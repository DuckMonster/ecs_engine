using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Diagnostics;

namespace CodeGenerator
{
	class Utils
	{
		public static string[] FindNamespaces(string source, int index)
		{
			int level = 1;

			Regex namespaceRgx = new Regex(@"namespace (\w+)");
			Match match;

			List<string> result = new List<string>();

			while (--index >= 0)
			{
				if (source[index] == '{')
					level = Math.Max(level - 1, 0);
				if (source[index] == '}')
					level++;

				// Found a namespace 
				if (level == 0 && string.Compare(source, index, "namespace", 0, 9) == 0)
				{
					match = namespaceRgx.Match(source, index);
					if (!match.Success)
						continue;

					result.Add(match.Groups[1].Value);
				}
			}

			return result.ToArray();
		}

		public static void Print(string format, params object[] args)
		{
			Console.WriteLine(format, args);
			Debug.WriteLine(format, args);
		}

		public static void PrintVerbal(string format, params object[] args)
		{
			if (!CodeGen.GenData.Verbal)
				return;

			Print(format, args);
		}
	}
}
