#ifndef LIST_SEQUENCE_H
#define LIST_SEQUENCE_H

#include "sequence.h"
#include "linked_list.h"

template <class T>
class ListSequenceBase : public Sequence<T> 
{
protected:
    LinkedList<T>* list;

public:
    ListSequenceBase(const T* items, int count) : list(new LinkedList<T>(items, count)) {}
    ListSequenceBase() : list(new LinkedList<T>()) {}
    ListSequenceBase(const ListSequenceBase<T>& other) : list(new LinkedList<T>(*other.list)) {}

    ~ListSequenceBase() override 
    {
        delete list;
    }

    const T& GetFirst() const override 
    {
        return list->GetFirst();
    }

    const T& GetLast() const override 
    {
        return list->GetLast();
    }

    const T& Get(int index) const override 
    {
        return list->Get(index);
    }

    int GetLength() const override 
    {
        return list->GetLength();
    }

    Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override 
    {
        if (startIndex < 0 || endIndex >= GetLength() || startIndex > endIndex) 
        {
            throw IndexOutOfRangeException();
        }
        
        LinkedList<T> subList = list->GetSubList(startIndex, endIndex);
        
        T* tempArray = new T[subList.GetLength()];
        for (int i = 0; i < subList.GetLength(); ++i) 
        {
            tempArray[i] = subList.Get(i);
        }
        
        Sequence<T>* subSeq = this->CreateSequence(tempArray, subList.GetLength());
        delete[] tempArray;
        return subSeq;
    }


    virtual Sequence<T>* CreateSequence(const T* arr, int count) const = 0;
};

//Mutable версия
template <class T>
class MutableListSequence : public ListSequenceBase<T> 
{
public:
    using ListSequenceBase<T>::list;
    using ListSequenceBase<T>::GetLength;

    MutableListSequence(const T* items, int count) : ListSequenceBase<T>(items, count) {}
    MutableListSequence() : ListSequenceBase<T>() {}
    MutableListSequence(const MutableListSequence<T>& other) : ListSequenceBase<T>(other) {}

    Sequence<T>* CreateSequence(const T* arr, int count) const override 
    {
        return new MutableListSequence<T>(arr, count);
    }

    Sequence<T>* Append(const T& item) override 
    {
        return InsertAt(item, GetLength());
    }

    Sequence<T>* Prepend(const T& item) override 
    {
        return InsertAt(item, 0);
    }

    Sequence<T>* InsertAt(const T& item, int index) override 
    {
        list->InsertAt(item, index);
        return this;
    }

    Sequence<T>* RemoveAt(int index) override 
    {
        if (index < 0 || index >= GetLength()) throw IndexOutOfRangeException();
        
        // Создаём новый список без удаляемого элемента
        LinkedList<T> newList;
        for (int i = 0; i < GetLength(); ++i) 
        {
            if (i != index) 
            {
                newList.Append(list->Get(i));
            }
        }
        
        // Заменяем старый список новым
        delete list;
        list = new LinkedList<T>(newList);
        return this;
    }

    Sequence<T>* Concat(Sequence<T>* other) override 
    {
        for (int i = 0; i < other->GetLength(); ++i) 
        {
            list->Append(other->Get(i));
        }
        return this;
    }
};

//Immutable версия
template <class T>
class ImmutableListSequence : public ListSequenceBase<T> 
{
private:
    // Вспомогательный метод: создаёт новую последовательность из массива
    ImmutableListSequence<T>* createAndDelete(T* arr, int size) const 
    {
        ImmutableListSequence<T>* newSeq = new ImmutableListSequence<T>(arr, size);
        delete[] arr;
        return newSeq;
    }

    // Вспомогательный метод: преобразует LinkedList в массив
    T* listToArray(const LinkedList<T>& lst) const 
    {
        T* arr = new T[lst.GetLength()];
        for (int i = 0; i < lst.GetLength(); ++i) 
        {
            arr[i] = lst.Get(i);
        }
        return arr;
    }
public:
    using ListSequenceBase<T>::list;
    using ListSequenceBase<T>::GetLength;

    ImmutableListSequence(const T* items, int count) : ListSequenceBase<T>(items, count) {}
    ImmutableListSequence() : ListSequenceBase<T>() {}
    ImmutableListSequence(const ImmutableListSequence<T>& other) : ListSequenceBase<T>(other) {}

    Sequence<T>* CreateSequence(const T* arr, int count) const override 
    {
        return new ImmutableListSequence<T>(arr, count);
    }

    Sequence<T>* Append(const T& item) const 
    {
        return InsertAt(item, GetLength());
    }

    Sequence<T>* Prepend(const T& item) const 
    {
        return InsertAt(item, 0);
    }

    Sequence<T>* InsertAt(const T& item, int index) const 
    {
        if (index < 0 || index > GetLength()) throw IndexOutOfRangeException();
        
        LinkedList<T> newList(*list);
        newList.InsertAt(item, index);
        
        T* tempArray = listToArray(newList);
        return createAndDelete(tempArray, newList.GetLength());
    }

    Sequence<T>* RemoveAt(int index) const 
    {
        if (index < 0 || index >= GetLength()) throw IndexOutOfRangeException();
        
        LinkedList<T> newList;
        for (int i = 0; i < GetLength(); ++i) 
        {
            if (i != index) {
                newList.Append(list->Get(i));
            }
        }
        
        T* tempArray = listToArray(newList);
        return createAndDelete(tempArray, newList.GetLength());
    }

    Sequence<T>* Concat(Sequence<T>* other) const 
    {
        LinkedList<T> newList(*list);
        for (int i = 0; i < other->GetLength(); ++i) 
        {
            newList.Append(other->Get(i));
        }
        
        T* tempArray = listToArray(newList);
        return createAndDelete(tempArray, newList.GetLength());
    }

    Sequence<T>* Append(const T& item) override { return const_cast<const ImmutableListSequence<T>*>(this)->Append(item); }
    Sequence<T>* Prepend(const T& item) override { return const_cast<const ImmutableListSequence<T>*>(this)->Prepend(item); }
    Sequence<T>* InsertAt(const T& item, int index) override { return const_cast<const ImmutableListSequence<T>*>(this)->InsertAt(item, index); }
    Sequence<T>* RemoveAt(int index) override { return const_cast<const ImmutableListSequence<T>*>(this)->RemoveAt(index); }
    Sequence<T>* Concat(Sequence<T>* other) override { return const_cast<const ImmutableListSequence<T>*>(this)->Concat(other); }
};

#endif // LIST_SEQUENCE_H