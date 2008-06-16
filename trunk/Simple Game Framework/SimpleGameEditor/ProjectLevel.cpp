#include "ProjectLevel.h"
#include "CGridSceneNode.h"
#include "StandardTools.h"
#include "CyoDecode.h"
#include "CyoEncode.h"
//#include "FileDialog.h"

using namespace irr;
using namespace scene;
using namespace core;
using namespace io;
using namespace gui;

static const char* untitled="resources/untitled.irr";

ProjectLevel::ProjectLevel()
:selectedNode(0),currentTool(0),lockX(0),lockY(0),oldX(0),oldY(0),useKeyboard(true),editingScript(false),setting(0)
{
	guiDelegate=new sgfMethodDelegate<ProjectLevel,irr::SEvent::SGUIEvent>(this,&ProjectLevel::onGUIEvent);
	keyDelegate=new sgfMethodDelegate<ProjectLevel,SKeyboardEvent>(this,&ProjectLevel::onKey);
	mouseDelegate=new sgfMethodDelegate<ProjectLevel,SMouseEvent>(this,&ProjectLevel::onMouse);
}

ProjectLevel::~ProjectLevel()
{
	delete setting;
}

void ProjectLevel::onEnter(sgfEntityManager* emgr)
{
	emgr->getCore()->getGUIEvent()->addDelegate(guiDelegate);
	emgr->getCore()->getInputManager()->getKeyboardEvent()->addDelegate(keyDelegate);
	emgr->getCore()->getInputManager()->getMouseEvent()->addDelegate(mouseDelegate);
	this->emgr=emgr;
	att=emgr->getCore()->getFileSystem()->createEmptyAttributes(emgr->getCore()->getGraphicDevice()->getVideoDriver());
	setting=new SGEProjectSetting(emgr->getCore()->getFileSystem());
	createGUI();
	load(untitled);
	fileName="";
}

void ProjectLevel::createScene()
{
	ISceneManager* smgr=emgr->getCore()->getGraphicDevice()->getSceneManager();
	CGridSceneNode* grid=new CGridSceneNode(smgr->getRootSceneNode(),smgr,-1,8,2048,SColor(255,128,128,128),32,SColor(255,0,0,0),false);
	grid->setName("Grid");
	grid->setIsDebugObject(true);
	grid->drop();
	SKeyMap keyMap[4];
	keyMap[0].Action=EKA_MOVE_FORWARD;
	keyMap[0].KeyCode=KEY_KEY_W;
	keyMap[1].Action=EKA_MOVE_BACKWARD;
	keyMap[1].KeyCode=KEY_KEY_S;
	keyMap[2].Action=EKA_STRAFE_LEFT;
	keyMap[2].KeyCode=KEY_KEY_A;
	keyMap[3].Action=EKA_STRAFE_RIGHT;
	keyMap[3].KeyCode=KEY_KEY_D;
	camera=smgr->addCameraSceneNodeFPS(0,100,500,-1,keyMap,4);
	camera->setInputReceiverEnabled(false);
	camera->setIsDebugObject(true);
	camera->setPosition(vector3df(0,100,-100));
	camera->setTarget(vector3df(0,0,0));
	camera->setFOV(PI/3.0f);
}

void ProjectLevel::onExit(sgfEntityManager* emgr)
{
	emgr->getCore()->getGUIEvent()->removeDelegate(guiDelegate);
	emgr->getCore()->getInputManager()->getKeyboardEvent()->removeDelegate(keyDelegate);
	emgr->getCore()->getInputManager()->getMouseEvent()->removeDelegate(mouseDelegate);
	this->emgr=0;
	att->drop();
	textEditor->drop();
	projectWindow->drop();
	emgr->getCore()->getGraphicDevice()->getSceneManager()->clear();
	emgr->getCore()->getGraphicDevice()->getGUIEnvironment()->clear();
}

