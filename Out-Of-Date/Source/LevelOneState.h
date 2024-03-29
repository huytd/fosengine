#ifndef LEVELONESTATE_H_
#define LEVELONESTATE_H_

#include <IAnimatedMeshSceneNode.h>
#include <ILightSceneNode.h>
#include <ICameraSceneNode.h>
#include <IGUIEnvironment.h>
#include <IGUIButton.h>
#include "ThayCung.h"
#include "Terrain.h"
#include "CCharacter.h"
#include "HUD.h"
#include "SimpleCameraManager.h"
#include "Map.h"

#include "CGUIIconSlot.h"
#include "CGUIIcon.h"
#include "CGUISlotWindow.h"
#include "CGUIBringUpSlotWindowButton.h"
class LevelOneState : public State<Game, irr::SEvent>
{

private:
	irr::scene::ILightSceneNode* light;

	//Main character
	ThayCung* thaycung;
	//Enemy
	//CCharacter* ninja;
	//Game's terrain
	Terrain* terrain;

	//Game Heads-Up Display
	HUD *hud;

	//Game Mini-Map.
	Map *map;
	core::vector3df curPos;

	/// Selector to get collition
	scene::ITriangleSelector* selector;

	scene::SMeshBufferLightMap buffer;
	scene::SMesh mesh;

	core::triangle3df collisionTriangle;
	core::vector3df collisionPoint;
	core::vector3df targetPoint;


	SimpleCameraManager *cameraManager; 

	//--------------HUD Button Group--------------------
	irr::gui::IGUIButton *btn1;
	irr::gui::IGUIButton *btn2;
	irr::gui::IGUIButton *btn3;
	//--------------================--------------------
public:

	LevelOneState();

	virtual ~LevelOneState();

	virtual void onEnter(Game* game);

	virtual void onLeave(Game* game);

	virtual const bool onEvent(Game* game, const irr::SEvent& event);

	virtual void onUpdate(Game* game);
};

LevelOneState::LevelOneState() :
light(0), thaycung(0), terrain(0), 	hud(0), selector(0), btn1(0), btn2(0), btn3(0)
{

}

LevelOneState::~LevelOneState()
{
	if (light)
	{
		light->remove();
		light->drop();
	}

	if(thaycung)
		delete thaycung;

	if(terrain)
		delete terrain;

	if (hud)
		delete hud;

	if(selector)
	{		
		selector->drop();	
	}

	if (btn1)
	{
		btn1->remove();
		btn1->drop();
	}

	if (btn2)
	{
		btn2->remove();
		btn2->drop();
	}

	if (btn3)
	{
		btn3->remove();
		btn3->drop();
	}
}

/*
 * Some method for new 3rd camera system 
 */

/// Move the scenenode
void moveN(irr::scene::ISceneNode *node, irr::core::vector3df vel) 
{
	irr::core::matrix4 m;
	m.setRotationDegrees(node->getRotation());
	m.transformVect(vel);
	node->setPosition(node->getPosition() + vel);
} 

/// Make the camera look like 3rd camera
void makeCockpit(irr::scene::ICameraSceneNode* camera,
				 irr::scene::ISceneNode* node, 
				 irr::core::vector3df offset)
{
	irr::core::matrix4 m;
	m.setRotationDegrees(node->getRotation());

	irr::core::vector3df frv = irr::core::vector3df (0.0f, 0.0f, 1.0f);
	m.transformVect(frv);

	irr::core::vector3df upv = irr::core::vector3df (0.0f, 1.0f, 0.0f);
	m.transformVect(upv);

	m.transformVect(offset);

	camera->setPosition(node->getPosition() + offset); 
	camera->setUpVector(upv); 
	camera->setTarget(node->getPosition() + frv);
} 

/// Rotate the scenenode
void rotateN(irr::scene::ISceneNode *node, irr::core::vector3df rot)
{
	irr::core::matrix4 m;
	m.setRotationDegrees(node->getRotation());
	irr::core::matrix4 n;
	n.setRotationDegrees(rot);
	m *= n;
	node->setRotation( m.getRotationDegrees() );
	node->updateAbsolutePosition();
}

// Method for GUI Button

gui::IGUIButton* createGUIButton(Game* game, video::ITexture* tex, core::position2df pos, wchar_t* tooltiptext=0)
{
	IGUIButton* bt;
	bt = game->getGuiEnvironment()->addButton(irr::core::rect<irr::s32>(pos.X, pos.Y, pos.X + 36, pos.Y + 36),
		0,-1,0,tooltiptext);
	bt->setImage(tex);
	bt->setUseAlphaChannel(true);
	bt->grab();
	return bt;
}

