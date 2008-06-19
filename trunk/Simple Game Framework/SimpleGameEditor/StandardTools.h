#ifndef _STANDARD_TOOLS_H_
#define _STANDARD_TOOLS_H_

#include "ITool.h"
#include "ProjectLevel.h"

using namespace irr;
using namespace gui;
using namespace scene;

class MoveTool:public ITool
{
public:
	MoveTool()
		:ITool()
	{
		image="resources/ui/move.png";
		toolTipText=L"Move a scene node";
	}
	void onActivate()
	{
		registerMouse();
		ldown=false;
		rdown=false;
	}
	void onDeactivate()
	{
		unregisterMouse();
	}
	void onMouse(SMouseEvent& args)
	{
		
		switch(args.mouseEvent)
		{
		case EMIE_LMOUSE_PRESSED_DOWN:
			ldown=true;
			oldX=args.x;
			oldY=args.y;
			break;
		case EMIE_RMOUSE_PRESSED_DOWN:
			rdown=true;
			oldX=args.x;
			oldY=args.y;
			break;
		case EMIE_LMOUSE_LEFT_UP:
			ldown=false;
			break;
		case EMIE_RMOUSE_LEFT_UP:
			rdown=false;
			break;
		case EMIE_MOUSE_MOVED:
			core::vector3df moveVect;
			if(ldown)
			{
				moveVect=core::vector3df((f32)(args.x-oldX),0,(f32)(oldY-args.y));
			}
			if(rdown)
			{
				moveVect=core::vector3df(0,(f32)(oldY-args.y),0);
			}
			moveVect=moveVect/800;
			core::matrix4 m;
			core::vector3df camRot=manager->getCore()->getGraphicDevice()->getSceneManager()->getActiveCamera()->getRotation();
			camRot.X=0;
			m.setRotationDegrees(camRot);
			m.transformVect(moveVect);
			ProjectLevel* level=(ProjectLevel*)(manager->getCurrentLevel());
			ISceneNode* node=level->getSelectedNode();
			if(node&&(node!=manager->getCore()->getGraphicDevice()->getSceneManager()->getRootSceneNode()))
			{
				moveVect=moveVect*node->getAbsolutePosition().getDistanceFrom(manager->getCore()->getGraphicDevice()->getSceneManager()->getActiveCamera()->getPosition());
				node->setPosition(node->getPosition()+moveVect);
				node->updateAbsolutePosition();
			}
			oldX=args.x;
			oldY=args.y;
			break;
		}
	}
protected:
	bool ldown;
	bool rdown;
	int oldX;
	int oldY;
};

class SelectTool:public ITool
{
public:
	SelectTool()
		:ITool()		
	{
		image="resources/ui/select.bmp";
		toolTipText=L"Select a scene node";
	}

	void onActivate()
	{
		registerMouse();
	}
	void onDeactivate()
	{
		unregisterMouse();
	}
	void onMouse(SMouseEvent& args)
	{
		if(args.mouseEvent==EMIE_LMOUSE_PRESSED_DOWN)
		{
			ProjectLevel* level=(ProjectLevel*)(manager->getCurrentLevel());
			//IGUIElement* el=manager->getCore()->getGraphicDevice()->getGUIEnvironment()->getRootGUIElement()->getElementFromPoint(position2d<s32>(args.x,args.y));
			//if(el)
			//	return;
			if(manager->getCore()->getInputManager()->isKeyPressed(KEY_CONTROL))//hold control and click to select the root sceneNode
			{
				level->setSelectedNode(manager->getCore()->getGraphicDevice()->getSceneManager()->getRootSceneNode());
				return;
			}
			ISceneNode* node=manager->getCore()->getGraphicDevice()->getSceneManager()->getSceneCollisionManager()->getSceneNodeFromScreenCoordinatesBB(
				core::position2d<s32>(args.x,args.y),0,true);
			if(node)
			{
				level->setSelectedNode(node);
				level->updateNodeList();
			}
		}
	}

};

