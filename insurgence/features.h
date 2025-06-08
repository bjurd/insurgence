#pragma once

#include <unordered_map>
#include <string>

class Feature
{
public:
	virtual void Create() = 0;
	virtual void Destroy() = 0;
};

class Features
{
public:
	std::unordered_map<std::string, Feature*> List;

public:
	bool Create();
	void Destroy();

	Feature* Get(std::string Name);
};

inline Features* g_Features;
