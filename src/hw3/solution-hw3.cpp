#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <ctime>
#include <climits>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <memory>
using namespace std;
class InvalidRangeException
{
};
#endif /* __PROGTEST__ */

// uncomment if your code implements initializer lists
// #define EXTENDED_SYNTAX

class CRange
{
private:
    long long low, high;
public:
    CRange(long long low, long long high) : low (low) , high (high)
    {
        if(high < low)
        {
            throw InvalidRangeException();
        }
    }
    long long getLow() const  { return  low; }
    long long getHigh() const { return  high; }
};

class CRangeList
{
public:
    std::vector<std::pair<long long, long long>> listOfIntervals;
private:
    //     CRangeList x { { 5, 20 }, { 150, 200 }, { -9, 12 }, { 48, 93 } };
public:
    CRangeList();
    ~CRangeList();
    CRangeList (const CRangeList & rhs);
    friend  ostream &  operator << (ostream & ostream1, const CRangeList & rangeList1 );
    friend CRangeList  operator +  (const CRange & lhs, const CRange & rhs);
    friend CRangeList  operator -  (const CRange &lhs, const CRange & rhs);
    friend CRangeList  operator ,  (const CRange &lhs, const CRange & rhs);
    CRangeList  operator ,  (const CRange & rhs);
    friend bool operator ==  (const CRangeList &lhs, const CRangeList & rhs);
    friend bool operator !=  (const CRangeList &lhs, const CRangeList & rhs);
    CRangeList & operator -  (const CRange &rhs);
    CRangeList & operator +  (const CRange & rhs);
    CRangeList & operator =  (const CRange & rhs);
    CRangeList & operator =  (const CRangeList & rhs);
    CRangeList & operator += (const CRange & rhs);
    CRangeList & operator += (const CRangeList & rhs);
    CRangeList & operator -= (const CRange & rhs);
    CRangeList & operator -= (const CRangeList & rhs);
    static void mergeIntervals (std::vector<std::pair<long long, long long>> & arr);
    static void removeInterval (std::vector<std::pair<long long, long long>> & arr, std::pair<long long, long long> interval);
    static bool comparatorPairsDescending ( const std::pair<long long, long long> & lhs, const std::pair<long long, long long> & rhs);
    static bool findRange (const std::vector<std::pair<long long, long long>> & arr, const std::pair<long long, long long> &range);

    bool Includes  (const long long & num) const;
    bool Includes  (const CRange & range) const;

};
CRangeList::CRangeList() {}
CRangeList::~CRangeList() {}
CRangeList::CRangeList(const CRangeList & rhs)
{
    this->listOfIntervals.assign(rhs.listOfIntervals.begin(), rhs.listOfIntervals.end());
    this->mergeIntervals(this->listOfIntervals);
}
bool CRangeList::Includes(const long long & num) const
{
    return findRange(this->listOfIntervals, std::make_pair(num, num));
}

bool CRangeList::Includes  (const CRange & range) const
{
    return findRange(this->listOfIntervals,  std::make_pair(range.getLow(), range.getHigh()));
}

ostream & operator << (ostream & ostream1, const CRangeList & rangeList1 )
{
    ostream1 << "{";
    for (size_t i = 0; i < rangeList1.listOfIntervals.size(); i++) {
        if(i != 0) { ostream1 << ","; }
        if(rangeList1.listOfIntervals[i].first == rangeList1.listOfIntervals[i].second)
        {
            ostream1 <<  rangeList1.listOfIntervals[i].first;
        }else{
            ostream1 << "<" << rangeList1.listOfIntervals[i].first << ".." << rangeList1.listOfIntervals[i].second << ">";
        }

    }
    ostream1 << "}";
    return ostream1;
}

CRangeList & CRangeList::operator = (const CRange & rhs)
{
    this->listOfIntervals.clear();
    this->listOfIntervals.emplace_back(std::make_pair(rhs.getLow(), rhs.getHigh()));
    return *this;
}

CRangeList & CRangeList::operator = (const CRangeList& rhs)
{
    this->listOfIntervals.clear();
    this->listOfIntervals.assign(rhs.listOfIntervals.begin(), rhs.listOfIntervals.end());
    mergeIntervals(this->listOfIntervals);
    return *this;
}

CRangeList & CRangeList::operator += (const CRange & rhs)
{
    this->listOfIntervals.emplace_back(std::make_pair(rhs.getLow(), rhs.getHigh()));
    mergeIntervals(this->listOfIntervals);
    return *this;
}

CRangeList & CRangeList::operator += (const CRangeList & rhs)
{
    for( auto i : rhs.listOfIntervals)
    {
        std::pair<long long, long long> newpair = i;
        this->listOfIntervals.push_back(newpair);
    }
    mergeIntervals(this->listOfIntervals);
    return  *this;
}

CRangeList  operator + (const CRange & lhs, const CRange & rhs)
{

    CRangeList intervalslist;
    intervalslist.listOfIntervals.emplace_back(std::make_pair(lhs.getLow(), lhs.getHigh()));
    intervalslist.listOfIntervals.emplace_back(std::make_pair(rhs.getLow(), rhs.getHigh()));
    intervalslist.mergeIntervals(intervalslist.listOfIntervals); // ?????

    return intervalslist;
}

CRangeList & CRangeList::operator + (const CRange & rhs)
{
    this->listOfIntervals.emplace_back(std::make_pair(rhs.getLow(), rhs.getHigh()));
    this->mergeIntervals(this->listOfIntervals);
    return  *this;
}


CRangeList & CRangeList::operator -= (const CRange & rhs)
{
    removeInterval(this->listOfIntervals, std::make_pair(rhs.getLow(), rhs.getHigh()));
    mergeIntervals(this->listOfIntervals);
    return *this;
}

