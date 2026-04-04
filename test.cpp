#include <iostream>
#include <string>

#include "dynamic_array.h"
#include "linked_list.h"
#include "array_sequence.h"
#include "list_sequence.h"
#include "bit_sequence.h"
#include "adaptive_sequence.h"
#include "builder.h"


//Тест-раннер
static int totalPassed = 0;
static int totalFailed = 0;

void beginSuite(const std::string& name)
{
    std::cout << "\n┌─────────────────────────────────────────────\n";
    std::cout << "│  " << name << "\n";
    std::cout << "└─────────────────────────────────────────────\n";
}

void check(const std::string& name, bool condition)
{
    if (condition)
    {
        std::cout << "  [ПРОЙДЕН]  " << name << "\n";
        ++totalPassed;
    }
    else
    {
        std::cout << "  [ПРОВАЛЕН] " << name << "\n";
        ++totalFailed;
    }
}

template <typename ExceptionType, typename Func>
void checkThrows(const std::string& name, Func func)
{
    try
    {
        func();
        std::cout << "  [ПРОВАЛЕН] " << name << " (исключение не брошено)\n";
        ++totalFailed;
    }
    catch (const ExceptionType&)
    {
        std::cout << "  [ПРОЙДЕН]  " << name << "\n";
        ++totalPassed;
    }
    catch (...)
    {
        std::cout << "  [ПРОВАЛЕН] " << name << " (неверный тип исключения)\n";
        ++totalFailed;
    }
}

void printSummary()
{
    std::cout << "\n═══════════════════════════════════════════════\n";
    std::cout << "  ИТОГО: пройдено " << totalPassed
              << ", провалено " << totalFailed
              << ", всего " << (totalPassed + totalFailed) << "\n";
    std::cout << "═══════════════════════════════════════════════\n";
}

//Всп-ые функции для Map/Where/Reduce
static int    doubleInt(const int& x)          
{ 
    return x * 2; 
}

static bool   isEven(const int& x)             
{ 
    return x % 2 == 0; 
}

static int    sumInts(int acc, const int& x)   
{ 
    return acc + x; 
}

static bool   isPositive(const int& x)         
{ 
    return x > 0; 
}

static std::string toUpper(const std::string& s)
{
    std::string r = s;
    for (int i = 0; i < (int)r.size(); ++i)
        if (r[i] >= 'a' && r[i] <= 'z') r[i] = r[i] - 'a' + 'A';
    return r;
}
static bool longWord(const std::string& s) { return (int)s.size() > 4; }

//Тесты
void testDynamicArrayInt()
{
    beginSuite("DynamicArray<int>");

    int arr[] = {10, 20, 30, 40, 50};
    DynamicArray<int> da(arr, 5);
    check("GetSize()==5",      da.GetSize() == 5);
    check("Get(0)==10",        da.Get(0) == 10);
    check("Get(4)==50",        da.Get(4) == 50);
    check("operator[2]==30",   da[2] == 30);

    da.Set(2, 99);
    check("Set(2,99)→Get(2)==99", da.Get(2) == 99);

    da.Resize(7);
    check("Resize(7)→size==7",        da.GetSize() == 7);
    check("Resize(7)→старые данные",  da.Get(1) == 20);

    da.Resize(3);
    check("Resize(3)→size==3", da.GetSize() == 3);

    DynamicArray<int> copy(da);
    copy.Set(0, 777);
    check("Копирующий конструктор", da.Get(0) == 10);

    DynamicArray<int> assigned(1);
    assigned = da;
    assigned.Set(0, 555);
    check("operator=", da.Get(0) == 10);

    checkThrows<IndexOutOfRangeException>(
        "Get(-1)  исключение",       [&]{ da.Get(-1); });
    checkThrows<IndexOutOfRangeException>(
        "Get(100)  исключение",      [&]{ da.Get(100); });
    checkThrows<InvalidArgumentException>(
        "Resize(-1)  исключение",    [&]{ da.Resize(-1); });
    checkThrows<InvalidArgumentException>(
        "DynamicArray(nullptr,3)  исключение",
        [&]{ DynamicArray<int> bad(nullptr, 3); });
}

void testDynamicArrayString()
{
    beginSuite("DynamicArray<std::string>");

    std::string arr[] = {"apple", "banana", "cherry"};
    DynamicArray<std::string> da(arr, 3);
    check("GetSize()==3",          da.GetSize() == 3);
    check("Get(0)==\"apple\"",     da.Get(0) == "apple");
    check("Get(2)==\"cherry\"",    da.Get(2) == "cherry");

    da.Set(1, "blueberry");
    check("Set(1)→\"blueberry\"",  da.Get(1) == "blueberry");

    DynamicArray<std::string> copy(da);
    copy.Set(0, "CHANGED");
    check("Глубокая копия строк",  da.Get(0) == "apple");
}

