#pragma once

#include <string>
#include <typeindex>
#include <typeinfo>
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
	std::unordered_map<std::type_index, Feature*> List;

public:
	bool Create();
	void Destroy();

	template <typename T>
		requires std::is_base_of_v<Feature, T>
	T* Get()
	{
		auto Found = this->List.find(std::type_index(typeid(T)));

		if (Found != this->List.end())
			return static_cast<T*>(Found->second);

		return nullptr;
	}
};

inline Features* g_Features;