// Set all gui elements transparent
void transGUI(Game* game, bool isTrans)
{
	/*if (isTrans == true)
	{
		for (u32 i=0; i<EGDC_COUNT; ++i)
		{
			/*SColor col = game->getGuiEnvironment()->getSkin()->getColor((EGUI_DEFAULT_COLOR)i);
			//col.setAlpha(0);
			//game->getGuiEnvironment()->getSkin()->setColor((EGUI_DEFAULT_COLOR)i, col);
			// Only tooltip backgound is not transparent
			SColor cols = game->getGuiEnvironment()->getSkin()->getColor(EGDC_TOOLTIP_BACKGROUND);
			col.setAlpha(255);
			col.setRed(150);
			col.setGreen(203);
			col.setBlue(114);
			game->getGuiEnvironment()->getSkin()->setColor(EGDC_TOOLTIP_BACKGROUND, col);*/
		/*}
	}
	else if (isTrans == false)
	{
		for (u32 i=0; i<EGDC_COUNT; ++i)
		{
			SColor col = game->getGuiEnvironment()->getSkin()->getColor((EGUI_DEFAULT_COLOR)i);
			col.setAlpha(150);
			game->getGuiEnvironment()->getSkin()->setColor((EGUI_DEFAULT_COLOR)i, col);
		}
	}*/
}


void LevelOneState::onEnter(Game* game)
{
	// setup skydom
	game->getSceneManager()->addSkyDomeSceneNode(game->getVideoDriver()->getTexture("media/images/skydome.jpg"),16,16,1.0f,1.0f);

	//setup terrain
	terrain = new Terrain(game);

	//Setup thaycung character
	thaycung = new ThayCung(game->getDevice(),game->getSceneManager(),game->getVideoDriver(),"Media\\Model\\thaycung\\thaycung.b3d");
	video::ITexture *thaycungtexture = game->getVideoDriver()->getTexture("Media\\Model\\thaycung\\thaycung.png");

	//thaycung->getNode()->setMaterialType(video::E_MATERIAL_TYPE::EMT_TRANSPARENT_ALPHA_CHANNEL);
	thaycung->getNode()->setMaterialTexture(0, thaycungtexture);

	thaycung->getNode()->setScale(core::vector3df(0.02f, 0.02f, 0.02f));

	thaycung->setPosition(irr::core::vector3df(1275.0f, 25.0f, 2675.0f));
	targetPoint = irr::core::vector3df(1275.0f, 60.0f, 2675.0f);
	
	game->getGuiEnvironment()->addStaticText(L"Nhấn phím ESC để quay lại menu.",rect<s32>(5, 5, 200, 100),false,true,0,-1,false); 
	//Get Terrain Size.
	core::vector3df centerPos = terrain->getTerrain()->getTerrainCenter();
    //Setup Map.
	this->map = new Map(irr::core::rect<irr::s32>(SX(100),SY(100),SX(100),SY(100)),game->getGuiEnvironment(),game->getGuiEnvironment()->getRootGUIElement());
	this->map->setCharacterTexture("media/images/map/character.jpg");
	this->map->setMapTexture("media/images/map/LevelOneState.jpg");
	this->map->setWorldSize(centerPos.X * 2,centerPos.Z * 2);
	curPos = thaycung->getPosition();
	//Setup HUD
	this->hud = new HUD(game);
    
	cameraManager = new SimpleCameraManager(thaycung->getNode(), game->getSceneManager());
	game->getSceneManager()->setActiveCamera( cameraManager->getCam() ); 
	
	selector = game->getSceneManager()->createTerrainTriangleSelector(terrain->getTerrain(), 0);
	terrain->getTerrain()->setTriangleSelector(selector);

	selector->drop(); selector = 0;

	/*createBtn(game, *btn1, game->getVideoDriver()->getTexture("media/images/gui/hud/btn1.png"), position2df(52,565),L"Nút này làm gì hổng bit");
	createBtn(game, *btn2, game->getVideoDriver()->getTexture("media/images/gui/hud/btn2.png"), position2df(82,564),L"Ẩn/Hiện Minimap");
	createBtn(game, *btn3, game->getVideoDriver()->getTexture("media/images/gui/hud/btn3.png"), position2df(113,565),L"Chưa làm gì cả");*/
	btn1 = createGUIButton(game, game->getVideoDriver()->getTexture("media/images/gui/hud/btn1.png"), position2df(52,565),0);
	btn2 = createGUIButton(game, game->getVideoDriver()->getTexture("media/images/gui/hud/btn2.png"), position2df(82,564),L"Ẩn/Hiện bản đồ");
	btn3 = createGUIButton(game, game->getVideoDriver()->getTexture("media/images/gui/hud/btn3.png"), position2df(113,565),0);
	//transGUI(game, true);

	CGUISlotWindow* window = new CGUISlotWindow(game->getDevice(), 
													game->getGuiEnvironment()->getRootGUIElement(),
													-1, 
													rect<s32>(25, 25, 300, 200));
		window->drop();

		//! create a button to show/hide the window
		IGUIButton* button = window->createBringUpButton(rect<s32>(10,210,110,242));
		button->drop();

		//! load some very beautiful textures
		ITexture* slotTex = game->getGuiEnvironment()->getVideoDriver()->getTexture("hud\\slot.png");
		ITexture* iconTex = game->getGuiEnvironment()->getVideoDriver()->getTexture("hud\\icon.png");
		ITexture* iconTex2 = game->getGuiEnvironment()->getVideoDriver()->getTexture("hud\\icon2.png");

		//! create an array of slots in the window
		core::array<IGUIElement*> slotArray = window->addSlotArray(core::rect<s32>(0,0,32,32), slotTex, game->getGuiEnvironment()->getRootGUIElement(), -1,
		core::position2d<s32>(10,30),core::dimension2d<s32>(6,3),core::dimension2d<s32>(2,2));

		//! create an icon
		CGUIIcon *icon = new CGUIIcon(game->getGuiEnvironment(), game->getGuiEnvironment()->getRootGUIElement(), -1, rect<s32>(0,0,32,32));
		icon->drop();
		icon->setImage(iconTex);

		//! create another icon
		CGUIIcon *icon2 = new CGUIIcon(game->getGuiEnvironment(), game->getGuiEnvironment()->getRootGUIElement(), -1, rect<s32>(0,0,32,32));
		icon2->drop();
		icon2->setImage(iconTex2);

		//! let the icons know about the slots
		icon->setUsableSlotArray(&slotArray);
		icon2->setUsableSlotArray(&slotArray); 
}