CRangeList & CRangeList::operator -= (const CRangeList & rhs)
{
    for(size_t i = 0; i < rhs.listOfIntervals.size(); ++i)
    {
        removeInterval(this->listOfIntervals, rhs.listOfIntervals[i]);
    }
    mergeIntervals(this->listOfIntervals);
    return *this;
}

CRangeList  operator - (const CRange &lhs, const CRange & rhs) // might be error
{
    CRangeList newlist;
    newlist.listOfIntervals.emplace_back(std::make_pair(lhs.getLow(), lhs.getHigh()));
    newlist.removeInterval(newlist.listOfIntervals, std::make_pair(rhs.getLow(), rhs.getHigh()));
    return newlist;
}

CRangeList & CRangeList::operator - (const CRange &rhs) // might be error
{
    this->removeInterval(this->listOfIntervals, std::make_pair(rhs.getLow(), rhs.getHigh()));
    mergeIntervals(this->listOfIntervals);

    return *this;
}

bool operator == (const CRangeList &lhs, const CRangeList & rhs)
{
    return lhs.listOfIntervals == rhs.listOfIntervals;
}

bool operator != (const CRangeList &lhs, const CRangeList & rhs)
{
    return !(lhs.listOfIntervals == rhs.listOfIntervals);
}

void CRangeList::mergeIntervals (std::vector<std::pair<long long, long long>> & arr)
{
    if(arr.size() <= 1)
    {
        return;
    }

    sort(arr.begin(), arr.end());

    if((arr.size() == 2 && arr[0] == arr[1] )|| (arr.size() == 3 && arr[0] == arr[1] && arr[1] == arr[2]))
    {
        std::pair<long long, long long> temp = arr[0];
        arr.clear();
        arr.push_back(temp);
        return;
    }


    if(arr.size() == 2 && arr[0].first == arr[1].first )
    {
        std::pair<long long, long long> temp ;
        if(arr[0].second < arr[1].second)
        {
            temp = std::make_pair(arr[0].first, arr[1].second);
        }else
        {
            temp = std::make_pair(arr[0].first, arr[1].first);
        }
        arr.clear();
        arr.push_back(temp);
        return;
    }


    sort(arr.begin(), arr.end(), comparatorPairsDescending);
    size_t index = 0;
    size_t arr_size = arr.size();

    for (size_t i = 0; i < arr_size; ++i)
    {
        if (index != 0 && arr[index-1].first - 1 <= arr[i].second)
        {
            while (index != 0 && arr[index-1].first - 1 <= arr[i].second)
            {
                arr[index-1].second = max(arr[index-1].second, arr[i].second);
                arr[index-1].first = min(arr[index-1].first, arr[i].first);
                index--;
            }
        }
        else
            arr[index] = arr[i];

        index++;
    }

    std::vector<std::pair<long long, long long>> temp;
    temp.assign(arr.begin(), arr.begin()+index);
    arr.clear();
    arr.assign(temp.begin(), temp.end());
    temp.clear();

    sort(arr.begin(), arr.end());
}

bool CRangeList::comparatorPairsDescending ( const std::pair<long long, long long> & lhs, const std::pair<long long, long long> & rhs)
{
    return lhs.first > rhs.first;
}

void CRangeList::removeInterval(std::vector<std::pair<long long, long long>> & arr,  std::pair<long long, long long> interval)
{
    if(arr.empty())
        return;

    if(interval.second < arr[0].first)
        return;

    if(arr.size() > 1 && interval.first > arr[arr.size()-1].second)
        return;

    else if(arr.size() == 1 && interval.first > arr[0].second)
        return;

    std::vector<std::pair<long long, long long>> newintervals;
    sort(arr.begin(), arr.end());

    for(size_t i = 0; i < arr.size(); ++i)
    {
        if(interval.first <= arr[i].first && interval.second >= arr[i].second)     // interval >= arr[i]
        {
            // do nothing
        }
        else if(interval.first > arr[i].first && interval.second < arr[i].second)  // interval < arr[i]
        {
            newintervals.emplace_back(std::make_pair(arr[i].first, interval.first-1));
            newintervals.emplace_back(std::make_pair(interval.second+1, arr[i].second));
        }
        else if(interval.first > arr[i].first &&  interval.first <= arr[i].second && interval.second >= arr[i].second)
        {
            newintervals.emplace_back(std::make_pair(arr[i].first, interval.first-1));
            interval.first = arr[i].second;
        }
        else if(interval.first == arr[i].first && interval.second < arr[i].second)
        {
            newintervals.emplace_back(std::make_pair(interval.second+1, arr[i].second));
        }
        else if(interval.first <= arr[i].first && interval.second >= arr[i].first && interval.second < arr[i].second)
        {
            newintervals.emplace_back(std::make_pair(interval.second+1, arr[i].second));
        }
        else
        {
            std::pair<long long, long long> newpair = arr[i];
            newintervals.push_back(newpair);
        }
    }

    arr.clear();
    arr.assign(newintervals.begin(), newintervals.end());
    newintervals.clear();
    sort(arr.begin(), arr.end());
}

bool CRangeList::findRange (const std::vector<std::pair<long long, long long>> & arr, const std::pair<long long, long long> &range)
{
    if(arr.empty())
        return false;
    if(arr.size() > 1 && (arr[0].first > range.second || arr[arr.size()-1].second < range.first))
        return false;
    else if(arr.size() == 1 && (arr[0].first > range.second || arr[0].second < range.first))
        return false;
    for(auto i : arr)
        if(i.first <= range.first && i.second >= range.second)
            return true;
    return false;
}

