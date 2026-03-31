#include <iostream>
#include <string>
#include <sstream>

#include "dynamic_array.h"
#include "linked_list.h"
#include "array_sequence.h"
#include "list_sequence.h"
#include "bit_sequence.h"
#include "adaptive_sequence.h"
#include "builder.h"

// Вспомогательные функции
template <class T>
int readArray(T* arr, int maxSize)
{
    std::string line;
    std::getline(std::cin, line);
    std::istringstream iss(line);

    int count = 0;
    std::string token;

    while (count < maxSize && iss >> token)
    {
        std::istringstream tokenStream(token);
        T value;
        char leftover;
        //Если прочиталось значение и после него нет лишних символов — берём
        if (tokenStream >> value && !(tokenStream >> leftover))
            arr[count++] = value;
    }
    return count;
}

template <>
int readArray<Bit>(Bit* arr, int maxSize)
{
    std::string line;
    std::getline(std::cin, line);
    std::istringstream iss(line);

    int count = 0;
    int val;

    while (count < maxSize && iss >> val)
    {
        arr[count++] = Bit(val != 0);
    }
    return count;
}

bool readInt(int& val)
{
    if (!(std::cin >> val))
    {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Ошибка: введите целое число\n";

        val = -1; 
        return false;
    }
    std::cin.ignore(10000, '\n');
    return true;
}

template <class T>
void printSequence(const Sequence<T>* seq, const char* name)
{
    if (!seq) 
    { 
        std::cout << name << " is null\n"; 
        return; 
    }
    std::cout << name << " (len=" << seq->GetLength() << "): [";
    for (int i = 0; i < seq->GetLength(); ++i)
    {
        if (i > 0) std::cout << ", ";
        std::cout << seq->Get(i);
    }
    std::cout << "]\n";
}

template <>
void printSequence<Bit>(const Sequence<Bit>* seq, const char* name)
{
    if (!seq) 
    { 
        std::cout << name << " is null\n"; 
        return; 
    }
    std::cout << name << " (len=" << seq->GetLength() << "): [";
    for (int i = 0; i < seq->GetLength(); ++i)
    {
        if (i > 0) std::cout << ", ";
        std::cout << (seq->Get(i) ? '1' : '0');
    }
    std::cout << "]\n";
}

template <>
void printSequence<std::string>(const Sequence<std::string>* seq, const char* name)
{
    if (!seq) 
    { 
        std::cout << name << " is null\n"; 
        return; 
    }
    std::cout << name << " (len=" << seq->GetLength() << "): [";
    for (int i = 0; i < seq->GetLength(); ++i)
    {
        if (i > 0) std::cout << ", ";
        std::cout << "\"" << seq->Get(i) << "\"";
    }
    std::cout << "]\n";
}

//Операции над последовательностью
template <class T>
void opGet(Sequence<T>* seq)
{
    if (!seq) 
    { 
        std::cout << "Последовательность не создана\n";
        return; 
    }
    int idx;
    std::cout << "Введите индекс: ";

    if (!readInt(idx)) 
    {
        return;
    }
    if (idx < 0 || idx >= seq->GetLength())
    {
        std::cout << "Ошибка: индекс вне диапазона [0, " << seq->GetLength()-1 << "]\n";
    } else
    {
        std::cout << "Get(" << idx << ") = " << seq->Get(idx) << "\n";
    }
}

template <class T>
void opAppend(Sequence<T>*& seq)
{
    if (!seq) 
    { 
        std::cout << "Последовательность не создана\n"; 
        return; 
    }
    T val;
    std::cout << "Введите значение: ";
    std::cin >> val;
    std::cin.ignore(10000, '\n');

    Sequence<T>* r = seq->Append(val);
    if (r != seq) 
    { 
        delete seq; seq = r; 
    }
    printSequence<T>(seq, "После Append");
}

template <class T>
void opPrepend(Sequence<T>*& seq)
{
    if (!seq) 
    { 
        std::cout << "Последовательность не создана\n"; 
        return; 
    }
    T val;
    std::cout << "Введите значение: ";
    std::cin >> val;
    std::cin.ignore(10000, '\n');

    Sequence<T>* r = seq->Prepend(val);
    if (r != seq) 
    { 
        delete seq; seq = r; 
    }
    printSequence<T>(seq, "После Prepend");
}

