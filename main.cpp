#include <iostream>
#include "array_sequence.h"
#include "list_sequence.h"
#include "bit_sequence.h"

template <typename T>
int readArray(T* arr, int maxSize) 
{
    int count = 0;          
    T value;                
    
    while (count < maxSize && std::cin >> value) 
    {
        arr[count++] = value;   
    }
    
    std::cin.clear();
    std::cin.ignore(10000, '\n');
    
    return count;  
}

//Специ-я для Bit
template <>
int readArray<Bit>(Bit* arr, int maxSize) 
{
    int count = 0;
    int val;                   
    
    while (count < maxSize && std::cin >> val) 
    {
        
        arr[count++] = Bit(val != 0);
    }
    
    std::cin.clear();
    std::cin.ignore(10000, '\n');
    return count;
}

// Печатает последовательность
template <typename T>
void printSequence(const Sequence<T>* seq, const char* name) 
{
    if (!seq) 
    {
        std::cout << name << " is null." << std::endl;
        return;
    }

    std::cout << name << " (len=" << seq->GetLength() << "): [";
    
    for (int i = 0; i < seq->GetLength(); ++i) 
    {
        if (i > 0) std::cout << ", ";   
        std::cout << seq->Get(i);       
    }
    
    std::cout << "]" << std::endl;
}

//для Bit
template <>
void printSequence<Bit>(const Sequence<Bit>* seq, const char* name) 
{
    if (!seq) 
    {
        std::cout << name << " is null." << std::endl;
        return;
    }
    
    std::cout << name << " (len=" << seq->GetLength() << "): [";
    for (int i = 0; i < seq->GetLength(); ++i) 
    {
        if (i > 0) std::cout << ", ";
        std::cout << (seq->Get(i) ? '1' : '0');
    }
    std::cout << "]" << std::endl;
}

template <>
void printSequence<std::string>(const Sequence<std::string>* seq, const char* name) 
{
    if (!seq) 
    {
        std::cout << name << " is null." << std::endl;
        return;
    }
    
    std::cout << name << " (len=" << seq->GetLength() << "): [";
    for (int i = 0; i < seq->GetLength(); ++i) 
    {
        if (i > 0) std::cout << ", ";
        std::cout << "\"" << seq->Get(i) << "\"";  
    }
    std::cout << "]" << std::endl;
}

void printMenu() 
{
    std::cout << "\n===== Sequence Test Menu =====" << std::endl;
    std::cout << "=== INTEGER SEQUENCE ===" << std::endl;
    std::cout << "1. Create Mutable ArraySequence<int>" << std::endl;
    std::cout << "2. Create Immutable ArraySequence<int>" << std::endl;
    std::cout << "3. Create Mutable ListSequence<int>" << std::endl;
    std::cout << "4. Create Immutable ListSequence<int>" << std::endl;
    std::cout << "=== STRING SEQUENCE ===" << std::endl;
    std::cout << "5. Create Mutable ArraySequence<string>" << std::endl;
    std::cout << "6. Create Immutable ArraySequence<string>" << std::endl;
    std::cout << "7. Create Mutable ListSequence<string>" << std::endl;
    std::cout << "8. Create Immutable ListSequence<string>" << std::endl;
    std::cout << "=== OPERATIONS ===" << std::endl;
    std::cout << "9. Print current sequence" << std::endl;
    std::cout << "10. Get element by index" << std::endl;
    std::cout << "11. Get length" << std::endl;
    std::cout << "12. Append element" << std::endl;
    std::cout << "13. Prepend element" << std::endl;
    std::cout << "14. Insert at index" << std::endl;
    std::cout << "15. Remove at index" << std::endl;
    std::cout << "16. Get subsequence" << std::endl;
    std::cout << "17. Concat with another sequence" << std::endl;
    std::cout << "18. Test BitSequence" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "Choose option: ";
}

