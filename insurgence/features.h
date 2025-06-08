#pragma once

#include <string>
#include <unordered_map>

class Feature
{
public:
	virtual ~Feature() = default; // Dumb language

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

	template <typename T>
	T* Get(std::string Name)
	{
		Feature* Found = this->Get(Name);

		if (!Found)
			return nullptr;

		return dynamic_cast<T*>(Found);
	}
};

inline Features* g_Features;