template <class T>
void opInsertAt(Sequence<T>*& seq)
{
    if (!seq) 
    { 
        std::cout << "Последовательность не создана\n"; 
        return; 
    }
    int idx;
    std::cout << "Введите индекс: ";

    if (!readInt(idx)) 
    {
        return;
    }
    if (idx < 0 || idx > seq->GetLength())
    {
        std::cout << "Ошибка: индекс вне диапазона [0, " << seq->GetLength() << "].\n";
        return;
    }
    T val;
    std::cout << "Введите значение: ";
    std::cin >> val;
    std::cin.ignore(10000, '\n');
    Sequence<T>* r = seq->InsertAt(val, idx);
    if (r != seq) 
    { 
        delete seq; seq = r; 
    }
    printSequence<T>(seq, "После InsertAt");
}

template <class T>
void opRemoveAt(Sequence<T>*& seq)
{
    if (!seq) 
    { 
        std::cout << "Последовательность не создана\n"; 
        return; 
    }
    int idx;
    std::cout << "Введите индекс: ";

    if (!readInt(idx)) 
    {
        return;
    }
    if (idx < 0 || idx >= seq->GetLength())
    {
        std::cout << "Ошибка: индекс вне диапазона [0, " << seq->GetLength()-1 << "]\n";
        return;
    }
    Sequence<T>* r = seq->RemoveAt(idx);
    if (r != seq) 
    { 
        delete seq; seq = r; 
    }
    printSequence<T>(seq, "После RemoveAt");
}

template <class T>
void opSubsequence(Sequence<T>* seq)
{
    if (!seq) 
    { 
        std::cout << "Последовательность не создана\n"; 
        return; 
    }
    int start, end;
    std::cout << "Введите startIndex endIndex: ";
    std::cin >> start >> end;
    std::cin.ignore(10000, '\n');

    if (start < 0 || end < start)
    {
        std::cout << "Ошибка: нужно 0 <= start <= end\n";
        return;
    }
    if (end >= seq->GetLength())
    {
        std::cout << "Ошибка: end вне диапазона [0, " << seq->GetLength()-1 << "]\n";
        return;
    }

    Sequence<T>* sub = seq->GetSubsequence(start, end);
    printSequence<T>(sub, "Подпоследовательность");
    delete sub;
}

template <class T>
void opConcat(Sequence<T>*& seq)
{
    if (!seq) 
    { 
        std::cout << "Последовательность не создана\n"; 
        return; 
    }
    std::cout << "Введите элементы второй последовательности: ";
    T temp[100];
    int count = readArray<T>(temp, 100);

    if (count == 0) 
    { 
        std::cout << "Ошибка: не введено ни одного элемента\n"; 
        return; 
    }

    MutableArraySequence<T>* other = new MutableArraySequence<T>(temp, count);
    Sequence<T>* r = seq->Concat(other);
    if (r != seq) 
    { 
        delete seq; seq = r;
    }
    delete other;
    printSequence<T>(seq, "После Concat");
    return;
}

//Меню
void printMenu()
{
    std::cout << "\n===== Sequence Menu =====\n"
              << "--- Создание (int) ---\n"
              << "1. MutableArraySequence<int>\n"
              << "2. ImmutableArraySequence<int>\n"
              << "3. MutableListSequence<int>\n"
              << "4. ImmutableListSequence<int>\n"
              << "--- Создание (string) ---\n"
              << "5. MutableArraySequence<string>\n"
              << "6. ImmutableArraySequence<string>\n"
              << "7. MutableListSequence<string>\n"
              << "8. ImmutableListSequence<string>\n"
              << "--- Операции ---\n"
              << "9.  Напечатать текущую\n"
              << "10. Get\n"
              << "11. GetLength\n"
              << "12. Append\n"
              << "13. Prepend\n"
              << "14. InsertAt\n"
              << "15. RemoveAt\n"
              << "16. GetSubsequence\n"
              << "17. Concat\n"
              << "18. BitSequence (NOT/AND/OR/XOR)\n"
              << "19. AdaptiveSequence\n"
              << "0.  Выход\n"
              << "Выбор: ";
}

