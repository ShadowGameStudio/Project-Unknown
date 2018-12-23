#pragma once
#include <CryEntitySystem/IEntityComponent.h>
#include <CryEntitySystem/IEntity.h>

#include "ItemComponent.h"

/*-------------------------------------------------------------------------------
Author : Ivar Jönsson
Project : Project Unknown
Purpose : Handels all the medical items, such as bandages
-------------------------------------------------------------------------------*/

class CMedicalItemComponent : public SItemComponent
{
	struct SMedicalItemProperties
	{
		inline bool operator==(const SMedicalItemProperties& rhs) const { return 0 == memcmp(this, &rhs, sizeof(rhs)); }
		inline bool operator!=(const SMedicalItemProperties& rhs) const { return 0 != memcmp(this, &rhs, sizeof(rhs)); }

		float healthAmount;

		static void ReflectType(Schematyc::CTypeDesc<SMedicalItemProperties>& desc)
		{
			desc.SetGUID("{B6ABBE36-3524-4431-82E6-47D3D3D0CA73}"_cry_guid);
			desc.SetLabel("Medical Item Properties");
			desc.SetDescription("Properties for the medical item");

			desc.AddMember(&SMedicalItemProperties::healthAmount, 'ha', "HealthAmount", "Health Amount", "The amount of health this item will add", 0.f);
		}
	};

public:
	CMedicalItemComponent() = default;

	virtual ~CMedicalItemComponent() {}
	virtual void InitializeClass() override;
	virtual void ProcessClassEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	// and provide additional information to expose it in the sandbox
	static void ReflectType(Schematyc::CTypeDesc<CMedicalItemComponent>& desc)
	{
		desc.SetGUID("{FB215B01-3415-4A69-B6B4-6C0C40437FCC}"_cry_guid);
		desc.AddBase<SItemComponent>();
		desc.SetEditorCategory("Gameplay");

		desc.SetLabel("Medical Item Component");
		desc.SetDescription("The component to use on all medical items such as bandages");
		desc.AddMember(&CMedicalItemComponent::m_sItemProperties, 'itep', "ItemProperties", "Item Properties", "All the base properties for this item", SItemProperties());

		desc.AddMember(&CMedicalItemComponent::m_sMedicalProperties, 'smp', "MedicalProperties", "Medical Properties", "All the medical specific properties", SMedicalItemProperties());
	}

public:

	//Functions

private:

	//Member vars
	SMedicalItemProperties m_sMedicalProperties;

};

