#pragma once
#ifdef DEFINE_TYPE
#error "Shit son"
#endif
#define DEFINE_TYPE(type) virtual bool Serialize(const char* name, type& value) = 0\

class IArchive;

class ISerializeable
{
public:
	virtual bool Serialize(IArchive& ar) = 0;
};

class IArchive 
{
public:
	// Default types
	DEFINE_TYPE(int8);
	DEFINE_TYPE(uint8);
	DEFINE_TYPE(int16);
	DEFINE_TYPE(uint16);
	DEFINE_TYPE(int32);
	DEFINE_TYPE(uint32);
	DEFINE_TYPE(float);
	DEFINE_TYPE(double);
	DEFINE_TYPE(char*);
	DEFINE_TYPE(std::string);
	DEFINE_TYPE(glm::vec2);
	DEFINE_TYPE(glm::vec3);
	DEFINE_TYPE(glm::vec4);
	DEFINE_TYPE(glm::quat);

	// Engine types
	void Serialize(const char* name, ISerializeable& object)
	{
		object.Serialize(*this);
	}
};

#undef DEFINE_TYPE