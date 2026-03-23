#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H


class BaseLabException 
{
private:
    const char* message;
    
public:
    BaseLabException(const char* msg = "Exception") : message(msg) {}
    const char* what() const { return message; }
};

class IndexOutOfRangeException : public BaseLabException 
{
public:
    IndexOutOfRangeException(const char* msg = "Index out of range") 
        : BaseLabException(msg) {}
};

class EmptyStructureException : public BaseLabException 
{
public:
    EmptyStructureException(const char* msg = "Operation is not allowed on an empty structure") 
        : BaseLabException(msg) {}
};

class InvalidArgumentException : public BaseLabException 
{
public:
    InvalidArgumentException(const char* msg = "Invalid argument provided") 
        : BaseLabException(msg) {}
};

class MemoryAllocationException : public BaseLabException 
{
public:
    MemoryAllocationException(const char* msg = "Failed to allocate memory") 
        : BaseLabException(msg) {}
};

#endif // EXCEPTIONS_H