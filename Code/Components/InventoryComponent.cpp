#include "InventoryComponent.h"

void CInventoryComponent::Initialize()
{
}

uint64 CInventoryComponent::GetEventMask() const
{
	return uint64();
}

void CInventoryComponent::ProcessEvent(const SEntityEvent & event)
{
}

//Adds an item to the inventory
void CInventoryComponent::AddItem(SItemComponent * pItemToPickup)
{
	//If the item is null return
	if (pItemToPickup == nullptr)
	{
		return;
	}

	//Check what type of item it is and pick it up based on that
	if (pItemToPickup->GetItemType() == eIT_Weapon)
	{
		if (true)
		{

		}

		for (int i = 0; i < WEAPON_SPACE; i++)
		{
			if (!m_pWeapons[i])
			{
				m_pWeapons[i] = pItemToPickup;
			}
		}

		//Add UI stuff
	}
	else
	{
		m_pInventory.push_back(pItemToPickup);
		//Add UI stuff
	}
}

void CInventoryComponent::RemoveItem(SItemComponent * pItemToRemove)
{
}

float CInventoryComponent::GetInventorySpaceLeft()
{
	float diff = 
}
