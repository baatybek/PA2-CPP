#ifndef __PROGTEST__
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <deque>
#include <map>
#include <set>
#include <list>
#include <algorithm>

#if defined ( __cplusplus ) && __cplusplus > 199711L /* C++ 11 */
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <functional>
#endif /* C++ 11 */

using namespace std;
#endif /* __PROGTEST__ */

class NoRouteException
{
};

template <typename _T, typename _E>
class CRoute
{
public:
    CRoute(void) {}
    ~CRoute(void) {}
    static bool Else (const _E & e) { return true;}
    CRoute<_T, _E> & Add (const _T & u1, const _T & u2, const _E & e);
    list<_T> Find (const _T & u1, const _T & u2, const function<bool(const _E & e)> func = CRoute<_T, _E>::Else) const;
private:
    map<_T, vector<pair<_T, _E>>> mmap;
};

template <typename _T, typename _E>
CRoute<_T, _E> & CRoute<_T, _E>::Add (const _T & u1, const _T & u2, const _E & e)
{
    typedef typename map<_T, vector<pair<_T, _E>>>::iterator mapiterator;

    pair<_T, _E> pair1 (u1, e);
    pair<_T, _E> pair2 (u2, e);

    mapiterator iter = mmap.find(u1);
    if( iter != mmap.end()) iter->second.push_back(pair2);
    else{
        vector<pair<_T,_E> > myvector = {pair2};
        mmap.emplace(u1, myvector);
    }

    iter = mmap.find(u2);
    if( iter != mmap.end()) iter->second.push_back(pair1);
    else{
        vector<pair<_T,_E> > myvector = {pair1};
        mmap.emplace(u2, myvector);
    }
    return *this;
}


template <typename _T, typename _E>
list<_T> CRoute<_T, _E>::Find (const _T & u1, const _T & u2, const function<bool(const _E & e)> Else) const
{
    list<_T> path;

    if (u1 == u2)  {
        path.push_back(u1);
        return path;
    }

    if ( mmap.find(u1) == mmap.end() ) throw NoRouteException();
    if ( mmap.find(u2) == mmap.end() ) throw NoRouteException();

    typedef typename map<_T, vector<pair<_T, _E>>>::iterator mapiterator;
    queue<_T> queue_find;
    map<_T, _T> map_temp;
    set<_T> set_visited;
    mapiterator myit;
    queue_find.push(u1);
    while(queue_find.front() != u2)
    {
        set_visited.insert(queue_find.front());
        for (size_t i = 0; i < mmap.at(queue_find.front()).size(); ++i)
        {
            if (Else(mmap.at(queue_find.front()).at(i).second)){
                if (set_visited.find(mmap.at(queue_find.front()).at(i).first) == set_visited.end()) {
                    queue_find.push(mmap.at(queue_find.front()).at(i).first);
                    map_temp.insert(pair<_T, _T>(mmap.at(queue_find.front()).at(i).first, queue_find.front()));
                };
            }
        }
        queue_find.pop();
        if(queue_find.empty()) throw NoRouteException();
    }

    _T tmp = map_temp.at(u2);
    path.push_front(u2);
    while(tmp != u1)
    {
        path.push_front(tmp);
        tmp = map_temp.at(tmp);
    }

    path.push_front(u1);

    return path;
}