// Показывает работу методов на int
void demonstrateIntSequence() 
{
    std::cout << "\n=== DEMONSTRATION: INTEGER SEQUENCE ===\n" << std::endl;
    
    int arr[] = {10, 20, 30, 40, 50};
    MutableArraySequence<int> seq(arr, 5);
    std::cout << "1. Created MutableArraySequence<int> from {10, 20, 30, 40, 50}" << std::endl;
    printSequence(&seq, "  Original");
    
    seq.Append(60);
    std::cout << "\n2. Append(60):" << std::endl;
    printSequence(&seq, "  Result");
    
    seq.Prepend(5);
    std::cout << "\n3. Prepend(5):" << std::endl;
    printSequence(&seq, "  Result");
    
    seq.InsertAt(25, 3);
    std::cout << "\n4. InsertAt(25, 3):" << std::endl;
    printSequence(&seq, "  Result");
    
    std::cout << "\n5. Get(3): " << seq.Get(3) << std::endl;
    
    seq.RemoveAt(4);
    std::cout << "\n6. RemoveAt(4):" << std::endl;
    printSequence(&seq, "  Result");
    
    Sequence<int>* subSeq = seq.GetSubsequence(2, 5);
    std::cout << "\n7. GetSubsequence(2, 5):" << std::endl;
    printSequence(subSeq, "  Subsequence");
    delete subSeq;  
    
    int arr2[] = {100, 200};
    MutableArraySequence<int> seq2(arr2, 2);
    Sequence<int>* concatSeq = seq.Concat(&seq2);
    std::cout << "\n8. Concat with [100, 200]:" << std::endl;
    printSequence(concatSeq, "  Result");
    if (concatSeq != &seq) delete concatSeq; 
}

// для string
void demonstrateStringSequence() 
{
    std::cout << "\n=== DEMONSTRATION: STRING SEQUENCE ===\n" << std::endl;
    
    std::string arr[] = {"apple", "banana", "cherry", "date"};
    MutableArraySequence<std::string> seq(arr, 4);
    std::cout << "1. Created MutableArraySequence<string> from {\"apple\", \"banana\", \"cherry\", \"date\"}" << std::endl;
    printSequence(&seq, "  Original");
    
    seq.Append("tomato");
    std::cout << "\n2. Append(\"tomato\"):" << std::endl;
    printSequence(&seq, "  Result");

    seq.Prepend("apricot");
    std::cout << "\n3. Prepend(\"apricot\"):" << std::endl;
    printSequence(&seq, "  Result");
    
    seq.InsertAt("blueberry", 3);
    std::cout << "\n4. InsertAt(\"blueberry\", 3):" << std::endl;
    printSequence(&seq, "  Result");

    std::cout << "\n5. Get(3): " << seq.Get(3) << std::endl;
    
    seq.RemoveAt(2);
    std::cout << "\n6. RemoveAt(2):" << std::endl;
    printSequence(&seq, "  Result");
    
    Sequence<std::string>* subSeq = seq.GetSubsequence(1, 4);
    std::cout << "\n7. GetSubsequence(1, 4):" << std::endl;
    printSequence(subSeq, "  Subsequence");
    delete subSeq;
    
    std::string arr2[] = {"fig", "grape"};
    MutableArraySequence<std::string> seq2(arr2, 2);
    Sequence<std::string>* concatSeq = seq.Concat(&seq2);
    std::cout << "\n8. Concat with [\"fig\", \"grape\"]:" << std::endl;
    printSequence(concatSeq, "  Result");
    if (concatSeq != &seq) delete concatSeq;
}


