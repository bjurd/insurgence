#pragma once

#include "vector.h"
#include "matrix.h"
#include "buttoncode.h"

class IVEngineClient
{
public:
	virtual int					GetIntersectingSurfaces(
		const model_t* model,
		const Vector& vCenter,
		const float radius,
		const bool bOnlyVisibleSurfaces,	// Only return surfaces visible to vCenter.
		void* pInfos, // TODO: SurfInfo
		const int nMaxInfos) = 0;

	virtual Vector				GetLightForPoint(const Vector& pos, bool bClamp) = 0;

	virtual void* TraceLineMaterialAndLighting(const Vector& start, const Vector& end, // TODO: IMaterial
		Vector& diffuseLightColor, Vector& baseColor) = 0;

	// Given an input text buffer data pointer, parses a single token into the variable token and returns the new
	//  reading position
	virtual const char* ParseFile(const char* data, char* token, int maxlen) = 0;
	virtual bool				CopyLocalFile(const char* source, const char* destination) = 0;

	// Gets the dimensions of the game window
	virtual void				GetScreenSize(int& width, int& height) = 0;

	// Forwards szCmdString to the server, sent reliably if bReliable is set
	virtual void				ServerCmd(const char* szCmdString, bool bReliable = true) = 0;
	// Inserts szCmdString into the command buffer as if it was typed by the client to his/her console.
	// Note: Calls to this are checked against FCVAR_CLIENTCMD_CAN_EXECUTE (if that bit is not set, then this function can't change it).
	//       Call ClientCmd_Unrestricted to have access to FCVAR_CLIENTCMD_CAN_EXECUTE vars.
	virtual void				ClientCmd(const char* szCmdString) = 0;

	// Fill in the player info structure for the specified player index (name, model, etc.)
	virtual bool				GetPlayerInfo(int ent_num, void* pinfo) = 0; // TODO: player_info_t

	// Retrieve the player entity number for a specified userID
	virtual int					GetPlayerForUserID(int userID) = 0;

	// Retrieves text message system information for the specified message by name
	virtual void* TextMessageGet(const char* pName) = 0; // TODO: client_textmessage_t

	// Returns true if the console is visible
	virtual bool				Con_IsVisible(void) = 0;

	// Get the entity index of the local player
	virtual int					GetLocalPlayer(void) = 0;

	virtual const model_t* LoadModel(const char* pName, bool bProp = false) = 0;

	// Get accurate, sub-frame clock ( profiling use )
	virtual float				Time(void) = 0;

	// Get the exact server timesstamp ( server time ) from the last message received from the server
	virtual float				GetLastTimeStamp(void) = 0;

	// Given a CAudioSource (opaque pointer), retrieve the underlying CSentence object ( stores the words, phonemes, and close
	//  captioning data )
	virtual void* GetSentence(void* pAudioSource) = 0; // TODO: CSentence, CAudioSource
	// Given a CAudioSource, determines the length of the underlying audio file (.wav, .mp3, etc.)
	virtual float				GetSentenceLength(void* pAudioSource) = 0; // TODO: CAudioSource
	// Returns true if the sound is streaming off of the hard disk (instead of being memory resident)
	virtual bool				IsStreaming(void* pAudioSource) const = 0; // TODO: CAudioSource

	// Copy current view orientation into va
	virtual void				GetViewAngles(Angle& va) = 0;
	// Set current view orientation from va
	virtual void				SetViewAngles(Angle& va) = 0;

	// Retrieve the current game's maxclients setting
	virtual int					GetMaxClients(void) = 0;

	// Given the string pBinding which may be bound to a key, 
	//  returns the string name of the key to which this string is bound. Returns NULL if no such binding exists
	virtual	const char* Key_LookupBinding(const char* pBinding) = 0;

	// Given the name of the key "mouse1", "e", "tab", etc., return the string it is bound to "+jump", "impulse 50", etc.
	virtual const char* Key_BindingForKey(ButtonCode_t code) = 0;

	// key trapping (for binding keys)
	virtual void				StartKeyTrapMode(void) = 0;
	virtual bool				CheckDoneKeyTrapping(ButtonCode_t& code) = 0;

	// Returns true if the player is fully connected and active in game (i.e, not still loading)
	virtual bool				IsInGame(void) = 0;
	// Returns true if the player is connected, but not necessarily active in game (could still be loading)
	virtual bool				IsConnected(void) = 0;
	// Returns true if the loading plaque should be drawn
	virtual bool				IsDrawingLoadingImage(void) = 0;

	// Prints the formatted string to the notification area of the screen ( down the right hand edge
	//  numbered lines starting at position 0
	virtual void				Con_NPrintf(int pos, const char* fmt, ...) = 0; // TODO: PRINTF_FORMAT_STRING
	// Similar to Con_NPrintf, but allows specifying custom text color and duration information
	virtual void				Con_NXPrintf(const struct con_nprint_s* info, const char* fmt, ...) = 0; // TODO: PRINTF_FORMAT_STRING

	// Is the specified world-space bounding box inside the view frustum?
	virtual int					IsBoxVisible(const Vector& mins, const Vector& maxs) = 0;

	// Is the specified world-space boudning box in the same PVS cluster as the view origin?
	virtual int					IsBoxInViewCluster(const Vector& mins, const Vector& maxs) = 0;

	// Returns true if the specified box is outside of the view frustum and should be culled
	virtual bool				CullBox(const Vector& mins, const Vector& maxs) = 0;

	// Allow the sound system to paint additional data (during lengthy rendering operations) to prevent stuttering sound.
	virtual void				Sound_ExtraUpdate(void) = 0;

	// Get the current game directory ( e.g., hl2, tf2, cstrike, hl1 )
	virtual const char* GetGameDirectory(void) = 0;

	// Get access to the world to screen transformation matrix
	virtual const VMatrix& WorldToScreenMatrix() = 0;

	// Get the matrix to move a point from world space into view space
	// (translate and rotate so the camera is at the origin looking down X).
	virtual const VMatrix& WorldToViewMatrix() = 0;
};