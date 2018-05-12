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

	private:
		FuncType m_Func;
	};

	template<class TClass>
	class ReceiverObject : public ReceiverBase
	{
	public:
		typedef TReturn(TClass::*FuncType)(TArgs...);

		ReceiverObject(TClass* object, FuncType func) : 
			m_Object(object), m_Func(func) {}

		TReturn Call(TArgs... args) override
		{
			return ((*m_Object).*m_Func)(args...);
		}

	private:
		TClass* m_Object;
		FuncType m_Func;
	};

public:
	Delegate() {}
	~Delegate() {}

	void BindStatic(typename ReceiverStatic::FuncType func);

	template<typename TObject>
	void BindObject(TObject* object, typename ReceiverObject<TObject>::FuncType func);

	void Broadcast(TArgs... args);

private:
	std::vector<ReceiverBase*> m_BoundReceivers;
};

template<typename TReturn, typename... TArgs>
template<typename TObject>
void Delegate<TReturn, TArgs...>::BindObject( TObject* object, typename ReceiverObject<TObject>::FuncType func )
{
	ReceiverBase* receiver = new ReceiverObject<TObject>(object, func);
	m_BoundReceivers.push_back(receiver);
}

template<typename TReturn, typename... TArgs>
void Delegate<TReturn, TArgs...>::BindStatic( typename ReceiverStatic::FuncType func )
{
	ReceiverBase* receiver = new ReceiverStatic(func);
	m_BoundReceivers.push_back(receiver);
}

/**	Broadcast
*******************************************************************************/
template<typename TReturn, typename... TArgs>
void Delegate<TReturn, TArgs...>::Broadcast(TArgs... args)
{
	for (ReceiverBase* receiver : m_BoundReceivers)
		receiver->Call(args...);
}