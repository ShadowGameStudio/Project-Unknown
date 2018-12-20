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
bool CInventoryComponent::AddItem(SItemComponent * pItemToPickup)
{
	//If the item is null return
	if (pItemToPickup == nullptr)
	{
		return false;
	}

	//Check what type of item it is and pick it up based on that
	if (pItemToPickup->GetItemType() == eIT_Weapon)
	{
		//FIX
		return false;
	}
	else
	{
		//Check if there is space left
		if (GetInventorySpaceLeft() > pItemToPickup->GetItemWeight())
		{
			//Add the item to the vector and remove the weight
			m_pInventory.push_back(pItemToPickup);
			m_CurrentWeightLeft -= pItemToPickup->GetItemWeight();

			//Add UI stuff
			return true;
		}

		return false;
	}
}

//Removes an item from the inventory
void CInventoryComponent::RemoveItem(SItemComponent * pItemToRemove)
{
}

//Selects the wanted slot
void CInventoryComponent::SelectSlot(int slotId)
{
	//Check so that it isn't an invalid slot
	if (slotId < 0)
	{
		return;
	}

	//Sets the last selected weapon to the currently selected weapon
	m_pLastSelectedItem = m_pSelectedItem;

	//Check if it's a weapon, if it is attach it to the back
	if (m_pSelectedItem->GetItemType() == eIT_Weapon)
	{

	}
	else
	{
		//Detach the item from the hand
		DetachFromHand();

		//Add the item to the inventory again
		AddItem(m_pSelectedItem);
	}

	//if (m_pLastSelectedItem != )
	//{

	//}

}

//Detaches the currently selected item from the hand
void CInventoryComponent::DetachFromHand()
{
	//Get the player component
	CPlayerComponent *pPlayer = m_pEntity->GetComponent<CPlayerComponent>();

	//Try to get the player character
	if (ICharacterInstance *pCharacter = pPlayer->GetAnimations()->GetCharacter())
	{
		//Get the attachments
		if (IAttachmentManager *pAttMan = pCharacter->GetIAttachmentManager())
		{
			//Attachment name
			string attName = "hand";

			//Try to get the attachment
			if (IAttachment *pAttachment = pAttMan->GetInterfaceByName(attName))
			{
				pAttachment->ClearBinding();
			}
		}
	}
}

//Gets the weapon in the hand
int CInventoryComponent::GetSelectedWeapon()
{
	return 0;
}

//Gets the amount of weapons in the inventory
int CInventoryComponent::GetWeaponsInInventory()
{
	//Temp value
	int weaponCount = 0;

	//Go through the vector and find all the weapons
	for (int i = 0; i < m_pInventory.size(); i++)
	{
		if (m_pInventory[i]->GetItemType() == eIT_Weapon)
		{
			weaponCount++;
		}
	}

	//Return the count
	return weaponCount;
}

//Gets the position that an item is located
int CInventoryComponent::GetInventoryIndex(SItemComponent * pItem)
{
	//Get the item
	auto it = std::find(m_pInventory.begin(), m_pInventory.end(), pItem);

	//If it's not the end of the vector
	if (it != m_pInventory.end())
	{
		//Get the distance and thereby the index and return it
		auto index = std::distance(m_pInventory.begin(), it);
		return (int)index;
	}

	return -1;
}

//Gets the position that an item is located, in the selectable slots
int CInventoryComponent::GetSelectableSlotsIndex(SItemComponent * pItem)
{
	//If it's not the item return -1
	if (!pItem)
	{
		return -1;
	}

	//Go through the slots and check for the item
	//If it finds it, return the index
	for (int i = 0; i < SELECTABLE_SLOTS; i++)
	{
		if (m_pSelectableSlots[i] == pItem)
		{
			return i;
		}
	}

	return -1;
}