void testLinkedListInt()
{
    beginSuite("LinkedList<int>");

    int arr[] = {1, 2, 3, 4, 5};
    LinkedList<int> ll(arr, 5);
    check("GetLength()==5",  ll.GetLength() == 5);
    check("GetFirst()==1",   ll.GetFirst() == 1);
    check("GetLast()==5",    ll.GetLast() == 5);
    check("Get(2)==3",       ll.Get(2) == 3);

    ll.Append(6);
    check("Append(6) GetLast()==6",    ll.GetLast() == 6);
    check("Append(6) GetLength()==6",  ll.GetLength() == 6);

    ll.Prepend(0);
    check("Prepend(0) GetFirst()==0",  ll.GetFirst() == 0);

    ll.InsertAt(99, 3);
    check("InsertAt(99,3) Get(3)==99", ll.Get(3) == 99);

    ll.RemoveAt(3);
    check("RemoveAt(3) Get(3)==3",     ll.Get(3) == 3);

    LinkedList<int> sub = ll.GetSubList(1, 3);
    check("GetSubList(1,3) length==3", sub.GetLength() == 3);

    int arr2[] = {10, 20};
    LinkedList<int> ll2(arr2, 2);
    LinkedList<int> cat = ll.Concat(ll2);
    check("Concat→GetLast()==20", cat.GetLast() == 20);

    LinkedList<int> copy(ll);
    copy.Append(777);
    check("Копирующий конструктор ", ll.GetLength() == 7);

    checkThrows<IndexOutOfRangeException>(
        "Get(-1)  исключение",            [&]{ ll.Get(-1); });
    checkThrows<EmptyStructureException>(
        "GetFirst() на пустом списке",     [&]{ LinkedList<int> e; e.GetFirst(); });
    checkThrows<EmptyStructureException>(
        "GetLast() на пустом списке",      [&]{ LinkedList<int> e; e.GetLast(); });
}

void testLinkedListString()
{
    beginSuite("LinkedList<std::string>");

    std::string arr[] = {"cat", "dog", "bird"};
    LinkedList<std::string> ll(arr, 3);
    check("GetFirst()==\"cat\"",   ll.GetFirst() == "cat");
    check("GetLast()==\"bird\"",   ll.GetLast() == "bird");

    ll.Prepend("ant");
    check("Prepend→GetFirst()==\"ant\"", ll.GetFirst() == "ant");

    ll.Append("fish");
    check("Append→GetLast()==\"fish\"",  ll.GetLast() == "fish");

    ll.RemoveAt(0);
    check("RemoveAt(0)→GetFirst()==\"cat\"", ll.GetFirst() == "cat");
}

void testMutableArraySequenceInt()
{
    beginSuite("MutableArraySequence<int>");

    int arr[] = {10, 20, 30, 40, 50};
    MutableArraySequence<int> seq(arr, 5);
    check("GetLength()==5",  seq.GetLength() == 5);
    check("GetFirst()==10",  seq.GetFirst() == 10);
    check("GetLast()==50",   seq.GetLast() == 50);

    Sequence<int>* r = seq.Append(60);
    check("Mutable Append возвращает this", r == &seq);
    check("Append(60) GetLast()==60",       seq.GetLast() == 60);

    seq.Prepend(5);
    check("Prepend(5) GetFirst()==5", seq.GetFirst() == 5);

    seq.InsertAt(25, 3);
    check("InsertAt(25,3) Get(3)==25", seq.Get(3) == 25);

    seq.RemoveAt(3);
    check("RemoveAt(3) отработал", seq.GetLength() == 7);

    Sequence<int>* sub = seq.GetSubsequence(1, 3);
    check("GetSubsequence(1,3) length==3", sub->GetLength() == 3);
    delete sub;

    int arr2[] = {100, 200};
    MutableArraySequence<int> seq2(arr2, 2);
    Sequence<int>* cat = seq.Concat(&seq2);
    check("Concat GetLast()==200", cat->GetLast() == 200);
    if (cat != &seq) delete cat;

    //Map
    MutableArraySequence<int> forMap(arr, 5);
    Sequence<int>* mapped = forMap.Map(doubleInt);
    check("Map(*2) Get(0)==20",  mapped->Get(0) == 20);
    check("Map(*2) Get(4)==100", mapped->Get(4) == 100);
    delete mapped;

    //Where
    int arr3[] = {1, 2, 3, 4, 5, 6};
    MutableArraySequence<int> forWhere(arr3, 6);
    Sequence<int>* filtered = forWhere.Where(isEven);
    check("Where(isEven)→length==3", filtered->GetLength() == 3);
    check("Where(isEven)→Get(0)==2", filtered->Get(0) == 2);
    delete filtered;

    //Reduce
    int arr4[] = {1, 2, 3, 4, 5};
    MutableArraySequence<int> forReduce(arr4, 5);
    check("Reduce(sum)==15", forReduce.Reduce(sumInts, 0) == 15);

    //GetFirst с предикатом
    Option<int> found = forReduce.GetFirst(isPositive);
    check("GetFirst(isPositive)→HasValue()", found.HasValue());
    check("GetFirst(isPositive)==1",         found.GetValue() == 1);

    checkThrows<IndexOutOfRangeException>(
        "Get(-1) исключение",
        [&]{ forReduce.Get(-1); });
    checkThrows<IndexOutOfRangeException>(
        "GetSubsequence(-1,2) исключение",
        [&]{ forReduce.GetSubsequence(-1, 2); });
}

