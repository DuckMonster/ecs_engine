#pragma once

template<typename TReturn, typename... TArgs>
class DelegateDispatcher
{
	typedef TReturn(*ObjectCallHelperFunc)(void*, void*, TArgs...);
	typedef TReturn(*DelegateFunc)(TArgs...);

	struct Delegate
	{
		void* Object = nullptr;
		void* FPtr = nullptr;
		ObjectCallHelperFunc CallHelper = nullptr;
	};

	template<class TObject>
	static TReturn ObjectCallHelper(void* object, void* funcPtr, TArgs... args)
	{
		typedef TReturn(*ObjDelegateFunc)(TObject*, TArgs...);

		TObject* obj = (TObject*)object;
		ObjDelegateFunc objDelegate = (ObjDelegateFunc)funcPtr;

		objDelegate(obj, args...);
	}

public:
	DelegateDispatcher() {}
	~DelegateDispatcher() {}

	void Bind(TReturn(*funcPtr)(TArgs...));

	template<typename TObject>
	void Bind(TObject* objPtr, TReturn(*)(TObject*, TArgs...));

	void Broadcast(TArgs... args);

private:
	std::vector<Delegate> m_BoundDelegates;
};

template<typename TReturn, typename... TArgs>
void DelegateDispatcher<TReturn, TArgs...>::Broadcast(TArgs... args)
{
	for (Delegate& del : m_BoundDelegates)
	{
		if (del.Object == nullptr)
			((TReturn(*)(TArgs...))del.FPtr)(args...);
		else
			del.CallHelper(del.Object, del.FPtr, args...);
	}
}

template<typename TReturn, typename... TArgs>
template<typename TObject>
void DelegateDispatcher<TReturn, TArgs...>::Bind(TObject* objPtr, TReturn(*funcPtr)(TObject*, TArgs...))
{
	Delegate del;
	del.Object = (void*)objPtr;
	del.FPtr = (void*)funcPtr;
	del.CallHelper = ObjectCallHelper<TObject>;

	m_BoundDelegates.push_back(del);
}

template<typename TReturn, typename... TArgs>
void DelegateDispatcher<TReturn, TArgs...>::Bind(DelegateFunc funcPtr)
{
	Delegate del;
	del.FPtr = (void*)funcPtr;

	m_BoundDelegates.push_back(del);
}