#pragma once
template<class T>
class SingletonClass
{
public:
	static T* GetInstance()
	{
		if (!s_Instance)
			s_Instance = new T();

		return s_Instance;
	}

private:
	static T* s_Instance;
};

template<class T>
T* SingletonClass<T>::s_Instance = nullptr;