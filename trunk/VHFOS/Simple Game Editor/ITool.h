#ifndef _ITOOL_H_
#define _ITOOL_H_

#include <SGF.h>

class ProjectLevel;

class ITool:public sgfEntity
{
friend class ProjectLevel;
public:
	ITool();
	virtual ~ITool();
	virtual void onAdd();
	virtual void onRemove();
	virtual void onActivate();
	virtual void onDeactivate();
	virtual void onMouse(SMouseEvent& mouseEvent);
	void registerMouse();
	void unregisterMouse();

	bool toggle;
	char* image;
	wchar_t* toolTipText;
private:
	irr::gui::IGUIButton* button;
	sgfPtr<sgfDelegate<SMouseEvent>> mouseDelegate;
};

#endif