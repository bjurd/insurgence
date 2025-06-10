#pragma once

#include <cstdint>

class Color
{
public:
	uint8_t r, g, b, a;

public:
	Color()
	{
		this->r = 0;
		this->g = 0;
		this->b = 0;
		this->a = 0;
	}

	Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
};

namespace Colors
{
	inline const Color Black = Color(0, 0, 0, 255);
	inline const Color White = Color(255, 255, 255, 255);

	inline const Color Red = Color(255, 0, 0, 255);
	inline const Color Green = Color(0, 255, 0, 255);
}
