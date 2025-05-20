#pragma once

class CClientState // TODO: CBaseClientState, CClientFrameManager
{
public:
	float LastTickTime;

	bool InSimulation;

	int OldTickCount;
	float TickRemainder;
	float FrameTime;

	int LastOutgoingCommand;
	int ChokedCommands;
	int LastCommandAck;
	int CommandAck;
	int SoundSequence;

	bool IsHLTV;

	// Replay?
};