void ProjectLevel::onKey(SKeyboardEvent& args)
{
	if(!useKeyboard)
		return;
	if((args.keyEvent==SKeyboardEvent::EKS_PRESSED))
	{
		if(args.keyCode==KEY_KEY_Q)
			camera->setInputReceiverEnabled(!camera->isInputReceiverEnabled());//toggle;
		if((args.keyCode==KEY_KEY_Z)||(args.keyCode==KEY_KEY_X))
		{//lock mouse axis
			if(args.keyCode==KEY_KEY_Z)
				lockX=!lockX;
			if(args.keyCode==KEY_KEY_X)
				lockY=!lockY;
			stringw text;
			if(!lockX)
				text+="X ";
			else
				text+="   ";
			if(!lockY)
				text+="Y";
			lockStatus->setText(text.c_str());
		}
		if(args.keyCode==KEY_DELETE&&selectedNode&&(selectedNode!=emgr->getCore()->getGraphicDevice()->getSceneManager()->getRootSceneNode()))
		{
			ISceneNode* node=selectedNode;
			setSelectedNode(0);
			node->remove();	
			updateNodeList();
		}

	}

}

void ProjectLevel::onMouse(SMouseEvent& args)
{
	if(lockX)//mouse axis locking
		args.x=oldX;
	if(lockY)
		args.y=oldY;
	oldX=args.x;
	oldY=args.y;
}

