#include "CorePCH.h"
#include "ScriptLibrary.h"
#include <angelscript.h>

namespace
{
	/**	Namespace Scope Struct
	*******************************************************************************/
	struct NamespaceScope
	{
		NamespaceScope(asIScriptEngine* engine, const char* name) :
			m_Engine(engine)
		{
			m_Engine->SetDefaultNamespace(name);
		}
		~NamespaceScope()
		{
			m_Engine->SetDefaultNamespace("");
		}

	private:
		asIScriptEngine* m_Engine;
	};
}

//--------------------------------------------------- Debug Functions
namespace
{
	/**	Print
	*******************************************************************************/
	void Print(std::string message)
	{
		//Debug_Log_Minimal(message.c_str());
		printf("%s\n", message.c_str());
	}
}

//--------------------------------------------------- Time
#include "Core/Utils/Time.h"
namespace TimeFunc
{
	/**	Time::Elapsed
	*******************************************************************************/
	float Elapsed()
	{
		return Time::TotalElapsed();
	}
}

//--------------------------------------------------- Entity
#include "Core/Entity/Entity.h"
#include "Engine/Component/TransformComponent.h"
namespace EntityFunc
{
	/**	Get Name
	*******************************************************************************/
	std::string GetName(Entity* entity)
	{
		return entity->GetName().c_str();
	}

	/**	Get Transform
	*******************************************************************************/
	TransformComponent* GetTransform(Entity* entity)
	{
		return entity->GetComponent<TransformComponent>();
	}
}

//--------------------------------------------------- Math
namespace MathFunc
{
	/**	Vec3 to string
	*******************************************************************************/
	std::string Vec3ToString(const glm::vec3& vec)
	{
		std::stringstream stream;
		stream << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";

		return stream.str();
	}

	void ConstructVec3(glm::vec3* memory)
	{
		new(memory) glm::vec3();
	}
	void ConstructVec3AllSame(glm::vec3* memory, float v)
	{
		new(memory) glm::vec3(v);
	}
	void ConstructVec3Members(glm::vec3* memory, float x, float y, float z)
	{
		new(memory) glm::vec3(x, y, z);
	}
	void ConstructVec3Copy(glm::vec3* memory, const glm::vec3& other)
	{
		new(memory) glm::vec3(other);
	}
}

#define REGISTER_FUNC(decl, funcptr) do { int r = engine->RegisterGlobalFunction(decl, asFUNCTION(funcptr), asCALL_CDECL); Ensure(r >= 0); } while(0)

/**	Register Default Script Library
*******************************************************************************/
void RegisterDefaultScriptLibrary(class asIScriptEngine* engine)
{
	REGISTER_FUNC("void Print(string message)", Print);

	//--------------------------------------------------- Time
	{
		NamespaceScope scope(engine, "Time");
		REGISTER_FUNC("float Elapsed()", TimeFunc::Elapsed);
	}

	//--------------------------------------------------- Vec3
	{
		engine->RegisterObjectType("vec3", sizeof(glm::vec3), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<glm::vec3>());
		engine->RegisterObjectBehaviour("vec3", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(MathFunc::ConstructVec3), asCALL_CDECL_OBJFIRST);
		engine->RegisterObjectBehaviour("vec3", asBEHAVE_CONSTRUCT, "void f(const vec3 other)", asFUNCTION(MathFunc::ConstructVec3Copy), asCALL_CDECL_OBJFIRST);
		engine->RegisterObjectBehaviour("vec3", asBEHAVE_CONSTRUCT, "void f(float x, float y, float z)", asFUNCTION(MathFunc::ConstructVec3Members), asCALL_CDECL_OBJFIRST);
		engine->RegisterObjectBehaviour("vec3", asBEHAVE_CONSTRUCT, "void f(float v)", asFUNCTION(MathFunc::ConstructVec3AllSame), asCALL_CDECL_OBJFIRST);
		engine->RegisterObjectProperty("vec3", "float x", asOFFSET(glm::vec3, x));
		engine->RegisterObjectProperty("vec3", "float y", asOFFSET(glm::vec3, y));
		engine->RegisterObjectProperty("vec3", "float z", asOFFSET(glm::vec3, z));
		engine->RegisterObjectMethod("vec3", "string ToString()", asFUNCTION(MathFunc::Vec3ToString), asCALL_CDECL_OBJLAST);
	}

	//--------------------------------------------------- Entity
	{
		engine->RegisterObjectType("Entity", 0, asOBJ_REF | asOBJ_NOCOUNT);
		engine->RegisterObjectMethod("Entity", "string GetName()", asFUNCTION(EntityFunc::GetName), asCALL_CDECL_OBJLAST);

		engine->RegisterObjectType("TransformComponent", 0, asOBJ_REF | asOBJ_NOCOUNT);
		engine->RegisterObjectMethod("TransformComponent", "vec3& get_Position() const", asMETHOD(TransformComponent, GetPosition), asCALL_THISCALL);
		engine->RegisterObjectMethod("TransformComponent", "void set_Position(const vec3 position)", asMETHOD(TransformComponent, SetPosition), asCALL_THISCALL);
		engine->RegisterObjectMethod("TransformComponent", "void LookAt(const vec3 location, const vec3 up)", asMETHOD(TransformComponent, LookAt), asCALL_THISCALL);

		engine->RegisterObjectMethod("Entity", "TransformComponent& get_Transform()", asFUNCTION(EntityFunc::GetTransform), asCALL_CDECL_OBJLAST);
	}
}