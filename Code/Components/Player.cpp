#include "StdAfx.h"
#include "Player.h"

#include "Bullet.h"
#include "SpawnPoint.h"

#include <CryRenderer/IRenderAuxGeom.h>
#include "CrySchematyc\Env\Elements\EnvComponent.h"

static void RegisterPlayer(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CPlayerComponent));
		// Functions
		{
		}
	}
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterPlayer)

void CPlayerComponent::Initialize()
{
	//Get or create the camera and other components
	m_pInputComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CInputComponent>();
	m_pCameraComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CCameraComponent>();
	m_pInventoryComponent = m_pEntity->GetOrCreateComponentClass<CInventoryComponent>();

	//Initialize the input and set up the base player params
	InitializeInput();
	SetPlayerParams();

	if (gEnv->IsEditor())
	{
		//Create the character controller
		m_pCharacterController = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CCharacterControllerComponent>();
		m_pCharacterController->SetTransformMatrix(Matrix34::Create(Vec3(1.f), IDENTITY, Vec3(0, 0, 1.f)));

		//Create the animation component and set the needed values
		m_pAnimationComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>();
		m_pAnimationComponent->SetMannequinAnimationDatabaseFile("Animations/Mannequin/ADB/FirstPerson.adb");
		m_pAnimationComponent->SetCharacterFile("Objects/Characters/SampleCharacter/firstperson.cdf");

		m_pAnimationComponent->SetControllerDefinitionFile("Animations/Mannequin/ADB/FirstPersonControllerDefinition.xml");
		m_pAnimationComponent->SetDefaultFragmentName("Idle");
		m_pAnimationComponent->SetAnimationDrivenMotion(false);

		m_pAnimationComponent->LoadFromDisk();

		m_idleFragmentId = m_pAnimationComponent->GetFragmentId("Idle");
		m_walkFragmentId = m_pAnimationComponent->GetFragmentId("Walk");
		m_rotateTagId = m_pAnimationComponent->GetTagId("Rotate");

		Revive();
	}
	else
	{
		//Create the character controller
		m_pCharacterController = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CCharacterControllerComponent>();
		m_pCharacterController->SetTransformMatrix(Matrix34::Create(Vec3(1.f), IDENTITY, Vec3(0, 0, 1.f)));

		//Create the animation component and set the needed values
		m_pAnimationComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>();
		m_pAnimationComponent->SetMannequinAnimationDatabaseFile("Animations/Mannequin/ADB/FirstPerson.adb");
		m_pAnimationComponent->SetCharacterFile("Objects/Characters/SampleCharacter/thirdperson.cdf");

		m_pAnimationComponent->SetControllerDefinitionFile("Animations/Mannequin/ADB/FirstPersonControllerDefinition.xml");
		m_pAnimationComponent->SetDefaultFragmentName("Idle");
		m_pAnimationComponent->SetAnimationDrivenMotion(false);

		m_pAnimationComponent->LoadFromDisk();

		m_idleFragmentId = m_pAnimationComponent->GetFragmentId("Idle");
		m_walkFragmentId = m_pAnimationComponent->GetFragmentId("Walk");
		m_rotateTagId = m_pAnimationComponent->GetTagId("Rotate");

		Revive();
	}

}

uint64 CPlayerComponent::GetEventMask() const
{
	return ENTITY_EVENT_BIT(ENTITY_EVENT_START_GAME) | ENTITY_EVENT_BIT(ENTITY_EVENT_UPDATE);
}

void CPlayerComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case ENTITY_EVENT_START_GAME:
	{
		// Revive the entity when gameplay starts
		Revive();
	}
	break;
	case ENTITY_EVENT_UPDATE:
	{
		SEntityUpdateContext* pCtx = (SEntityUpdateContext*)event.nParam[0];

		// Start by updating the movement request we want to send to the character controller
		// This results in the physical representation of the character moving
		UpdateMovementRequest(pCtx->fFrameTime);

		// Process mouse input to update look orientation.
		UpdateLookDirectionRequest(pCtx->fFrameTime);

		// Update the animation state of the character
		UpdateAnimation(pCtx->fFrameTime);

		// Update the camera component offset
		UpdateCamera(pCtx->fFrameTime);

		//The main updates
		Update(pCtx->fFrameTime);
	}
	break;
	}
}

void CPlayerComponent::ReflectType(Schematyc::CTypeDesc<CPlayerComponent>& desc)
{
	desc.SetGUID("{63F4C0C6-32AF-4ACB-8FB0-57D45DD14725}"_cry_guid);
	desc.SetEditorCategory("Players");
	desc.SetLabel("Player Component");

	desc.SetDescription("The main player component, needs to be used to create a player");
	desc.SetComponentFlags({ IEntityComponent::EFlags::Transform, 
		IEntityComponent::EFlags::Socket, 
		IEntityComponent::EFlags::Attach });
}

void CPlayerComponent::Revive()
{
	// Find a spawn point and move the entity there
	SpawnAtSpawnPoint();

	// Unhide the entity in case hidden by the Editor
	GetEntity()->Hide(false);

	// Make sure that the player spawns upright
	GetEntity()->SetWorldTM(Matrix34::Create(Vec3(1, 1, 1), IDENTITY, GetEntity()->GetWorldPos()));

	// Apply the character to the entity and queue animations
	m_pAnimationComponent->ResetCharacter();
	m_pCharacterController->Physicalize();

	// Reset input now that the player respawned
	m_inputFlags = 0;
	m_mouseDeltaRotation = ZERO;
	m_lookOrientation = IDENTITY;

	m_mouseDeltaSmoothingFilter.Reset();

	m_activeFragmentId = FRAGMENT_ID_INVALID;

	m_horizontalAngularVelocity = 0.0f;
	m_averagedHorizontalAngularVelocity.Reset();

	if (ICharacterInstance *pCharacter = m_pAnimationComponent->GetCharacter())
	{
		// Cache the camera joint id so that we don't need to look it up every frame in UpdateView
		m_cameraJointId = pCharacter->GetIDefaultSkeleton().GetJointIDByName("head");
	}
}

void CPlayerComponent::SpawnAtSpawnPoint()
{
	// We only handle default spawning below for the Launcher
	// Editor has special logic in CEditorGame
	if (gEnv->IsEditor())
		return;

	// Spawn at first default spawner
	IEntityItPtr pEntityIterator = gEnv->pEntitySystem->GetEntityIterator();
	pEntityIterator->MoveFirst();

	while (!pEntityIterator->IsEnd())
	{
		IEntity *pEntity = pEntityIterator->Next();

		if (auto* pSpawner = pEntity->GetComponent<CSpawnPointComponent>())
		{
			pSpawner->SpawnEntity(m_pEntity);
			break;
		}
	}
}

//Sets some base player params
void CPlayerComponent::SetPlayerParams()
{
	m_pCameraComponent->SetNearPlane(0.0001f);
}

//Picks up an item
void CPlayerComponent::Pickup(SItemComponent * pNewItem)
{
	//If the item is null, return
	if (!pNewItem)
	{
		return;
	}

	//Add the item to the inventory
	if (GetInventory()->AddItem(pNewItem))
	{
		//Pick up the item
		pNewItem->Pickup(m_pEntity);

		if (pNewItem->GetItemType() == eIT_Weapon)
		{
			//Attach to back
		}
	}
}
