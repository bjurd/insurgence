#include "binds.h"

#include <Windows.h>

Bind::Bind(uint8_t KeyCode, BindMode Mode, BindCallback Callback)
{
	this->KeyCode = KeyCode;
	this->Mode = Mode;
	this->Callback = Callback;

	if (Mode == BindMode::Toggle) // Stop spammy
		this->WasPressed = true;
}

bool Bind::IsPressed()
{
	switch (this->Mode)
	{
	default:
	case BindMode::None:
		return false;

	case BindMode::Always:
		return true;

	case BindMode::Hold:
		return GetAsyncKeyState(this->KeyCode);

	case BindMode::Toggle:
		return GetAsyncKeyState(this->KeyCode) & 0x0001;
	}
}

void Binds::Create(uint8_t KeyCode, BindMode Mode, BindCallback Callback)
{
	Binds::List.push_back(new Bind(KeyCode, Mode, Callback));
}

void Binds::Process()
{
	for (Bind* Current : Binds::List)
	{
		bool IsPressed = Current->IsPressed();

		switch (Current->Mode)
		{
		default:
		case BindMode::None:
			if (!Current->WasPressed)
				continue;
			break;

		case BindMode::Always:
			break;

		case BindMode::Hold:
			if (!IsPressed && !Current->WasPressed)
				continue;
			break;

		case BindMode::Toggle:
			if (!IsPressed && Current->WasPressed)
				continue;
			break;
		}

		Current->Callback(IsPressed, Current->WasPressed);
		Current->WasPressed = IsPressed;
	}
}

void Binds::Destroy()
{
	for (Bind* Current : Binds::List)
		delete Current;

	Binds::List.clear();
}
