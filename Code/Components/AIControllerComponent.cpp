#include "AIControllerComponent.h"
#include "CrySchematyc\Env\Elements\EnvComponent.h"
#include "CrySchematyc\Env\IEnvRegistrar.h"

static void RegisterAIController(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CAIControllerComponent));
		// Functions
		{
		}
	}
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterAIController)

void CAIControllerComponent::Initialize()
{
}

void CAIControllerComponent::ProcessEvent(const SEntityEvent & event)
{
}

uint64 CAIControllerComponent::GetEventMask() const
{
	return uint64();
}

void CAIControllerComponent::AddVelocity(const Vec3 vel)
{
	//Get the physical entity
	if (IPhysicalEntity *pPhysicalEntity = m_pEntity->GetPhysicalEntity())
	{
		//Create an action from the params
		pe_action_move moveAction;

		moveAction.iJump = 1;
		moveAction.dir = vel;
		
		//Push the action to the AI
		pPhysicalEntity->Action(&moveAction);
	}
}

void CAIControllerComponent::MoveTo(const Vec3 cords)
{
}

void CAIControllerComponent::Physicalize()
{
	SEntityPhysicalizeParams physParams;
	
	physParams.type = PE_LIVING;
	physParams.nSlot = GetOrMakeEntitySlotId();
	physParams.mass = m_Controller.mass;

	pe_player_dimensions playerDimensions;

	playerDimensions.bUseCapsule = true;
	playerDimensions.sizeCollider = Vec3(m_Controller.radius * 0.5f, 1.f, m_Controller.height * 0.5f);

	if (playerDimensions.bUseCapsule)
	{
		playerDimensions.sizeCollider.z *= 0.5f;
	}

	playerDimensions.heightPivot = 0.f;
	playerDimensions.heightCollider = m_pTransform != nullptr ? m_pTransform->GetTranslation().z : 0.f;
	playerDimensions.groundContactEps = 0.004f;

	physParams.pPlayerDimensions = &playerDimensions;
	m_pEntity->Physicalize(physParams);
	m_pEntity->UpdateComponentEventMask(this);

}
