#pragma once

#include "vector.h"
#include "ihandleentity.h"
#include "icollideable.h"
#include "gametrace.h"
#include "cmodel.h"

enum TraceType_t
{
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,				// NOTE: This does *not* test static props!!!
	TRACE_ENTITIES_ONLY,			// NOTE: This version will *not* test static props
	TRACE_EVERYTHING_FILTER_PROPS,	// NOTE: This version will pass the IHandleEntity for props through the filter, unlike all other filters
};

class ITraceFilter
{
public:
	virtual bool ShouldHitEntity(IHandleEntity* pEntity, int contentsMask) = 0;
	virtual TraceType_t	GetTraceType() const = 0;
};

class CTraceFilter : public ITraceFilter
{
public:
	virtual TraceType_t	GetTraceType() const
	{
		return TRACE_EVERYTHING;
	}
};

class CTraceFilterEntitiesOnly : public ITraceFilter
{
public:
	virtual TraceType_t	GetTraceType() const
	{
		return TRACE_ENTITIES_ONLY;
	}
};

typedef bool (*ShouldHitFunc_t)(IHandleEntity* pHandleEntity, int contentsMask);

class CTraceFilterSimple : public CTraceFilter
{
public:
	// It does have a base, but we'll never network anything below here..
	// DECLARE_CLASS_NOBASE(CTraceFilterSimple);

	CTraceFilterSimple(const IHandleEntity* passentity, int collisionGroup, ShouldHitFunc_t pExtraShouldHitCheckFn = NULL)
	{
		this->m_pPassEnt = passentity;
		this->m_collisionGroup = collisionGroup;
		this->m_pExtraShouldHitCheckFunction = pExtraShouldHitCheckFn;
	}

	virtual bool ShouldHitEntity(IHandleEntity* pHandleEntity, int contentsMask) { return true; }; // TODO: Actual trace logic
	virtual void SetPassEntity(const IHandleEntity* pPassEntity) { m_pPassEnt = pPassEntity; }
	virtual void SetCollisionGroup(int iCollisionGroup) { m_collisionGroup = iCollisionGroup; }

	const IHandleEntity* GetPassEntity(void) { return m_pPassEnt; }

public:
	const IHandleEntity* m_pPassEnt;
	int m_collisionGroup;
	ShouldHitFunc_t m_pExtraShouldHitCheckFunction;
};

class IEngineTrace
{
public:
	// Returns the contents mask + entity at a particular world-space position
	virtual int		GetPointContents(const Vector& vecAbsPosition, IHandleEntity** ppEntity = NULL) = 0;

	virtual int		GetPointContents_WorldOnly(const Vector& vecAbsPosition) = 0; // NWI

	// Get the point contents, but only test the specific entity. This works
	// on static props and brush models.
	//
	// If the entity isn't a static prop or a brush model, it returns CONTENTS_EMPTY and sets
	// bFailed to true if bFailed is non-null.
	virtual int		GetPointContents_Collideable(ICollideable* pCollide, const Vector& vecAbsPosition) = 0;

	// Traces a ray against a particular entity
	virtual void	ClipRayToEntity(const Ray_t& ray, unsigned int fMask, IHandleEntity* pEnt, CGameTrace* pTrace) = 0;

	// Traces a ray against a particular entity
	virtual void	ClipRayToCollideable(const Ray_t& ray, unsigned int fMask, ICollideable* pCollide, CGameTrace* pTrace) = 0;

	// A version that simply accepts a ray (can work as a traceline or tracehull)
	virtual void	TraceRay(const Ray_t& ray, unsigned int fMask, ITraceFilter* pTraceFilter, CGameTrace* pTrace) = 0;

	// A version that sets up the leaf and entity lists and allows you to pass those in for collision.
	virtual void	SetupLeafAndEntityListRay(const Ray_t& ray, void* traceData) = 0; // TODO: CTraceListData&
	virtual void    SetupLeafAndEntityListBox(const Vector& vecBoxMin, const Vector& vecBoxMax, void* traceData) = 0; // TODO: CTraceListData&
	virtual void	TraceRayAgainstLeafAndEntityList(const Ray_t& ray, void* traceData, unsigned int fMask, ITraceFilter* pTraceFilter, CGameTrace* pTrace) = 0; // TODO: CTraceListData&

	// A version that sweeps a collideable through the world
	// abs start + abs end represents the collision origins you want to sweep the collideable through
	// vecAngles represents the collision angles of the collideable during the sweep
	virtual void	SweepCollideable(ICollideable* pCollide, const Vector& vecAbsStart, const Vector& vecAbsEnd,
		const Angle& vecAngles, unsigned int fMask, ITraceFilter* pTraceFilter, CGameTrace* pTrace) = 0;

	// Enumerates over all entities along a ray
	// If triggers == true, it enumerates all triggers along a ray
	virtual void	EnumerateEntities(const Ray_t& ray, bool triggers, void* pEnumerator) = 0; // TODO: IEntityEnumerator*

	// Same thing, but enumerate entitys within a box
	virtual void	EnumerateEntities(const Vector& vecAbsMins, const Vector& vecAbsMaxs, void* pEnumerator) = 0; // TODO: IEntityEnumerator*

	// Convert a handle entity to a collideable.  Useful inside enumer
	virtual ICollideable* GetCollideable(IHandleEntity* pEntity) = 0;

	// HACKHACK: Temp for performance measurments
	virtual int GetStatByIndex(int index, bool bClear) = 0;

	//finds brushes in an AABB, prone to some false positives
	virtual void GetBrushesInAABB(const Vector& vMins, const Vector& vMaxs, void* pOutput, int iContentsMask = 0xFFFFFFFF) = 0; // TODO: CUtlVector<int>

	//Creates a CPhysCollide out of all displacements wholly or partially contained in the specified AABB
	virtual void* GetCollidableFromDisplacementsInAABB(const Vector& vMins, const Vector& vMaxs) = 0; // TODO: CPhysCollide

	virtual int GetNumDisplacements() = 0; // NWI
	virtual int GetDisplacementMesh(int, void*) = 0; // NWI, TODO: virtualmeshlist_t*

	//retrieve brush planes and contents, returns true if data is being returned in the output pointers, false if the brush doesn't exist
	virtual bool GetBrushInfo(int iBrush, void* pPlanesOut, int* pContentsOut) = 0; // TODO: CUtlVector<Vector4D>*

	virtual bool PointOutsideWorld(const Vector& ptTest) = 0; //Tests a point to see if it's outside any playable area

	// Walks bsp to find the leaf containing the specified point
	virtual int GetLeafContainingPoint(const Vector& ptTest) = 0;
};

class CEngineTrace : public IEngineTrace // Actually CEngineTraceClient
{
public:
};
