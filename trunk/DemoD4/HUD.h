#ifndef HUD_H_
#define HUD_H_


/**
 * Class to display a head up display.
 * 
 * Currently is displays
 *  - the positon of the ship
 *  - a compass
 * 
 * 
 */
class HUD
{
	
private:


	irr::gui::IGUIStaticText* infoText;
	
public:

	HUD(Game* game) :
		 infoText(0)
	{
		
		//add info text 
		this->infoText = game->getGuiEnvironment()->addStaticText(
			L"", irr::core::rect<irr::s32>(SX(20), SY(500), SX(300), SY(580)),
			true, true, 0, -1, true);
		this->infoText->setTextAlignment(irr::gui::EGUIA_CENTER, irr::gui::EGUIA_CENTER);
		this->infoText->grab();
		
	}

	~HUD()
	{
		if (infoText)
		{
			infoText->remove();
			infoText->drop();
			infoText = 0;
		}

	}

	
	const void update(Game* game) const
	{
		//update info text
		irr::core::stringw text(L"Vex count:  ");
		text.append(irr::core::stringw(game->getVideoDriver()->getPrimitiveCountDrawn() ));
		
		this->infoText->setText(text.c_str());
		
	}
};

#endif /*HUD_H_*/
