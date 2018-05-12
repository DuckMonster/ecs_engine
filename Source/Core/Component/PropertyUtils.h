#pragma once

class Resource;
class MeshResource;
class ScriptResource;
class MaterialResource;

namespace propertyutils
{
	std::stringstream& operator <<(std::stringstream& stream, glm::vec2& data);
	std::stringstream& operator <<(std::stringstream& stream, glm::vec3& data);
	std::stringstream& operator <<(std::stringstream& stream, glm::vec4& data);
	std::stringstream& operator <<(std::stringstream& stream, glm::quat& data);
	std::stringstream& operator <<(std::stringstream& stream, Resource*& data);
	inline std::stringstream& operator <<(std::stringstream& stream, MeshResource*& data) { return operator<<(stream, (Resource*&)data); }
	inline std::stringstream& operator <<(std::stringstream& stream, ScriptResource*& data) { return operator<<(stream, (Resource*&)data); }
	inline std::stringstream& operator <<(std::stringstream& stream, MaterialResource*& data) { return operator<<(stream, (Resource*&)data); }

	template<typename T>
	std::string TypeToString(T& data)
	{
		std::stringstream stream;
		stream << data;

		return stream.str();
	}

	template<typename T>
	std::string PropertyToString(const void* data) { return TypeToString<T>(*(T*)data); }
}