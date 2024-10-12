#pragma once
#include <Windows.h>
#include <initializer_list>
#include <type_traits>
#include <stdint.h>
#include <stdio.h>
#include "hook_output.h"

typedef unsigned char byte;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned long long uint64;

#define INLINE_CONSTEXPR        constexpr __forceinline
#define declfield(t) decltype(t) t

struct auto_ptr
{
    union {
        const DWORD dwHandler;
        const LPVOID lpHandler;
    };

    template<typename T>
    constexpr auto_ptr(T ptr)
        : dwHandler(*(DWORD*)&ptr) {};

    constexpr auto_ptr(DWORD ptr)
        : dwHandler(ptr) {};

    template <typename T>
    constexpr operator T* () const {
        return reinterpret_cast<T*>(lpHandler);
    };

    constexpr operator LPVOID() const {
        return lpHandler;
    };

    constexpr operator DWORD() const {
        return dwHandler;
    };
};


class cb {
public:
    enum class type : int { jmp, call, push, _count };

    struct info {
        using list = std::initializer_list<info>;

        auto_ptr addr;
        type type;
    };

    template <type _Type>
    struct hook : info {
        constexpr hook(const DWORD addr) : info{ addr, _Type } {};
    };

    using jmp = hook<type::jmp>;
    using call = hook<type::call>;
    using push = hook<type::push>;
};

template <typename TRet, typename... TArgs>
using MethodCall = TRet(*)(TArgs...);

template <typename TRet, class TThis, typename... TArgs>
using MemberCall = TRet(__thiscall*)(const TThis, TArgs...);

template <typename TRet, typename... TArgs>
using StdMethodCall = TRet(__stdcall*)(TArgs...);

template <typename TRet, class TThis, typename... TArgs>
using VirtualCall = TRet(TThis::*)(TArgs...);

namespace hook {
    template <typename TType, bool is_pointer = std::is_pointer<TType>::value, bool is_array = std::is_array<TType>::value>
    class Type {};

    /*
        Hook template for value types
    */
    template <typename TType>
    class Type<TType, false, false> {
    protected:
        TType* lpValue;
    public:
        constexpr Type(int address) : lpValue(reinterpret_cast<TType*>(address)) {};

        inline TType& get() const { return *lpValue; }
        inline void set(TType value) { *lpValue = value; }

        inline TType* ptr() const { return lpValue; }

        /*
            Operators
        */

        inline TType* operator->() const { return lpValue; };
        inline TType* operator&() const { return lpValue; };
        inline TType& operator*() const { return *lpValue; };
        inline TType* operator[](int index) const { return &lpValue[index]; }
        inline TType& operator=(TType value) { return (*lpValue = value); }

        inline operator TType& () const { return *lpValue; }

        /*
            Comparison operators
        */

        inline bool operator==(const TType& rhs) const { return *lpValue == rhs; }
        inline bool operator!=(const TType& rhs) const { return *lpValue != rhs; }

        /*
            Value-type operators
        */

        inline bool operator<(const TType& rhs) const { return *lpValue < rhs; }
        inline bool operator>(const TType& rhs) const { return *lpValue > rhs; }
        inline bool operator<=(const TType& rhs) const { return *lpValue <= rhs; }
        inline bool operator>=(const TType& rhs) const { return *lpValue >= rhs; }

        inline TType operator+() const { return +(*lpValue); }
        inline TType operator-() const { return -(*lpValue); }
        inline TType operator~() const { return ~(*lpValue); }

        inline TType operator+(const TType& rhs) const { return *lpValue + rhs; }
        inline TType operator-(const TType& rhs) const { return *lpValue - rhs; }
        inline TType operator*(const TType& rhs) const { return *lpValue * rhs; }
        inline TType operator/(const TType& rhs) const { return *lpValue / rhs; }
        inline TType operator%(const TType& rhs) const { return *lpValue % rhs; }
        inline TType operator&(const TType& rhs) const { return *lpValue & rhs; }
        inline TType operator|(const TType& rhs) const { return *lpValue | rhs; }
        inline TType operator^(const TType& rhs) const { return *lpValue ^ rhs; }
        inline TType operator<<(const TType& rhs) const { return *lpValue << rhs; }
        inline TType operator>>(const TType& rhs) const { return *lpValue >> rhs; }