void ProjectLevel::onGUIEvent(irr::SEvent::SGUIEvent& args)
{
	IGUIElement* caller=args.Caller;
	IGUIElement* other=args.Element;
	IGUIEnvironment* env=emgr->getCore()->getGraphicDevice()->getGUIEnvironment();
	int id;
	IGUIFileOpenDialog* dialog;
	IGUIContextMenu* menu;
	const char* null=0;
	const char* tempStr;
	switch(args.EventType)
	{
		case EGET_MENU_ITEM_SELECTED:
			menu=(IGUIContextMenu*)caller;
			id=menu->getItemCommandId(menu->getSelectedItem());
			switch(id)
			{
			case 1://new
				load(untitled);
				fileName="";
			break;
			case 2://open
				getFile(L"Select a file to open",new sgfMethodDelegate<ProjectLevel,const char*>(this,&ProjectLevel::load));
			break;
			case 3://save
				if(fileName.size())
				{
					tempStr=fileName.c_str();
					save(tempStr);
					break;
				}
			case 4://save as
				getFile(L"Select a file to save",new sgfMethodDelegate<ProjectLevel,const char*>(this,&ProjectLevel::save));
			break;
			case 5://exit
				emgr->getCore()->getGraphicDevice()->closeDevice();
			break;
			IGUIEditBox* editbox;
			case 11://Edit
				env->getRootGUIElement()->addChild(projectWindow);
				env->getRootGUIElement()->bringToFront(projectWindow);
				editbox=(IGUIEditBox*)(projectWindow->getElementFromId(1));
				mbstowcs(wideCharBuffer,setting->workingDir.c_str(),800);
				editbox->setText(wideCharBuffer);
				editbox=(IGUIEditBox*)(projectWindow->getElementFromId(2));
				mbstowcs(wideCharBuffer,setting->meshPath.c_str(),800);
				editbox->setText(wideCharBuffer);
				editbox=(IGUIEditBox*)(projectWindow->getElementFromId(3));
				mbstowcs(wideCharBuffer,setting->texturePath.c_str(),800);
				editbox->setText(wideCharBuffer);
			break;
			case 12://save
				if(setting->fileName.size())
				{
					setting->write();
					break;
				}
			case 13://save as
				getFile(L"Select a file to save",new sgfMethodDelegate<ProjectLevel,const char*>(this,&ProjectLevel::saveSetting));
			break;
			case 14://load
				getFile(L"Select a file to load",new sgfMethodDelegate<ProjectLevel,const char*>(this,&ProjectLevel::loadSetting));
			break;
			}
		break;
		case EGET_FILE_SELECTED:
			dialog=(IGUIFileOpenDialog*)caller;
			wcstombs(openFileName,dialog->getFileName(),1024);//who created this function with such a fucking name?
			tempStr=openFileName;
			emgr->getCore()->getFileSystem()->changeWorkingDirectoryTo(setting->workingDir.c_str());//a fix for the openFileDialog
			getFileDelegate->invoke(tempStr);
			getFileDelegate=0;
		break;
		case EGET_FILE_CHOOSE_DIALOG_CANCELLED:
			emgr->getCore()->getFileSystem()->changeWorkingDirectoryTo(setting->workingDir.c_str());//a fix for the openFileDialog
			getFileDelegate->invoke(null);
			getFileDelegate=0;
		break;
		case EGET_BUTTON_CLICKED:		
			if(caller->getID()==999)//toolbar buttons
			{
				if(currentTool)
				{
					//printf("Deactivate %s\n",currentTool->image);
					currentTool->onDeactivate();
					currentTool->button->setPressed(false);
				}
				ITool* tool=(ITool*)getData(caller);
				//printf("Activate %s\n",tool->image);
				if(tool->button->isPressed())
				{
					tool->onActivate();
					currentTool=tool;
				}
				else
				{
					currentTool=0;
					if(!(tool->toggle))//one shot tool
						tool->onActivate();
				}
			}
			if(caller->getParent()==textEditor)
			{
				IGUIEditBox* editbox=(IGUIEditBox*)getData(textEditor);
				wcstombs(editorBuffer,editbox->getText(),800);//Don't write all your game script here . 800 chars only
				if(caller->getID()==1)//save
					getTextDelegate->invoke(SGetText(editorBuffer,1));
				if(caller->getID()==2)//cancel
					getTextDelegate->invoke(SGetText(0,2));
				textEditor->remove();
				getTextDelegate=NULL;//we are done here
				setUseKeyboard(true);//re-enable keyboard shortcuts
			}
			if((caller->getParent()==propertyWindow)&&selectedNode)
			{
				if(caller->getID()==1000)//apply
				{
					IGUIEditBox* editBox=(IGUIEditBox*)(propertyWindow->getElementFromId(8));
					static char scriptBuffer[800];
					wcstombs(scriptBuffer,editBox->getText(),800);
					setData(selectedNode,scriptBuffer);
				}
			}
			if(caller->getParent()==projectWindow)
			{
				IGUIEditBox* editbox;
				switch(caller->getID())
				{
				case 1:
					getFile(L"Select working directory",new sgfMethodDelegate<ProjectLevel,const char*>(this,&ProjectLevel::setWorkingDir));
				break;
				case 2:
					getFile(L"Select mesh directory",new sgfMethodDelegate<ProjectLevel,const char*>(this,&ProjectLevel::setMeshPath));
				break;
				case 3:
					getFile(L"Select texture directory",new sgfMethodDelegate<ProjectLevel,const char*>(this,&ProjectLevel::setTexturePath));
				break;
				case 4:
					editbox=(IGUIEditBox*)(projectWindow->getElementFromId(1));
					wcstombs(editorBuffer,editbox->getText(),800);
					setting->workingDir=editorBuffer;
					editbox=(IGUIEditBox*)(projectWindow->getElementFromId(2));
					wcstombs(editorBuffer,editbox->getText(),800);
					setting->meshPath=editorBuffer;
					editbox=(IGUIEditBox*)(projectWindow->getElementFromId(3));
					wcstombs(editorBuffer,editbox->getText(),800);
					setting->texturePath=editorBuffer;
					projectWindow->remove();
				break;
				}
			}
		break;
		case EGET_LISTBOX_CHANGED:
			if(caller==nodeList)
			{
				int id=nodeList->getSelected();
				ISceneNode* node=emgr->getCore()->getGraphicDevice()->getSceneManager()->getSceneNodeFromId(id);
				setSelectedNode(node);
				env->removeFocus(nodeList);
			}
		break;
		case EGDT_WINDOW_CLOSE:
			if(caller==textEditor)
			{
				getTextDelegate->invoke(SGetText(0,0));
				getTextDelegate=NULL;
			}
			break;
		case EGET_ELEMENT_FOCUSED:
			if((caller==propertyWindow)||(caller->getParent()==propertyWindow))
			{
				updateProperties();
				setUseKeyboard(false);
			}
			break;
		case EGET_ELEMENT_FOCUS_LOST:
			if((caller==propertyWindow)||(caller->getParent()==propertyWindow))
			{
				if(other)
				{
					if((other!=propertyWindow)&&(other->getParent()!=propertyWindow))
					{
						setUseKeyboard(true);
					}
				}
				else
					setUseKeyboard(true);
			}
		break;
		case EGET_EDITBOX_ENTER:
			if((caller->getParent()==propertyWindow)&&(selectedNode))
			{
				IGUIEditBox* editBox=(IGUIEditBox*)caller;
				const wchar_t* text=editBox->getText();
				//updateProperties();
				switch(caller->getID())
				{
				case 1:
					att->setAttribute("Name",text);
				break;
				case 2:
					att->setAttribute("Position",text);
				break;
				case 3:
					att->setAttribute("Rotation",text);
				break;
				case 4:
					att->setAttribute("Scale",text);
				break;
				}
				selectedNode->deserializeAttributes(att);
				if(caller->getID()==1)
					updateNodeList();
			}
		break;
		case EGET_CHECKBOX_CHANGED:
			if((caller->getParent()==propertyWindow)&&(selectedNode))
			{
				IGUICheckBox* checkBox=(IGUICheckBox*)caller;
				switch(caller->getID())
				{
				case 5:
					selectedNode->setMaterialFlag(EMF_FOG_ENABLE,checkBox->isChecked());
				break;
				case 6:
					selectedNode->setVisible(checkBox->isChecked());
				break;
				case 7:
					selectedNode->setMaterialFlag(EMF_LIGHTING,checkBox->isChecked());
				break;
				}
			}
	}
}
void ProjectLevel::updateProperties()
{
	static IGUIEditBox* name=(IGUIEditBox*)propertyWindow->getElementFromId(1);
	static IGUIEditBox* position=(IGUIEditBox*)propertyWindow->getElementFromId(2);
	static IGUIEditBox* rotation=(IGUIEditBox*)propertyWindow->getElementFromId(3);
	static IGUIEditBox* scale=(IGUIEditBox*)propertyWindow->getElementFromId(4);
	static IGUICheckBox* fog=(IGUICheckBox*)propertyWindow->getElementFromId(5);
	static IGUICheckBox* visible=(IGUICheckBox*)propertyWindow->getElementFromId(6);
	static IGUICheckBox* lighting=(IGUICheckBox*)propertyWindow->getElementFromId(7);
	if(selectedNode)
	{
		att->clear();
		selectedNode->serializeAttributes(att);
		name->setText(att->getAttributeAsStringW("Name").c_str());
		position->setText(att->getAttributeAsStringW("Position").c_str());
		rotation->setText(att->getAttributeAsStringW("Rotation").c_str());
		scale->setText(att->getAttributeAsStringW("Scale").c_str());
		if(selectedNode->getMaterialCount())
		{
			SMaterial material=selectedNode->getMaterial(0);
			fog->setChecked(material.FogEnable);
			lighting->setChecked(material.Lighting);
		}
		visible->setChecked(selectedNode->isVisible());
	}
}
void ProjectLevel::createGUI()
{
	IGUIEnvironment* env=emgr->getCore()->getGraphicDevice()->getGUIEnvironment();
	
	env->loadGUI("resources/ui/project.gui");
	projectWindow=(IGUIWindow*)(env->getRootGUIElement()->getElementFromId(9999));
	projectWindow->grab();
	projectWindow->remove();
	env->loadGUI("resources/ui/property.gui");
	propertyWindow=(IGUIWindow*)(env->getRootGUIElement()->getElementFromId(9999));
	propertyWindow->getCloseButton()->remove();
	IGUIEditBox* scriptBox=(IGUIEditBox*)(propertyWindow->getElementFromId(8));
	scriptBox->setTextAlignment(EGUIA_UPPERLEFT,EGUIA_UPPERLEFT);
	
	IGUIContextMenu* menu=env->addMenu();
	
	s32 item=menu->addItem(L"File",-1,true,true);
	IGUIContextMenu* sub=menu->getSubMenu(item);
	sub->addItem(L"New",1);
	sub->addItem(L"Open",2);
	sub->addItem(L"Save",3);
	sub->addItem(L"Save as...",4);
	sub->addItem(L"Exit",5);

	item=menu->addItem(L"Project",-1,true,true);
	sub=menu->getSubMenu(item);
	sub->addItem(L"Edit",11);
	sub->addItem(L"Save",12);
	sub->addItem(L"Save as...",13);
	sub->addItem(L"Load",14);

	menu->addItem(L"Help",-1,true,true);

	toolbar=env->addToolBar();
	addTool(new SelectTool);
	addTool(new MoveTool);
	addTool(new RotateTool);
	addTool(new ScaleTool);
	addTool(new RunNodeScript);
	addTool(new CloneTool);
	addTool(new LoadMeshTool);
	addTool(new CreateAnimatedMeshTool);

	lockStatus=env->addStaticText(L"X Y",rect<s32>(1,50,40,70),true);
	nodeList=env->addListBox(rect<s32>(1,72,80,200),0,-1,true);
	meshList=env->addListBox(rect<s32>(1,202,200,402),0,-1,true);
		
	textEditor=env->addWindow  (  rect<s32>(50,50,350,550),
		true,
		L"Editor");
	textEditor->grab();
	IGUIEditBox* editbox=env->addEditBox(0,rect<s32>(1,20,300,450),true,textEditor,999);
	editbox->setMultiLine(true);
	//editbox->setAutoScroll(true);
	editbox->setTextAlignment(EGUIA_UPPERLEFT,EGUIA_UPPERLEFT);
	editbox->setMax(800);
	env->addButton(rect<s32>(1,450,100,500),textEditor,1,L"Save");
	env->addButton(rect<s32>(101,450,200,500),textEditor,2,L"Cancel");
	setData(textEditor,editbox);
	textEditor->remove();
}