#ifndef __PROGTEST__
//=================================================================================================
class CTrain
{
public:
    CTrain                        ( const string    & company,
                                    int               speed )
            : m_Company ( company ),
              m_Speed ( speed )
    {
    }
    //---------------------------------------------------------------------------------------------
    string                   m_Company;
    int                      m_Speed;
};
//=================================================================================================
class TrainFilterCompany
{
public:
    TrainFilterCompany            ( const set<string> & companies )
            : m_Companies ( companies )
    {
    }
    //---------------------------------------------------------------------------------------------
    bool                     operator ()                   ( const CTrain & train ) const
    {
        return m_Companies . find ( train . m_Company ) != m_Companies . end ();
    }
    //---------------------------------------------------------------------------------------------
private:
    set <string>             m_Companies;
};
//=================================================================================================
class TrainFilterSpeed
{
public:
    TrainFilterSpeed              ( int               min,
                                    int               max )
            : m_Min ( min ),
              m_Max ( max )
    {
    }
    //---------------------------------------------------------------------------------------------
    bool                     operator ()                   ( const CTrain    & train ) const
    {
        return train . m_Speed >= m_Min && train . m_Speed <= m_Max;
    }
    //---------------------------------------------------------------------------------------------
private:
    int                      m_Min;
    int                      m_Max;
};
//=================================================================================================
bool               NurSchnellzug                           ( const CTrain    & zug )
{
    return ( zug . m_Company == "OBB" || zug . m_Company == "DB" ) && zug . m_Speed > 100;
}
//=================================================================================================
static string      toText                                  ( const list<string> & path )
{
    ostringstream oss;

    auto it = path . cbegin();
    oss << *it;
    for ( ++it; it != path . cend (); ++it )
        oss << " > " << *it;
    return oss . str ();
}
//=================================================================================================
int main ( void )
{
    CRoute<string,CTrain> lines;

    lines . Add ( "Berlin", "Prague", CTrain ( "DB", 120 ) )
            . Add ( "Berlin", "Prague", CTrain ( "CD",  80 ) )
            . Add ( "Berlin", "Dresden", CTrain ( "DB", 160 ) )
            . Add ( "Dresden", "Munchen", CTrain ( "DB", 160 ) )
            . Add ( "Munchen", "Prague", CTrain ( "CD",  90 ) )
            . Add ( "Munchen", "Linz", CTrain ( "DB", 200 ) )
            . Add ( "Munchen", "Linz", CTrain ( "OBB", 90 ) )
            . Add ( "Linz", "Prague", CTrain ( "CD", 50 ) )
            . Add ( "Prague", "Wien", CTrain ( "CD", 100 ) )
            . Add ( "Linz", "Wien", CTrain ( "OBB", 160 ) )
            . Add ( "Paris", "Marseille", CTrain ( "SNCF", 300 ))
            . Add ( "Paris", "Dresden",  CTrain ( "SNCF", 250 ) );

    list<string> r1 = lines . Find ( "Berlin", "Linz" );
    assert ( toText ( r1 ) == "Berlin > Prague > Linz" );

    list<string> r2 = lines . Find ( "Linz", "Berlin" );
    assert ( toText ( r2 ) == "Linz > Prague > Berlin" );

    list<string> r3 = lines . Find ( "Wien", "Berlin" );
    assert ( toText ( r3 ) == "Wien > Prague > Berlin" );

    list<string> r4 = lines . Find ( "Wien", "Berlin", NurSchnellzug );
    assert ( toText ( r4 ) == "Wien > Linz > Munchen > Dresden > Berlin" );

    list<string> r5 = lines . Find ( "Wien", "Munchen", TrainFilterCompany ( set<string> { "CD", "DB" } ) );
    assert ( toText ( r5 ) == "Wien > Prague > Munchen" );

    list<string> r6 = lines . Find ( "Wien", "Munchen", TrainFilterSpeed ( 120, 200 ) );
    assert ( toText ( r6 ) == "Wien > Linz > Munchen" );

    list<string> r7 = lines . Find ( "Wien", "Munchen", [] ( const CTrain & x ) { return x . m_Company == "CD"; } );
    assert ( toText ( r7 ) == "Wien > Prague > Munchen" );

    list<string> r8 = lines . Find ( "Munchen", "Munchen" );
    assert ( toText ( r8 ) == "Munchen" );

    list<string> r9 = lines . Find ( "Marseille", "Prague" );
    assert ( toText ( r9 ) == "Marseille > Paris > Dresden > Berlin > Prague"
             || toText ( r9 ) == "Marseille > Paris > Dresden > Munchen > Prague" );

    try
    {
        list<string> r10 = lines . Find ( "Marseille", "Prague", NurSchnellzug );
        assert ( "Marseille > Prague connection does not exist!!" == NULL );
    }
    catch ( const NoRouteException & e )
    {
    }

    list<string> r11 = lines . Find ( "Salzburg", "Salzburg" );
    assert ( toText ( r11 ) == "Salzburg" );

    list<string> r12 = lines . Find ( "Salzburg", "Salzburg", [] ( const CTrain & x ) { return x . m_Company == "SNCF"; }  );
    assert ( toText ( r12 ) == "Salzburg" );

    try
    {
        list<string> r13 = lines . Find ( "London", "Oxford" );
        assert ( "London > Oxford connection does not exist!!" == NULL );
    }
    catch ( const NoRouteException & e )
    {
    }
    return 0;
}
#endif  /* __PROGTEST__ */
