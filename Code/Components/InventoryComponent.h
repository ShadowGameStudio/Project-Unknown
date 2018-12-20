#pragma once
#include <CryEntitySystem/IEntityComponent.h>
#include <CryEntitySystem/IEntity.h>

#include <vector>
#include "ItemComponent.h"

#define SELECTABLE_SLOTS 5

/*-------------------------------------------------------------------------------
Author : Ivar Jönsson
Project : Project Unknown
Purpose : Handels the players inventory
-------------------------------------------------------------------------------*/

class CInventoryComponent final : public IEntityComponent
{

public:
	CInventoryComponent() = default;
	virtual ~CInventoryComponent() {}

	virtual void Initialize() override;
	virtual uint64 GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	// Reflect type to set a unique identifier for this component
	// and provide additional information to expose it in the sandbox
	static void ReflectType(Schematyc::CTypeDesc<CInventoryComponent>& desc)
	{
		desc.SetGUID("{955A2BAD-4B66-45F0-96EF-A329F1A07E51}"_cry_guid);
	}

public:

	//Functions
	bool AddItem(SItemComponent *pItemToPickup);
	void RemoveItem(SItemComponent *pItemToRemove);
	void SelectSlot(int slotId);

	void DetachFromHand();

	//Getting
	int GetSelectedWeapon();
	int GetWeaponsInInventory();
	int GetInventoryIndex(SItemComponent *pItem);

	int GetSelectableSlotsIndex(SItemComponent *pItem);
	float GetInventorySpaceLeft() { return m_CurrentWeightLeft; }

private:

	//Member vars
	std::vector<SItemComponent*> m_pInventory = {};
	SItemComponent* m_pSelectableSlots[SELECTABLE_SLOTS];

	float m_CurrentWeightLeft = 200.f;
	SItemComponent *m_pLastSelectedItem = nullptr;
	SItemComponent *m_pSelectedItem = nullptr;

};