        inline TType operator+=(const TType& rhs) { return (*lpValue += rhs); }
        inline TType operator-=(const TType& rhs) { return (*lpValue -= rhs); }
        inline TType operator*=(const TType& rhs) { return (*lpValue *= rhs); }
        inline TType operator/=(const TType& rhs) { return (*lpValue /= rhs); }
        inline TType operator%=(const TType& rhs) { return (*lpValue %= rhs); }
        inline TType operator&=(const TType& rhs) { return (*lpValue &= rhs); }
        inline TType operator|=(const TType& rhs) { return (*lpValue |= rhs); }
        inline TType operator^=(const TType& rhs) { return (*lpValue ^= rhs); }
        inline TType operator<<=(const TType& rhs) { return (*lpValue <<= rhs); }
        inline TType operator>>=(const TType& rhs) { return (*lpValue >>= rhs); }
    };

    /*
        Hook template for pointer types
    */
    template <typename TType>
    class Type<TType, true, false> {
    protected:
        TType* lpValue;
    public:
        constexpr Type(int address) : lpValue(reinterpret_cast<TType*>(address)) {};

        inline TType& get() const { return *lpValue; }
        inline void set(TType value) { *lpValue = value; }

        inline TType* ptr() const { return lpValue; }

        /*
            Operators
        */

        inline TType& operator->() const { return *lpValue; };
        inline TType* operator&() const { return lpValue; };
        inline TType& operator*() const { return *lpValue; };
        inline TType operator[](int index) const { return lpValue[index]; }
        inline TType& operator=(TType value) { return (*lpValue = value); }

        inline operator TType& () const { return *lpValue; }

        /*
            Comparison operators
        */

        inline bool operator==(const TType& rhs) const { return *lpValue == rhs; }
        inline bool operator!=(const TType& rhs) const { return *lpValue != rhs; }

        inline bool operator==(const std::nullptr_t& rhs) const
        {
            return *lpValue == nullptr;
        }
        inline bool operator!=(const std::nullptr_t& rhs) const
        {
            return *lpValue != nullptr;
        }

        template <typename... TArgs>
        inline auto operator()(TArgs... args) {
            return (*lpValue)(args...);
        }
    };

    /*
        Hook template for array types
    */
    template <typename TArray>
    class Type<TArray, false, true> {
        /*
            we need all this spaghett to resolve the actual array type
            because the fucking template isn't smart enough to do so
        */

        template <typename _T, int N>
        static constexpr _T _type(_T(*ary)[N]);

        template <typename _T, int N>
        static constexpr int _count(_T(*ary)[N]) {
            return N;
        };

        using type = decltype(_type((TArray*)nullptr));

        template <typename TRet, typename ...TArgs>
        using rtype = TRet;
    protected:
        using TValue = rtype<type>;

        TValue* lpValue;
    public:
        constexpr Type(int address) : lpValue(reinterpret_cast<TValue*>(address)) {};

        inline int count() const {
            return _count((TArray*)nullptr);
        }

        inline TValue* ptr() const { return lpValue; }
        inline TValue* ptr(int index) const { return lpValue + index; }

        /*
            Operators
        */

        inline TValue* operator&() const { return lpValue; };
        inline TValue& operator[](int index) const { return lpValue[index]; }

        template <typename TType>
        inline operator TType* () const { return reinterpret_cast<TType*>(lpValue); }
    };

    template <typename TType>
    class TypeProxy {
    protected:
        TType* lpValue;
    public:
        static_assert(!std::is_pointer<TType>::value, "Type proxy cannot be a pointer to a class.");

        constexpr TypeProxy(int address) : lpValue(reinterpret_cast<TType*>(address)) {};

        inline void read(TType& value) { memcpy(&value, lpValue, sizeof(TType)); }
        inline void write(TType& value) { memcpy(lpValue, &value, sizeof(TType)); }

        inline TType* operator->() const { return lpValue; }
        inline TType* operator&() const { return lpValue; }
        inline TType& operator*() const { return *lpValue; }
        inline TType& operator[](int index) const { return &lpValue[index]; }

        inline operator TType* () const { return lpValue; }
        inline operator TType& () const { return *lpValue; }
    };

    template<int offset, typename TValue>
    struct Field {
    public:
        template <class TThis>
        static INLINE_CONSTEXPR TValue get(const TThis* p) {
            return *(TValue*)((BYTE*)p + offset);
        };

