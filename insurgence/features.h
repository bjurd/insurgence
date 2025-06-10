#pragma once

#include <string>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>

#define TYPE_INDEX(Type) std::type_index(typeid(Type))

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
		auto Found = this->List.find(TYPE_INDEX(T));

		if (Found != this->List.end())
			return static_cast<T*>(Found->second);

		return nullptr;
	}
};

inline Features* g_Features;