void LevelOneState::onLeave(Game* game)
{
	if (light)
	{
		this->light->remove();
		this->light->drop();
		this->light = 0;
	}

	game->getGuiEnvironment()->clear();

	if(thaycung)
	{
		delete thaycung;
		this->thaycung = 0;
	}

	if(terrain)
	{
		delete terrain;
		this->terrain = 0;
	}

	if (hud)
	{
		delete hud;
		hud = 0;
	}

	if(selector)
	{		
		selector->drop();
		selector = 0;
	}

	if (btn1)
	{
		btn1->remove();
		btn1->drop();
		btn1=0;
	}

	if (btn2)
	{
		btn2->remove();
		btn2->drop();
		btn2=0;
	}

	if (btn3)
	{
		btn3->remove();
		btn3->drop();
		btn3=0;
	}

	//transGUI(game, false);

}


const bool LevelOneState::onEvent(Game* game, const irr::SEvent& event)
{
	switch(event.EventType)
	{
	case irr::EET_GUI_EVENT:
		if (event.GUIEvent.EventType == irr::gui::EGET_BUTTON_CLICKED)
		{				
			if (event.GUIEvent.Caller == this->btn2)
			{
				if (map->getIsExpand())
					{
						map->setExpand(false);
					}
					else
					{
						map->setExpand(true);
					}
				return true;
			}
		}
		break;

	case irr::EET_MOUSE_INPUT_EVENT:
		if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
		{				
			if(event.MouseInput.Event == irr::EMIE_LMOUSE_PRESSED_DOWN)
			{
				core::line3df ray(game->getSceneManager()->getSceneCollisionManager()->getRayFromScreenCoordinates(game->getDevice()->getCursorControl()->getPosition()));

				if(game->getSceneManager()->getSceneCollisionManager()->getCollisionPoint(ray, terrain->getTerrain()->getTriangleSelector(), collisionPoint, collisionTriangle))
				{
					targetPoint = collisionPoint;
					thaycung->run();
				}
			}
			else if(event.MouseInput.Event == irr::EMIE_RMOUSE_PRESSED_DOWN)
			{
				core::line3df ray(game->getSceneManager()->getSceneCollisionManager()->getRayFromScreenCoordinates(game->getDevice()->getCursorControl()->getPosition()));

				if(game->getSceneManager()->getSceneCollisionManager()->getCollisionPoint(ray, terrain->getTerrain()->getTriangleSelector(), collisionPoint, collisionTriangle))
				{
					thaycung->attack(ThayCung::MagicAttack);
					
				}
			}
			
		}
		break;


	case irr::EET_KEY_INPUT_EVENT:
		{
			if (!event.KeyInput.PressedDown)
			{
				if (event.KeyInput.Key == irr::KEY_ESCAPE)
				{
					game->setState(game->findGameState("intro"));
					return true;
				}
			}
			if (event.KeyInput.PressedDown)
			{
				if (event.KeyInput.Key == irr::KEY_UP)
				{
					moveN(thaycung->getNode(), vector3df(0,0,5));
					return true;
				}
				if (event.KeyInput.Key == irr::KEY_DOWN)
				{
					moveN(thaycung->getNode(), vector3df(0,0,-5));
					return true;
				}
				if (event.KeyInput.Key == irr::KEY_LEFT)
				{
					rotateN(thaycung->getNode(),vector3df(0,10,0));
					return true;
				}
				if (event.KeyInput.Key == irr::KEY_RIGHT)
				{
					rotateN(thaycung->getNode(),vector3df(0,-10,0));
					return true;
				}
				if (event.KeyInput.Key == irr::KEY_TAB)
				{
					if (map->getIsExpand())
					{
						map->setExpand(false);
					}
					else
					{
						map->setExpand(true);
					}
				}
			}
		}
		break;

	default: return false;
	}

	return false;
}