        template <class TThis>
        static INLINE_CONSTEXPR void set(const TThis* p, TValue value) {
            *(TValue*)((BYTE*)p + offset) = value;
        };

        template <class TThis>
        static INLINE_CONSTEXPR TValue* ptr(const TThis* p) {
            return (TValue*)((BYTE*)p + offset);
        };
    };

    template <int address>
    struct Thunk {
    private:
        template<typename TRet, class TThis, typename ...TArgs>
        static INLINE_CONSTEXPR const TRet _ConstCall(int callback, const TThis* This, TArgs ...args) {
            return (This->*reinterpret_cast<VirtualCall<const TRet, const TThis, TArgs...> &>(callback))(args...);
        };

        template<typename TRet, class TThis, typename ...TArgs>
        static INLINE_CONSTEXPR TRet _ThisCall(int callback, TThis* This, TArgs ...args) {
            return (This->*reinterpret_cast<VirtualCall<TRet, TThis, TArgs...> &>(callback))(args...);
        };
    public:
        template<typename TRet, class TThis, typename ...TArgs>
        static INLINE_CONSTEXPR TRet Call(const TThis&& This, TArgs ...args) {
            return static_cast<MemberCall<TRet, TThis, TArgs...>>(reinterpret_cast<LPVOID>(address))(This, args...);
        };

        template<class TThis, typename ...TArgs>
        static INLINE_CONSTEXPR const TThis Call(const TThis* This, TArgs ...args) {
            return _ConstCall<TThis>(address, This, args...);
        };

        // HACK: call from a pointer to a class...
        template<typename TRet, class TThis, typename ...TArgs>
        static INLINE_CONSTEXPR TRet ThisCall(TThis* This, TArgs ...args) {
            return _ThisCall<TRet>(address, This, args...);
        };
    };

    template <int address>
    struct StaticThunk {
    public:
        template<typename TRet, typename ...TArgs>
        static INLINE_CONSTEXPR TRet Call(TArgs ...args) {
            return static_cast<MethodCall<TRet, TArgs...>>(reinterpret_cast<LPVOID>(address))(args...);
        };
    };

    class FuncBase
    {
    protected:
        LPVOID lpFunc;
    public:
        FuncBase(LPVOID address) : lpFunc(address) {}
    };

    template <typename TRet>
    class Func : protected FuncBase {
    public:
        constexpr Func(int address) : FuncBase(reinterpret_cast<LPVOID>(address)) {};

        template<typename ...TArgs>
        INLINE_CONSTEXPR TRet operator()(TArgs ...args) const {
            return static_cast<MethodCall<TRet, TArgs...>>(lpFunc)(args...);
        };

        class StdCall : protected Func<TRet> {
        public:
            constexpr StdCall(int address) : Func<TRet>(address) {};

            template<typename ...TArgs>
            INLINE_CONSTEXPR TRet operator()(TArgs ...args) const {
                return static_cast<StdMethodCall<TRet, TArgs...>>(lpFunc)(args...);
            };

            template <typename TRet, typename... TArgs>
            INLINE_CONSTEXPR operator StdMethodCall<TRet, TArgs...>() const {
                return static_cast<StdMethodCall<TRet, TArgs...>>(lpFunc);
            };
        };

        class ThisCall : protected Func<TRet> {
        public:
            constexpr ThisCall(int address) : Func<TRet>(address) {};

            template<typename ...TArgs, class TThis>
            INLINE_CONSTEXPR TRet operator()(const TThis&& This, TArgs ...args) const {
                return static_cast<MemberCall<TRet, TThis, TArgs...>>(lpFunc)(This, args...);
            };

            template <typename TRet, class TThis, typename... TArgs>
            INLINE_CONSTEXPR operator MemberCall<TRet, TThis, TArgs...>() const {
                return static_cast<MemberCall<TRet, TThis, TArgs...>>(lpFunc);
            };
        };
    };

    template <typename TRet>
    class MemberFunc : protected Func<TRet> {
    public:
        constexpr MemberFunc(int address) : Func<TRet>(address) {};

        template<typename ...TArgs, class TThis>
        INLINE_CONSTEXPR TRet operator()(const TThis&& This, TArgs ...args) const {
            return static_cast<MemberCall<TRet, TThis, TArgs...>>(lpFunc)(This, args...);
        };

