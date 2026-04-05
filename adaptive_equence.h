#ifndef ADAPTIVE_SEQUENCE_H
#define ADAPTIVE_SEQUENCE_H

#include "array_sequence.h"
#include "list_sequence.h"

template <class T>
class AdaptiveSequence : public Sequence<T>
{
private:
    Sequence<T>* impl;
    mutable int getCount;
    mutable int insertCount;

    int threshold; 
    
    template <typename NewSequenceType>
    void SwitchTo()
    {
        //Если уже нужный тип, то не пересоз-м
        if (dynamic_cast<NewSequenceType*>(impl))
        {
            return;
        }

        int len = impl->GetLength();
        T* temp = new T[len];

        for (int i = 0; i < len; ++i)
        {
            temp[i] = impl->Get(i);
        }

        Sequence<T>* newImpl = new NewSequenceType(temp, len);
        delete[] temp;

        delete impl;
        impl = newImpl;
    }

    void SwitchToArray()
    {
        SwitchTo<MutableArraySequence<T>>();
    }

    void SwitchToList()
    {
        SwitchTo<MutableListSequence<T>>();
    }
    
    //Общий метод для всех модифицирующих операций
    template <typename Operation>
    bool ExecuteModify(Operation&& op)
    {
        ++insertCount;
        
        Sequence<T>* newImpl = op();
        
        if (newImpl == nullptr)
        {
            throw InvalidOperationException("Operation returned nullptr");
        }
        bool wasReplaced = (newImpl != impl);
        
        if (wasReplaced)
        {
            Sequence<T>* oldImpl = impl;
            impl = newImpl;      
            delete oldImpl;      
        }
    
        if (insertCount > threshold)
        {
            Rebalance();
        }
        
        return wasReplaced;
    }

    void Rebalance() noexcept
    {
        try
        {
            int total = getCount + insertCount;
            if (total == 0) return;
            
            // Проверяем, нужно ли переключение
            bool needArray = (getCount * 100 / total > 70);
            bool needList = (insertCount * 100 / total > 50);
            
            if (needArray && dynamic_cast<MutableArraySequence<T>*>(impl) == nullptr)
            {
                SwitchToArray();
            }
            else if (needList && dynamic_cast<MutableListSequence<T>*>(impl) == nullptr)
            {
                SwitchToList();
            }
            
            getCount = 0;
            insertCount = 0;
        }
        catch (...)
        {
        }
    }

    //Метод для чтения операций
    void OnRead() const
    {
        ++getCount;
        //Проверяем каждые 100 операций
        if ((getCount + insertCount) >= threshold)
        {
            const_cast<AdaptiveSequence<T>*>(this)->Rebalance();
        }
    }    

public:
    explicit AdaptiveSequence(int t = 100) : impl(new MutableArraySequence<T>()),
        getCount(0), insertCount(0), threshold(t) {}

    AdaptiveSequence(const T* items, int count, int t = 100) : impl(new MutableArraySequence<T>(items, count)),
         getCount(0), insertCount(0), threshold(t){}

    AdaptiveSequence(const AdaptiveSequence<T>& other): getCount(0), insertCount(0), threshold(other.threshold)
    {
        int len = other.impl->GetLength();
        T* temp = new T[len];
        for (int i = 0; i < len; ++i)
        {
            temp[i] = other.impl->Get(i);
        }
        impl = new MutableArraySequence<T>(temp, len);
        delete[] temp;
    }

    ~AdaptiveSequence() override
    {
        delete impl;
    }
    
    // Декомпозиция
    const T& GetFirst() const override
    {
        OnRead();
        return impl->GetFirst();
    }
    
    const T& GetLast() const override
    {
        OnRead();
        return impl->GetLast();
    }
    
    const T& Get(int index) const override
    {
        OnRead();
        return impl->Get(index);
    }
    
    int GetLength() const override
    {
        return impl->GetLength();
    }
    
    // Операции
   Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override
    {
        if (startIndex < 0 || endIndex >= GetLength() || startIndex > endIndex)
        {
            throw IndexOutOfRangeException();
        }

        int len = endIndex - startIndex + 1;
        T* temp = new T[len];

        for (int i = startIndex; i <= endIndex; ++i)
        {
            temp[i - startIndex] = impl->Get(i);
        }

        AdaptiveSequence<T>* result = new AdaptiveSequence<T>(temp, len, threshold);
        delete[] temp;
        return result;
    }
    
    Sequence<T>* Append(const T& item) override
    {
        ExecuteModify([this, &item]() -> Sequence<T>* 
        {
            return impl->Append(item);
        });
        return this;
    }

    Sequence<T>* Prepend(const T& item) override
    {
        ExecuteModify([this, &item]() -> Sequence<T>* 
        {
            return impl->Prepend(item);
        });
        return this;
    }

    Sequence<T>* InsertAt(const T& item, int index) override
    {
        ExecuteModify([this, &item, index]() -> Sequence<T>* 
        {
            return impl->InsertAt(item, index);
        });
        return this;
    }

    Sequence<T>* RemoveAt(int index) override
    {
        ExecuteModify([this, index]() -> Sequence<T>* 
        {
            return impl->RemoveAt(index);
        });
        return this;
    }
    
    Sequence<T>* Concat(Sequence<T>* other) override
    {
        if (!other) throw InvalidArgumentException();
        
        int otherLen = other->GetLength();
        for (int i = 0; i < otherLen; ++i)
        {
            ExecuteModify([this, other, i]() -> Sequence<T>* 
            {
                return impl->Append(other->Get(i));
            });
        }
        return this;
    }

    IEnumerator<T>* GetEnumerator() const override
    {
        return impl->GetEnumerator();
    }

    //Map-Reduce
    Sequence<T>* Map(T (*f)(const T&)) const override
    {
        return impl->Map(f);
    }
    
    Sequence<T>* Where(bool (*predicate)(const T&)) const override
    {
        return impl->Where(predicate);
    }
    
    T Reduce(T (*func)(T, const T&), T initial) const override
    {
        return impl->Reduce(func, initial);
    }
    
    Option<T> GetFirst(bool (*predicate)(const T&)) const override
    {
        return impl->GetFirst(predicate);
    }

    Option<T> GetLast(bool (*predicate)(const T&)) const override
    {
        return impl->GetLast(predicate);
    }
    
    //Вручную настраивает порог переключения
    void SetThreshold(int t)
    {
        threshold = t;
    }

    //Для отладки и тестов
    const char* GetImplType() const
    {
        if (dynamic_cast<MutableArraySequence<T>*>(impl))
        {
            return "ArraySequence";
        }
        if (dynamic_cast<MutableListSequence<T>*>(impl))
        {
            return "ListSequence";
        }
        return "Unknown";
    }
};

#endif // ADAPTIVE_SEQUENCE_H
