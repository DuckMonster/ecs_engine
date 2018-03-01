#include "CorePCH.h"
#include "PropertyUtils.h"
#include "Core/Resource/Resource.h"

namespace propertyutils
{
	std::stringstream& operator<<(std::stringstream& stream, glm::vec2& data)
	{
		stream << "(" << data.x << ", " << data.y << ")";
		return stream;
	}

	std::stringstream& operator<<(std::stringstream& stream, glm::vec3& data)
	{
		stream << "(" << data.x << ", " << data.y << ", " << data.z << ")";
		return stream;
	}

	std::stringstream& operator<<(std::stringstream& stream, glm::vec4& data)
	{
		stream << "(" << data.x << ", " << data.y << ", " << data.z << ", " << data.w << ")";
		return stream;
	}

	std::stringstream& operator<<(std::stringstream& stream, glm::quat& data)
	{
		stream << "(" << data.w << " | " << data.x << ", " << data.y << ", " << data.z << ")";
		return stream;
	}

	std::stringstream& operator<<(std::stringstream& stream, class Resource*& data)
	{
		if (data == nullptr)
			stream << "(NULL)";
		else
			stream << data->GetPath();

		return stream;
	}
}