void testImmutableArraySequenceInt()
{
    beginSuite("ImmutableArraySequence<int>");

    int arr[] = {1, 2, 3};
    ImmutableArraySequence<int> seq(arr, 3);
    check("GetLength()==3", seq.GetLength() == 3);

    Sequence<int>* n = seq.Append(4);
    check("Append новый объект",        n != &seq);
    check("Append старый length==3",    seq.GetLength() == 3);
    check("Append новый length==4",     n->GetLength() == 4);
    check("Append новый GetLast()==4",  n->GetLast() == 4);
    delete n;

    Sequence<int>* n2 = seq.Prepend(0);
    check("Prepend новый GetFirst()==0", n2->GetFirst() == 0);
    check("Prepend старый не изменён",   seq.GetFirst() == 1);
    delete n2;

    Sequence<int>* n3 = seq.RemoveAt(1);
    check("RemoveAt→новый length==2",  n3->GetLength() == 2);
    check("RemoveAt→старый length==3", seq.GetLength() == 3);
    delete n3;
}

void testMutableListSequenceInt()
{
    beginSuite("MutableListSequence<int>");

    int arr[] = {5, 10, 15, 20};
    MutableListSequence<int> seq(arr, 4);
    check("GetLength()==4", seq.GetLength() == 4);
    check("GetFirst()==5",  seq.GetFirst() == 5);
    check("GetLast()==20",  seq.GetLast() == 20);

    Sequence<int>* r = seq.Append(25);
    check("Mutable Append возвращает this", r == &seq);
    check("Append(25) GetLast()==25",       seq.GetLast() == 25);

    seq.Prepend(0);
    check("Prepend(0) GetFirst()==0", seq.GetFirst() == 0);

    seq.InsertAt(12, 3);
    check("InsertAt(12,3) Get(3)==12", seq.Get(3) == 12);

    seq.RemoveAt(0);
    check("RemoveAt(0) GetFirst()==5", seq.GetFirst() == 5);

    Sequence<int>* mapped = seq.Map(doubleInt);
    check("Map(*2) Get(0)==10", mapped->Get(0) == 10);
    delete mapped;

    int arr2[] = {1, 2, 3, 4};
    MutableListSequence<int> fr(arr2, 4);
    check("Reduce(sum)==10", fr.Reduce(sumInts, 0) == 10);

    checkThrows<IndexOutOfRangeException>(
        "Get(100) исключение", [&]{ seq.Get(100); });
}

void testImmutableListSequenceInt()
{
    beginSuite("ImmutableListSequence<int>");

    int arr[] = {10, 20, 30};
    ImmutableListSequence<int> seq(arr, 3);

    Sequence<int>* n = seq.Append(40);
    check("Append новый объект",      n != &seq);
    check("Append старый length==3",  seq.GetLength() == 3);
    check("Append новый length==4",   n->GetLength() == 4);
    delete n;

    Sequence<int>* n2 = seq.RemoveAt(0);
    check("RemoveAt→новый GetFirst()==20", n2->GetFirst() == 20);
    check("RemoveAt старый не изменён",    seq.GetFirst() == 10);
    delete n2;
}