class RotateTool:public ITool
{
public:
	RotateTool()
		:ITool()
	{
		image="resources/ui/rotate.png";
		toolTipText=L"Rotate a scene node";
	}
	void onActivate()
	{
		registerMouse();
		ldown=false;
		rdown=false;
	}
	void onDeactivate()
	{
		unregisterMouse();
	}
	void onMouse(SMouseEvent& args)
	{
		
		switch(args.mouseEvent)
		{
		case EMIE_LMOUSE_PRESSED_DOWN:
			ldown=true;
			oldX=args.x;
			oldY=args.y;
			break;
		case EMIE_RMOUSE_PRESSED_DOWN:
			rdown=true;
			oldX=args.x;
			oldY=args.y;
			break;
		case EMIE_LMOUSE_LEFT_UP:
			ldown=false;
			break;
		case EMIE_RMOUSE_LEFT_UP:
			rdown=false;
			break;
		case EMIE_MOUSE_MOVED:
			core::vector3df rotVect;
			if(ldown)
			{
				rotVect=core::vector3df(0,(f32)(args.x-oldX),(f32)(oldY-args.y));
			}
			if(rdown)
			{
				rotVect=core::vector3df((f32)(oldY-args.y),0,0);
			}
			ProjectLevel* level=(ProjectLevel*)(manager->getCurrentLevel());
			ISceneNode* node=level->getSelectedNode();
			if(node&&(node!=manager->getCore()->getGraphicDevice()->getSceneManager()->getRootSceneNode()))
			{
				node->setRotation(node->getRotation()+rotVect);
				node->updateAbsolutePosition();
			}
			oldX=args.x;
			oldY=args.y;
			break;
		}
	}
protected:
	bool ldown;
	bool rdown;
	int oldX;
	int oldY;
};

class ScaleTool:public ITool
{
public:
	ScaleTool()
		:ITool()
	{
		image="resources/ui/scale.png";
		toolTipText=L"Scale a scene node";
	}
	void onActivate()
	{
		registerMouse();
		ldown=false;
		rdown=false;
	}
	void onDeactivate()
	{
		unregisterMouse();
	}
	void onMouse(SMouseEvent& args)
	{
		
		switch(args.mouseEvent)
		{
		case EMIE_LMOUSE_PRESSED_DOWN:
			ldown=true;
			oldX=args.x;
			oldY=args.y;
			break;
		case EMIE_RMOUSE_PRESSED_DOWN:
			rdown=true;
			oldX=args.x;
			oldY=args.y;
			break;
		case EMIE_LMOUSE_LEFT_UP:
			ldown=false;
			break;
		case EMIE_RMOUSE_LEFT_UP:
			rdown=false;
			break;
		case EMIE_MOUSE_MOVED:
			core::vector3df scaleVect;
			if(ldown)
			{
				scaleVect=core::vector3df((f32)(args.x-oldX),0,(f32)(oldY-args.y));
			}
			if(rdown)
			{
				scaleVect=core::vector3df(0,(f32)(oldY-args.y),0);
			}
			scaleVect=scaleVect/800;
			core::matrix4 m;
			core::vector3df camRot=manager->getCore()->getGraphicDevice()->getSceneManager()->getActiveCamera()->getRotation();
			camRot.X=0;
			m.setRotationDegrees(camRot);
			m.transformVect(scaleVect);
			ProjectLevel* level=(ProjectLevel*)(manager->getCurrentLevel());
			ISceneNode* node=level->getSelectedNode();
			if(node&&(node!=manager->getCore()->getGraphicDevice()->getSceneManager()->getRootSceneNode()))
			{
				scaleVect=scaleVect*node->getAbsolutePosition().getDistanceFrom(manager->getCore()->getGraphicDevice()->getSceneManager()->getActiveCamera()->getPosition());
				node->setScale(node->getScale()+scaleVect);
				node->updateAbsolutePosition();
			}
			oldX=args.x;
			oldY=args.y;
			break;
		}
	}
protected:
	bool ldown;
	bool rdown;
	int oldX;
	int oldY;
};