CRangeList  operator ,  (const CRange &lhs, const CRange & rhs)
{
    CRangeList newlist;
    newlist = lhs + rhs;
    return newlist;
}
CRangeList  CRangeList::operator ,  (const CRange & rhs)
{
    *this += rhs;
    return  *this;
}


#ifndef __PROGTEST__
string             toString                                ( const CRangeList& x )
{
    ostringstream oss;
    oss << x;
    return oss . str ();
}

void testbasic()
{
    CRangeList a, b;

    assert ( sizeof ( CRange ) <= 2 * sizeof ( long long ) );
    a = CRange ( 5, 10 );
    a += CRange ( 25, 100 );
    assert ( toString ( a ) == "{<5..10>,<25..100>}" );
    a += CRange ( -5, 0 );
    a += CRange ( 8, 50 );
    assert ( toString ( a ) == "{<-5..0>,<5..100>}" );
    a += CRange ( 101, 105 ) + CRange ( 120, 150 ) + CRange ( 160, 180 ) + CRange ( 190, 210 );
    assert ( toString ( a ) == "{<-5..0>,<5..105>,<120..150>,<160..180>,<190..210>}" );
    a += CRange ( 106, 119 ) + CRange ( 152, 158 );
    assert ( toString ( a ) == "{<-5..0>,<5..150>,<152..158>,<160..180>,<190..210>}" );
    a += CRange ( -3, 170 );
    a += CRange ( -30, 1000 );
    assert ( toString ( a ) == "{<-30..1000>}" );
    b = CRange ( -500, -300 ) + CRange ( 2000, 3000 ) + CRange ( 700, 1001 );
    a += b;
    assert ( toString ( a ) == "{<-500..-300>,<-30..1001>,<2000..3000>}" );
    a -= CRange ( -400, -400 );
    assert ( toString ( a ) == "{<-500..-401>,<-399..-300>,<-30..1001>,<2000..3000>}" );
    a -= CRange ( 10, 20 ) + CRange ( 900, 2500 ) + CRange ( 30, 40 ) + CRange ( 10000, 20000 );
    assert ( toString ( a ) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}" );
    try
    {
        a += CRange ( 15, 18 ) + CRange ( 10, 0 ) + CRange ( 35, 38 );
        assert ( "Exception not thrown" == NULL );
    }
    catch ( const InvalidRangeException & e )
    {
    }
    catch ( ... )
    {
        assert ( "Invalid exception thrown" == NULL );
    }
    assert ( toString ( a ) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}" );
    b = a;
    assert ( a == b );
    assert ( !( a != b ) );
    b += CRange ( 2600, 2700 );
    assert ( toString ( b ) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}" );
    assert ( a == b );
    assert ( !( a != b ) );
    b += CRange ( 15, 15 );
    assert ( toString ( b ) == "{<-500..-401>,<-399..-300>,<-30..9>,15,<21..29>,<41..899>,<2501..3000>}" );
    assert ( !( a == b ) );
    assert ( a != b );
    assert ( b . Includes ( 15 ) );
    assert ( b . Includes ( 2900 ) );
    assert ( b . Includes ( CRange ( 15, 15 ) ) );
    assert ( b . Includes ( CRange ( -350, -350 ) ) );
    assert ( b . Includes ( CRange ( 100, 200 ) ) );
    assert ( !b . Includes ( CRange ( 800, 900 ) ) );
    assert ( !b . Includes ( CRange ( -1000, -450 ) ) );
    assert ( !b . Includes ( CRange ( 0, 500 ) ) );
    a += CRange ( -10000, 10000 ) + CRange ( 10000000, 1000000000 );
    assert ( toString ( a ) == "{<-10000..10000>,<10000000..1000000000>}" );
    b += a;
    assert ( toString ( b ) == "{<-10000..10000>,<10000000..1000000000>}" );
    b -= a;
    assert ( toString ( b ) == "{}" );
    b += CRange ( 0, 100 ) + CRange ( 200, 300 ) - CRange ( 150, 250 ) + CRange ( 160, 180 ) - CRange ( 170, 170 );
    assert ( toString ( b ) == "{<0..100>,<160..169>,<171..180>,<251..300>}" );
    b -= CRange ( 10, 90 ) - CRange ( 20, 30 ) - CRange ( 40, 50 ) - CRange ( 60, 90 ) + CRange ( 70, 80 );
    assert ( toString ( b ) == "{<0..9>,<20..30>,<40..50>,<60..69>,<81..100>,<160..169>,<171..180>,<251..300>}" );
}

void test0 ()
{
    CRangeList a;
    a += CRange(1,10);
    CRange b(10,10);
    assert(a.Includes(b));
}

void test1 ()
{
    CRangeList a;
    a += CRange(-10000, 10000);
    a += CRange(10000000, 1000000000);
    a += CRange(LLONG_MAX - 1, LLONG_MAX);
    assert(toString(a) == "{<-10000..10000>,<10000000..1000000000>,<9223372036854775806..9223372036854775807>}");
    a += CRange(LLONG_MAX, LLONG_MAX) + CRange(LLONG_MIN, LLONG_MAX);
    assert(toString(a) == "{<-9223372036854775808..9223372036854775807>}");
    a -= a;
    assert(toString(a) == "{}");
    a += CRange(-10000, 10000) + CRange(10000000, 1000000000) + CRange(LLONG_MIN, LLONG_MIN + 1);
    assert(toString(a) == "{<-9223372036854775808..-9223372036854775807>,<-10000..10000>,<10000000..1000000000>}");
    a += CRange(LLONG_MIN, LLONG_MAX);
    assert(toString(a) == "{<-9223372036854775808..9223372036854775807>}");
}

