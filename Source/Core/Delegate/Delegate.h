#pragma once

/* Huge class to handle delegate dispatching
* TODO: Add lambda support
*/
template<typename... TArgs>
class Delegate
{
	class ReceiverBase
	{
	public:
		virtual void Call(TArgs...) = 0;
	};

	/* Static receivers */
	class ReceiverStatic : public ReceiverBase
	{
	public:
		typedef void(*FuncType)(TArgs...);

		ReceiverStatic(FuncType func) : m_Func(func) {}
		void Call(TArgs... args) override
		{
			m_Func(args...);
		}

	private:
		FuncType m_Func;
	};

	/* Object receivers */
	template<class TClass>
	class ReceiverObject : public ReceiverBase
	{
	public:
		typedef void(TClass::*FuncType)(TArgs...);

		ReceiverObject(TClass* object, FuncType func) :
			m_Object(object), m_Func(func) {}

		void Call(TArgs... args) override
		{
			((*m_Object).*m_Func)(args...);
		}

	private:
		TClass* m_Object;
		FuncType m_Func;
	};

	/* Functor (lambda) receivers */
	template<class TFunctor>
	class ReceiverFunctor : public ReceiverBase
	{
	public:
		ReceiverFunctor(TFunctor& functor) :
			m_Functor(functor) {}

		void Call(TArgs... args) override
		{
			m_Functor(args...);
		}

	private:
		TFunctor& m_Functor;
	};

public:
	Delegate() {}
	~Delegate() {}

	void BindStatic(typename ReceiverStatic::FuncType func);
	template<typename TObject>
	void BindObject(TObject* object, typename ReceiverObject<TObject>::FuncType func);
	template<typename TFunctor>
	void BindLambda(TFunctor& functor);

	void Broadcast(TArgs... args);
	void operator()(TArgs... args) { Broadcast(args...); }

private:
	std::vector<ReceiverBase*> m_BoundReceivers;
};

/**	Bind Static
*******************************************************************************/
template<typename... TArgs>
void Delegate<TArgs...>::BindStatic(typename ReceiverStatic::FuncType func)
{
	ReceiverBase* receiver = new ReceiverStatic(func);
	m_BoundReceivers.push_back(receiver);
}

/**	Bind Object
*******************************************************************************/
template<typename... TArgs>
template<typename TObject>
void Delegate<TArgs...>::BindObject(TObject* object, typename ReceiverObject<TObject>::FuncType func)
{
	ReceiverBase* receiver = new ReceiverObject<TObject>(object, func);
	m_BoundReceivers.push_back(receiver);
}

/**	Bind Lambda
*******************************************************************************/
template<typename... TArgs>
template<typename TFunctor>
void Delegate<TArgs...>::BindLambda(TFunctor& functor)
{
	ReceiverBase* receiver = new ReceiverFunctor<TFunctor>(functor);
	m_BoundReceivers.push_back(receiver);
}

/**	Broadcast
*******************************************************************************/
template<typename... TArgs>
void Delegate<TArgs...>::Broadcast(TArgs... args)
{
	for (ReceiverBase* receiver : m_BoundReceivers)
		receiver->Call(args...);
}