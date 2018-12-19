#include "MedicalItemComponent.h"

static void RegisterMedicalItem(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CMedicalItemComponent));
		// Functions
		{
		}
	}
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterMedicalItem)

void CMedicalItemComponent::InitializeClass()
{
}

void CMedicalItemComponent::ProcessClassEvent(const SEntityEvent & event)
{
}