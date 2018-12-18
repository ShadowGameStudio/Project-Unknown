#include "ItemComponent.h"

//Called when the player wants to drop and item
void SItemComponent::Drop(SItemComponent * pItemToDrop)
{
}

//Checks if an item is pickable
void SItemComponent::IsPickable()
{
}

//Picks up an item
void SItemComponent::Pickup(IEntity * pOwnerEntity)
{
}

//Called on initialization
void SItemComponent::Initialize()
{
}

uint64 SItemComponent::GetEventMask() const
{
	return uint64();
}

void SItemComponent::ProcessEvent(const SEntityEvent & event)
{
}

void SItemComponent::ReflectType(Schematyc::CTypeDesc<SItemComponent>& desc)
{
}

void SItemComponent::LoadGeometry()
{
}

void SItemComponent::Physicalize()
{
}

void SItemComponent::CreateItemName()
{
}