//Демонстрации
template <class T>
void demonstrateSequence(T* arr, int size, const char* typeName)
{
    std::cout << "\n=== Демонстрация: " << typeName << " ===\n";
    Sequence<T>* seq = new MutableArraySequence<T>(arr, size);
    printSequence<T>(seq, "Исходная");

    Sequence<T>* r;

    r = seq->Append(arr[size - 1]);
    if (r != seq) 
    { 
        delete seq; seq = r;
    }
    printSequence<T>(seq, "После Append");

    r = seq->Prepend(arr[0]);
    if (r != seq) 
    { 
        delete seq; seq = r; 
    }
    printSequence<T>(seq, "После Prepend");

    r = seq->RemoveAt(0);
    if (r != seq) 
    { 
        delete seq; seq = r; 
    }
    printSequence<T>(seq, "После RemoveAt(0)");

    Sequence<T>* sub = seq->GetSubsequence(0, seq->GetLength() - 1);
    printSequence<T>(sub, "GetSubsequence(0, len-1)");
    delete sub;
    delete seq; 
}

void demonstrateInt()
{
    int arr[] = {10, 20, 30, 40, 50};
    demonstrateSequence<int>(arr, 5, "interer");
}

void demonstrateString()
{
    std::string arr[] = {"apple", "banana", "cherry", "date"};
    demonstrateSequence<std::string>(arr, 4, "string");
}

