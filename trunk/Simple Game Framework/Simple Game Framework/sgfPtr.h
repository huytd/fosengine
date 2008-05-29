#ifndef _SGF_PTR_H_
#define _SGF_PTR_H_

/// \brief A smart pointer for sgfObject
/// \see sgfObject
template<class T>
class sgfPtr
{
public:
	sgfPtr()
		:ptr(0)
	{
	}
	explicit sgfPtr(const T* obj)
		:ptr(0)
	{
		assign(obj);
	}
	sgfPtr(const sgfPtr<T>& obj)
		:ptr(0)
	{
		assign(obj.ptr);
	}
	~sgfPtr()
	{
		if(ptr)
			ptr->decRef();
	}

	sgfPtr& operator=(T* obj)
	{
		assign(obj);
		return *this;
	}

	sgfPtr& operator=(const sgfPtr<T>& obj)
	{
		assign(obj.ptr);
		return *this;
	}

	inline friend bool operator==(const T* other, const sgfPtr<T>& me)
	{
		return(me.ptr==other);
	}

	inline friend bool operator==(const sgfPtr<T>& me, const sgfPtr<T>& other)
	{
		return(me.ptr==other.ptr);
	}

	inline friend bool operator!=(const T* other, const sgfPtr<T>& me)
	{
		return(me.ptr!=other);
	}

	inline friend bool operator!=(const sgfPtr<T>& me, const sgfPtr<T>& other)
	{
		return(me.ptr!=other.ptr);
	}

	T* getPtr() const
	{
		return ptr;
	}

	T* operator->()
	{
		return ptr;
	}

	T& operator*()
	{
		  return ptr;
	}

	operator T* ()
	{
		  return ptr;
	}
	
	///\brief Perform static_cast on the object
	template<class T2>
	T2* staticCast() const
	{
		return static_cast<T2>(this);
	}
	
	/// \brief Perform dynamic_cast on the object
	template<class T2>
	T2* dynamicCast() const
	{
		return dynamic_cast<T2>(this);
	}

private:
	void assign(const T* obj)
	{
		T* o=const_cast<T*>(obj);
		if(o)
			o->addRef();
		if(ptr)
			ptr->decRef();
		ptr=o;
	}
	T* ptr;
};

#endif