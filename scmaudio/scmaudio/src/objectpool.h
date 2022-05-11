#include "types.h"

namespace ScmAudio
{

template <class T> class ObjectPool;

template <class T>
struct ObjectPoolDeleter
{
    ObjectPoolDeleter(ObjectPool<T>& pool)
        : pool(pool)
    {
    }

    void operator()(T* pointer)
    {
        pool.FreeObject(pointer);
    }

private:
    ObjectPool<T>& pool;
};

template <class T>
class Poolable
{
protected:
    Poolable() : _next(nullptr)
    {
    }

private:
    friend class ObjectPool<T>;

    T* GetNext()
    {
        return _next;
    }

    void SetNext(T* next)
    {
        _next = next;
    };

private:
    T* _next;
};


template <class T>
class ObjectPool
{
public:
    ObjectPool(unsigned size)
        : _objects(size) 
        , _firstAvailable(_objects.data())
    {
        static_assert(std::is_base_of<Poolable<T>, T>::value, "Object in pool must implement Poolable<T>");
        for(unsigned i = 0; i < (size - 1); ++i)
            _objects[i].Poolable<T>::SetNext(&_objects[i+1]);
    }

    T* GetObject()
    {
        ASSERT(_firstAvailable != nullptr);
        T* tmp = _firstAvailable;
        _firstAvailable = tmp->Poolable<T>::GetNext();
        return tmp;
    }

    template <class... Args>
    T* ConstructObject(Args&&... args)
    {
        ASSERT(_firstAvailable != nullptr);
        T* tmp = _firstAvailable;
        _firstAvailable = tmp->Poolable<T>::GetNext();
        return new (tmp) T(std::forward<Args>(args)...);
    }

    void FreeObject(T* object)
    {
        ASSERT(object != nullptr);
        ASSERT(ObjectOwnedByPool(object));
        object->Poolable<T>::SetNext(_firstAvailable);
        _firstAvailable = object;
    }

    bool ObjectOwnedByPool(const T* object) const
    {
        return GetAddress(object) >= GetAddress(&_objects.front())
            && GetAddress(object) <= GetAddress(&_objects.back());
    }

    ObjectPoolDeleter<T> GetDeleter()
    {
        return ObjectPoolDeleter(*this);
    }

private:
    static uintptr_t GetAddress(const T* object)
    {
        return reinterpret_cast<uintptr_t>(object);
    }

    Vector<T> _objects;
    T* _firstAvailable;
};

//
//
// Need a lockless refcounted pool!
//
//

} // namespace ScmAudio