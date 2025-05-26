#pragma once

#include "datamap.h"
#include "vector.h"
#include "matrix.h"

#define MAXSTUDIOBONES				256

#define BONE_CALCULATE_MASK			0x1F
#define BONE_PHYSICALLY_SIMULATED	0x01	// bone is physically simulated when physics are active
#define BONE_PHYSICS_PROCEDURAL		0x02	// procedural when physics is active
#define BONE_ALWAYS_PROCEDURAL		0x04	// bone is always procedurally animated
#define BONE_SCREEN_ALIGN_SPHERE	0x08	// bone aligns to the screen, not constrained in motion.
#define BONE_SCREEN_ALIGN_CYLINDER	0x10	// bone aligns to the screen, constrained by it's own axis.

#define BONE_USED_MASK				0x0007FF00
#define BONE_USED_BY_ANYTHING		0x0007FF00
#define BONE_USED_BY_HITBOX			0x00000100	// bone (or child) is used by a hit box
#define BONE_USED_BY_ATTACHMENT		0x00000200	// bone (or child) is used by an attachment point
#define BONE_USED_BY_VERTEX_MASK	0x0003FC00
#define BONE_USED_BY_VERTEX_LOD0	0x00000400	// bone (or child) is used by the toplevel model via skinned vertex
#define BONE_USED_BY_VERTEX_LOD1	0x00000800	
#define BONE_USED_BY_VERTEX_LOD2	0x00001000  
#define BONE_USED_BY_VERTEX_LOD3	0x00002000
#define BONE_USED_BY_VERTEX_LOD4	0x00004000
#define BONE_USED_BY_VERTEX_LOD5	0x00008000
#define BONE_USED_BY_VERTEX_LOD6	0x00010000
#define BONE_USED_BY_VERTEX_LOD7	0x00020000
#define BONE_USED_BY_BONE_MERGE		0x00040000	// bone is available for bone merge to occur against it

#define BONE_USED_BY_VERTEX_AT_LOD(lod) ( BONE_USED_BY_VERTEX_LOD0 << (lod) )
#define BONE_USED_BY_ANYTHING_AT_LOD(lod) ( ( BONE_USED_BY_ANYTHING & ~BONE_USED_BY_VERTEX_MASK ) | BONE_USED_BY_VERTEX_AT_LOD(lod) )

#define	HITGROUP_GENERIC	0 // shareddefs.h
#define	HITGROUP_HEAD		1
#define	HITGROUP_CHEST		2
#define	HITGROUP_STOMACH	3
#define HITGROUP_LEFTARM	4	
#define HITGROUP_RIGHTARM	5
#define HITGROUP_LEFTLEG	6
#define HITGROUP_RIGHTLEG	7
#define HITGROUP_GEAR		10

struct mstudiobone_t
{
	// static datamap_t	m_DataMap;

	int					sznameindex;

	int		 			parent;
	int					bonecontroller[6];

	Vector				pos;
	Quaternion			quat;
	RadianEuler			rot;

	Vector				posscale;
	Vector				rotscale;

	matrix3x4_t			poseToBone;
	Quaternion			qAlignment;
	int					flags;
};

struct mstudiobbox_t
{
	// static datamap_t	m_DataMap;

	int					bone;

	int					group;				// intersection group
	Vector				bbmin;				// bounding box
	Vector				bbmax;

	int					szhitboxnameindex;	// offset to the name of the hitbox.
};

struct mstudiohitboxset_t
{
	// static datamap_t	m_DataMap;

	int					sznameindex;

	int					numhitboxes;
	int					hitboxindex;

	inline mstudiobbox_t* GetHitbox(int i) const
	{
		return (mstudiobbox_t*)(((char*)this) + hitboxindex) + i;
	};
};

struct studiohdr_t
{
	// static datamap_t	m_DataMap;

	int					id;
	int					version;

	int					checksum;

	char				name[64];
	int					length;

	Vector				eyeposition;	// ideal eye position

	Vector				illumposition;	// illumination center

	Vector				hull_min;		// ideal movement hull size
	Vector				hull_max;

	Vector				view_bbmin;		// clipping bounding box
	Vector				view_bbmax;

	int					flags;

	int					numbones;			// bones
	int					boneindex;

	inline mstudiobone_t* GetBone(int i) const
	{
		return (mstudiobone_t*)(((char*)this) + boneindex) + i;
	};

	int					numbonecontrollers;		// bone controllers
	int					bonecontrollerindex;

	int					numhitboxsets;
	int					hitboxsetindex;

	mstudiohitboxset_t* GetHitboxSet(int i) const
	{
		return (mstudiohitboxset_t*)(((char*)this) + hitboxsetindex) + i;
	};

	inline mstudiobbox_t* GetHitbox(int i, int set) const
	{
		mstudiohitboxset_t const* Set = GetHitboxSet(set);

		if (!Set)
			return NULL;

		return Set->GetHitbox(i);
	};
};