void test2 ()
{
    CRangeList a;
    a = CRange(10, 10) + CRange(20, 20) + CRange(12, 12) + CRange(18, 18);
    assert ( toString ( a ) == "{10,12,18,20}" );
    a -= CRange(11, 19);
    assert ( toString ( a ) == "{10,20}" );
    a = CRange(10, 10) + CRange(20, 20) + CRange(12, 12) + CRange(18, 18);
    assert ( toString ( a ) == "{10,12,18,20}" );
    a -= CRange(10, 20);
    assert ( toString ( a ) == "{}" );
    a = CRange(10, 100);
    a -= CRange(20, 80);
    assert(toString(a) == "{<10..19>,<81..100>}");
    a = CRange(10, 100);
    a -= CRange(11, 99);
    assert(toString(a) == "{10,100}");
    a = CRange(10, 100);
    a -= CRange(11, 101);
    assert(toString(a) == "{10}");
    a = CRange(10, 100);
    a -= CRange(50, 150);
    assert(toString(a) == "{<10..49>}");
    a = CRange(10, 100);
    a -= CRange(0, 50);
    assert(toString(a) == "{<51..100>}");
    a = CRange(10, 100);
    a -= CRange(0, 99);
    assert(toString(a) == "{100}");
    a = CRange(10, 100);
    a -= CRange(10, 100);
    assert(toString(a) == "{}");
    a = CRange(10, 100);
    a -= CRange(0, 80);
    assert(toString(a) == "{<81..100>}");
    a = CRange(0, 10);
    a += CRange(12, 28);
    a += CRange(30, 40);
    a -= CRange(11, 29);
    assert(toString(a) == "{<0..10>,<30..40>}");
    a = CRange(0, 10);
    a += CRange(12, 28);
    a += CRange(30, 40);
    a -= CRange(10, 30);
    assert(toString(a) == "{<0..9>,<31..40>}");
    a = CRange(0, 10);
    a += CRange(12, 28);
    a += CRange(30, 40);
    a += CRange(-10, -5);
    a += CRange(50, 60);
    a -= CRange(0, 40);
    assert ( toString ( a ) == "{<-10..-5>,<50..60>}" );
    a = CRange(0, 10);
    a += CRange(12, 28);
    a += CRange(30, 40);
    a += CRange(-10, -5);
    a += CRange(50, 60);
    a -= CRange(1, 39);
    assert ( toString ( a ) == "{<-10..-5>,0,40,<50..60>}" );
    a = CRange(0, 10);
    a += CRange(12, 28);
    a += CRange(30, 40);
    a += CRange(-10, -5);
    a += CRange(50, 60);
    a -= CRange(2, 38);
    assert ( toString ( a ) == "{<-10..-5>,<0..1>,<39..40>,<50..60>}" );
    a = CRange(0, 10);
    a += CRange(12, 28);
    a += CRange(30, 40);
    a += CRange(-10, -5);
    a += CRange(50, 60);
    a -= CRange(-5, 50);
    assert ( toString ( a ) == "{<-10..-6>,<51..60>}" );
    a = CRange(0, 10);
    a += CRange(12, 28);
    a += CRange(30, 40);
    a += CRange(-10, -5);
    a += CRange(50, 60);
    a -= CRange(-8, 55);
    assert ( toString ( a ) == "{<-10..-9>,<56..60>}" );
    a = CRange(0, 10);
    a += CRange(12, 28);
    a += CRange(30, 40);
    a += CRange(-10, -5);
    a += CRange(50, 60);
    a -= CRange(-9, 59);
    assert ( toString ( a ) == "{-10,60}" );
    a = CRange(0, 10);
    a += CRange(12, 28);
    a += CRange(30, 40);
    a += CRange(-10, -5);
    a += CRange(50, 60);
    a -= CRange(-10, 60);
    assert ( toString ( a ) == "{}" );
    a = CRange(2, 2);
    a += CRange(4, 4);
    a -= CRange(2, 2);
    a -= CRange(4, 4);
    assert ( toString ( a ) == "{}" );
    a = CRange(2, 2);
    a += CRange(4, 4);
    a += CRange(0, 0);
    a += CRange(6, 6);
    a -= CRange(2, 2);
    a -= CRange(4, 4);
    assert ( toString ( a ) == "{0,6}" );
    a = CRange(0, 0);
    a += CRange(10, 10);
    a += CRange(2, 8);
    a -= CRange(2, 2);
    a -= CRange(8, 8);
    assert ( toString ( a ) == "{0,<3..7>,10}" );
    a = CRange(0, 0);
    a += CRange(10, 10);
    a += CRange(5, 5);
    a -= CRange(5, 5);
    assert ( toString ( a ) == "{0,10}" );
    a = CRange(0, 10);
    a -= CRange(0, 0);
    a -= CRange(10, 10);
    assert ( toString ( a ) == "{<1..9>}" );
    a = CRange(0, 10);
    a += CRange(12, 20);
    a += CRange(22, 30);
    a -= CRange(11, 11);
    a -= CRange(21, 21);
    assert ( toString ( a ) == "{<0..10>,<12..20>,<22..30>}" );
    a = CRange(0, 10);
    a += CRange(12, 20);
    a += CRange(22, 30);
    a -= CRange(5, 5);
    a -= CRange(25, 25);
    assert ( toString ( a ) == "{<0..4>,<6..10>,<12..20>,<22..24>,<26..30>}" );
    a = CRange(0, 10);
    a += CRange(12, 20);
    a += CRange(22, 30);
    a -= CRange(-1, -1);
    a -= CRange(31, 31);
    assert ( toString ( a ) == "{<0..10>,<12..20>,<22..30>}" );
    a -= a;
    for (int sf = 0; sf <= 60; sf++) {
        if (sf % 2 == 0) {
            a -= CRange(sf, sf);
        } else {
            a += CRange(sf, sf);
        }
    }
    assert ( toString ( a ) == "{1,3,5,7,9,11,13,15,17,19,21,23,25,27,29,31,33,35,37,39,41,43,45,47,49,51,53,55,57,59}" );
    a -= CRange(2, 58);
    assert ( toString ( a ) == "{1,59}" );
    a = CRange(10, 50);
    a += CRange(0, 8);
    a += CRange(52, 60);
    a += CRange(9, 51);
    assert ( toString ( a ) == "{<0..60>}" );
}


