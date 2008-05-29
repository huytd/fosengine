#ifndef _ISGF_OBJECT_H_
#define _ISGF_OBJECT_H_

#ifdef DEBUG
#include <exception>
#endif
/// \brief Base class for most objects in this framework
/// This class provides reference counting mechanism
/// \see sgfPtr<T>
class sgfObject
{
public:
	sgfObject()
		:refCounter(0)
	{
	}
protected:
	virtual ~sgfObject()
	{
	}
public:
	/// \brief add reference
	void addRef() 
	{
		++refCounter;
	}

	/// \brief remove reference
	/// and delete the object if reference ==0
	void decRef()
	{
		--refCounter;
#ifdef DEBUG
		if(refCounter<0)
			throw std::exception("Bad reference counting");
#endif
		if(!refCounter)
		{
			delete this;
		}
	}

private:
	int refCounter;
};

#endif