#pragma once

#include <algorithm>
#include <vector>

class Hook
{
public:
	virtual ~Hook() = default; // Dumb language

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

	template <typename T>
	T* Get()
	{
		for (Hook* Hook : this->List)
		{
			T* Found = dynamic_cast<T*>(Hook);

			if (Found)
				return Found;
		}

		return nullptr;
	}
};

inline Hooks* g_Hooks;
