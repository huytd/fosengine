#ifndef HUD_H_
#define HUD_H_

#include "GUIHealthStatusBar.h"
void SetupQuadMesh( scene::SMeshBuffer &mesh, f32 f32Width )
{
    f32 f32HalfWidth = f32Width/2.0f;
    mesh.Vertices.set_used(4);
    mesh.Indices .set_used(6);

    video::SColor white(255, 255, 255, 255);

    mesh.Vertices[0] = video::S3DVertex(-f32HalfWidth, -f32HalfWidth, 0.f, 0.f, 0.f, 1.f, white, 0.f, 1.f);
    mesh.Vertices[1] = video::S3DVertex(-f32HalfWidth,  f32HalfWidth, 0.f, 0.f, 0.f, 1.f, white, 0.f, 0.f);
    mesh.Vertices[2] = video::S3DVertex( f32HalfWidth,  f32HalfWidth, 0.f, 0.f, 0.f, 1.f, white, 1.f, 0.f);
    mesh.Vertices[3] = video::S3DVertex( f32HalfWidth, -f32HalfWidth, 0.f, 0.f, 0.f, 1.f, white, 1.f, 1.f);

    mesh.Indices[0] = 0;
    mesh.Indices[1] = 1;
    mesh.Indices[2] = 2;
    mesh.Indices[3] = 2;
    mesh.Indices[4] = 3;
    mesh.Indices[5] = 0;

    mesh.getMaterial().Lighting = false;
    //mesh.getMaterial().BackfaceCulling = false;
    //mesh.getMaterial().Wireframe = true;
    mesh.getMaterial().MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;
} 

class VxCompass : public gui::IGUIElement
{
public:
   //=== constructor ===//
   VxCompass( core::rect<s32> rect, gui::IGUIEnvironment* env, gui::IGUIElement* parent );

   //! set body of compass texture
   void SetCompassBodyTexture( video::ITexture* texture );

   //! set needle of compass texture
   void SetCompassNeedleTexture( video::ITexture* texture );

   //! set compass heading
   void SetCompassHeading( f32 deg );

   //! render the compass
   virtual void draw();

private:
   scene::SMeshBuffer BodyMesh;
   scene::SMeshBuffer NeedleMesh;
   core::matrix4 Matrix;
}; 
//------------------------------------------------------------------------------
//=== constructor ===//
VxCompass::VxCompass(core::rect<s32> rect, gui::IGUIEnvironment* env, gui::IGUIElement* parent)
      : gui::IGUIElement(gui::EGUIET_ELEMENT, env, parent, -1, rect)
{
   SetupQuadMesh( BodyMesh, 1.0f );
   SetupQuadMesh( NeedleMesh, 1.0f );
}

//------------------------------------------------------------------------------
//! set body of compass texture
void VxCompass::SetCompassBodyTexture(video::ITexture* texture)
{
    BodyMesh.getMaterial().setTexture(0,texture); 
}
//------------------------------------------------------------------------------
//! set needle of compass texture
void VxCompass::SetCompassNeedleTexture(video::ITexture* texture)
{
    NeedleMesh.getMaterial().setTexture(0,texture); 
}

//------------------------------------------------------------------------------
//! set compass heading
void VxCompass::SetCompassHeading( f32 deg )
{
    Matrix.makeIdentity();
    Matrix.setRotationDegrees(core::vector3df(0, 0, deg));
}

//------------------------------------------------------------------------------
//! render the compass
void VxCompass::draw()
{
    video::IVideoDriver* driver = Environment->getVideoDriver();
    if (! (driver && IsVisible))
        return;

    core::rect<s32> oldViewPort = driver->getViewPort();
    driver->setViewPort(getAbsolutePosition());

    // clear the projection matrix
    core::matrix4 oldProjMat = driver->getTransform(video::ETS_PROJECTION);
    driver->setTransform(video::ETS_PROJECTION, core::matrix4());

    // clear the view matrix
    core::matrix4 oldViewMat = driver->getTransform(video::ETS_VIEW);
    driver->setTransform(video::ETS_VIEW, core::matrix4());

    driver->setTransform(video::ETS_WORLD, Matrix);

   // draw compass body
    driver->setMaterial(BodyMesh.Material);
    driver->drawMeshBuffer(&BodyMesh);

    driver->setTransform(video::ETS_WORLD, core::matrix4());

   // draw the needle
    driver->setMaterial(NeedleMesh.Material);
    driver->drawMeshBuffer(&NeedleMesh);

    // restore view matrix
    driver->setTransform(video::ETS_VIEW, oldViewMat);

    // restore projection matrix
    driver->setTransform(video::ETS_PROJECTION, oldProjMat);

    // restore the view area
    driver->setViewPort(oldViewPort);
}




/**
 * Class to display a head up display.
 * 
 * Currently is displays
 *  - the positon of the ship
 *  - a compass
 *
 */
class HUD
{
	
private:
	irr::gui::IGUIStaticText* infoText;
	GUIHealthStatusBar* statusbar;
	irr::gui::IGUIButton *attackButton;
    VxCompass * pgCompass;
	
public:
	HUD(Game* game) :
		 infoText(0), statusbar(0), attackButton(0), pgCompass(0)
	{
		//add status bar
		statusbar = new GUIHealthStatusBar(irr::core::rect<irr::s32>(SX(20), SY(20), SX(100), SY(100)), game->getGuiEnvironment(), game->getGuiEnvironment()->getRootGUIElement()); 
		//add info text 
		//this->infoText = game->getGuiEnvironment()->addStaticText(
		//	L"", irr::core::rect<irr::s32>(SX(20), SY(500), SX(300), SY(580)),
		//	true, true, 0, -1, true);
		//this->infoText->setTextAlignment(irr::gui::EGUIA_CENTER, irr::gui::EGUIA_CENTER);
		//this->infoText->grab();

		this->attackButton = game->getGuiEnvironment()->addButton(
			irr::core::rect<irr::s32>(SX(800), SY(550), SX(1020), SY(680)),
			0, -1, L"", L"Tấn công");
		this->attackButton->grab();

		pgCompass = new VxCompass( irr::core::rect<irr::s32>(400, 0, 800, 400), game->getGuiEnvironment(), game->getGuiEnvironment()->getRootGUIElement());
		video::ITexture * pgCompassBodyTexture =   game->getVideoDriver()->getTexture("media/images/compass/body.png");
		video::ITexture * pgCompassNeedleTexture =  game->getVideoDriver()->getTexture("media/images/compass/heading.png");
		pgCompass->SetCompassBodyTexture(pgCompassBodyTexture);
		pgCompass->SetCompassNeedleTexture(pgCompassNeedleTexture); 
	}

	~HUD()
	{
		if (infoText)
		{
			infoText->remove();
			infoText->drop();
			infoText = 0;
		}

		if (attackButton)
		{
			attackButton->remove();
			attackButton->drop();
			attackButton = 0;
		}
	}

	const void update(Game* game) const
	{
		//update info text
		irr::core::stringw text(L"Ves:");
		text.append(irr::core::stringw(game->getVideoDriver()->getPrimitiveCountDrawn() ));
		
		this->attackButton->setText(text.c_str());

		pgCompass->SetCompassHeading(game->getPlayerRotation());
	}
};

#endif /*HUD_H_*/
