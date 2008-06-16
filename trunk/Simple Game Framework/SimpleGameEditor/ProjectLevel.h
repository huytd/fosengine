#ifndef _PROJECT_LEVEL_H_
#define _PROJECT_LEVEL_H_

#include <SGF.h>
#include <map>
#include <string>
#include "SGEProjectSetting.h"

struct SGetText
{
	const char* text;
	int choice;//0 for close, 1 for save, 2 for cancel
	inline SGetText(const char* text,int choice)
		:text(text),choice(choice)
	{}
};
class ITool;
/// \brief A special Level, which is a project
class ProjectLevel:public sgfLevel,public irr::scene::ISceneUserDataSerializer
{
public:
	ProjectLevel();
	virtual ~ProjectLevel();
	void onEnter(sgfEntityManager* emgr);
	void onExit(sgfEntityManager* emgr);
	irr::scene::ISceneNode* getSelectedNode() const;
	void setSelectedNode(irr::scene::ISceneNode* node);
	void addTool(ITool* tool);
	void updateNodeList();
	bool promptText(const wchar_t* msg,const wchar_t* defaultText,sgfDelegate<SGetText>* getTextDelegate);
	void getFile(const wchar_t* msg,sgfDelegate<const char*>* getFileDelegate);
	void setData(irr::scene::ISceneNode* node,const char* data);
	const char* getData(irr::scene::ISceneNode* node) const;

	irr::io::IAttributes *  createUserData (irr::scene::ISceneNode *forSceneNode);
	void  OnReadUserData (irr::scene::ISceneNode *forSceneNode, irr::io::IAttributes *userData);
	
	irr::scene::ICameraSceneNode* camera;
	irr::gui::IGUIListBox* meshList;
	SGEProjectSetting* setting;
private:
	void onGUIEvent(irr::SEvent::SGUIEvent& args);
	void onKey(SKeyboardEvent& args);
	void onMouse(SMouseEvent& args);
	void editNodeData(SGetText& args);
	void setUseKeyboard(bool value);
	void createGUI();
	void createScene();
	void updateProperties();
	void setData(irr::gui::IGUIElement* element,void* data);
	void* getData(irr::gui::IGUIElement* element) const;

	void load(const char*& fileName);
	void save(const char*& fileName);

	void loadSetting(const char*& fileName);
	void saveSetting(const char*&fileName);

	void setWorkingDir(const char*&fileName);
	void setTexturePath(const char*&fileName);
	void setMeshPath(const char*&fileName);

	sgfEntityManager* emgr;
	ITool* currentTool;
	irr::scene::ISceneNode* selectedNode;
	irr::gui::IGUIToolBar* toolbar;
	irr::gui::IGUIListBox* nodeList;
	sgfPtr<sgfDelegate<SGetText>> getTextDelegate;
	sgfPtr<sgfDelegate<const char*>> getFileDelegate;
	irr::core::stringc fileName;
	sgfPtr<sgfDelegate<irr::SEvent::SGUIEvent>> guiDelegate;
	sgfPtr<sgfDelegate<SKeyboardEvent>> keyDelegate;
	sgfPtr<sgfDelegate<SMouseEvent>> mouseDelegate;
	std::map<irr::gui::IGUIElement*,void*> guiMap;
	irr::io::IAttributes* att;
	bool lockX;
	bool lockY;
	bool useKeyboard;
	bool editingScript;
	int oldX;
	int oldY;
	irr::gui::IGUIWindow* propertyWindow;
	irr::gui::IGUIWindow* projectWindow;
	irr::gui::IGUIElement* lockStatus;
	irr::gui::IGUIWindow* textEditor;
	std::map<irr::scene::ISceneNode*,std::string> sceneNodeData;
	char openFileName[1024];
	char editorBuffer[800];
	wchar_t wideCharBuffer[800];//what a waste
};

#endif