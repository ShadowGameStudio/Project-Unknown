#pragma once

#include <CryEntitySystem/IEntityComponent.h>
#include <CryEntitySystem/IEntity.h>

#include "ItemProperties.h"

/*-------------------------------------------------------------------------------
Author : Ivar Jönsson
Project : Project Unknown
Purpose : The base component for items
-------------------------------------------------------------------------------*/

class SItemComponent : public IEntityComponent
{
public:
	SItemComponent() = default;
	virtual ~SItemComponent() {}

	//Main functions
	virtual void Initialize() override;
	virtual void InitializeClass() = 0;
	virtual uint64 GetEventMask() const override;

	virtual void ProcessEvent(const SEntityEvent& event) override;
	virtual void ProcessClassEvent(const SEntityEvent& event) = 0;
	static void ReflectType(Schematyc::CTypeDesc<SItemComponent>& desc);

	//Geometry and physics
	virtual void LoadGeometry();
	virtual void Physicalize();

	//Setting
	virtual void CreateItemName();

	//Getting
	virtual SItemProperties *GetProperties() { return &m_sItemProperties; }
	virtual string GetItemName() { return GetProperties()->itemName; }
	virtual int GetItemLevel() { return GetProperties()->itemLevel; }

	virtual EItemType GetItemType() { return GetProperties()->eItemType; }
	virtual float GetItemWeight() { return GetProperties()->sPhysicsProperties.mass; }

	virtual void Drop(SItemComponent *pItemToDrop);
	virtual bool IsPickable();

	void Pickup(IEntity *pOwnerEntity);

protected:

	//Member vars
	SItemProperties m_sItemProperties, m_sPrevItemProperties;

	IEntity *m_pOwnerEntity = nullptr;
	IEntity *m_pNewOwner = nullptr;

	int childConstraintId = 0;
	int ownerConstraintId = 0;

};

