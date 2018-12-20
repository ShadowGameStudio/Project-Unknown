#include "Player.h"

#define MOUSE_DELTA_TRESHOLD 0.0001f
#define PICKUP_RANGE 0.5f

//Updates the movement
void CPlayerComponent::UpdateMovementRequest(float frameTime)
{
	// Don't handle input if we are in air
	if (!m_pCharacterController->IsOnGround())
		return;

	Vec3 velocity = ZERO;

	const float moveSpeed = 20.5f;

	if (m_inputFlags & (TInputFlags)EInputFlag::MoveLeft)
	{
		velocity.x -= moveSpeed * frameTime;
	}
	if (m_inputFlags & (TInputFlags)EInputFlag::MoveRight)
	{
		velocity.x += moveSpeed * frameTime;
	}
	if (m_inputFlags & (TInputFlags)EInputFlag::MoveForward)
	{
		velocity.y += moveSpeed * frameTime;
	}
	if (m_inputFlags & (TInputFlags)EInputFlag::MoveBack)
	{
		velocity.y -= moveSpeed * frameTime;
	}

	m_pCharacterController->AddVelocity(GetEntity()->GetWorldRotation() * velocity);
}

//Updates the player direction
void CPlayerComponent::UpdateLookDirectionRequest(float frameTime)
{
	const float rotationSpeed = 0.002f;
	const float rotationLimitsMinPitch = -0.84f;
	const float rotationLimitsMaxPitch = 1.5f;

	if (m_mouseDeltaRotation.IsEquivalent(ZERO, MOUSE_DELTA_TRESHOLD))
		return;

	// Apply smoothing filter to the mouse input
	m_mouseDeltaRotation = m_mouseDeltaSmoothingFilter.Push(m_mouseDeltaRotation).Get();

	// Update angular velocity metrics
	m_horizontalAngularVelocity = (m_mouseDeltaRotation.x * rotationSpeed) / frameTime;
	m_averagedHorizontalAngularVelocity.Push(m_horizontalAngularVelocity);

	// Start with updating look orientation from the latest input
	Ang3 ypr = CCamera::CreateAnglesYPR(Matrix33(m_lookOrientation));

	// Yaw
	ypr.x += m_mouseDeltaRotation.x * rotationSpeed;

	// Pitch
	// TODO: Perform soft clamp here instead of hard wall, should reduce rot speed in this direction when close to limit.
	ypr.y = CLAMP(ypr.y + m_mouseDeltaRotation.y * rotationSpeed, rotationLimitsMinPitch, rotationLimitsMaxPitch);

	// Roll (skip)
	ypr.z = 0;

	m_lookOrientation = Quat(CCamera::CreateOrientationYPR(ypr));

	// Reset the mouse delta accumulator every frame
	m_mouseDeltaRotation = ZERO;
}

//Updates the animations
void CPlayerComponent::UpdateAnimation(float frameTime)
{
	const float angularVelocityTurningThreshold = 0.174; // [rad/s]

	// Update tags and motion parameters used for turning
	const bool isTurning = std::abs(m_averagedHorizontalAngularVelocity.Get()) > angularVelocityTurningThreshold;
	m_pAnimationComponent->SetTagWithId(m_rotateTagId, isTurning);
	if (isTurning)
	{
		// TODO: This is a very rough predictive estimation of eMotionParamID_TurnAngle that could easily be replaced with accurate reactive motion 
		// if we introduced IK look/aim setup to the character's model and decoupled entity's orientation from the look direction derived from mouse input.

		const float turnDuration = 1.0f; // Expect the turning motion to take approximately one second.
		m_pAnimationComponent->SetMotionParameter(eMotionParamID_TurnAngle, m_horizontalAngularVelocity * turnDuration);
	}

	// Update active fragment
	const auto& desiredFragmentId = m_pCharacterController->IsWalking() ? m_walkFragmentId : m_idleFragmentId;
	if (m_activeFragmentId != desiredFragmentId)
	{
		m_activeFragmentId = desiredFragmentId;
		m_pAnimationComponent->QueueFragmentWithId(m_activeFragmentId);
	}

	// Update entity rotation as the player turns
	// We only want to affect Z-axis rotation, zero pitch and roll
	Ang3 ypr = CCamera::CreateAnglesYPR(Matrix33(m_lookOrientation));
	ypr.y = 0;
	ypr.z = 0;
	const Quat correctedOrientation = Quat(CCamera::CreateOrientationYPR(ypr));

	// Send updated transform to the entity, only orientation changes
	GetEntity()->SetPosRotScale(GetEntity()->GetWorldPos(), correctedOrientation, Vec3(1, 1, 1));
}

