
// AnimalChestWindow.cpp

// Representing the UI window for a animal chest

#include "Globals.h"
#include "AnimalChestWindow.h"
#include "../Entities/Player.h"





cAnimalChestWindow::cAnimalChestWindow(cHorse * a_Horse) :
	cWindow(wtAnimalChest, "Horse Chest"),
	m_OwningAnimal(a_Horse)
{
}





cAnimalChestWindow::~cAnimalChestWindow()
{
}





bool cAnimalChestWindow::ClosedByPlayer(cPlayer & a_Player, bool a_CanRefuse)
{
	cWindow::ClosedByPlayer(a_Player, a_CanRefuse);
	return true;
}





void cAnimalChestWindow::OpenedByPlayer(cPlayer & a_Player)
{
	cWindow::OpenedByPlayer(a_Player);
}





void cAnimalChestWindow::DistributeStack(cItem & a_ItemStack, int a_Slot, cPlayer & a_Player, cSlotArea * a_ClickedArea, bool a_ShouldApply)
{
	cSlotAreas AreasInOrder;

	if (a_ClickedArea == m_SlotAreas[0])
	{
		// Chest Area
		AreasInOrder.push_back(m_SlotAreas[2]);  /* Hotbar    */
		AreasInOrder.push_back(m_SlotAreas[1]);  /* Inventory */
		super::DistributeStackToAreas(a_ItemStack, a_Player, AreasInOrder, a_ShouldApply, true);
	}
	else
	{
		// Hotbar or Inventory
		AreasInOrder.push_back(m_SlotAreas[0]);  /* Chest */
		super::DistributeStackToAreas(a_ItemStack, a_Player, AreasInOrder, a_ShouldApply, false);
	}
}




