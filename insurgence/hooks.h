#pragma once

#include <vector>

class Hook
{
public:
	virtual void Create() = 0;
	virtual void Destroy() = 0;
};

class Hooks
{
public:
	std::vector<Hook*> List;

public:
	bool Create();
	void Destroy();
};