void testMutableArraySequenceString()
{
    beginSuite("MutableArraySequence<std::string>");

    std::string arr[] = {"apple", "banana", "cherry", "date", "elderberry"};
    MutableArraySequence<std::string> seq(arr, 5);
    check("GetFirst()==\"apple\"",       seq.GetFirst() == "apple");
    check("GetLast()==\"elderberry\"",   seq.GetLast() == "elderberry");

    seq.Append("fig");
    check("Append GetLast()==\"fig\"",   seq.GetLast() == "fig");

    seq.Prepend("apricot");
    check("Prepend GetFirst()==\"apricot\"", seq.GetFirst() == "apricot");

    seq.InsertAt("blueberry", 2);
    check("InsertAt Get(2)==\"blueberry\"",  seq.Get(2) == "blueberry");

    seq.RemoveAt(0);
    check("RemoveAt(0) GetFirst()==\"apple\"", seq.GetFirst() == "apple");

    Sequence<std::string>* mapped = seq.Map(toUpper);
    check("Map(toUpper) Get(0)==\"APPLE\"", mapped->Get(0) == "APPLE");
    delete mapped;

    Sequence<std::string>* filtered = seq.Where(longWord);
    check("Where(len>4) length>0", filtered->GetLength() > 0);
    delete filtered;

    Option<std::string> found = seq.GetFirst(longWord);
    check("GetFirst(longWord) HasValue()", found.HasValue());

    std::string arr2[] = {"grape", "kiwi"};
    MutableArraySequence<std::string> seq2(arr2, 2);
    Sequence<std::string>* cat = seq.Concat(&seq2);
    check("Concat→GetLast()==\"kiwi\"", cat->GetLast() == "kiwi");
    if (cat != &seq) delete cat;
}

void testMutableListSequenceString()
{
    beginSuite("MutableListSequence<std::string>");

    std::string arr[] = {"one", "two", "three"};
    MutableListSequence<std::string> seq(arr, 3);
    check("GetFirst()==\"one\"",   seq.GetFirst() == "one");
    check("GetLast()==\"three\"",  seq.GetLast() == "three");

    seq.Append("four");
    check("Append GetLast()==\"four\"",  seq.GetLast() == "four");

    seq.Prepend("zero");
    check("Prepend GetFirst()==\"zero\"", seq.GetFirst() == "zero");

    seq.RemoveAt(seq.GetLength() - 1);
    check("RemoveAt(last) GetLast()==\"three\"", seq.GetLast() == "three");

    Sequence<std::string>* mapped = seq.Map(toUpper);
    check("Map(toUpper) Get(0)==\"ZERO\"", mapped->Get(0) == "ZERO");
    delete mapped;
}

void testBitSequence()
{
    beginSuite("BitSequence");

    Bit b1[] = {Bit(1), Bit(0), Bit(1), Bit(1)};
    Bit b2[] = {Bit(1), Bit(1), Bit(0), Bit(1)};
    BitSequence bs1(b1, 4);
    BitSequence bs2(b2, 4);
    check("GetLength()==4",  bs1.GetLength() == 4);
    check("Get(0)==1",       bs1.Get(0).getValue() == true);
    check("Get(1)==0",       bs1.Get(1).getValue() == false);

    //NOT
    BitSequence* bsNot = bs1.Not();
    check("NOT[0]: 1 в 0", bsNot->Get(0).getValue() == false);
    check("NOT[1]: 0 в 1", bsNot->Get(1).getValue() == true);
    check("NOT[2]: 1 в 0", bsNot->Get(2).getValue() == false);
    check("NOT[3]: 1 в 0", bsNot->Get(3).getValue() == false);
    delete bsNot;

    //AND
    BitSequence* bsAnd = bs1.And(bs2);
    check("AND[0]: 1&1==1", bsAnd->Get(0).getValue() == true);
    check("AND[1]: 0&1==0", bsAnd->Get(1).getValue() == false);
    check("AND[2]: 1&0==0", bsAnd->Get(2).getValue() == false);
    check("AND[3]: 1&1==1", bsAnd->Get(3).getValue() == true);
    delete bsAnd;

    //OR
    BitSequence* bsOr = bs1.Or(bs2);
    check("OR[0]: 1|1==1", bsOr->Get(0).getValue() == true);
    check("OR[1]: 0|1==1", bsOr->Get(1).getValue() == true);
    check("OR[2]: 1|0==1", bsOr->Get(2).getValue() == true);
    check("OR[3]: 1|1==1", bsOr->Get(3).getValue() == true);
    delete bsOr;

    //XOR
    BitSequence* bsXor = bs1.Xor(bs2);
    check("XOR[0]: 1^1==0", bsXor->Get(0).getValue() == false);
    check("XOR[1]: 0^1==1", bsXor->Get(1).getValue() == true);
    check("XOR[2]: 1^0==1", bsXor->Get(2).getValue() == true);
    check("XOR[3]: 1^1==0", bsXor->Get(3).getValue() == false);
    delete bsXor;

    Bit b3[] = {Bit(1), Bit(0)};
    BitSequence bs3(b3, 2);
    checkThrows<InvalidArgumentException>(
        "And с разной длиной исключение", [&]{ bs1.And(bs3); });
    checkThrows<InvalidArgumentException>(
        "Or с разной длиной исключение",  [&]{ bs1.Or(bs3); });
    checkThrows<InvalidArgumentException>(
        "Xor с разной длиной исключение", [&]{ bs1.Xor(bs3); });
}