void test3 ()
{
    CRangeList a;
    a = CRange(LLONG_MIN+1, LLONG_MAX-1);
    assert(toString(a) == "{<" + to_string(LLONG_MIN+1) + ".." + to_string(LLONG_MAX-1) + ">}");
    a += CRange(LLONG_MIN, LLONG_MAX);
    assert(toString(a) == "{<" + to_string(LLONG_MIN) + ".." + to_string(LLONG_MAX) + ">}");
    a -= CRange(LLONG_MIN, LLONG_MIN);
    a -= CRange(LLONG_MAX, LLONG_MAX);
    assert(toString(a) == "{<" + to_string(LLONG_MIN+1) + ".." + to_string(LLONG_MAX-1) + ">}");
    a += CRange(LLONG_MIN, LLONG_MIN);
    a += CRange(LLONG_MAX, LLONG_MAX);
    assert(toString(a) == "{<" + to_string(LLONG_MIN) + ".." + to_string(LLONG_MAX) + ">}");
    a -= CRange(0, 0);
    assert(toString(a) == "{<" + to_string(LLONG_MIN) + "..-1>,<1.." + to_string(LLONG_MAX) + ">}");
    a += CRange(0, 0);
    assert(toString(a) == "{<" + to_string(LLONG_MIN) + ".." + to_string(LLONG_MAX) + ">}");
    a -= CRange(LLONG_MIN+1, LLONG_MAX-1);
    assert(toString(a) == "{" + to_string(LLONG_MIN) + "," + to_string(LLONG_MAX) + "}");
}


void test4 ()
{
    CRangeList list;
    int a, b;
    cout << "add 2 integers";
    cin >> a >> b;
    srand(a*a + b );
    for (int i = 0; i< 10000; ++i){
        int tmp = random();
        int lel = random()%2;
        if (lel){
            list+=CRange(tmp, tmp+random());
        }else{
            list+=CRange(tmp, tmp+random());
        }
    }
}


void test5 ()
{
    CRangeList a;
    assert(toString(CRange(-1595,0) + CRange(0,LLONG_MAX) + CRange(LLONG_MAX,LLONG_MAX)) ==
           "{<-1595..9223372036854775807>}");
}


