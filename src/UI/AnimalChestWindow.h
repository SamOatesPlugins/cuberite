
// AnimalChestWindow.h

// Representing the UI window for a animal chest





#pragma once

#include "Window.h"
#include "../Mobs/Horse.h"




class cAnimalChestWindow :
	public cWindow
{
	typedef cWindow super;

public:
	cAnimalChestWindow(cHorse * a_Horse);

	~cAnimalChestWindow();

	virtual bool ClosedByPlayer(cPlayer & a_Player, bool a_CanRefuse) override;

	virtual void OpenedByPlayer(cPlayer & a_Player) override;

	virtual void DistributeStack(cItem & a_ItemStack, int a_Slot, cPlayer & a_Player, cSlotArea * a_ClickedArea, bool a_ShouldApply) override;
	
	cHorse * GetOwningAnimal() const { return m_OwningAnimal; }

protected:
	cHorse * m_OwningAnimal;
};




