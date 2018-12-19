#include "ItemComponent.h"

//Called on initialization
void SItemComponent::Initialize()
{
	//Load basic item stuff
	LoadGeometry();
	Physicalize();
	CreateItemName();
	InitializeClass();
}

uint64 SItemComponent::GetEventMask() const
{
	return ENTITY_EVENT_BIT(ENTITY_EVENT_COMPONENT_PROPERTY_CHANGED) | ENTITY_EVENT_BIT(ENTITY_EVENT_COLLISION) | ENTITY_EVENT_BIT(ENTITY_EVENT_TIMER);
}

void SItemComponent::ProcessEvent(const SEntityEvent & event)
{
	switch (event.event)
	{
	case ENTITY_EVENT_COMPONENT_PROPERTY_CHANGED:

		//If there has been any changes to the properties in the editor
		if (m_sItemProperties != m_sPrevItemProperties)
		{
			//Set the correct properties
			m_sPrevItemProperties = m_sItemProperties;

			//Load and physicalize the item
			LoadGeometry();
			Physicalize();
		}

		break;
	}

	ProcessClassEvent(event);
}

void SItemComponent::ReflectType(Schematyc::CTypeDesc<SItemComponent>& desc)
{
	desc.SetGUID("{05DC29AE-3D39-4B11-B642-2131BC87A687}"_cry_guid);
}

//Called when the player wants to drop and item
void SItemComponent::Drop(SItemComponent * pItemToDrop)
{
	//If this entity is not the owner entity
	if (!m_pOwnerEntity)
	{
		return;
	}

	//If it's a player
	if (CPlayerComponent *pOwnerPlayer = m_pOwnerEntity->GetComponent<CPlayerComponent>())
	{
		//Drop the item
	}
}

//Checks if an item is pickable
bool SItemComponent::IsPickable()
{
	return !m_pOwnerEntity;
}

//Picks up an item
void SItemComponent::Pickup(IEntity * pNewOwner)
{
	//Check if this entity is the right one
	if (!pNewOwner)
	{
		return;
	}

	m_pOwnerEntity = pNewOwner;
	
	//Attach / Pickup item
}

//Gets and sets the items geometry
void SItemComponent::LoadGeometry()
{
	//Get the path into a string
	string geomPath = GetProperties()->sRenderProperties.geomPath.value.c_str();

	//Check if the path is empty and return if it is
	if (geomPath.empty())
	{
		return;
	}

	m_pEntity->LoadGeometry(GEOMETRY_SLOT, geomPath);
}

//Physicalizes the item
void SItemComponent::Physicalize()
{
	//Create the items physics params and set them
	SEntityPhysicalizeParams physParams;
	physParams.mass = GetProperties()->sPhysicsProperties.mass;
	physParams.type = PE_RIGID;

	m_pEntity->Physicalize(physParams);
}

//Creates a name for the item
void SItemComponent::CreateItemName()
{
	//Get the "long" name and remove some of the characters
	string longName = m_pEntity->GetClass()->GetName();
	longName.erase(0, longName.find_last_of(':') + 1);

	//Make the first letter upper case
	string first;
	first.SetAt(0, longName[0]);
	first.MakeUpper();

	//Remove the first letter and set the name
	longName.erase(0, 1);
	GetProperties()->itemName = first + longName;
}
