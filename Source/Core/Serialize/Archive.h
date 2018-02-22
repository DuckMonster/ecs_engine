#pragma once
class IArchive 
{
public:
	template<class T>
	void Serialize(const char* name, T* value) {}
};