#pragma once
#include <CryEntitySystem/IEntityComponent.h>
#include <CryEntitySystem/IEntity.h>

#include <vector>

#define WEAPON_SPACE 2

/*-------------------------------------------------------------------------------
Author : Ivar Jönsson
Project : PU
Purpose :
-------------------------------------------------------------------------------*/

class CInventoryComponent final : public IEntityComponent {

	struct SInventoryProperties
	{
		float weightCap;

		static void ReflectType(Schematyc::CTypeDesc<SInventoryProperties>& desc)
		{
			desc.SetGUID("{7F2B7738-4397-411D-AE34-3C33744A624B}"_cry_guid);
		}
	};

public:
	CInventoryComponent() = default;
	virtual ~CInventoryComponent() {}

	virtual void Initialize() override;
	virtual uint64 GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	// Reflect type to set a unique identifier for this component
	// and provide additional information to expose it in the sandbox
	static void ReflectType(Schematyc::CTypeDesc<CInventoryComponent>& desc) {
		desc.SetGUID("{955A2BAD-4B66-45F0-96EF-A329F1A07E51}"_cry_guid);
		desc.SetEditorCategory("Player");
		desc.SetLabel("Inventory Component");
		desc.SetDescription("A component that handels the players inventory");
		desc.SetComponentFlags({ IEntityComponent::EFlags::Transform, IEntityComponent::EFlags::Socket, IEntityComponent::EFlags::Attach });
	}

public:

	//Functions
	void AddItem(SItemComponent *pItemToPickup);
	void RemoveItem(SItemComponent *pItemToRemove);

	//Getting
	float GetInventorySpaceLeft();
	SInventoryProperties *GetInventoryProperties() { return &m_SInventoryProperties; };

private:

	//Member vars
	std::vector<SItemComponent*> m_pInventory = {};
	SItemComponent* m_pWeapons[WEAPON_SPACE];
	SInventoryProperties m_SInventoryProperties;


	float m_CurrentWeightLeft = GetInventoryProperties()->weightCap;

};

