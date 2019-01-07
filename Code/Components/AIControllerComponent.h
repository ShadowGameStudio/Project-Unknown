#pragma once
#include <CryEntitySystem/IEntityComponent.h>
#include <CryEntitySystem/IEntity.h>

/*-------------------------------------------------------------------------------
Author : Ivar Jönsson
Project : PU
Purpose : Handels all the AI controlling
-------------------------------------------------------------------------------*/

class CAIControllerComponent final : public IEntityComponent {

	struct SControllerParams
	{
		inline bool operator==(const SControllerParams &rhs) const { return 0 == memcmp(this, &rhs, sizeof(rhs)); }
		inline bool operator!=(const SControllerParams &rhs) const { return 0 != memcmp(this, &rhs, sizeof(rhs)); }

		float mass;
		float radius;
		float height;

		static void ReflectType(Schematyc::CTypeDesc<SControllerParams>& desc)
		{
			desc.SetGUID("{CF9DDAAE-54E7-40C8-B810-0673A7E55210}"_cry_guid);
			desc.SetLabel("Controller Params");
			desc.SetDescription("All the parameters for the AI's controller");
			
			desc.AddMember(&SControllerParams::height, 'fhe', "Height", "Height", "Sets the height of the controller in meters", 2.f);
			desc.AddMember(&SControllerParams::mass, 'fma', "Mass", "Mass", "Sets the mass of the AI in kilos", 80.f);
			desc.AddMember(&SControllerParams::radius, 'fra', "Radius", "Radius", "Sets the radius of the controller", 1.f);
		}

	};

public:
	CAIControllerComponent() = default;
	virtual ~CAIControllerComponent() {}

	virtual void Initialize() final;
	virtual void ProcessEvent(const SEntityEvent &event) final;
	virtual uint64 GetEventMask() const final;

	// Reflect type to set a unique identifier for this component
	// and provide additional information to expose it in the sandbox
	static void ReflectType(Schematyc::CTypeDesc<CAIControllerComponent>& desc) {
		desc.SetGUID("{6C58CA90-B36B-4F77-86BE-FC194D640922}"_cry_guid);
		desc.SetEditorCategory("AI");
		desc.SetLabel("AI Controller Component");

		desc.SetDescription("Handels the control of the AI");
		desc.SetComponentFlags({ IEntityComponent::EFlags::Transform, IEntityComponent::EFlags::Socket, IEntityComponent::EFlags::Attach });
		desc.AddMember(&CAIControllerComponent::m_Controller, 'con', "ControllerParams", "Controller Parameters", "The properties for the AI", SControllerParams());
	}

public:

	//Functions
	virtual void AddVelocity(const Vec3 vel);
	virtual void MoveTo(const Vec3 cords);
	virtual void Physicalize();

	bool IsWalking() const { return m_Velocity.GetLength2D() > 0.2f && m_OnGround; }

	Ang3 CreateAnglesYPR(const Matrix33& m);
	Ang3 CreateAnglesYPR(const Vec3& vDir, f32 r);
	Matrix33 CreateOrientationYPR(const Ang3& ypr);

protected:

	//Vars
	bool m_OnGround;
	Vec3 m_Velocity = ZERO;

private:

	//Member vars
	SControllerParams m_Controller;

};