void ProjectLevel::addTool(ITool* tool)
{
	IVideoDriver* vd=emgr->getCore()->getGraphicDevice()->getVideoDriver();
	emgr->addEntity(tool);
	IGUIButton* button=toolbar->addButton(999,0,tool->toolTipText,vd->getTexture(tool->image),0,tool->toggle,true);
	tool->button=button;
	setData(button,tool);
}
void ProjectLevel::updateNodeList()
{
	nodeList->clear();
	ISceneManager* smgr=emgr->getCore()->getGraphicDevice()->getSceneManager();
	list<ISceneNode*> children=smgr->getRootSceneNode()->getChildren();
	list<ISceneNode*>::Iterator i;
	for(i=children.begin();i!=children.end();i++)
	{
		if((*i)->isDebugObject())
			continue;
		(*i)->setID(-1);//reset id
		const char* name=0;
		if(!strlen((*i)->getName()))
		{
			name=smgr->getSceneNodeTypeName((*i)->getType());
			if(name)
			{
				mbstowcs(wideCharBuffer,name,800);
				(*i)->setID(nodeList->addItem(wideCharBuffer));
			}
		}
		else
		{
			name=(*i)->getName();
			mbstowcs(wideCharBuffer,name,800);
			(*i)->setID(nodeList->addItem(wideCharBuffer));
		}
		if(*i==selectedNode)
			nodeList->setSelected((*i)->getID());		
	}
}

