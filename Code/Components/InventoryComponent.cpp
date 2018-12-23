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
		DetachFromHand();
		AttachToBack(m_pSelectedItem, GetSelectableSlotsIndex(m_pSelectedItem));
		m_pSelectedItem = nullptr;
	}
	else
	{
		//Detach the item from the hand
		DetachFromHand();
		m_pSelectedItem = nullptr;
	}

	//If the item isn't already selected, select it
	if (m_pLastSelectedItem != m_pSelectableSlots[slotId])
	{
		SelectItem(m_pSelectableSlots[slotId]);
	}

}

//Selects an item
void CInventoryComponent::SelectItem(SItemComponent * pItemToSelect)
{
	//Check that the item isn't null
	if (!pItemToSelect)
	{
		return;
	}

	if (pItemToSelect->GetItemType() == eIT_Weapon)
	{
		//Detach the item from the back and attach it to the hand
		DetachFromBack(GetSelectableSlotsIndex(pItemToSelect));
		m_pSelectedItem = pItemToSelect;
		AttachToHand(pItemToSelect);
	}
	else
	{
		m_pSelectedItem = pItemToSelect;
		AttachToHand(pItemToSelect);
	}
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

//Detaches an item from the back
void CInventoryComponent::DetachFromBack(int slotId)
{
	//Check that the slot id isn't invalid
	if (slotId < 0)
	{
		return;
	}

	//Create string from slot id and get the player component
	string slotString = ToString(slotId);
	CPlayerComponent *pPlayer = m_pEntity->GetComponent<CPlayerComponent>();

	//Get the players character and the attachments of it
	if (ICharacterInstance *pCharacter = pPlayer->GetAnimations()->GetCharacter())
	{
		if (IAttachmentManager *pAttMan = pCharacter->GetIAttachmentManager())
		{
			//The string to hold the attachment name
			string attName = "back_att_0" + slotString;

			//Get the attachment and clear the binding
			if (IAttachment *pAttachment = pAttMan->GetInterfaceByName(attName))
			{
				pAttachment->ClearBinding();
			}
		} 
	}
}

void CInventoryComponent::AttachToBack(SItemComponent * pWeaponToAttach, int slotId)
{
	//Check the weapon to attach and the slot id
	if (!pWeaponToAttach || slotId < 0)
	{
		return;
	}

	string slotString = ToString(slotId);
	CPlayerComponent *pPlayer = m_pEntity->GetComponent<CPlayerComponent>();
	CEntityAttachment *pAttachmentWeapon = new CEntityAttachment();

	pAttachmentWeapon->SetEntityId(pWeaponToAttach->GetEntityId());

	if (ICharacterInstance *pCharacter = pPlayer->GetAnimations()->GetCharacter())
	{
		if (IAttachmentManager *pAttMan = pCharacter->GetIAttachmentManager())
		{
			string attName = "back_att_0" + slotString;

			if (IAttachment *pAttachment = pAttMan->GetInterfaceByName(attName))
			{
				pAttachment->AddBinding(pAttachmentWeapon);
			}
		}
	}
}

//Attaches an item to the hand
void CInventoryComponent::AttachToHand(SItemComponent * pItemToAttach)
{
	//If the item to attach is null, return
	if (!pItemToAttach)
	{
		return;
	}

	//Get the player component and create and attachment item
	CPlayerComponent *pPlayer = m_pEntity->GetComponent<CPlayerComponent>();
	CEntityAttachment *pAttachmentItem = new CEntityAttachment();

	//Set the attachment item
	pAttachmentItem->SetEntityId(pItemToAttach->GetEntityId());

	//Get the player character and it's attachments
	if (ICharacterInstance *pCharacter = pPlayer->GetAnimations()->GetCharacter())
	{
		if (IAttachmentManager *pAttMan = pCharacter->GetIAttachmentManager())
		{
			//The attachment name
			string attName = "hand";

			//Get the attachment and bind it
			if (IAttachment *pAttachment = pAttMan->GetInterfaceByName(attName))
			{
				pAttachment->AddBinding(pAttachmentItem);
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