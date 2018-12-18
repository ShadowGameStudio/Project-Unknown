#pragma once

#include "StdAfx.h"

#include <CrySchematyc\ResourceTypes.h>
#include <CrySchematyc\Reflection\TypeDesc.h>

#include <CrySchematyc/MathTypes.h>
#include <CrySchematyc/IObject.h>
#include <CrySchematyc/Utils/SharedString.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>

enum ItemGeometrySlot
{
	GEOMETRY_SLOT = 0
};

//Rendering
struct SRenderProperties
{
	inline bool operator==(const SRenderProperties& rhs) const { return 0 == memcmp(this, &rhs, sizeof(rhs)); }
	inline bool operator!=(const SRenderProperties& rhs) const { return 0 != memcmp(this, &rhs, sizeof(rhs)); }

	Schematyc::GeomFileName geomPath;
	Schematyc::MaterialFileName material;
};

static void ReflectType(Schematyc::CTypeDesc<SRenderProperties>& desc)
{
	desc.SetGUID("{96D11B27-FF88-4648-9BAD-029F5BBBF669}"_cry_guid);
	desc.SetLabel("Render Properties");
	desc.SetDescription("The items render properties");

	desc.AddMember(&SRenderProperties::geomPath, 'geom', "GeometryPath", "Geometry path", "The path of the geometry to use on the item", "");
	desc.AddMember(&SRenderProperties::material, 'mat', "MaterialPath", "Material path", "The path of the material to use on the geometry", "");
}

//Physics
struct SPhysicsProperties
{
	inline bool operator==(const SPhysicsProperties& rhs) const { return 0 == memcmp(this, &rhs, sizeof(rhs)); }
	inline bool operator!=(const SPhysicsProperties& rhs) const { return 0 != memcmp(this, &rhs, sizeof(rhs)); }

	float mass;
};

static void ReflectType(Schematyc::CTypeDesc<SPhysicsProperties>& desc)
{
	desc.SetGUID("{51FEA35E-C836-4FFA-B279-F3B6AC89C96C}"_cry_guid);
	desc.SetLabel("Physics Properties");
	desc.SetDescription("The items physics properties");

	desc.AddMember(&SPhysicsProperties::mass, 'mass', "Mass", "Mass", "The mass of the item", 1.f);
}

//Main properties
struct SItemProperties
{
	inline bool operator==(const SItemProperties& rhs) const { return 0 == memcmp(this, &rhs, sizeof(rhs)); }
	inline bool operator!=(const SItemProperties& rhs) const { return 0 != memcmp(this, &rhs, sizeof(rhs)); }

	SRenderProperties sRenderProperties, sPrevRenderProperties;
	SPhysicsProperties sPhysicsProperties;

	//Item specific
	int itemLevel;
	string itemName;
};

static void ReflectType(Schematyc::CTypeDesc<SItemProperties>& desc)
{
	desc.SetGUID("{9A71B672-3C40-4E11-929A-773AC244F2DF}"_cry_guid);
	desc.SetLabel("Item Properties");
	desc.SetDescription("The items properties");

	desc.AddMember(&SItemProperties::sRenderProperties, 'renp', "RenderProperties", "Render Properties", "The items render properties", SRenderProperties());
	desc.AddMember(&SItemProperties::sPhysicsProperties, 'spp', "PhysicsProperties", "Physics Properties", "The items physics properties", SPhysicsProperties());
	desc.AddMember(&SItemProperties::itemLevel, 'il', "ItemLevel", "Item Level", "The items level", 1);
}