class RunNodeScript:public ITool
{
public:
	RunNodeScript()
		:ITool()		
	{
		toggle=false;
		image="resources/ui/script.bmp";
		toolTipText=L"Run the script in a node";
	}

	void onActivate()
	{
		ProjectLevel* level=(ProjectLevel*)(manager->getCurrentLevel());
		ISceneNode* node=level->getSelectedNode();
		const char* script=level->getData(node);
		if(script)
			manager->getCore()->getScriptVM()->ExecuteString(script);
	}

};

class CloneTool:public ITool
{
public:
	CloneTool()
	{
		toggle=false;
		image="resources/ui/clone.png";
		toolTipText=L"Clone the selected node";
	}

	void onActivate()
	{
		ProjectLevel* level=(ProjectLevel*)(manager->getCurrentLevel());
		ISceneNode* node=level->getSelectedNode();
		if(node)
		{
			ISceneNode* newNode=node->clone();
			if(newNode)
			{
				newNode->setDebugDataVisible(EDS_OFF);
				level->updateNodeList();
			}
			if(newNode&&(level->getData(node)))
				level->setData(newNode,level->getData(node));
		}
	}

};

class LoadMeshTool:public ITool
{
public:
	LoadMeshTool()
	{
		toggle=false;
		image="resources/ui/clone.png";
		toolTipText=L"Load a new mesh";
	}

	void onActivate()
	{
		ProjectLevel* level=(ProjectLevel*)(manager->getCurrentLevel());
		manager->getCore()->getFileSystem()->changeWorkingDirectoryTo(level->setting->meshPath.c_str());
		level->getFile(L"Select a mesh to load",new sgfMethodDelegate<LoadMeshTool,const char*>(this,&LoadMeshTool::loadMesh));
		//manager->getCore()->getFileSystem()->changeWorkingDirectoryTo(level->setting->workingDir.c_str());
	}

	void loadMesh(const char*& file)
	{
		if(!file)
			return;
		ProjectLevel* level=(ProjectLevel*)(manager->getCurrentLevel());
		stringc relativePath(file);
		int pos=relativePath.find(level->setting->meshPath.c_str());
		int meshPathLen=level->setting->workingDir.size();
		if(pos>=0)
		{
			relativePath=relativePath.subString(meshPathLen+1,relativePath.size()-meshPathLen-1);
		}
		relativePath.replace('\\','/');
		ISceneManager* smgr=manager->getCore()->getGraphicDevice()->getSceneManager();
		if(!smgr->getMeshCache()->isMeshLoaded(relativePath.c_str()))//if not loaded
		{
			if(smgr->getMesh(relativePath.c_str()))//if loading is successful
			{
				static wchar_t wideCharBuffer[400];
				wideCharBuffer[0]='\0';
				mbstowcs(wideCharBuffer,relativePath.c_str(),400);
				level->meshList->addItem(wideCharBuffer);
			}
		}
	}
};

class CreateAnimatedMeshTool:public ITool
{
public:
	CreateAnimatedMeshTool()
	{
		toggle=false;
		image="resources/ui/clone.png";
		toolTipText=L"Create an animated mesh";
	}

	void onActivate()
	{
		ProjectLevel* level=(ProjectLevel*)(manager->getCurrentLevel());
		if(level->meshList->getSelected()==-1)
			return;
		const wchar_t* mesh=level->meshList->getListItem(level->meshList->getSelected());
		static char charBuffer[400];
		charBuffer[0]='\0';
		wcstombs(charBuffer,mesh,400);

		vector3df trans(0,0,100);
		vector3df pos=level->camera->getAbsolutePosition();
		matrix4 m;
		m.setRotationDegrees(level->camera->getRotation());
		m.transformVect(trans);
		pos+=trans;//put the mesh in front of camera
		ISceneManager* smgr=manager->getCore()->getGraphicDevice()->getSceneManager();
		smgr->addAnimatedMeshSceneNode(smgr->getMesh(charBuffer),0,-1,pos);
		level->updateNodeList();
	}
};
#endif