        template <typename TRet, class TThis, typename... TArgs>
        INLINE_CONSTEXPR operator MemberCall<TRet, TThis, TArgs...>() const {
            return static_cast<MemberCall<TRet, TThis, TArgs...>>(lpFunc);
        };
    };

    /*
        __cdecl function declaration
    */
    template <typename TRet, typename... TArgs>
    class Func<MethodCall<TRet, TArgs...>> : protected Func<TRet> {
        using MethodCall = MethodCall<TRet, TArgs...>;
    public:
        constexpr Func(int address) : Func<TRet>(address) {};

        INLINE_CONSTEXPR TRet operator()(TArgs ...args) const {
            return static_cast<MethodCall>(lpFunc)(args...);
        };

        INLINE_CONSTEXPR operator MethodCall() const {
            return static_cast<MethodCall>(lpFunc);
        };
    };

    /*
        __stdcall function declaration
    */
    template <typename TRet, typename... TArgs>
    class Func<StdMethodCall<TRet, TArgs...>> : protected Func<TRet> {
        using StdMethodCall = StdMethodCall<TRet, TArgs...>;
    public:
        constexpr Func(int address) : Func<TRet>(address) {};

        INLINE_CONSTEXPR TRet operator()(TArgs ...args) const {
            return static_cast<StdMethodCall>(lpFunc)(args...);
        };

        INLINE_CONSTEXPR operator StdMethodCall() const {
            return static_cast<StdMethodCall>(lpFunc);
        };
    };

    /*
        __thiscall function declaration
    */
    template <typename TRet, class TThis, typename... TArgs>
    class Func<MemberCall<TRet, TThis, TArgs...>> : protected Func<TRet> {
        using MemberCall = MemberCall<TRet, TThis, TArgs...>;
    public:
        constexpr Func(int address) : Func<TRet>(address) {};

        INLINE_CONSTEXPR TRet operator()(const TThis&& This, TArgs ...args) const {
            return static_cast<MemberCall>(lpFunc)(args...);
        };

        INLINE_CONSTEXPR operator MemberCall() const {
            return static_cast<MemberCall>(lpFunc);
        };
    };
};

template <typename TType>
using _Type = hook::Type<TType>;

template <typename TType>
using _TypeProxy = hook::TypeProxy<TType>;

template <typename ...TArgs>
using _Func = hook::Func<TArgs...>;

template <typename ...TArgs>
using _MemberFunc = hook::MemberFunc<TArgs...>;

template <int address>
using _Thunk = hook::Thunk<address>;

template <int address>
using _StaticThunk = hook::StaticThunk<address>;

namespace mem
{
    inline bool copy(void* lpAddress, const void* lpReadAddress, size_t dwReadSize)
    {
        DWORD dwOldProtect;

        if (VirtualProtect(lpAddress, dwReadSize, PAGE_EXECUTE_READWRITE, &dwOldProtect))
        {
            memcpy(lpAddress, lpReadAddress, dwReadSize);
            VirtualProtect(lpAddress, dwReadSize, dwOldProtect, &dwOldProtect);

            return true;
        }

        return false;
    }

    template <typename T>
    inline T read(auto_ptr address) {
        DWORD dwOldProtect;

        if (VirtualProtect(address, sizeof(T), PAGE_EXECUTE_READ, &dwOldProtect))
        {
            T value = *(T*)address;
            VirtualProtect(address, sizeof(T), dwOldProtect, &dwOldProtect);

            return value;
        }

        return NULL;
    }

    template <typename Func, typename... Args>
    void unpack(Func func, Args&&... args)
    {
        using unpacker_t = int[];

        (void)unpacker_t
        {
            (func(std::forward<Args>(args)), 0)..., 0
        };
    }

    template <typename ...TArgs>
    inline bool write(auto_ptr address, TArgs ...args)
    {
        static_assert(sizeof...(args) > 0,
            "No arguments provided");

        static_assert(variadic::true_for_all<std::is_trivially_copyable<TArgs>::value...>,
            "Not all arguments are trivially copyable");

        constexpr size_t totalSize = variadic::sum<sizeof(TArgs)...>;

        DWORD dwOldProtect;

        if (VirtualProtect(address, totalSize, PAGE_EXECUTE_READWRITE, &dwOldProtect))
        {
            void* lpDst = address;

            unpack([&](const auto& value)
            {
                memcpy(lpDst, &value, sizeof(value));
                lpDst = static_cast<char*>(lpDst) + sizeof(value);
            }, std::forward<TArgs>(args)...);

            VirtualProtect(address, totalSize, dwOldProtect, &dwOldProtect);

            return true;
        }

        return false;
    }

