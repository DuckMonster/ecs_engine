#pragma once

/* Huge class to handle delegate dispatching
 * Supports simple functions, as well as object-prefaced functions (so just regular functions
 * in the form 'foo(Object*, args...)'. Failed to do member functions :( Sadness
 * TODO: Add lambda support
 */
template<typename TReturn, typename... TArgs>
class DelegateDispatcher
{
	typedef TReturn(*ObjectCallHelperFunc)(void*, void*, TArgs...);
	typedef TReturn(*DelegateFunc)(TArgs...);

	// Data structure to store data needed to invoke both regular functions and object-functions
	struct Delegate
	{
		void* Object = nullptr;
		void* FPtr = nullptr;
		ObjectCallHelperFunc CallHelper = nullptr;
	};

	// Helper function to cast objects to their specified class
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

/**	Broadcast
*******************************************************************************/
template<typename TReturn, typename... TArgs>
void DelegateDispatcher<TReturn, TArgs...>::Broadcast(TArgs... args)
{
	for (Delegate& del : m_BoundDelegates)
	{
		// If no object specified, this is just ye olde function
		if (del.Object == nullptr)
			((DelegateFunc)del.FPtr)(args...);

		// Otherwise, invoke the callhelper to cast the object into its specified type
		// then call function with additional 'this' argument
		else
			del.CallHelper(del.Object, del.FPtr, args...);
	}
}

/**	Bind Object Function
*******************************************************************************/
template<typename TReturn, typename... TArgs>
template<typename TObject>
void DelegateDispatcher<TReturn, TArgs...>::Bind(TObject* objPtr, TReturn(*funcPtr)(TObject*, TArgs...))
{
	/*
	So,
	And object function is just a regular function with a fake 'this' pointer at the beginning.
	We use a pointer to a generic function to recast the void* to the specified type when invoking.
	*/

	Delegate del;
	del.Object = (void*)objPtr;
	del.FPtr = (void*)funcPtr;
	del.CallHelper = ObjectCallHelper<TObject>; // <-- This will handle casting and calling

	m_BoundDelegates.push_back(del);
}

/**	Bind Function
*******************************************************************************/
template<typename TReturn, typename... TArgs>
void DelegateDispatcher<TReturn, TArgs...>::Bind(DelegateFunc funcPtr)
{
	// Just cast to void*, super easy
	Delegate del;
	del.FPtr = (void*)funcPtr;

	m_BoundDelegates.push_back(del);
}