void testAdaptiveSequenceInt()
{
    beginSuite("AdaptiveSequence<int>");

    int arr[] = {1, 2, 3, 4, 5};
    AdaptiveSequence<int> seq(arr, 5, 5);
    check("GetLength()==5", seq.GetLength() == 5);
    check("GetFirst()==1",  seq.GetFirst() == 1);
    check("GetLast()==5",   seq.GetLast() == 5);
    check("Начальная реализация — ArraySequence",
          std::string(seq.GetImplType()) == "ArraySequence");

    //20 чтений - остаётся ArraySequence
    for (int i = 0; i < 20; ++i) seq.Get(i % 5);
    check("После 20 чтений ArraySequence",
          std::string(seq.GetImplType()) == "ArraySequence");

    //10 вставок - переключается на ListSequence
    AdaptiveSequence<int> seq2(arr, 5, 5);
    for (int i = 0; i < 10; ++i) seq2.Append(i);
    check("После 10 вставок ListSequence",
          std::string(seq2.GetImplType()) == "ListSequence");

    //Операции работают после переключения
    check("GetLength() после вставок корректен", seq2.GetLength() == 15);

    Sequence<int>* sub = seq.GetSubsequence(0, 2);
    check("GetSubsequence(0,2) length==3", sub->GetLength() == 3);
    delete sub;

    Sequence<int>* mapped = seq.Map(doubleInt);
    check("Map(*2) Get(0)==2", mapped->Get(0) == 2);
    delete mapped;

    int arr2[] = {1, 2, 3};
    AdaptiveSequence<int> fr(arr2, 3);
    check("Reduce(sum)==6", fr.Reduce(sumInts, 0) == 6);

    AdaptiveSequence<int> copy(fr);
    copy.Append(100);
    check("Глубокая копия — оригинал не изменён", fr.GetLength() == 3);

    checkThrows<IndexOutOfRangeException>(
        "GetSubsequence(-1,2) исключение",
        [&]{ seq.GetSubsequence(-1, 2); });
}

void testBuilders()
{
    beginSuite("ArraySequenceBuilder и ListSequenceBuilder");

    ArraySequenceBuilder<int> ab;
    ab.Add(10).Add(20).Add(30);

    MutableArraySequence<int>* ma = ab.BuildMutable();
    check("ArrayBuilder Mutable length==3",  ma->GetLength() == 3);
    check("ArrayBuilder Mutable Get(0)==10", ma->Get(0) == 10);
    check("ArrayBuilder Mutable Get(2)==30", ma->Get(2) == 30);
    delete ma;

    ImmutableArraySequence<int>* ia = ab.BuildImmutable();
    check("ArrayBuilder Immutable→length==3",  ia->GetLength() == 3);
    check("ArrayBuilder Immutable→Get(1)==20", ia->Get(1) == 20);
    delete ia;

    ab.Clear();
    int range[] = {5, 10, 15, 20};
    ab.AddRange(range, 4);
    MutableArraySequence<int>* ranged = ab.BuildMutable();
    check("AddRange→length==4",  ranged->GetLength() == 4);
    check("AddRange→Get(3)==20", ranged->Get(3) == 20);
    delete ranged;

    ListSequenceBuilder<int> lb;
    lb.Add(1).Add(2).Add(3).Add(4);

    MutableListSequence<int>* ml = lb.BuildMutable();
    check("ListBuilder Mutable→length==4",  ml->GetLength() == 4);
    check("ListBuilder Mutable→Get(0)==1",  ml->Get(0) == 1);
    check("ListBuilder Mutable→Get(3)==4",  ml->Get(3) == 4);
    delete ml;

    ImmutableListSequence<int>* il = lb.BuildImmutable();
    check("ListBuilder Immutable length==4", il->GetLength() == 4);
    delete il;

    ArraySequenceBuilder<std::string> sb;
    sb.Add("hello").Add("world");
    MutableArraySequence<std::string>* ss = sb.BuildMutable();
    check("ArrayBuilder<string> length==2",         ss->GetLength() == 2);
    check("ArrayBuilder<string> Get(0)==\"hello\"", ss->Get(0) == "hello");
    delete ss;
}