    inline bool write_string(auto_ptr address, const char* str, size_t size)
    {
        DWORD dwOldProtect;

        if (VirtualProtect(address, size, PAGE_EXECUTE_READWRITE, &dwOldProtect))
        {
            char* lpDst = address;
            memcpy(lpDst, str, size);
            VirtualProtect(address, size, dwOldProtect, &dwOldProtect);
            return true;
        }

        return false;
    }

    inline bool write_string(auto_ptr address, const char* str)
    {
        size_t size = strlen(str);
        return write_string(address, str, size);
    }

    inline bool nop(auto_ptr address, int count = 1)
    {
        constexpr int OP_NOP = 0x90;

        DWORD dwOldProtect;

        if (VirtualProtect(address, count, PAGE_EXECUTE_READWRITE, &dwOldProtect))
        {
            uint8_t* lpDst = address;

            for (int i = 0; i < count; i++)
                lpDst[i] = OP_NOP;

            VirtualProtect(address, count, dwOldProtect, &dwOldProtect);

            return true;
        }

        return false;
    }
}

namespace variadic
{
    template <unsigned long long...>
    constexpr unsigned long long sum = 0;

    template <unsigned long long S, unsigned long long... SS>
    constexpr unsigned long long sum<S, SS...> = S + sum<SS...>;

    // true_for_all
    template <bool...>
    constexpr bool true_for_all = true;

    template <bool B, bool... BB>
    constexpr bool true_for_all<B, BB...> = B && true_for_all<BB...>;

    // true_for_any
    template <bool...>
    constexpr bool true_for_any = false;

    template <bool B, bool... BB>
    constexpr bool true_for_any<B, BB...> = B || true_for_any<BB...>;
}

struct scoped_vtable {
private:
    const intptr_t obj;
    const intptr_t vtbl;
public:
    template <typename TClass>
    inline scoped_vtable(const TClass* obj) noexcept :
        obj(reinterpret_cast<const intptr_t>(obj)),
        vtbl(*reinterpret_cast<const intptr_t*>(obj)) { }

    inline ~scoped_vtable() noexcept {
        *reinterpret_cast<intptr_t*>(obj) = vtbl;
    }
};

template <typename TR = void>
constexpr inline TR* getPtr(LPVOID p, int offset) {
    return (TR*)((BYTE*)p + offset);
};

template <typename TR = void, class TC>
constexpr inline TR* getPtr(const TC* p, int offset) {
    return (TR*)getPtr((LPVOID)p, offset);
};

template <typename TV>
constexpr inline void setPtr(LPVOID p, int offset, TV value) {
    *(TV*)((BYTE*)p + offset) = value;
};

template <class TC, typename TV>
constexpr inline void setPtr(const TC* p, int offset, TV value) {
    *(TV*)((BYTE*)p + offset) = value;
};

void InstallPatch(LPCSTR description,
    std::initializer_list<unsigned char> bytes,
    std::initializer_list<unsigned int> addresses);

inline void InstallPatch(std::initializer_list<unsigned char> bytes,
    std::initializer_list<unsigned int> addresses) {
    InstallPatch(NULL, bytes, addresses);
}

void InstallCallback(auto_ptr lpAddr, const cb::info& callback);

void InstallCallback(LPCSTR name,
    LPCSTR description,
    auto_ptr lpAddr,
    cb::info::list callbacks);

inline void InstallCallback(LPCSTR name, auto_ptr lpAddr, cb::info::list callbacks) {
    InstallCallback(name, NULL, lpAddr, callbacks);
}

inline void InstallCallback(auto_ptr lpAddr, LPCSTR description, cb::info::list callbacks) {
    InstallCallback(NULL, description, lpAddr, callbacks);
}

inline void InstallCallback(auto_ptr lpAddr, cb::info::list callbacks, LPCSTR description) {
    InstallCallback(NULL, description, lpAddr, callbacks);
}

inline void InstallCallback(auto_ptr lpAddr, cb::info::list callbacks) {
    InstallCallback(NULL, NULL, lpAddr, callbacks);
}

void InstallVTableHook(LPCSTR name, auto_ptr lpAddr, std::initializer_list<unsigned int> addresses);