void test6 ()
{
    CRangeList a;
    a = CRange(0, 0) + CRange(6, 6);
    a -= CRange(-1, 1);
    a -= CRange(5, 7);
    assert(toString(a) == "{}");
    a = CRange(0, 0) + CRange(6, 6);
    assert(toString(a) == "{0,6}");
    a += CRange(1, 2) + CRange(3, 4) + CRange(5, 6) + CRange(7, 8) + CRange(9, 10);
    a -= a;
    assert(toString(a) == "{}");
    a = CRange(1, 1);
    a += CRange(2, 2);
    a -= CRange(2, 2);
    a -= CRange(2, 2) + CRange(1, 1);
    assert(toString(a) == "{}");
    a = CRange(10, 15);
    a += CRange(1, 5);
    assert(toString(a) == "{<1..5>,<10..15>}");
    a = CRange(10, 15) + CRange(20, 25);
    a += CRange(1, 5);
    assert(toString(a) == "{<1..5>,<10..15>,<20..25>}");
    a = CRange(1, 5);
    a += CRange(10, 15);
    assert(toString(a) == "{<1..5>,<10..15>}");
    a = CRange(1, 5) + CRange(10, 15);
    a += CRange(20, 25);
    assert(toString(a) == "{<1..5>,<10..15>,<20..25>}");
    a = CRange(10, 15);
    a -= CRange(1, 5);
    assert(toString(a) == "{<10..15>}");
    a = CRange(10, 15) + CRange(20, 25);
    a -= CRange(1, 5);
    assert(toString(a) == "{<10..15>,<20..25>}");
    a = CRange(1, 5);
    a -= CRange(10, 15);
    assert(toString(a) == "{<1..5>}");
    a = CRange(1, 5) + CRange(10, 15);
    a -= CRange(20, 25);
    assert(toString(a) == "{<1..5>,<10..15>}");
    a = CRange(10, 15);
    a += CRange(1, 9);
    assert(toString(a) == "{<1..15>}");
    a = CRange(10, 15);
    a += CRange(1, 10);
    assert(toString(a) == "{<1..15>}");
    a = CRange(10, 15);
    a += CRange(1, 11);
    assert(toString(a) == "{<1..15>}");
    a = CRange(10, 15) + CRange(20, 25);
    a += CRange(1, 9);
    assert(toString(a) == "{<1..15>,<20..25>}");
    a = CRange(10, 15) + CRange(20, 25);
    a += CRange(1, 10);
    assert(toString(a) == "{<1..15>,<20..25>}");
    a = CRange(10, 15) + CRange(20, 25);
    a += CRange(1, 11);
    assert(toString(a) == "{<1..15>,<20..25>}");
    a = CRange(1, 5);
    a += CRange(6, 15);
    assert(toString(a) == "{<1..15>}");
    a = CRange(1, 5);
    a += CRange(5, 15);
    assert(toString(a) == "{<1..15>}");
    a = CRange(1, 5);
    a += CRange(4, 15);
    assert(toString(a) == "{<1..15>}");
    a = CRange(1, 5) + CRange(10, 15);
    a += CRange(16, 25);
    assert(toString(a) == "{<1..5>,<10..25>}");
    a = CRange(1, 5) + CRange(10, 15);
    a += CRange(15, 25);
    assert(toString(a) == "{<1..5>,<10..25>}");
    a = CRange(1, 5) + CRange(10, 15);
    a += CRange(14, 25);
    assert(toString(a) == "{<1..5>,<10..25>}");
    a = CRange(10, 15) + CRange(20, 25);
    a += CRange(16, 19);
    assert(toString(a) == "{<10..25>}");
    a = CRange(10, 15) + CRange(20, 25);
    a += CRange(15, 20);
    assert(toString(a) == "{<10..25>}");
    a = CRange(10, 15) + CRange(20, 25);
    a += CRange(14, 21);
    assert(toString(a) == "{<10..25>}");
    a = CRange(10, 15);
    a -= CRange(1, 9);
    assert(toString(a) == "{<10..15>}");
    a = CRange(10, 15);
    a -= CRange(1, 10);
    assert(toString(a) == "{<11..15>}");
    a = CRange(10, 15);
    a -= CRange(1, 11);
    assert(toString(a) == "{<12..15>}");
    a = CRange(1, 5);
    a -= CRange(6, 15);
    assert(toString(a) == "{<1..5>}");
    a = CRange(1, 5);
    a -= CRange(5, 15);
    assert(toString(a) == "{<1..4>}");
    a = CRange(1, 5);
    a -= CRange(4, 15);
    assert(toString(a) == "{<1..3>}");
    a = CRange(10, 15) + CRange(20, 25);
    a -= CRange(16, 19);
    assert(toString(a) == "{<10..15>,<20..25>}");
    a = CRange(10, 15) + CRange(20, 25);
    a -= CRange(15, 20);
    assert(toString(a) == "{<10..14>,<21..25>}");
    a = CRange(10, 15) + CRange(20, 25);
    a -= CRange(14, 21);
    assert(toString(a) == "{<10..13>,<22..25>}");
    a = CRange(1, 10);
    a += CRange(2, 8);
    assert(toString(a) == "{<1..10>}");
    a = CRange(1, 10);
    a += CRange(1, 10);
    assert(toString(a) == "{<1..10>}");
    a = CRange(1, 10) + CRange(20, 30);
    a += CRange(2, 8);
    assert(toString(a) == "{<1..10>,<20..30>}");
    a = CRange(1, 10) + CRange(20, 30);
    a += CRange(1, 10);
    assert(toString(a) == "{<1..10>,<20..30>}");
    a = CRange(1, 10) + CRange(20, 30);
    a += CRange(2, 8) + CRange(22, 28);
    assert(toString(a) == "{<1..10>,<20..30>}");
    a = CRange(1, 10) + CRange(20, 30);
    a += CRange(1, 10) + CRange(20, 30);
    assert(toString(a) == "{<1..10>,<20..30>}");
    a = CRange(1, 10);
    a -= CRange(2, 9);
    assert(toString(a) == "{1,10}");
    a = CRange(1, 10);
    a -= CRange(3, 8);
    assert(toString(a) == "{<1..2>,<9..10>}");
    a = CRange(1, 10);
    a -= CRange(1, 10);
    assert(toString(a) == "{}");
    a = CRange(1, 10);
    a -= CRange(0, 11);
    assert(toString(a) == "{}");
    a = CRange(1, 10) + CRange(20, 30);
    a -= CRange(1, 10);
    assert(toString(a) == "{<20..30>}");
    a = CRange(1, 10) + CRange(20, 30);
    a -= CRange(2, 9);
    assert(toString(a) == "{1,10,<20..30>}");
    a = CRange(1, 10) + CRange(20, 30);
    a -= CRange(3, 8);
    assert(toString(a) == "{<1..2>,<9..10>,<20..30>}");
    a = CRange(1, 10) + CRange(20, 30);
    a -= CRange(1, 10) + CRange(20, 30);
    assert(toString(a) == "{}");
    a = CRange(1, 10) + CRange(20, 30);
    a -= CRange(2, 9) + CRange(21, 29);
    assert(toString(a) == "{1,10,20,30}");
    a = CRange(1, 10) + CRange(20, 30);
    a -= CRange(3, 8) + CRange(22, 28);
    assert(toString(a) == "{<1..2>,<9..10>,<20..21>,<29..30>}");
}


