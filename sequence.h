#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "exceptions.h"
#include "option.h"
#include "iterator.h"

template <class T>
class Sequence : public IEnumerable<T>
{
public:
    virtual ~Sequence() {}

    //Декомпозиция
    virtual const T& GetFirst() const = 0;
    virtual const T& GetLast()  const = 0;
    virtual const T& Get(int index) const = 0;
    virtual Sequence<T>* GetSubsequence(int startIndex, int endIndex) const = 0;
    virtual int GetLength() const = 0;

    //Операции
    virtual Sequence<T>* Append(const T& item) = 0;
    virtual Sequence<T>* Prepend(const T& item) = 0;
    virtual Sequence<T>* InsertAt(const T& item, int index) = 0;
    virtual Sequence<T>* RemoveAt(int index) = 0;
    virtual Sequence<T>* Concat(Sequence<T>* other) = 0;

    //Базовый комплект операций map-reduce
    virtual Sequence<T>* Map(T (*f)(const T&)) const = 0;
    virtual Sequence<T>* Where(bool (*predicate)(const T&)) const = 0;
    virtual T Reduce(T (*func)(T, const T&), T initial) const = 0;
    virtual Option<T> GetFirst(bool (*predicate)(const T&)) const = 0;
    virtual Option<T> GetLast (bool (*predicate)(const T&)) const = 0;
 
    virtual IEnumerator<T>* GetEnumerator() const override = 0;
};

#endif // SEQUENCE_H
