#pragma once

_TYPE_DATABASE()
class ComponentType
{
	typedef char id_t;

public:
	_DATABASE_INIT_FUNC()
	static void InitalizeTypes();

private:
	static std::map<std::string, ComponentType> s_TypeMap;

public:
	ComponentType(const id_t id) : m_Id(id) {}

private:
	const id_t m_Id;
};