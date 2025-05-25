#pragma once

class Color
{
public:
	unsigned char r, g, b, a;

public:
	Color()
	{
		this->r = 0;
		this->g = 0;
		this->b = 0;
		this->a = 0;
	}

	Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
};

#define COLOR_BLACK (Color(0, 0, 0, 255))
#define COLOR_WHITE (Color(255, 255, 255, 255))
