#include "CCursorController.h"

CCursorController::CCursorController(ICursorControl* irrCursor, IVideoDriver* irrVideoDriver) :
m_pMouseCursor(0), used(0), irrCursorControl(irrCursor), videoDriver(irrVideoDriver)
{
   updateMousePos();

   setVisible(true);
   setOSCursorVisible(false);
}

CCursorController::~CCursorController()
{
   Clear();
}

void CCursorController::setVisible(bool visible)
{
   IsVisible = visible;
}

bool CCursorController::isVisible() const
{
   return IsVisible;
}

void CCursorController::setOSCursorVisible(bool visible)
{
   IsOSCursorVisible = visible;
}

bool CCursorController::isOSCursorVisible() const
{
   return IsOSCursorVisible;
}

void CCursorController::render()
{
   updateMousePos();

   if(isVisible() && used)
   {
      _IRR_DEBUG_BREAK_IF(!m_pMouseCursor); // There isn't any cursor texture loaded
      if(m_pMouseCursor)
      {
         videoDriver->draw2DImage(m_pMouseCursor,
            position2di(m_MousePos.X - m_pMouseCursor->getSize().Width/2+1,
            m_MousePos.Y - m_pMouseCursor->getSize().Height/2+2),
            rect<s32>(position2di(0,0),m_pMouseCursor->getSize()),
            0, SColor(255,255,255,255), true);
      }
   }

   if(isOSCursorVisible())
   {
      irrCursorControl->setVisible(true);
      irrCursorControl->setPosition(irrCursorControl->getPosition());
   }
   else
   {
      irrCursorControl->setVisible(false);
      irrCursorControl->setPosition(irrCursorControl->getPosition());
   }
}

void CCursorController::addMouseCursorTexture(c8* Cursor_file)
{
   m_pMouseCursor = videoDriver->getTexture(Cursor_file);
   
   bool isAlreadyLoaded = false;
   for(u32 i = 0; i < m_aMouseCursors.size() && !isAlreadyLoaded; i++)
   {
      if(m_aMouseCursors[i]->getName() == m_pMouseCursor->getName())
      {
         isAlreadyLoaded = !isAlreadyLoaded;
         break;
      }
   }

   if (!isAlreadyLoaded)
   {
      videoDriver->makeColorKeyTexture(m_pMouseCursor, SColor(255,0,0,0));

      m_aMouseCursors.push_back(m_pMouseCursor);
      used++;
   }

   // So the first loaded cursor will be active
   if(used > 0)
      setActiveCursor(0);
}

ITexture* CCursorController::getCursorTexture(u32 index) const
{
   _IRR_DEBUG_BREAK_IF(index>used); // access violation

   return m_aMouseCursors[index];
}

void CCursorController::removeCursor(u32 index)
{
   _IRR_DEBUG_BREAK_IF(index>used); // access violation

   videoDriver->removeTexture(m_aMouseCursors[index]);
   m_aMouseCursors.erase(index);
   used--;
}

void CCursorController::setActiveCursor(u32 index)
{
   _IRR_DEBUG_BREAK_IF(index>used); // access violation

   m_pMouseCursor = m_aMouseCursors[index];
}

void CCursorController::Clear()
{
   for(u32 i = 0; i < m_aMouseCursors.size(); i++)
   {
      removeCursor(i);
   }

   m_aMouseCursors.clear();
}

position2di& CCursorController::getMousePos()
{
   return updateMousePos();
}

position2di& CCursorController::updateMousePos()
{
   m_MousePos = irrCursorControl->getPosition();

   return m_MousePos;
}