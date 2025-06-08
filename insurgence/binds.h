#pragma once

#include <cstdint>
#include <functional>
#include <vector>

typedef std::function<void(bool, bool)> BindCallback; // IsPressed, WasPressed

enum BindMode : uint8_t
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

	uint8_t KeyCode; // Max VK is 0xFE
	BindMode Mode;
	BindCallback Callback;

public:
	Bind(uint8_t KeyCode, BindMode Mode, BindCallback Callback);

	bool IsPressed();
};

namespace Binds
{
	inline std::vector<Bind*> List;

	void Create(uint8_t KeyCode, BindMode Mode, BindCallback Callback);
	void Process();

	void Destroy();
}