void test7 ()
{
    CRangeList a;
    a = CRange(LLONG_MIN, LLONG_MAX);
    a -= CRange(LLONG_MIN, LLONG_MIN) + CRange(LLONG_MAX, LLONG_MAX);
    assert(!a.Includes(LLONG_MIN));
    assert(!a.Includes(LLONG_MAX));
    assert(a.Includes(0));
    a = CRange(LLONG_MIN, LLONG_MAX);
    a -= CRange(LLONG_MIN, LLONG_MIN) + CRange(0,0);
    assert(!a.Includes(LLONG_MIN));
    assert(!a.Includes(0));
    a = CRange(LLONG_MIN, LLONG_MAX);
    a -= CRange(LLONG_MAX, LLONG_MAX) + CRange(0,0);
    assert(a.Includes(LLONG_MIN));
    assert(!a.Includes(0));
    a = CRange(LLONG_MIN, LLONG_MAX);
    a -= CRange(LLONG_MIN, 0);
    assert(!a.Includes(LLONG_MIN));
    assert(!a.Includes(0));
    assert(!a.Includes(-100));
    assert(a.Includes(100));
    a = CRange(LLONG_MIN, LLONG_MAX);
    a -= CRange(LLONG_MIN, 0);
    assert(!a.Includes(LLONG_MIN));
    assert(!a.Includes(0));
    assert(!a.Includes(-100));
    assert(a.Includes(100));
    assert(a.Includes(LLONG_MAX));
    a = CRange(LLONG_MIN, LLONG_MAX);
    a -= CRange(0, LLONG_MAX);
    assert(!a.Includes(LLONG_MAX));
    assert(!a.Includes(0));
    assert(a.Includes(-100));
    assert(!a.Includes(100));
    assert(a.Includes(LLONG_MIN));
    a = CRange(LLONG_MIN, LLONG_MAX);
    a -= CRange(LLONG_MIN, LLONG_MAX);
    assert(toString(a) == "{}");
    a = CRange(LLONG_MIN, LLONG_MIN);
    a += CRange(LLONG_MAX, LLONG_MAX);
    a -= CRange(LLONG_MAX, LLONG_MAX);
    a -= CRange(LLONG_MIN, LLONG_MIN);
    assert(toString(a) == "{}");
    a = CRange(LLONG_MAX, LLONG_MAX);
    a += CRange(LLONG_MIN, LLONG_MIN);
    a -= CRange(LLONG_MAX, LLONG_MAX);
    a -= CRange(LLONG_MIN, LLONG_MIN);
    assert(toString(a) == "{}");
    a = CRange(LLONG_MIN, LLONG_MIN);
    a -= CRange(LLONG_MIN, LLONG_MIN);
    assert(toString(a) == "{}");
    a = CRange(LLONG_MAX, LLONG_MAX);
    a -= CRange(LLONG_MAX, LLONG_MAX);
    assert(toString(a) == "{}");
    a = CRange(LLONG_MAX, LLONG_MAX);
    a += CRange(LLONG_MAX, LLONG_MAX);
    assert(toString(a) == "{9223372036854775807}");
    a = CRange(LLONG_MIN, LLONG_MIN);
    a += CRange(LLONG_MIN, LLONG_MIN);
    assert(toString(a) == "{-9223372036854775808}");
    a = CRange(LLONG_MIN, LLONG_MAX);
    a += CRange(LLONG_MIN, LLONG_MAX);
    assert(toString(a) == "{<-9223372036854775808..9223372036854775807>}");
    a = CRange(LLONG_MIN, LLONG_MAX);
    a += CRange(LLONG_MIN, LLONG_MAX);
    a += CRange(LLONG_MIN, LLONG_MAX);
    a += CRange(LLONG_MIN, LLONG_MAX);
    assert(toString(a) == "{<-9223372036854775808..9223372036854775807>}");
    a = CRange(LLONG_MIN, LLONG_MAX);
    a += CRange(LLONG_MIN, LLONG_MAX);
    a += CRange(LLONG_MIN, LLONG_MAX);
    a += CRange(LLONG_MIN, LLONG_MAX);
    a -= CRange(LLONG_MIN, LLONG_MAX);
    a -= CRange(LLONG_MIN, LLONG_MAX);
    a -= CRange(LLONG_MIN, LLONG_MAX);
    assert(toString(a) == "{}");
    a = CRange(LLONG_MIN, LLONG_MAX);
    a -= CRange(LLONG_MIN + 1, LLONG_MAX - 1);
    assert(toString(a) == "{-9223372036854775808,9223372036854775807}");
    a = CRange(LLONG_MIN, LLONG_MIN) + CRange(LLONG_MAX, LLONG_MAX);
    a += CRange(LLONG_MIN + 1, LLONG_MAX - 1);
    assert(toString(a) == "{<-9223372036854775808..9223372036854775807>}");
    a = CRange(LLONG_MIN, LLONG_MIN) + CRange(LLONG_MAX, LLONG_MAX);
    a += CRange(LLONG_MIN + 2, LLONG_MAX - 2);
    assert(toString(a) == "{-9223372036854775808,<-9223372036854775806..9223372036854775805>,9223372036854775807}");
    a = CRange(LLONG_MAX, LLONG_MAX);
    a += CRange(LLONG_MIN, LLONG_MAX - 1);
    assert(toString(a) == "{<-9223372036854775808..9223372036854775807>}");
    a = CRange(LLONG_MIN, LLONG_MIN);
    a += CRange(LLONG_MIN + 1, LLONG_MAX);
    assert(toString(a) == "{<-9223372036854775808..9223372036854775807>}");
    a = CRange(LLONG_MIN, LLONG_MIN) + CRange(LLONG_MAX, LLONG_MAX);
    a -= CRange(LLONG_MIN, LLONG_MAX);
    assert(toString(a) == "{}");
    a = CRange(LLONG_MIN, LLONG_MIN);
    a -= CRange(LLONG_MIN, LLONG_MAX);
    assert(toString(a) == "{}");
    a = CRange(LLONG_MAX, LLONG_MAX);
    a -= CRange(LLONG_MIN, LLONG_MAX);
    assert(toString(a) == "{}");
    a = CRange(LLONG_MIN, LLONG_MIN) + CRange(LLONG_MAX, LLONG_MAX);
    a -= CRange(LLONG_MIN + 1, LLONG_MAX - 1);
    assert(toString(a) == "{-9223372036854775808,9223372036854775807}");
    a = CRange(LLONG_MIN, LLONG_MIN) + CRange(LLONG_MAX, LLONG_MAX);
    a += CRange(LLONG_MIN + 1, LLONG_MAX - 1);
    assert(toString(a) == "{<-9223372036854775808..9223372036854775807>}");
    a = CRange(LLONG_MIN, LLONG_MIN);
    a -= CRange(LLONG_MIN + 1, LLONG_MAX - 1);
    assert(toString(a) == "{-9223372036854775808}");
    a = CRange(LLONG_MAX, LLONG_MAX);
    a -= CRange(LLONG_MIN + 1, LLONG_MAX - 1);
    assert(toString(a) == "{9223372036854775807}");
}


