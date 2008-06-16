#include "SGE.h"
#include "ProjectLevel.h"

using namespace irr;
using namespace gui;


SGE::SGE()
{
	core.config.getIrrlichtParams().DriverType=irr::video::EDT_DIRECT3D9;//override default setting
	core.init();
	core.getGraphicDevice()->setResizeAble(true);
	core.getGraphicDevice()->setWindowCaption(L"Simple Game Editor - untitled.irr");
	//gui
	//core.getGUISkin()->setSkin("guiskin/guiskin.xml");
	
	IGUIEnvironment* env=core.getGraphicDevice()->getGUIEnvironment();
	IGUISkin* skin=env->createSkin(gui::EGST_WINDOWS_METALLIC);
	env->setSkin(skin);
	IGUIFont* font = env->getFont("resources/font.bmp"); 
	if (font)
		skin->setFont(font); 
    for (s32 i=0; i<gui::EGDC_COUNT ; ++i)
	{ 
		video::SColor col = env->getSkin()->getColor((gui::EGUI_DEFAULT_COLOR)i); 
		col.setAlpha(255); 
		env->getSkin()->setColor((gui::EGUI_DEFAULT_COLOR)i, col);
	} 
	
	core.getEntityManager()->loadLevel(new ProjectLevel);
}

SGE::~SGE()
{
}