void testIterator()
{
    beginSuite("IEnumerator");

    int arr[] = {10, 20, 30};

    //DynamicArray
    DynamicArray<int> da(arr, 3);
    IEnumerator<int>* it = da.GetEnumerator();
    bool ok = true; int idx = 0;
    while (it->MoveNext()) 
    { 
        if (it->Current() != arr[idx++]) 
        {
            ok = false; 
        }
    }
    check("DynamicArray итератор: все элементы", ok && idx == 3);
    it->Reset();
    check("Reset()→MoveNext() с начала", it->MoveNext() && it->Current() == 10);
    it->Release();

    //LinkedList
    LinkedList<int> ll(arr, 3);
    IEnumerator<int>* it2 = ll.GetEnumerator();
    ok = true; idx = 0;
    while (it2->MoveNext()) 
    { 
        if (it2->Current() != arr[idx++]) 
        {
            ok = false; 
        }
    }
    check("LinkedList итератор: все элементы", ok && idx == 3);
    it2->Release();

    // ArraySequence
    MutableArraySequence<int> seq(arr, 3);
    IEnumerator<int>* it3 = seq.GetEnumerator();
    ok = true; idx = 0;
    while (it3->MoveNext()) 
    { 
        if (it3->Current() != arr[idx++]) 
        {
            ok = false; 
        }
    }
    check("ArraySequence итератор: все элементы", ok && idx == 3);
    it3->Release();

    //Пустая коллекция
    DynamicArray<int> empty(0);
    IEnumerator<int>* it4 = empty.GetEnumerator();
    check("Итератор пустой коллекции: MoveNext()==false", !it4->MoveNext());
    it4->Release();
}

void testOption()
{
    beginSuite("Option<T>");

    Option<int> none;
    check("None: HasValue()==false",          !none.HasValue());
    check("None: operator bool == false",     !((bool)none));
    check("None: GetValueOrDefault(42)==42",  none.GetValueOrDefault(42) == 42);
    checkThrows<InvalidOperationException>(
        "None: GetValue() → исключение",  [&]{ none.GetValue(); });
    checkThrows<InvalidOperationException>(
        "None: operator* → исключение",   [&]{ *none; });

    Option<int> some(99);
    check("Some: HasValue()==true",           some.HasValue());
    check("Some: GetValue()==99",             some.GetValue() == 99);
    check("Some: operator*==99",              *some == 99);
    check("Some: GetValueOrDefault(0)==99",   some.GetValueOrDefault(0) == 99);

    Option<int> copy = some;
    check("Копирование: HasValue()",          copy.HasValue());
    check("Копирование: GetValue()==99",      copy.GetValue() == 99);

    copy = none;
    check("Присваивание None: HasValue()==false", !copy.HasValue());

    Option<std::string> strOpt(std::string("hello"));
    check("Option<string>: GetValue()==\"hello\"", strOpt.GetValue() == "hello");

    Option<int> a(5), b(5), c(6);
    check("operator==: a==b (оба 5)",    a == b);
    check("operator==: !(a==c) (5≠6)",   !(a == c));
    check("operator==: none==none",       none == Option<int>());
}

int main()
{
    std::cout << "═══════════════════════════════════════════════\n";
    std::cout << "   АВТОМАТИЧЕСКИЕ ТЕСТЫ \n";
    std::cout << "═══════════════════════════════════════════════\n";

    testDynamicArrayInt();
    testDynamicArrayString();
    testLinkedListInt();
    testLinkedListString();
    testMutableArraySequenceInt();
    testImmutableArraySequenceInt();
    testMutableListSequenceInt();
    testImmutableListSequenceInt();
    testMutableArraySequenceString();
    testMutableListSequenceString();
    testBitSequence();
    testAdaptiveSequenceInt();
    testBuilders();
    testIterator();
    testOption();

    printSummary();
    return (totalFailed == 0) ? 0 : 1;
}