void* ProjectLevel::getData(irr::gui::IGUIElement* element) const
{
	std::map<irr::gui::IGUIElement*,void*>::const_iterator i;
	i=guiMap.find(element);
	if(i==guiMap.end())//not found
	{
		return NULL;
	}
	return i->second;
}

void ProjectLevel::setData(irr::gui::IGUIElement* element,void* data)
{
	std::map<irr::gui::IGUIElement*,void*>::iterator i;
	i=guiMap.find(element);
	if(i==guiMap.end())//not found
	{
		guiMap.insert(std::make_pair<irr::gui::IGUIElement*,void*>(element,data));
	}
	else
	{
		i->second=data;
	}
}

void ProjectLevel::setData(irr::scene::ISceneNode* node,const char* data)
{
	std::map<ISceneNode*,std::string>::iterator i;
	i=sceneNodeData.find(node);
	if(i==sceneNodeData.end())//not found
	{
		sceneNodeData.insert(std::make_pair<ISceneNode*,std::string>(node,data));
	}
	else
	{
		i->second=data;
	}
}
const char* ProjectLevel::getData(irr::scene::ISceneNode* node) const
{
	std::map<ISceneNode*,std::string>::const_iterator i;
	i=sceneNodeData.find(node);
	if(i==sceneNodeData.end())//not found
	{
		return NULL;
	}
	return i->second.c_str();
}

