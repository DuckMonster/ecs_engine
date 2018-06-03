#include <set>

// NOTE: Not done

template<class T>
class TSharedPtr;

template<class T>
class TShareableObject
{
	friend class TSharedPtr<T>;

	int GetReferenceCount() { return m_RefPointers.size(); }

private:
	void AddRef(TSharedPtr<T>& ptr);
	void RemoveRef(TSharedPtr<T>& ptr);

	std::set<TSharedPtr<T>> m_RefPointers;
};

template<class T>
class TSharedPtr
{
};

template<class T>
class TWeakPtr
{
};