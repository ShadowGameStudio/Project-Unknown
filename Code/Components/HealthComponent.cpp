#include "HealthComponent.h"

void CHealthComponent::Initialize()
{
}

uint64 CHealthComponent::GetEventMask() const
{
	return ENTITY_EVENT_BIT(ENTITY_EVENT_UPDATE);
}

void CHealthComponent::ProcessEvent(const SEntityEvent & event)
{
	switch (event.event)
	{
	case ENTITY_EVENT_UPDATE:

		SEntityUpdateContext* pCtx = (SEntityUpdateContext*)event.nParam[0];
		RegenerateHealth(pCtx->fFrameTime);

	}
}

//Called every update and regenerates the players health
void CHealthComponent::RegenerateHealth(float frameTime)
{
	//Check that it always should regenerate, that it actually should regenerate and that the player is alive
	if (GetAlwaysRegenerate() && m_ShouldRegenerate && m_IsAlive)
	{
		//Add the ratio to the health
		GetHealthProperties()->health += GetRegenerationRatio();
	}
}                                                                                                                                         

void CHealthComponent::KillEntity()
{
	//Removes the entity from the system
	gEnv->pEntitySystem->RemoveEntity(m_pEntity->GetId());
}
