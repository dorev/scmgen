#include "types.h"

namespace ScmAudio
{
    template <class T>
    class LockfreeObjectPool
    {
    private:
        struct Node
        {
            Atomic<Node*> next;
            T object;
        };

    public:
        struct Deleter
        {
            Deleter(LockfreeObjectPool<T>& pool) : pool(pool) {}
            void operator()(T* pointer) { pool.DestroyObject(pointer); }

        private:
            LockfreeObjectPool<T>& pool;
        };

    public:
        template <class... Args>
        LockfreeObjectPool(U32 size = 0, Args... args)
            : _size(size)
            , _nodes(_size)
            , _firstNode(_nodes.data())
        {
            for (U32 i = 0; i < (size - 1); ++i)
                _nodes[i].next.store(&_nodes[i + 1]);
            _nodes[_size - 1].next.store(nullptr);

            if constexpr (sizeof...(Args) > 0)
            {
                for (Node& node : _nodes)
                    new (&node.object) T(std::forward<Args>(args)...);
            }
        }

        T* ObtainObject()
        {
            if (_firstNode != nullptr)
            {
                Node* poppedNode = _firstNode.load();
                while (!_firstNode.compare_exchange_strong(poppedNode, poppedNode->next));
                poppedNode->next = nullptr;
                return &(poppedNode->object);
            }
            return nullptr;
        }

        SharedPtr<T> ObtainRefCountedObject()
        {
            return SharedPtr<T>(ObtainObject(), GetDeleter());
        }

        template <class... Args>
        T* ConstructObject(Args&&... args)
        {
            T* objectPtr = ObtainObject();
            if (objectPtr != nullptr)
                return new (objectPtr) T(std::forward<Args>(args)...);
            return objectPtr;
        }

        template <class... Args>
        SharedPtr<T> ConstructRefCountedObject(Args&&... args)
        {
            return SharedPtr<T>(ConstructObject(std::forward<Args>(args)...), GetDeleter());
        }

        bool ReleaseObject(T* object)
        {
            if (object != nullptr && ObjectOwnedByPool(object))
            {
                Node* firstNode = _firstNode.load();
                Node* recycledNode = GetNodeOfObject(object);
                do
                {
                    if (firstNode == nullptr)
                        return false;
                    recycledNode->next = firstNode;
                }
                while (!_firstNode.compare_exchange_strong(firstNode, recycledNode));
                return true;
            }
            return false;
        }

        bool DestroyObject(T* object)
        {
            bool released = ReleaseObject(object);
            if (released)
                object->~T();
            return released;
        }

        bool ObjectOwnedByPool(const T* object) const
        {
            return object != nullptr && !_nodes.empty()
                && GetAddress(object) >= GetAddress(&_nodes.front().object)
                && GetAddress(object) <= GetAddress(&_nodes.back().object);
        }

    private:
        Deleter GetDeleter()
        {
            return Deleter(*this);
        }

        static uintptr_t GetAddress(const T* object)
        {
            return reinterpret_cast<uintptr_t>(object);
        }

        Node* GetNodeOfObject(const T* object)
        {
            // Preconditions: objects existence and pool ownership verified
            uintptr_t objectAddress = GetAddress(object);
            return reinterpret_cast<Node*>(objectAddress - sizeof(Atomic<Node*>));
        }

    private:

        const U32 _size;
        Vector<Node> _nodes;
        Atomic<Node*> _firstNode;
    };

} // namespace ScmAudio