#pragma once

#include "iclientnetworkable.h"
#include "iclientrenderable.h"
#include "iclientthinkable.h"
#include "iclientunknown.h"

enum soundlevel_t // TODO: soundflags.h
{
	SNDLVL_NONE = 0,

	SNDLVL_20dB = 20,			// rustling leaves
	SNDLVL_25dB = 25,			// whispering
	SNDLVL_30dB = 30,			// library
	SNDLVL_35dB = 35,
	SNDLVL_40dB = 40,
	SNDLVL_45dB = 45,			// refrigerator

	SNDLVL_50dB = 50,	// 3.9	// average home
	SNDLVL_55dB = 55,	// 3.0

	SNDLVL_IDLE = 60,	// 2.0	
	SNDLVL_60dB = 60,	// 2.0	// normal conversation, clothes dryer

	SNDLVL_65dB = 65,	// 1.5	// washing machine, dishwasher
	SNDLVL_STATIC = 66,	// 1.25

	SNDLVL_70dB = 70,	// 1.0	// car, vacuum cleaner, mixer, electric sewing machine

	SNDLVL_NORM = 75,
	SNDLVL_75dB = 75,	// 0.8	// busy traffic

	SNDLVL_80dB = 80,	// 0.7	// mini-bike, alarm clock, noisy restaurant, office tabulator, outboard motor, passing snowmobile
	SNDLVL_TALKING = 80,	// 0.7
	SNDLVL_85dB = 85,	// 0.6	// average factory, electric shaver
	SNDLVL_90dB = 90,	// 0.5	// screaming child, passing motorcycle, convertible ride on frw
	SNDLVL_95dB = 95,
	SNDLVL_100dB = 100,	// 0.4	// subway train, diesel truck, woodworking shop, pneumatic drill, boiler shop, jackhammer
	SNDLVL_105dB = 105,			// helicopter, power mower
	SNDLVL_110dB = 110,			// snowmobile drvrs seat, inboard motorboat, sandblasting
	SNDLVL_120dB = 120,			// auto horn, propeller aircraft
	SNDLVL_130dB = 130,			// air raid siren

	SNDLVL_GUNFIRE = 140,	// 0.27	// THRESHOLD OF PAIN, gunshot, jet engine
	SNDLVL_140dB = 140,	// 0.2

	SNDLVL_150dB = 150,	// 0.2

	SNDLVL_180dB = 180,			// rocket launching

	// NOTE: Valid soundlevel_t values are 0-255.
	//       256-511 are reserved for sounds using goldsrc compatibility attenuation.
};

struct SoundInfo_t // TODO: soundinfo.h
{
	int				nSequenceNumber;
	int				nEntityIndex;
	int				nChannel;
	const char* pszName;		// UNDONE: Make this a FilenameHandle_t to avoid bugs with arrays of these
	Vector			vOrigin;
	Vector			vDirection;
	float			fVolume;
	soundlevel_t	Soundlevel;
	bool			bLooping;
	int				nPitch;
	int				nSpecialDSP;
	Vector			vListenerOrigin;
	int				nFlags;
	int 			nSoundNum;
	float			fDelay;
	bool			bIsSentence;
	bool			bIsAmbient;
	int				nSpeakerEntity;
};

struct SpatializationInfo_t // TODO: soundinfo.h
{
	typedef enum
	{
		SI_INCREATION = 0,
		SI_INSPATIALIZATION
	} SPATIALIZATIONTYPE;

	// Inputs
	SPATIALIZATIONTYPE	type;
	// Info about the sound, channel, origin, direction, etc.
	SoundInfo_t			info;

	// Requested Outputs ( NULL == not requested )
	Vector* pOrigin;
	Angle* pAngles;
	float* pflRadius;
};

class IClientEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable, public IClientThinkable
{
public:
	// Delete yourself.
	virtual void			Release(void) = 0;

	// Network origin + angles
	virtual const Vector& GetAbsOrigin(void) const = 0;
	virtual const Angle& GetAbsAngles(void) const = 0;

	virtual void* GetMouth(void) = 0; // TODO: CMouthInfo

	// Retrieve sound spatialization info for the specified sound on this entity
	// Return false to indicate sound is not audible
	virtual bool			GetSoundSpatialization(SpatializationInfo_t& info) = 0;
};