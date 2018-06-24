#pragma once

class Component;
class Entity;

class ComponentType
{
public:
	typedef char id_t;
	static const id_t INVALID_ID;

	/* Structs used for binding component types, define these in the generated cpp files */
	struct BindBase
	{
		virtual ComponentType& ToType() = 0;
	};

	template<typename TComp>
	struct Bind : BindBase
	{
	public:
		Bind(const char* name, id_t id);
		ComponentType& ToType() override { return type; }

	private:
		ComponentType type;
	};

private:
	//--------------------------------------------------- Make Functions
	typedef Component* (*MakeFunc)(Entity*);
	template<class T>
	static Component* DefaultMakeFunc(Entity* e) { return (Component*)new T(e); }

	//--------------------------------------------------- Database
public:
	// Getter functions
	static const ComponentType& FromId(id_t id);
	static const ComponentType& FromString(const char* name);

private:
	// Inner database maps
	static std::map<std::string, ComponentType>& GetStringTypeMap()
	{
		static std::map<std::string, ComponentType> StaticMap;
		return StaticMap;
	}

	static std::unordered_map<id_t, ComponentType>& GetIdTypeMap()
	{
		static std::unordered_map<id_t, ComponentType> StaticMap;
		return StaticMap;
	}

	//--------------------------------------------------- Class

public:
	ComponentType() {}

protected:
	ComponentType(const char* name, id_t id) : name(name), id(id) {}

public:
	id_t Id() const { return id ; }
	const char* Name() const { return name; }

	bool IsValid() const { return id != INVALID_ID; }
	Component* Make(Entity* entity) const { if (Ensure(makeFunc)) return makeFunc(entity); return nullptr; }

	bool operator==(const ComponentType& other) const { return other.id == id; }
	bool operator!=(const ComponentType& other) const { return !(*this == other); }

private:
	id_t id = INVALID_ID;
	const char* name = nullptr;
	MakeFunc makeFunc = nullptr;
};

/**	Types get created and bound here
*******************************************************************************/
template<typename TComp>
ComponentType::Bind<TComp>::Bind(const char* name, id_t id)
{
	type = ComponentType(name, id);
	type.makeFunc = DefaultMakeFunc<TComp>;

	ComponentType::GetStringTypeMap()[name] = type;
	ComponentType::GetIdTypeMap()[id] = type;
}