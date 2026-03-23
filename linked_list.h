#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "exceptions.h"

template <class T>
class LinkedList 
{
public:
    struct Node 
    {
        T data;
        Node* next;
        Node(const T& value, Node* nextNode = nullptr) : data(value), next(nextNode) {}
    };

private:
    Node* head;
    Node* tail;
    int length;

    void copyFrom(const LinkedList<T>& other) 
    {
        Node* currentOther = other.head;
        while (currentOther != nullptr) 
        {
            this->Append(currentOther->data);
            currentOther = currentOther->next;
        }
    }

    void clear() {
        Node* current = head;
        while (current != nullptr) 
        {
            Node* toDelete = current;
            current = current->next;
            delete toDelete;
        }
        head = tail = nullptr;
        length = 0;
    }

    // Получение узла по индексу (const-версия)
    const Node* getNodeAt(int index) const 
    {
        if (index < 0 || index >= length) 
        {
            throw IndexOutOfRangeException();
        }
        
        const Node* current = head;
        for (int i = 0; i < index; ++i) 
        {
            current = current->next;
        }
        return current;
    }
    
    // Получение узла по индексу (не-const версия)
    Node* getNodeAt(int index) 
    {
        return const_cast<Node*>(static_cast<const LinkedList<T>&>(*this).getNodeAt(index));
    }

    // Централизует создание узлов
    Node* createNode(const T& value, Node* next = nullptr) 
    {
        Node* newNode = nullptr;
        try {
            newNode = new Node(value, next);
        } catch (...) {
            throw MemoryAllocationException();
        }
        return newNode;
    }

public:
    LinkedList(const T* items, int count) : head(nullptr), tail(nullptr), length(0) 
    {
        if (count < 0) throw InvalidArgumentException("Count cannot be negative");
        if (count > 0 && items == nullptr) throw InvalidArgumentException("Null pointer provided");
        
        for (int i = 0; i < count; ++i) 
        {
            Append(items[i]);
        }
    }

    LinkedList() : head(nullptr), tail(nullptr), length(0) {}

    LinkedList(const LinkedList<T>& other) : head(nullptr), tail(nullptr), length(0) 
    {
        copyFrom(other);
    }

    ~LinkedList() 
    {
        clear();
    }

    LinkedList<T>& operator=(const LinkedList<T>& other) 
    {
        if (this != &other) 
        {
            clear();
            copyFrom(other);
        }
        return *this;
    }

    const T& GetFirst() const 
    {
        if (head == nullptr) throw EmptyStructureException();
        return getNodeAt(0)->data;
    }

    T& GetFirst() 
    {
        return const_cast<T&>(static_cast<const LinkedList<T>&>(*this).GetFirst());
    }

    const T& GetLast() const 
    {
        if (tail == nullptr) throw EmptyStructureException();
        return getNodeAt(length - 1)->data;
    }

    T& GetLast() 
    {
        return const_cast<T&>(static_cast<const LinkedList<T>&>(*this).GetLast());
    }

    const T& Get(int index) const 
    {
        return getNodeAt(index)->data;
    }

    T& Get(int index)
    {
        return const_cast<T&>(static_cast<const LinkedList<T>&>(*this).Get(index));
    }

    LinkedList<T> GetSubList(int startIndex, int endIndex) const
    {
        if (startIndex < 0 || endIndex >= length || startIndex > endIndex) {
            throw IndexOutOfRangeException();
        }
        
        LinkedList<T> subList;
        const Node* current = getNodeAt(startIndex); 
        for (int i = startIndex; i <= endIndex; ++i) 
        {
            subList.Append(current->data);
            current = current->next;
        }
        return subList;
    }

    int GetLength() const 
    {
        return length;
    }

    void Append(const T& item) 
    {
        InsertAt(item, length);  
    }

    void Prepend(const T& item) 
    {
        InsertAt(item, 0);  
    }

    void InsertAt(const T& item, int index) 
    {
        if (index < 0 || index > length) throw IndexOutOfRangeException();

        Node** current = &head;
        for (int i = 0; i < index; ++i) 
        {
            current = &((*current)->next);
        }
        
        Node* newNode = createNode(item, *current);
        *current = newNode;
        
        if (index == length) 
        {
            tail = newNode;
        }
        length++;
    }

    LinkedList<T> Concat(const LinkedList<T>& list) const 
    {
        LinkedList<T> result(*this);
        Node* current = list.head;
        while (current != nullptr) 
        {
            result.Append(current->data);
            current = current->next;
        }
        return result;
    }
};

#endif // LINKED_LIST_H 