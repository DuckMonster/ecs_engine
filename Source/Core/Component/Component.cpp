#include "CorePCH.h"
#include "Component.h"
#include "Core/Tools/StringUtils.h"

void Component::DebugPrint()
{
	Debug_Log("\t[ %s ]", m_Type.Name());
	for (Property& prop : m_PropertyList)
	{
		Debug_Log("\t\t%s = %s", prop.m_Name, prop.m_StringFunc(prop.m_Value).c_str());
	}

	Debug_Log("");
}

void Component::Initialize()
{
}

void Component::Serialize(NamedArchive& archive)
{
}
