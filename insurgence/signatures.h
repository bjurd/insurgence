#pragma once

// client.dll
#define CL_CreateMove "40 53 48 83 EC ? 83 C9 ? 0F 29 74 24"

#define C_BaseEntity_GetAbsOrigin "40 53 48 83 EC ? 48 8B D9 E8 ? ? ? ? 48 8D 83 ? ? ? ? 48 83 C4 ? 5B C3 CC CC CC CC CC 48 89 5C 24" // May actually be on IClientEntity
#define C_BaseEntity_GetAbsAngles "40 53 48 83 EC ? 48 8B D9 E8 ? ? ? ? 48 8D 83 ? ? ? ? 48 83 C4 ? 5B C3 CC CC CC CC CC 40 53"
// #define C_BaseEntity_GetClassName "FF 90 ? ? ? ? 48 8B E8 48 85 C0 74 ? 80 38"
// #define C_BaseEntity_GetPredDescMap "48 8D 05 ? ? ? ? C3 CC CC CC CC CC CC CC CC 0F B6 81 ? ? ? ? C3 CC CC CC CC CC CC CC CC 48 63 C1"

#define IClientRenderable_SetupBones "40 55 57 48 8D AC 24 ? ? ? ? B8 ? ? ? ? E8 ? ? ? ? 48 2B E0 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 80 3D"

#define Render_GetMainViewOrigin "48 63 C1 48 8D 0C 40 48 8D 05 ? ? ? ? 48 8D 04 88 C3 CC CC CC CC CC CC CC CC CC CC CC CC CC 48 63 C1 48 8D 0C 40 48 8D 05 ? ? ? ? 48 8D 04 88 C3 CC CC CC CC CC CC CC CC CC CC CC CC CC 48 63 C1 48 8D 0C 40 48 8D 05 ? ? ? ? 48 8D 04 88 C3 CC CC CC CC CC CC CC CC CC CC CC CC CC 40 53"

// engine.dll
#define Engine_GetClientState "48 8B 05 ? ? ? ? 48 83 C0" // TODO: This is weird

#define CL_Move "48 89 5C 24 ? 57 48 83 EC ? 0F 29 74 24 ? 0F B6 DA"
#define CL_SendMove "48 89 5C 24 ? 57 B8 ? ? ? ? E8 ? ? ? ? 48 2B E0 48 8B 05 ? ? ? ? 48 33 C4 48 89 84 24 ? ? ? ? E8"
