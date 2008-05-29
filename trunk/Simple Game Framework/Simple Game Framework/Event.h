#ifndef _EVENT_H_
#define _EVENT_H_

#include "sgfObject.h"
#include "sgfPtr.h"
#include <list>

/// \brief Delegate of a function/method is an object that represents the function/method
/*!Its purpose is to pass an object pointer as parameter and make event handling easier.
 * This framework's delegates can only have 1 parameter.
 */
template<typename T>
class sgfDelegate:public sgfObject
{
public:
	  /// \brief invoke the delegate
	  /// \param param The parameters
      virtual void invoke(T& param)=0;
	  /// \brief This method is created for comparision
	  virtual unsigned int getSize()=0;
	  /// \return Whether the 2 delegates represent the same function/method
	  /// \param other The other delegate
	  virtual bool compare(sgfDelegate<T>* other)=0;
	  sgfDelegate(){}
	  virtual ~sgfDelegate()
	  {
	  }
};

/// \brief The delegate of a function or static method
template<typename T>
class sgfFunctionDelegate : public sgfDelegate<T>
{
public:
   /// \brief constructor
   /*! \param func A function
    *\code 
    * void print(const char* msg)
	* {
	*	std::cout<<msg;
	* }
	* sgfFunctionDelegate<const char*> printDelegate(print);
	* printDelegate("Hello");
	* \endcode
	*/
   sgfFunctionDelegate(void (*func)(T&)=0)
	   :func(func)
   {
   }
   sgfFunctionDelegate(const sgfFunctionDelegate<T>& other)
	   :func(other.func)
   {
   }
   virtual ~sgfFunctionDelegate()
   {
   }
public:
	/// \brief Bind the delegate to the function
	void bind(void (*func)(T&))
	{
		this->func=func;
	}
   virtual void invoke(T& param)
   {
	   func(param);
   }
   virtual unsigned int getSize()
   {
	   return sizeof(this);
   }
   virtual bool compare(sgfDelegate<T>* other)
   {
	   if(getSize()!=other->getSize())//compare size first
		   return false;
	   sgfFunctionDelegate<T>* _other=static_cast<sgfFunctionDelegate<T>*>(other);
	   return(_other->func==func);
   }
private:
   void (*func)(T&);
};

/// \brief The delegate that represents a class non-static method
template<class C,typename T>
class sgfMethodDelegate : public sgfDelegate<T>
{
public:
	/*!\param obj the owner of the metohd
	 * \param method the method to call
	 * \code 
	 * class TestClass
	 * {
	 * public:
	 *	void testMethod(SSomeStruct param);
	 * };
	 * TestClas* testObj=new TestClass;
	 * sgfMethodDelegate<TestClass,SSomeStruct> methodDelegate(testObj,&TestClass::testMethod);
	 * methodDelegate(...);
	 * \endcode
	 */
	sgfMethodDelegate(C* obj=0,void(C::*method)(T&)=0)
		:obj(obj),method(method)
	{
	}
	sgfMethodDelegate(const sgfMethodDelegate<C,T>& other)
		:obj(other.obj),method(other.method)
	{
	}
	virtual ~sgfMethodDelegate()
	{
	}
	void bind(C* obj,void(C::*method)(T&))
	{
		this->obj=obj;
		this->method=method;
	}
	virtual void invoke(T& param)
	{
		(obj->*method)(param);
	}
	virtual unsigned int getSize()
	{
	   return sizeof(this);
	}
    virtual bool compare(sgfDelegate<T>* other)
    {
	   if(getSize()!=other->getSize())//compare size first
		   return false;
	   sgfMethodDelegate<C,T>* _other=static_cast<sgfMethodDelegate<C,T>*>(other);
	   return((method==_other->method)||(obj==_other->obj));
    }
private:
   C* obj;
   void(C::*method)(T&);
};

/// \brief An event is a collection of delegates
/*! When the event "takes place" all delegates are called with the same
 *	parameter
 */
template<typename T>
class sgfEvent
{
typedef sgfPtr<sgfDelegate<T>> TDelegate;
public:
	sgfEvent()
	  :iterating(0)
	{
	}
	~sgfEvent()
	{
		removeAll();
	}
	
	/// \brief Trigger the event
	void fire(T& param)
	{
		//if(Delegates.size()==0)
		//	return;
		needRemoval=false;
		iterating=true;
		current=0;
		for(std::list<TDelegate>::iterator i=Delegates.begin();i!=Delegates.end();)
		{
			current=*i;
			(*i)->invoke(param);
			if(needRemoval)
			{
				i=Delegates.erase(i);
				needRemoval=false;
			}
			else
				++i;
		}
		iterating=false;
	}

	void addDelegate(sgfDelegate<T>* Delegate)
	{
		Delegates.push_back(TDelegate(Delegate));
	}

	void removeDelegate(sgfDelegate<T>* Delegate)
	{
		if((current->compare(Delegate))&&(iterating))
		{
			needRemoval=true;
		}
		else
		{
			for(std::list<TDelegate>::iterator i=Delegates.begin();i!=Delegates.end();)
			{
				if((*i)->compare(Delegate))
				{
					Delegates.erase(i);
					break;
				}
				else
					i++;
			}
		}
	}

	void removeAll()
	{
		Delegates.clear();
	}
	
	///\brief Short form of fire(param)
	inline void operator()(T& param)
	{
		fire(param);
	}

	int getNumDelegate() const
	{
		return Delegates.size();
	}

protected:
	std::list<TDelegate> Delegates;
private:
	sgfDelegate<T>* current;
	bool iterating;
	bool needRemoval;
};

///\brief An event with a delegate
///It relay an event params to other delegates
///Used for creating a tree structure
template<typename T>
class sgfEventRelay:public sgfEvent<T>
{
public:
	sgfEventRelay()
	{
		Delegate=new sgfMethodDelegate<sgfEventRelay<T>,T>(this,&sgfEventRelay<T>::invoke);
	}
	virtual void invoke(T& param)
	{
		fire(param);//relay the signal
	}
	///\brief The delegate to catch the event it wants to relay
	sgfPtr<sgfMethodDelegate<sgfEventRelay<T>,T>> Delegate;
};

#endif