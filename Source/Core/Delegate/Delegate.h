#pragma once

/* Huge class to handle delegate dispatching
 * TODO: Add lambda support
 */
template<typename TReturn, typename... TArgs>
class Delegate
{
	class ReceiverBase
	{
	public:
		virtual TReturn Call(TArgs...) = 0;
	};

	class ReceiverStatic : public ReceiverBase
	{
	public:
		typedef TReturn(*FuncType)(TArgs...);

		ReceiverStatic(FuncType func) : m_Func(func) {}
		TReturn Call(TArgs... args) override
		{
			return m_Func(args...);
		}

		FuncType m_Func;
	};

	class ReceiverObjectBase : public ReceiverBase
	{
	public:
		ReceiverObjectBase(void* object) : m_Object(object) {}
		void* m_Object;
	};

	template<class TClass>
	class ReceiverObject : public ReceiverObjectBase
	{
	public:
		typedef TReturn(TClass::*FuncType)(TArgs...);

		ReceiverObject(TClass* object, FuncType func) : 
			ReceiverObjectBase(object), m_Func(func) {}

		TReturn Call(TArgs... args) override
		{
			return ((*(TClass*)m_Object).*m_Func)(args...);
		}

		FuncType m_Func;
	};

public:
	Delegate() {}
	~Delegate() {}

	void BindStatic(typename ReceiverStatic::FuncType func);
	void RemoveStatic(typename ReceiverStatic::FuncType func);

	template<typename TObject>
	void BindObject(TObject* object, typename ReceiverObject<TObject>::FuncType func);
	template<typename TObject>
	void RemoveObject(TObject* object, typename ReceiverObject<TObject>::FuncType func);

	void Broadcast(TArgs... args);
	void operator()(TArgs... args) { Broadcast(args...); }

private:
	std::vector<ReceiverStatic*> m_BoundStatics;
	std::vector<ReceiverObjectBase*> m_BoundObjects;
};

/**	Bind Object
*******************************************************************************/
template<typename TReturn, typename... TArgs>
template<typename TObject>
void Delegate<TReturn, TArgs...>::BindObject( TObject* object, typename ReceiverObject<TObject>::FuncType func )
{
	ReceiverObjectBase* receiver = new ReceiverObject<TObject>(object, func);
	m_BoundObjects.push_back(receiver);
}

/**	Bind Static
*******************************************************************************/
template<typename TReturn, typename... TArgs>
void Delegate<TReturn, TArgs...>::BindStatic( typename ReceiverStatic::FuncType func )
{
	ReceiverStatic* receiver = new ReceiverStatic(func);
	m_BoundStatics.push_back(receiver);
}

/**	Broadcast
*******************************************************************************/
template<typename TReturn, typename... TArgs>
void Delegate<TReturn, TArgs...>::Broadcast(TArgs... args)
{
	for (ReceiverBase* receiver : m_BoundObjects)
		receiver->Call(args...);

	for (ReceiverBase* receiver : m_BoundStatics)
		receiver->Call(args...);
}

/**	Remove Static
*******************************************************************************/
template<typename TReturn, typename... TArgs>
void Delegate<TReturn, TArgs...>::RemoveStatic( typename ReceiverStatic::FuncType func )
{
	for(int i = m_BoundStatics.size(); i >= 0; --i)
	{
		if (m_BoundStatics[i].m_Func == func)
			m_BoundStatics.erase(m_BoundStatics.begin() + i);
	}
}

/**	Remove Object
*******************************************************************************/
template<typename TReturn, typename... TArgs>
template<typename TObject>
void Delegate<TReturn, TArgs...>::RemoveObject( TObject* object, typename ReceiverObject<TObject>::FuncType func )
{
	for(int i = (int)m_BoundObjects.size(); i >= 0; --i)
	{
		// First check if the raw pointer is equal
		if (m_BoundObjects[i]->m_Object == (void*)object)
		{
			// If it is, we feel confident enough to cast the receiverobject to the correct type
			ReceiverObject<TObject>* tReceiver = (ReceiverObject<TObject>*)m_BoundObjects[i];

			// And then check the function
			if (tReceiver->m_Func == func)
			{
				m_BoundObjects.erase(m_BoundObjects.begin() + i);
			}
		}
	}
}
