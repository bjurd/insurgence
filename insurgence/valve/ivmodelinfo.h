#pragma once

#include "studio.h"

class Vector;
class Angle;
class IClientRenderable;
struct model_t;

class IVModelInfo
{
public:
	virtual							~IVModelInfo(void) {}

	// Returns model_t* pointer for a model given a precached or dynamic model index.
	virtual const model_t* GetModel(int modelindex) = 0;

	// Returns index of model by name for precached or known dynamic models.
	// Does not adjust reference count for dynamic models.
	virtual int						GetModelIndex(const char* name) const = 0;

	// Returns name of model
	virtual const char* GetModelName(const model_t* model) const = 0;
	virtual void* GetVCollide(const model_t* model) = 0; // TODO: vcollide_t
	virtual void* GetVCollide(int modelindex) = 0; // TODO: vcollide_t
	virtual void					GetModelBounds(const model_t* model, Vector& mins, Vector& maxs) const = 0;
	virtual	void					GetModelRenderBounds(const model_t* model, Vector& mins, Vector& maxs) const = 0;
	virtual int						GetModelFrameCount(const model_t* model) const = 0;
	virtual int						GetModelType(const model_t* model) const = 0;
	virtual void* GetModelExtraData(const model_t* model) = 0;
	virtual bool					ModelHasMaterialProxy(const model_t* model) const = 0;
	virtual bool					IsTranslucent(model_t const* model) const = 0;
	virtual bool					IsTranslucentTwoPass(const model_t* model) const = 0;
	virtual void					RecomputeTranslucency(const model_t* model, int nSkin, int nBody, void /*IClientRenderable*/* pClientRenderable, float fInstanceAlphaModulate = 1.0f) = 0;
	virtual int						GetModelMaterialCount(const model_t* model) const = 0;
	virtual void					GetModelMaterials(const model_t* model, int count, void* ppMaterial) = 0; // IMaterial**
	virtual bool					IsModelVertexLit(const model_t* model) const = 0;
	virtual const char* GetModelKeyValueText(const model_t* model) = 0;
	virtual bool					GetModelKeyValue(const model_t* model, void* buf) = 0; // supports keyvalue blocks in submodels // TODO: CUtlBuffer&
	virtual bool GetModelKeyValues(const model_t* model) = 0; // Something NWI added /shrug
	virtual float					GetModelRadius(const model_t* model) = 0;

	virtual const studiohdr_t* FindModel(const studiohdr_t* pStudioHdr, void** cache, const char* modelname) const = 0;
	virtual const studiohdr_t* FindModel(void* cache) const = 0;
	virtual	void* GetVirtualModel(const studiohdr_t* pStudioHdr) const = 0; // TODO: virtualmodel_t*
	virtual char* GetAnimBlock(const studiohdr_t* pStudioHdr, int iBlock) const = 0;

	virtual bool HasAnimBlockBeenPreloaded(const studiohdr_t*) = 0; // Something NWI added /shrug

	// Available on client only!!!
	virtual void					GetModelMaterialColorAndLighting(const model_t* model, Vector const& origin,
		Angle const& angles, void* pTrace, // TODO: trace_t*
		Vector& lighting, Vector& matColor) = 0;
	virtual void					GetIlluminationPoint(const model_t* model, IClientRenderable* pRenderable, Vector const& origin,
		Angle const& angles, Vector* pLightingCenter) = 0;

	virtual int						GetModelContents(int modelIndex) = 0;
	virtual studiohdr_t* GetStudiomodel(const model_t* mod) = 0;
};