irr::scene::ISceneNode* ProjectLevel::getSelectedNode() const
{
	return selectedNode;
}
void ProjectLevel::setUseKeyboard(bool value)
{
	//camera->setInputReceiverEnabled(value);
	useKeyboard=value;
}
void ProjectLevel::setSelectedNode(irr::scene::ISceneNode* node)
{
	if(selectedNode)
		selectedNode->setDebugDataVisible(EDS_OFF);
	if((selectedNode!=node)&&node)//select a new node
	{
		static IGUIEditBox* script=(IGUIEditBox*)propertyWindow->getElementFromId(8);
		const wchar_t* scr=0;
		if(getData(node))
		{
			mbstowcs(wideCharBuffer,getData(node),800);
			scr=wideCharBuffer;
		}
		script->setText(scr);
	}	
	selectedNode=node;
	if(selectedNode)
	{
		selectedNode->setDebugDataVisible(EDS_BBOX);
		updateProperties();
	}	
}

irr::io::IAttributes*  ProjectLevel::createUserData (ISceneNode *node)
{
	IFileSystem* fs=emgr->getCore()->getFileSystem();
	const char* nodeData=getData(node);
	static char encodeBuffer[1600];
	encodeBuffer[0]='\0';
	if(nodeData)
	{
		CyoEncode::Base16Encode(encodeBuffer,nodeData,strlen(nodeData));
		IAttributes* att=fs->createEmptyAttributes(emgr->getCore()->getGraphicDevice()->getVideoDriver());
		att->addString("Script",encodeBuffer);		
		return att;
	}
	return 0;
}

void  ProjectLevel::OnReadUserData (ISceneNode *node, IAttributes *userData)
{
	static char decodeBuffer[800];
	static char stringBuffer[1600];
	decodeBuffer[0]='\0';
	userData->getAttributeAsString("Script",stringBuffer);
	CyoDecode::Base16Decode(decodeBuffer,stringBuffer,strlen(stringBuffer));
	setData(node,decodeBuffer);
}

