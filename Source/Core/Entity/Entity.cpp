#include "CorePCH.h"
#include "Entity.h"

Entity::Entity(const FName& name) :
	m_Name(name)
{
	Debug_Log("Entity Created: %s", m_Name.c_str());

}

Entity::~Entity()
{
	Debug_Log("Entity Destroyed: %s", m_Name.c_str());
}