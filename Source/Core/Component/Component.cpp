#include "CorePCH.h"
#include "Component.h"
#include "Core/Tools/StringUtils.h"
#include "Core/Entity/Entity.h"

/**	DebugPrint
*******************************************************************************/
void Component::DebugPrint()
{
	Debug_Log("\t[ %s ]", m_Type.Name());
	for (Property& prop : m_PropertyList)
	{
		Debug_Log("\t\t%s = %s", prop.m_Name, prop.m_StringFunc(prop.m_Value).c_str());
	}

	Debug_Log("");
}

/**	Get Sibling Component
*******************************************************************************/
Component* Component::GetSibling(const ComponentType& type)
{
	return m_Entity->GetComponent(type);
}

/**	Get Or Add Sibling Component
*******************************************************************************/
Component* Component::GetOrAddSibling(const ComponentType& type)
{
	return m_Entity->GetOrAddComponent(type);
}
