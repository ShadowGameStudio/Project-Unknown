#pragma once
#include <CryEntitySystem/IEntityComponent.h>
#include <CryEntitySystem/IEntity.h>

/*-------------------------------------------------------------------------------
Author : Ivar Jönsson
Project : Project Unknown
Purpose : Handels all the health
-------------------------------------------------------------------------------*/

class CHealthComponent final : public IEntityComponent {

	struct SHealthProperties
	{
		inline bool operator==(const SHealthProperties& rhs) const { return 0 == memcmp(this, &rhs, sizeof(rhs)); }
		inline bool operator!=(const SHealthProperties& rhs) const { return 0 != memcmp(this, &rhs, sizeof(rhs)); }

		float health;
		float regenerationRatio;
		bool alwaysRegenerate;

		static void ReflectType(Schematyc::CTypeDesc<SHealthProperties>& desc)
		{
			desc.SetGUID("{DC4C4436-70CF-4B49-A215-640F5467A2F3}"_cry_guid);
			desc.SetLabel("Health Properties");
			desc.SetDescription("All the properties for the entity's health");
			
			desc.AddMember(&SHealthProperties::health, 'helt', "EntityHealth", "Entity health", "The health of the entity it's attached to", 0.f);
			desc.AddMember(&SHealthProperties::regenerationRatio, 'reg', "RegenerationRatio", "Regeneration ratio", "The ratio at to heal the entity", 0.f);
			desc.AddMember(&SHealthProperties::alwaysRegenerate, 'ar', "AlwaysRegenerate", "Always regenerate", "Set to true if you want the entity to alway regenerate", false);
		}
	};

public:
	CHealthComponent() = default;
	virtual ~CHealthComponent() {}

	virtual void Initialize() override;
	virtual uint64 GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	// and provide additional information to expose it in the sandbox
	static void ReflectType(Schematyc::CTypeDesc<CHealthComponent>& desc) {
		desc.SetGUID("{48907003-BB24-48D0-921B-C0C54F413E75}"_cry_guid);
		desc.SetEditorCategory("Entities");
		desc.SetLabel("Health Component");
		desc.SetDescription("Handels the health for all entities with health");

		desc.AddMember(&CHealthComponent::m_SHealthProperties, 'hpr', "HealthProperties", "Health Properties", "The properties for the health of an entity", SHealthProperties());
		desc.SetComponentFlags({ IEntityComponent::EFlags::Transform, IEntityComponent::EFlags::Socket, IEntityComponent::EFlags::Attach });
	}

public:

	//Functions
	void RegenerateHealth(float frameTime);
	void KillEntity();
	void AddHealth(float amount) { GetHealthProperties()->health += amount; }

	//Getting
	SHealthProperties *GetHealthProperties() { return &m_SHealthProperties; }
	float GetHealth() { return GetHealthProperties()->health; }
	float GetRegenerationRatio() { return GetHealthProperties()->regenerationRatio; }

	bool GetAlwaysRegenerate() { return GetHealthProperties()->alwaysRegenerate; }

	//Setting
	void SetHealth(float amount) { GetHealthProperties()->health = amount; }
	void SetAlwaysRegenerate(bool state) { GetHealthProperties()->alwaysRegenerate = state; }
	void SetRegenerationRatio(float ratio) { GetHealthProperties()->regenerationRatio = ratio; }

private:

	//Member vars
	SHealthProperties m_SHealthProperties;
	bool m_IsAlive = true;
	bool m_ShouldRegenerate = true;
};

