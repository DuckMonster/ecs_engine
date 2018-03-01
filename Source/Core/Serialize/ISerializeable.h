#pragma once
class NamedArchive;

class ISerializeable
{
public:
	virtual void Serialize(const char* name, NamedArchive& archive) = 0;
};