void CPlayerComponent::UpdateCamera(float frameTime)
{
	Ang3 ypr = CCamera::CreateAnglesYPR(Matrix33(m_lookOrientation));

	// Ignore z-axis rotation, that's set by CPlayerAnimations
	ypr.x = 0;

	// Start with changing view rotation to the requested mouse look orientation
	Matrix34 localTransform = IDENTITY;
	localTransform.SetRotation33(CCamera::CreateOrientationYPR(ypr));

	// Offset the player along the forward axis (normally back)
	// Also offset upwards
	const float viewOffsetForward = -1.5f;
	const float viewOffsetUp = 2.f;
	localTransform.SetTranslation(Vec3(0, viewOffsetForward, viewOffsetUp));

	m_pCameraComponent->SetTransformMatrix(localTransform);
}

//Main update function
void CPlayerComponent::Update(float frameTime)
{
	//Checks if the are items to pickup nearby
	CheckForPickup();
}

//Checks if there is items to pickup nearby
void CPlayerComponent::CheckForPickup()
{
	//Create som base variables
	Vec3 pickupOrigin = m_pEntity->GetWorldPos() + (m_pEntity->GetForwardDir() * PICKUP_RANGE);
	IPhysicalEntity **pEntityList = NULL;
	int num = gEnv->pEntitySystem->GetPhysicalEntitiesInBox(pickupOrigin, PICKUP_RANGE, pEntityList);

	float lastDist = 10.f;
	float curDist = 0.f;
	SItemComponent *pNewItem = nullptr;

	//Go through the items and check if it's an pickable item
	//If it is set the item to the new item and set the distance to the object
	for (int i = 0; i < num; i++)
	{
		if (IPhysicalEntity *pPhys = pEntityList[i])
		{
			if (IEntity *pInteractingEntity = gEnv->pEntitySystem->GetEntityFromPhysics(pPhys))
			{
				if (SItemComponent *pPickupItem = pInteractingEntity->GetComponent<SItemComponent>())
				{
					if (pPickupItem->IsPickable())
					{
						const Vec3 newItemPos = pInteractingEntity->GetWorldPos();
						const Vec3 diff = newItemPos - m_pEntity->GetWorldPos();

						curDist = sqrt(powf(diff.x, 2.f) + powf(diff.y, 2.f));

						if (curDist < lastDist)
						{
							lastDist = curDist;
							pNewItem = pPickupItem;
						}
					}
				}
			}
		}
	}

	//Check that it isn't null
	if (pNewItem)
	{
		m_pTargetItem = pNewItem;
		ShowMessage(m_pTargetItem->GetItemName());
	}
}

//Shows a pickup message
void CPlayerComponent::ShowMessage(string name)
{
	Vec2 screenMid = { gEnv->pRenderer->GetWidth() / 2.f, gEnv->pRenderer->GetHeight() / 2.f };
	ColorF pfWhite = { 1.f, 1.f, 1.f, 1.f };

	string pickupMessage = "Pick up " + name;

	gEnv->pRenderer->GetIRenderAuxGeom()->Draw2dLabel(screenMid.x, screenMid.y, 1.5f, pfWhite, true, pickupMessage);

}


