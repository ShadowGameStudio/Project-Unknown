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

void CHealthComponent::RegenerateHealth(float frameTime)
{
	if (GetAlwaysRegenerate() && m_ShouldRegenerate && m_IsAlive)
	{

	}
}                                                                                                                                         

void CHealthComponent::KillEntity()
{
}
