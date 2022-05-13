#pragma once

#if _HAS_CXX17
#include <filesystem>
#endif
#include <thread>
#include <atomic>
#include <mutex>
#include <shared_mutex>
#include <memory>
#include <string>
#include <regex>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <vector>
#include <queue>
#include <utility>

namespace ScmAudio
{

///////////////////////////////////////////////////////////////////////////////
// Helper types
///////////////////////////////////////////////////////////////////////////////

struct Empty
{
};

template <int I>
struct IntToType
{
    static constexpr int value = I;
    using Next = IntToType<I + 1>;
    using Prev = IntToType<I - 1>;
};

///////////////////////////////////////////////////////////////////////////////
// Primitive type aliases
///////////////////////////////////////////////////////////////////////////////

using U8 = uint8_t;
constexpr U8 U8MAX = UINT8_MAX;
using U16 = uint16_t;
constexpr U16 U16MAX = UINT16_MAX;
using U32 = uint32_t;
constexpr U32 U32MAX = UINT32_MAX;
using U64 = uint64_t;
constexpr U64 U64MAX = UINT64_MAX;
using S8 = int8_t;
constexpr S8 S8MAX = INT8_MAX;
constexpr S8 S8MIN = INT8_MIN;
using S16 = int16_t;
constexpr S16 S16MAX = INT16_MAX;
constexpr S16 S16MIN = INT16_MIN;
using S32 = int32_t;
constexpr S32 I32MAX = INT32_MAX;
constexpr S32 S32MIN = INT32_MIN;
using S64 = int64_t;
constexpr S64 S64MAX = INT64_MAX;
constexpr S64 S64MIN = INT64_MIN;
using F32 = float;
constexpr F32 F32MAX = FLT_MAX;
constexpr F32 F32MIN = FLT_MIN;
using F64 = double;
constexpr F64 F64MAX = DBL_MAX;
constexpr F64 F64MIN = DBL_MIN;

///////////////////////////////////////////////////////////////////////////////
// Containers
///////////////////////////////////////////////////////////////////////////////

template <class T, class U>
using Pair = std::pair<T, U>;

template <class T>
using Vector = std::vector<T>;

template <class T>
using Queue = std::queue<T>;

template <class T>
using Deque = std::deque<T>;

template <class T>
using List = std::list<T>;

using String  = std::string;

template <class T>
using Set = std::set<T>;

template <class T>
using HashSet = std::unordered_set<T>;

template <class T, class U>
using Map = std::map<T, U>;

template <class T,class U>
using UnorderedMap = std::unordered_map<T, U>;

///////////////////////////////////////////////////////////////////////////////
// Pointers
///////////////////////////////////////////////////////////////////////////////

template <class T>
using SharedPtr = std::shared_ptr<T>;

template <class T, class... Args>
SharedPtr<T> MakeShared(Args... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template <class T>
using WeakPtr = std::weak_ptr<T>;

template <class T>
using UniquePtr = std::unique_ptr<T>;

template <class T, class... Args>
UniquePtr<T> MakeUnique(Args... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

///////////////////////////////////////////////////////////////////////////////
// Concurrency
///////////////////////////////////////////////////////////////////////////////

using Thread = std::thread;

template<class T>
using Atomic = std::atomic<T>;

using ConditionVariable = std::condition_variable;

using Mutex = std::mutex;

#if _HAS_CXX17
template <class... MutexTypes>
using ScopedLock = std::scoped_lock<MutexTypes...>;
#endif

using UniqueLock = std::unique_lock<Mutex>;

using WriteLock = std::unique_lock<Mutex>;

using ReadLock = std::shared_lock<Mutex>;

} // namespace ScmAudio
