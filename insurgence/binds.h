#pragma once

#include <functional>
#include <vector>

typedef std::function<void(bool, bool)> BindCallback; // IsPressed, WasPressed

enum BindMode : unsigned char
{
	None, // Disabled, runs once when pressed->unpressed
	Always, // Runs all the time
	Hold, // Runs while pressed and once when pressed->unpressed
	Toggle // Runs once when unpressed->pressed
};

class Bind
{
public:
	bool WasPressed = false;

	int KeyCode;
	BindMode Mode;
	BindCallback Callback;

public:
	Bind(int KeyCode, BindMode Mode, BindCallback Callback);

	bool IsPressed();
};

namespace Binds
{
	inline std::vector<Bind*> List;

	void Create(int KeyCode, BindMode Mode, BindCallback Callback);
	void Process();

	void Destroy();
}