void test8 ()
{
    CRangeList a;
    assert(!a.Includes(5));
    assert(!a.Includes(CRange(5, 10)));
    a = CRange(5, 10);
    assert(a.Includes(5));
    assert(a.Includes(10));
    assert(!a.Includes(4));
    assert(!a.Includes(11));
    assert(a.Includes(CRange(5, 10)));
    assert(!a.Includes(CRange(4, 10)));
    assert(!a.Includes(CRange(5, 11)));
}


void test9 ()
{
    CRangeList a = CRange(10, 20) + CRange(0, 9) + CRange(21, 30);
    assert ( toString ( a ) == "{<0..30>}" );
    a = CRange(10, 20) + CRange(0, 8) + CRange(22, 30);
    assert ( toString ( a ) == "{<0..8>,<10..20>,<22..30>}" );
    a = CRange(10, 20) + CRange(22, 30) + CRange(0, 50);
    assert ( toString ( a ) == "{<0..50>}" );
    a -= CRange(-5, 5) + CRange(25, 35) + CRange(45, 55);
    assert ( toString ( a ) == "{<6..24>,<36..44>}" );
    CRangeList b;
    b = CRange(0, 0);
    b -= CRange(0, 0);
    assert ( toString ( b ) == "{}" );
    b -= CRange(0, 10);
    assert ( toString ( b ) == "{}" );
    b += CRange ( 25, 100 );
    assert ( toString ( b ) == "{<25..100>}" );
    b -= CRange(25, 25);
    assert ( toString ( b ) == "{<26..100>}" );
    b += CRange(1000, 1200);
    b -= CRange(1000, 1000);
    assert ( toString ( b ) == "{<26..100>,<1001..1200>}" );
    b -= CRange(1200, 1200);
    assert ( toString ( b ) == "{<26..100>,<1001..1199>}" );
    b += CRange(30, 1100);
    assert ( toString ( b ) == "{<26..1199>}" );
    b -= CRange(30, 1100);
    assert ( toString ( b ) == "{<26..29>,<1101..1199>}" );
}

void test10 ()
{
    CRangeList a, b;
    a -= CRange(5, 10);
    assert(!a.Includes(5));
    assert(!a.Includes( CRange(5, 10) ));
    a += b;
    a = CRange(LLONG_MIN, LLONG_MAX);
    assert(a.Includes(5));
    assert(a.Includes(LLONG_MIN));
    assert(a.Includes(LLONG_MAX));
    a -= CRange(LLONG_MIN, 0);
    assert(a.Includes(5));
    assert(!a.Includes(-5));
    a = CRange(LLONG_MIN, LLONG_MAX);
    a += CRange(LLONG_MIN, LLONG_MAX);
    assert(a.Includes(LLONG_MIN));
    assert(a.Includes(LLONG_MAX));
    a -= CRange(LLONG_MIN, LLONG_MIN);
    assert(!a.Includes(LLONG_MIN));
    a += CRange(LLONG_MIN, LLONG_MIN);
    assert(a.Includes(LLONG_MIN));
    a -= CRange(LLONG_MAX, LLONG_MAX);
    assert(!a.Includes(LLONG_MAX));
    a += CRange(LLONG_MAX, LLONG_MAX);
    assert(a.Includes(LLONG_MAX));
}

void test11()
{
    CRangeList a;
    a += CRange(LLONG_MIN, LLONG_MAX-1) + CRange(LLONG_MIN, LLONG_MAX);
    assert(toString( a ) == "{<-9223372036854775808..9223372036854775807>}");
}



int                main                                    ( void )
{

    testbasic();
    cout << "Basic test passed" << endl;
    test0();
    cout << "Test0 passed" << endl;
    test1();
    cout << "Test1 passed" << endl;
    test2();
    cout << "Test2 passed" << endl;
    test3();
    cout << "Test3 passed" << endl;
    test4();
    cout << "Test4 passed" << endl;
    test5();
    cout << "Test5 passed" << endl;

    test6();
    cout << "Test6 passed" << endl;
    test7();
    cout << "Test7 passed" << endl;

    test8();
    cout << "Test8 passed" << endl;

    test9();
    cout << "Test9 passed" << endl;

    test10();
    cout << "Test10 passed" << endl;

    test11();
    cout << "Test11 passed" << endl;

#ifdef EXTENDED_SYNTAX
    CRangeList x { { 5, 20 }, { 150, 200 }, { -9, 12 }, { 48, 93 } };
   assert ( toString ( x ) == "{<-9..20>,<48..93>,<150..200>}" );
   ostringstream oss;
   oss << setfill ( '=' ) << hex << left;
   for ( const auto & v : x + CRange ( -100, -100 ) )
     oss << v << endl;
   oss << setw ( 10 ) << 1024;
   assert ( oss . str () == "-100\n<-9..20>\n<48..93>\n<150..200>\n400=======" );
#endif /* EXTENDED_SYNTAX */

    return 0;
}
#endif /* __PROGTEST__ */