void LevelOneState::onUpdate(Game* game)
{
	this->hud->update(game);

	//Control main character
	thaycung->move(targetPoint);
	float oldHeight = thaycung->getPosition().Y;
	float newHeight = terrain->getTerrain()->getHeight(thaycung->getPosition().X, thaycung->getPosition().Z);
	
	//printf("%d : %d\n", game->getDevice()->getCursorControl()->getPosition().X,game->getDevice()->getCursorControl()->getPosition().Y);
	
	if( (abs(newHeight-thaycung->getPosition().Y) < 0.25))
	{
		thaycung->setPosition(core::vector3df(thaycung->getPosition().X, newHeight, thaycung->getPosition().Z));
		//printf("\n case 1");
	}
	else
	{
		if(newHeight-thaycung->getPosition().Y > 0)
		{
			thaycung->setPosition(core::vector3df(thaycung->getPosition().X,oldHeight + 0.125f , thaycung->getPosition().Z));
			//printf("\n case 2");
		}
		else if(newHeight-thaycung->getPosition().Y < 0)
		{
			//printf("\n case 3");
			thaycung->setPosition(core::vector3df(thaycung->getPosition().X,oldHeight - 0.125f , thaycung->getPosition().Z));
		}
			else 
			{
				//printf("\n case 4");	
				thaycung->setPosition(core::vector3df(thaycung->getPosition().X,oldHeight, thaycung->getPosition().Z));
		}
		}


	if(cameraManager) cameraManager->Update();

	core::vector3df fore(0, 0, 1);	
	thaycung->getNode()->getAbsoluteTransformation().rotateVect(fore);
	//cameraManager->getCam()->getAbsoluteTransformation().rotateVect(fore);
	core::vector3df rot1 = fore.getHorizontalAngle(); 

	game->setPlayerRotation(rot1.Y);

	/*=======================================*/
	/*
	
	game->getCamera()->setPosition(thaycung->getPosition() + vector3df(-10, 10, 0));

	core::vector3df vim = core::vector3df(0,0,-15);
	irr::core::matrix4 m;
	m.setRotationDegrees(thaycung->getRotate());
	m.transformVect(vim);
	game->getCamera()->setPosition(thaycung->getPosition() + vim +  core::vector3df(0,15,0));
	game->getCamera()->updateAbsolutePosition();

	//game->getCamera()->setRotation(thaycung->getRotate());

	//game->getCamera()->setTarget(targetPoint*vector3df(0, 0, 1));
	game->getCamera()->setTarget(thaycung->getPosition()*vector3df(0,0,1));
	//smgr->getSceneCollisionManager()->getCollisionPoint(game->getSceneManager()->getSceneCollisionManager()->getRayFromScreenCoordinates(game->getDevice()->getCursorControl()->getPosition(),game->getSceneManager()->getActiveCamera()),selector,intersection,tri);
	*/
	/*=======================================*/
	// New 3rd camera system
	makeCockpit(game->getCamera(),thaycung->getNode(),vector3df(0,20,-10)); //Gia tri offset de dieu chinh vi tri camera

	//Update Map.
	if (thaycung->getPosition() != curPos)
	{
		map->setCharPosition(thaycung->getPosition());
        curPos = thaycung->getPosition();
	}

}

#endif /*INTROSTATE_H_*/