//Интерактивный режим
void runInteractive()
{
    Sequence<int>*         currentInt    = nullptr;
    Sequence<std::string>* currentString = nullptr;
    bool isIntMode = true;
    int choice;

    do
    {
        try
        {
            printMenu();
            choice = -1;
            readInt(choice);
            if (choice == -1) continue;

            //Создание int
            if (choice >= 1 && choice <= 4)
            {
                std::cout << "Введите целые числа через пробел: ";
                int temp[100];
                int count = readArray<int>(temp, 100);

                if (count == 0) 
                { 
                    std::cout << "Ошибка: нет элементов\n"; 
                    continue; 
                }
                delete currentInt;    currentInt    = nullptr;
                delete currentString; currentString = nullptr;
                isIntMode = true;
                switch (choice)
                {
                    case 1: currentInt = new MutableArraySequence<int>(temp, count);   break;
                    case 2: currentInt = new ImmutableArraySequence<int>(temp, count); break;
                    case 3: currentInt = new MutableListSequence<int>(temp, count);    break;
                    case 4: currentInt = new ImmutableListSequence<int>(temp, count);  break;
                }
                printSequence<int>(currentInt, "Создана");
            }

            //Создание string
            else if (choice >= 5 && choice <= 8)
            {
                std::cout << "Введите строки через пробел: ";
                std::string temp[100];
                int count = readArray<std::string>(temp, 100);

                if (count == 0) 
                { 
                    std::cout << "Ошибка: нет элементов\n"; 
                    continue; 
                }
                delete currentInt;    currentInt    = nullptr;
                delete currentString; currentString = nullptr;
                isIntMode = false;
                switch (choice)
                {
                    case 5: currentString = new MutableArraySequence<std::string>(temp, count);   break;
                    case 6: currentString = new ImmutableArraySequence<std::string>(temp, count); break;
                    case 7: currentString = new MutableListSequence<std::string>(temp, count);    break;
                    case 8: currentString = new ImmutableListSequence<std::string>(temp, count);  break;
                }
                printSequence<std::string>(currentString, "Создана");
            }

            else if (choice == 9)
            {
                if (isIntMode) 
                {
                    printSequence<int>(currentInt, "Текущая");
                } else
                {
                      printSequence<std::string>(currentString, "Текущая");
                }
            }
            else if (choice == 10)
            {
                if (isIntMode) 
                {
                    opGet<int>(currentInt);
                } else           
                {
                    opGet<std::string>(currentString);
                }
            }
            else if (choice == 11)
            {
                if (isIntMode && currentInt)
                {
                    std::cout << "Length = " << currentInt->GetLength() << "\n";
                } else if (!isIntMode && currentString)
                {
                    std::cout << "Length = " << currentString->GetLength() << "\n";
                } else
                {
                    std::cout << "Последовательность не создана\n";
                }
            }
            else if (choice == 12)
            {
                if (isIntMode) 
                {
                    opAppend<int>(currentInt);
                } else           
                {
                    opAppend<std::string>(currentString);
                }
            }
            else if (choice == 13)
            {
                if (isIntMode) 
                {
                    opPrepend<int>(currentInt);
                }else          
                {
                    opPrepend<std::string>(currentString);
                }
            }
            else if (choice == 14)
            {
                if (isIntMode) 
                {
                    opInsertAt<int>(currentInt);
                }else           
                {
                    opInsertAt<std::string>(currentString);
                }
            }
            else if (choice == 15)
            {
                if (isIntMode) 
                {
                    opRemoveAt<int>(currentInt);
                } else           
                {
                    opRemoveAt<std::string>(currentString);
                }
            }
            else if (choice == 16)
            {
                if (isIntMode) 
                {
                    opSubsequence<int>(currentInt);
                } else           
                {
                    opSubsequence<std::string>(currentString);
                }
            }
            else if (choice == 17)
            {
                if (isIntMode) 
                {
                    opConcat<int>(currentInt);
                } else           
                {
                    opConcat<std::string>(currentString);
                }
            }

            else if (choice == 18)
            {
                std::cout << "Введите биты первой последовательности (0/1): ";
                Bit temp[100];
                int count = readArray<Bit>(temp, 100);
                if (count == 0) 
                { 
                    std::cout << "Ошибка: нет битов\n"; 
                    continue;
                }
                BitSequence bs(temp, count);
                printSequence<Bit>(&bs, "BitSequence");

                BitSequence* bsNot = bs.Not();
                printSequence<Bit>(bsNot, "NOT");
                delete bsNot;

                std::cout << "Введите биты второй последовательности (той же длины): ";
                Bit temp2[100];
                int count2 = readArray<Bit>(temp2, 100);
                if (count2 != count)
                {
                    std::cout << "Ошибка: длины не совпадают\n";
                    continue;
                }
                BitSequence bs2(temp2, count2);
                BitSequence* bsAnd = bs.And(bs2);
                BitSequence* bsOr  = bs.Or(bs2);
                BitSequence* bsXor = bs.Xor(bs2);
                printSequence<Bit>(bsAnd, "AND");
                printSequence<Bit>(bsOr,  "OR");
                printSequence<Bit>(bsXor, "XOR");
                delete bsAnd; 
                delete bsOr; 
                delete bsXor;
            }

            else if (choice == 19)
            {
                std::cout << "Введите int-элементы для AdaptiveSequence: ";
                int temp[100];
                int count = readArray<int>(temp, 100);
                if (count == 0) 
                { 
                    std::cout << "Ошибка: нет элементов\n"; 
                    continue; 
                }
                AdaptiveSequence<int> adaptive(temp, count);
                std::cout << "Реализация: " << adaptive.GetImplType() << "\n";
                printSequence<int>(&adaptive, "AdaptiveSequence");
                for (int i = 0; i < 20; ++i) 
                { 
                    adaptive.Get(i % count);
                }
                std::cout << "Реализация после 20 чтений: " << adaptive.GetImplType() << "\n";
            }

            else if (choice != 0)
                std::cout << "Неизвестная команда. Введите число от 0 до 19\n";
        }
        catch (const IndexOutOfRangeException& e)  
        { 
            std::cerr << "Ошибка индекса: "   << e.what() << "\n"; 
        }
        catch (const EmptyStructureException& e)   
        { 
            std::cerr << "Пустая структура: " << e.what() << "\n"; 
        }
        catch (const InvalidArgumentException& e)  
        { 
            std::cerr << "Ошибка аргумента: " << e.what() << "\n"; 
        }
        catch (const std::exception& e)            
        { 
            std::cerr << "Ошибка: "           << e.what() << "\n"; 
        }

    } while (choice != 0);

    delete currentInt;
    delete currentString;
}

int main()
{
    std::cout << "========================================\n"
              << "   ДЕМОНСТРАЦИЯ\n"
              << "========================================\n";
    demonstrateInt();
    demonstrateString();

    std::cout << "\n========================================\n"
              << "   ИНТЕРАКТИВНЫЙ РЕЖИМ\n"
              << "========================================\n";
    runInteractive();

    std::cout << "\nПрограмма завершена\n";
    return 0;
}
