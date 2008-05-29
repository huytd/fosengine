#ifndef _SGF_CORE_COMPONENT_H_
#define _SGF_CORE_COMPONENT_H_

#include "sgfObject.h"

class sgfCore;

/*!\brief An interface for components of the core framework
 * Some example of components are: sound engine, physic engine, script machine ...
 * Through this interface, all component can find its core and "communicate" with others
 */
class sgfCoreComponent:public sgfObject
{
public:
	/*!\brief constructor
	 * \param core The core that this component belongs to
	 */
	sgfCoreComponent(sgfCore* core=0);
	virtual ~sgfCoreComponent();
	/*!\brief Get the core of this component
	 *\return The core of this component
	 */
	sgfCore* getCore() const;
protected:
	sgfCore* core;
};

#endif