bool ProjectLevel::promptText(const wchar_t* msg,const wchar_t* defaultText,sgfDelegate<SGetText>* getTextDelegate)
{
	setUseKeyboard(false);//disable keyboard shortcut
	if(this->getTextDelegate.getPtr())//if we're busy
	{
		getFileDelegate->addRef();
		getFileDelegate->decRef();
		return false;//bye bye
	}
	this->getTextDelegate=getTextDelegate;
	IGUIEnvironment* env=emgr->getCore()->getGraphicDevice()->getGUIEnvironment();
	IGUIElement* root=env->getRootGUIElement();
	root->addChild(textEditor);
	root->bringToFront(textEditor);
	textEditor->setText(msg);
	IGUIEditBox* editbox=(IGUIEditBox*)getData(textEditor);
	editbox->setText(defaultText);
	env->getRootGUIElement()->bringToFront(textEditor);
	return true;
}

void ProjectLevel::getFile(const wchar_t* msg,sgfDelegate<const char*>* getFileDelegate)
{
	if(this->getFileDelegate.getPtr())
	{
		getFileDelegate->addRef();
		getFileDelegate->decRef();
		return;
	}
	this->getFileDelegate=getFileDelegate;
	IGUIEnvironment* env=emgr->getCore()->getGraphicDevice()->getGUIEnvironment();
	env->addFileOpenDialog(msg,true);
	
	if(currentTool)//disable tools
	{
		currentTool->onDeactivate();
		currentTool->button->setPressed(false);
		currentTool=0;
	}
}

void ProjectLevel::editNodeData(SGetText& args)
{
	if(args.choice==1)//save
	{
		setData(selectedNode,args.text);
	}
	editingScript=false;
}

void ProjectLevel::load(const char*& fileName)
{
	if(!fileName)
		return;
	sceneNodeData.clear();
	ISceneManager* smgr=emgr->getCore()->getGraphicDevice()->getSceneManager();
	//printf("%s \n",emgr->getCore()->getFileSystem()->getWorkingDirectory());
	smgr->clear();
	createScene();
	selectedNode=0;
	smgr->loadScene(fileName,this);
	updateNodeList();
	setSelectedNode(smgr->getRootSceneNode());
	this->fileName=fileName;
}

void ProjectLevel::save(const char*& fileName)
{
	if(!fileName)
		return;
	if(selectedNode)
		selectedNode->setDebugDataVisible(EDS_OFF);//turn off debug data
	ISceneManager* smgr=emgr->getCore()->getGraphicDevice()->getSceneManager();
	smgr->saveScene(fileName,this);
	this->fileName=fileName;
}

void ProjectLevel::saveSetting(const char*& filename)
{
	if(filename)
		setting->write(filename);
}

void ProjectLevel::loadSetting(const char*& filename)
{
	if(filename)
		setting->read(filename);
}

stringc dirFromFilePath(const char* fileName)
{
	stringc ret;
	ret=fileName;
	int pos=ret.findLastChar("\\/",2);
	return ret.subString(0,pos);
}

void ProjectLevel::setWorkingDir(const char*&fileName)
{
	if(fileName)
	{
		//setting->workingDir=dirFromFilePath(fileName);
		IGUIEditBox* editbox=(IGUIEditBox*)(projectWindow->getElementFromId(1));
		mbstowcs(wideCharBuffer,dirFromFilePath(fileName).c_str(),800);
		editbox->setText(wideCharBuffer);		
	}
}
void ProjectLevel::setMeshPath(const char*&fileName)
{
	if(fileName)
	{
		//setting->meshPath=dirFromFilePath(fileName);
		IGUIEditBox* editbox=(IGUIEditBox*)(projectWindow->getElementFromId(2));
		mbstowcs(wideCharBuffer,dirFromFilePath(fileName).c_str(),800);
		editbox->setText(wideCharBuffer);		
	}
}
void ProjectLevel::setTexturePath(const char*&fileName)
{
	if(fileName)
	{
		//setting->texturePath=dirFromFilePath(fileName);
		IGUIEditBox* editbox=(IGUIEditBox*)(projectWindow->getElementFromId(3));
		mbstowcs(wideCharBuffer,dirFromFilePath(fileName).c_str(),800);
		editbox->setText(wideCharBuffer);				
	}
}