int main() 
{
    //автом-я дем-я
    std::cout << "========================================" << std::endl;
    std::cout << "   SEQUENCE DEMONSTRATION" << std::endl;
    std::cout << "========================================" << std::endl;
    
    demonstrateIntSequence();    
    demonstrateStringSequence();  

    //интер-й режим
    std::cout << "\n========================================" << std::endl;
    std::cout << "   INTERACTIVE MODE" << std::endl;
    std::cout << "========================================" << std::endl;
    
    Sequence<int>* currentSeq = nullptr; 
    Sequence<int>* otherSeq = nullptr;    
    int choice;                            
    
    //try для исключений
    try 
    {
        do 
        {
            printMenu();                    
            std::cin >> choice;             
            std::cin.ignore(10000, '\n');  
            
            //соз-е посл для int
            if (choice >= 1 && choice <= 4) 
            {
                std::cout << "Enter integers (space separated): ";
                int temp[100];
                int count = readArray(temp, 100);
                
                if (currentSeq) delete currentSeq;  
                
                switch(choice) 
                {
                    case 1:
                        currentSeq = new MutableArraySequence<int>(temp, count);
                        std::cout << "Mutable ArraySequence<int> created." << std::endl;
                        break;
                    case 2:
                        currentSeq = new ImmutableArraySequence<int>(temp, count);
                        std::cout << "Immutable ArraySequence<int> created." << std::endl;
                        break;
                    case 3:
                        currentSeq = new MutableListSequence<int>(temp, count);
                        std::cout << "Mutable ListSequence<int> created." << std::endl;
                        break;
                    case 4:
                        currentSeq = new ImmutableListSequence<int>(temp, count);
                        std::cout << "Immutable ListSequence<int> created." << std::endl;
                        break;
                }
                printSequence(currentSeq, "Created");
            }
            
            //соз-е посл для string
            else if (choice >= 5 && choice <= 8) 
            {
                std::cout << "Enter strings (space separated): ";
                std::string temp[100];
                readArray(temp, 100);
                
                //Оставляем int режим, строки видны в демонстрации
                std::cout << "String sequences are demonstrated in the demonstration part." << std::endl;
                std::cout << "Interactive mode uses int for simplicity." << std::endl;
            }
            
            //печать пос-и
            else if (choice == 9) 
            {
                if (!currentSeq) 
                {
                    std::cout << "No sequence. Create one first (options 1-4)." << std::endl;
                    continue;
                }
                printSequence(currentSeq, "Current Sequence");
            }
            
            else if (choice == 10) 
            {
                if (!currentSeq) 
                {
                    std::cout << "No sequence." << std::endl;
                    continue;
                }
                int idx;
                std::cout << "Enter index: ";
                std::cin >> idx;
                std::cout << "Element at " << idx << ": " << currentSeq->Get(idx) << std::endl;
                std::cin.ignore(10000, '\n');
            }
            
            else if (choice == 11) 
            {
                if (!currentSeq) 
                {
                    std::cout << "No sequence." << std::endl;
                    continue;
                }
                std::cout << "Length: " << currentSeq->GetLength() << std::endl;
            }
            
            else if (choice == 12) 
            {
                if (!currentSeq) 
                {
                    std::cout << "No sequence." << std::endl;
                    continue;
                }
                int val;
                std::cout << "Enter value to append: ";
                std::cin >> val;
                std::cin.ignore(10000, '\n');
                
                Sequence<int>* newSeq = currentSeq->Append(val);
                //Если вернулся другой объект (immutable), заменяем текущий
                if (newSeq != currentSeq) 
                {
                    delete currentSeq;
                    currentSeq = newSeq;
                }
                std::cout << "Appended." << std::endl;
                printSequence(currentSeq, "Now");
            }
            
            else if (choice == 13) 
            {
                if (!currentSeq) 
                {
                    std::cout << "No sequence." << std::endl;
                    continue;
                }
                int val;
                std::cout << "Enter value to prepend: ";
                std::cin >> val;
                std::cin.ignore(10000, '\n');
                
                Sequence<int>* newSeq = currentSeq->Prepend(val);
                if (newSeq != currentSeq) 
                {
                    delete currentSeq;
                    currentSeq = newSeq;
                }
                std::cout << "Prepended." << std::endl;
                printSequence(currentSeq, "Now");
            }
            
            else if (choice == 14) 
            {
                if (!currentSeq) 
                {
                    std::cout << "No sequence." << std::endl;
                    continue;
                }
                int val, idx;
                std::cout << "Enter index and value: ";
                std::cin >> idx >> val;
                std::cin.ignore(10000, '\n');
                
                Sequence<int>* newSeq = currentSeq->InsertAt(val, idx);
                if (newSeq != currentSeq) 
                {
                    delete currentSeq;
                    currentSeq = newSeq;
                }
                std::cout << "Inserted." << std::endl;
                printSequence(currentSeq, "Now");
            }
            
            else if (choice == 15) 
            {
                if (!currentSeq) 
                {
                    std::cout << "No sequence." << std::endl;
                    continue;
                }
                int idx;
                std::cout << "Enter index to remove: ";
                std::cin >> idx;
                std::cin.ignore(10000, '\n');
                
                Sequence<int>* newSeq = currentSeq->RemoveAt(idx);
                if (newSeq != currentSeq) 
                {
                    delete currentSeq;
                    currentSeq = newSeq;
                }
                std::cout << "Removed." << std::endl;
                printSequence(currentSeq, "Now");
            }
            
            else if (choice == 16) 
            {
                if (!currentSeq) 
                {
                    std::cout << "No sequence." << std::endl;
                    continue;
                }
                int start, end;
                std::cout << "Enter start and end index: ";
                std::cin >> start >> end;
                std::cin.ignore(10000, '\n');
                
                Sequence<int>* subSeq = currentSeq->GetSubsequence(start, end);
                printSequence(subSeq, "Subsequence");
                delete subSeq;
            }
            
            else if (choice == 17) 
            {
                std::cout << "Enter integers for the OTHER sequence: ";
                int temp[100];
                int count = readArray(temp, 100);
                
                if (otherSeq) delete otherSeq;
                otherSeq = new MutableArraySequence<int>(temp, count);
                
                if (!currentSeq) 
                {
                    std::cout << "No current sequence." << std::endl;
                    continue;
                }
                
                Sequence<int>* newSeq = currentSeq->Concat(otherSeq);
                if (newSeq != currentSeq) 
                {
                    delete currentSeq;
                    currentSeq = newSeq;
                }
                std::cout << "Concatenated." << std::endl;
                printSequence(currentSeq, "Now");
            }
            
            //тест Bit пос-и
            else if (choice == 18) 
            {
                std::cout << "Enter bits (e.g., 1 0 1): ";
                Bit temp[100];
                int count = readArray(temp, 100);
                
                BitSequence bs(temp, count);
                printSequence(&bs, "BitSequence");
                
                if (count > 0) 
                {
                    std::cout << "\nTesting NOT operation:" << std::endl;
                    BitSequence* bsNot = bs.Not();
                    printSequence(bsNot, "NOT");
                    delete bsNot;
                    
                    std::cout << "\nEnter second bit sequence (same length): ";
                    Bit temp2[100];
                    int count2 = readArray(temp2, 100);
                    
                    if (count2 == count) 
                    {
                        BitSequence bs2(temp2, count2);
                        
                        BitSequence* bsAnd = bs.And(bs2);
                        BitSequence* bsOr = bs.Or(bs2);
                        BitSequence* bsXor = bs.Xor(bs2);
                        
                        std::cout << "\nBitwise operations:" << std::endl;
                        printSequence(&bs2, "Second");
                        printSequence(bsAnd, "AND");
                        printSequence(bsOr, "OR");
                        printSequence(bsXor, "XOR");
                        
                        delete bsAnd;
                        delete bsOr;
                        delete bsXor;
                    } else {
                        std::cout << "Lengths don't match. Skipping AND/OR/XOR." << std::endl;
                    }
                }
            }
            
        } while (choice != 0);
        
    }
    //Ислючения
    catch (const IndexOutOfRangeException& e) 
    {
        std::cerr << "Index Error: " << e.what() << std::endl;
    }
    catch (const EmptyStructureException& e) 
    {
        std::cerr << "Empty Structure Error: " << e.what() << std::endl;
    }
    catch (const InvalidArgumentException& e) 
    {
        std::cerr << "Invalid Argument Error: " << e.what() << std::endl;
    }
    catch (const MemoryAllocationException& e) 
    {
        std::cerr << "Memory Error: " << e.what() << std::endl;
    }
    catch (const std::exception& e) 
    {
        std::cerr << "Standard Error: " << e.what() << std::endl;
    }
    catch (...) 
    {
        std::cerr << "Unknown error occurred" << std::endl;
    }

    delete currentSeq;
    delete otherSeq;
    
    std::cout << "\nProgram finished." << std::endl;
    return 0;
}




// g++ -Wall -std=c++11 -o sequence_test.exe main.cpp
// sequence_test.exe