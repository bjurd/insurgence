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

	inline char* const GetName(void) const
	{
		return ((char*)this) + sznameindex;
	}

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

	int					proctype;
	int					procindex;
	mutable int			physicsbone;

	int					surfacepropidx;

	int					contents;

	int					unused[8];
};

struct mstudiobbox_t
{
	// static datamap_t	m_DataMap;

	int					bone;

	int					group;
	Vector				bbmin;
	Vector				bbmax;

	int					szhitboxnameindex;

	int					unused[8];
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

	Vector				eyeposition;

	Vector				illumposition;

	Vector				hull_min;
	Vector				hull_max;

	Vector				view_bbmin;
	Vector				view_bbmax;

	int					flags;

	int					numbones;
	int					boneindex;

	inline mstudiobone_t* GetBone(int i) const
	{
		return (mstudiobone_t*)(((char*)this) + boneindex) + i;
	};

	int					numbonecontrollers;
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

	int					numlocalanim;
	int					localanimindex;

	int					numlocalseq;
	int					localseqindex;

	mutable int			activitylistversion;
	mutable int			eventsindexed;

	int					numtextures;
	int					textureindex;

	int					numcdtextures;
	int					cdtextureindex;

	int					numskinref;
	int					numskinfamilies;
	int					skinindex;

	int					numbodyparts;
	int					bodypartindex;

	int					numlocalattachments;
	int					localattachmentindex;

	int					numlocalnodes;
	int					localnodeindex;
	int					localnodenameindex;

	int					numflexdesc;
	int					flexdescindex;

	int					numflexcontrollers;
	int					flexcontrollerindex;

	int					numflexrules;
	int					flexruleindex;

	int					numikchains;
	int					ikchainindex;

	int					nummouths;
	int					mouthindex;

	int					numlocalposeparameters;
	int					localposeparamindex;

	int					surfacepropindex;

	int					keyvalueindex;
	int					keyvaluesize;

	int					numlocalikautoplaylocks;
	int					localikautoplaylockindex;

	float				mass;
	int					contents;

	int					numincludemodels;
	int					includemodelindex;

	mutable void* virtualModel;

	int					szanimblocknameindex;

	int					numanimblocks;
	int					animblockindex;

	mutable void* animblockModel;

	int					bonetablebynameindex;

	void* pVertexBase;
	void* pIndexBase;

	char				constdirectionallightdot;

	char				rootLOD;

	char				numAllowedRootLODs;

	char				unused[1];

	int					unused4;

	int					numflexcontrollerui;
	int					flexcontrolleruiindex;

	float				flVertAnimFixedPointScale;

	int					unused2[1];

	int					studiohdr2index;

	int					unused3[1];
};
