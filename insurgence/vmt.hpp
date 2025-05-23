#pragma once

#include <utility>
#include <cstdint>
#include "memory.h"

class VMT
{
public:
	static inline void** GetVTable(void* Object)
	{
		return *reinterpret_cast<void***>(Object);
	}

	static char* GetMethodPointerAt(char*** Object, uintptr_t Index)
	{
		return (*Object)[Index];
	}

	template <typename Type>
	static inline Type* GetVarAt(void* Object, uintptr_t Index)
	{
		return reinterpret_cast<Type*>(reinterpret_cast<std::uintptr_t>(Object) + Index);
	}

	template <typename T>
	struct RefWrapper
	{
		T& ref;
		explicit RefWrapper(T& r) : ref(r) {}
	};

	template <typename T>
	inline T& UnWrap(RefWrapper<T> Wrapper)
	{
		return Wrapper.ref;
	}

	template <typename T>
	inline T&& UnWrap(T&& Value)
	{
		return std::forward<T>(Value);
	}

	template <typename ReturnType, typename... TArguments>
	static inline ReturnType Call(void* Object, uintptr_t Index, TArguments&&... Arguments)
	{
		using MethodType = ReturnType(__thiscall*)(void*, std::decay_t<TArguments>...);

		void* pMethod = VMT::GetVTable(Object)[Index];
		MethodType vMethod = reinterpret_cast<MethodType>(pMethod);

		return vMethod(Object, std::forward<TArguments>(Arguments)...);
	}

	template <typename ReturnType, typename... TArguments>
	static inline ReturnType CallAddr(void* Object, uintptr_t MethodAddr, TArguments&&... Arguments)
	{
		using MethodType = ReturnType(__thiscall*)(void*, std::decay_t<TArguments>...);
		MethodType vMethod = reinterpret_cast<MethodType>(MethodAddr);

		return vMethod(Object, std::forward<TArguments>(Arguments)...);
	}
};

#define VWRAP(Argument)			\
	VMT::RefWrapper(Argument)

#define VPROXY(MethodName, Index, ReturnType, Arguments, ...)				\
	ReturnType MethodName Arguments											\
	{																		\
		return VMT::Call<ReturnType>((void*)this, Index, ## __VA_ARGS__);	\
	}

#define SIGPROXY(MethodName, ModuleName, Signature, ReturnType, Arguments, ...)							\
	ReturnType MethodName Arguments																		\
	{																									\
		static uintptr_t MethodAddr = Memory::FindSignature(##ModuleName, ##Signature);					\
		return VMT::CallAddr<ReturnType>((void*)this, MethodAddr, ## __VA_ARGS__);						\
	}

#define CLPROXY(MethodName, Signature, ReturnType, Arguments, ...) SIGPROXY(MethodName, "client.dll", Signature, ReturnType, Arguments, __VA_ARGS__)

#define ENPROXY(MethodName, Signature, ReturnType, Arguments, ...) SIGPROXY(MethodName, "engine.dll", Signature, ReturnType, Arguments, __VA_ARGS__)

#define PROXYVAR(MethodName, Type, Index)			\
	Type* MethodName()								\
	{												\
		return VMT::GetVarAt<Type>(this, Index);	\
	}