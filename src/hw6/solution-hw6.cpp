#ifndef __PROGTEST__
#include <cassert>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <algorithm>
#include <memory>
#include <functional>
using namespace std;
#endif /* __PROGTEST__ */


static bool NETWORK = false;
static bool LASTCOMPONENT = false;
static bool LASTHOST = false;
//=================================================================================================

/*
 * CComponent abstract class represents Computer components like CPU, Disk, RAM
 */
class CComponent {
public:
    CComponent () {
    }

    virtual ~CComponent ( ) = default;

    virtual CComponent * clone ( ) const & = 0;

    virtual CComponent * clone ( ) && = 0;

    /*
     * sends data to os CComponent using print fuctiion implemented in each child class
     * @param[in] os, ostream operator to send the data
     */
    virtual void print ( std::ostream & os ) const = 0;

    /*
     * output operator which displays CComponent using print ()
     * @param[in] os, to send the data
     * @param[in] component, represents data to display
     * @return  modified ostream os
     */
    friend std::ostream & operator << ( std::ostream & os, const CComponent & component ) {
        component.print ( os );
        return os;
    }
};

//=================================================================================================

class CCPU : public CComponent
{
    int m_cores;
    int m_frequency;
public:
    CCPU ( int cores, int frequency ) : m_cores ( cores ), m_frequency ( frequency ) {

    }

    /*
     * Assignment operator
     */
    CCPU & operator = ( const CCPU & other ) {
        if ( & other == this ) return  * this;
        m_cores = other.m_cores;
        m_frequency = other.m_frequency;
        return *this;
    }


    virtual CCPU * clone ( ) const & override {
        return new CCPU ( * this );
    }

    virtual CCPU * clone ( ) && override {
        return new CCPU ( move ( * this ) );
    }

    /*
     * @return number of cores
     */
    int getCores () const {
        return m_cores;
    }

    /*
    * @return frequency of CPU
    */
    int getFrequency () const {
        return m_frequency;
    }

    /*
     * display data: number of cores & frequency of CPU
     * @param[in] os
     */
    virtual void print ( std::ostream & os ) const override {
        os << "CPU, " << getCores() << " cores @ " << getFrequency() << "MHz" << endl;
    }

};

//=================================================================================================

class CMemory : public CComponent
{
    int m_size;

public:
    CMemory ( int size ) : m_size ( size ) {

    }

    virtual CMemory * clone ( ) const & override {
        return new CMemory ( * this );
    }

    virtual CMemory * clone ( ) && override {
        return new CMemory ( move ( * this ) );
    }

    /*
    * Assignment operator
    */
    CMemory & operator = ( const CMemory & other ) {
        if ( & other == this ) return * this;
        m_size = other.m_size;
        return * this;
    }

    /*
     * @return size of RAM
     */
    int getSize () const{
        return m_size;
    }

    /*
     * display data: memory size
     * @param[in] os
     */
    virtual void print ( std::ostream & os ) const override {
        os << "Memory, " << getSize() << " MiB" << endl;
    }
};

//=================================================================================================

class CDisk : public CComponent
{
public:
    /*
     * TType represents type of a disk
     */
    enum TType { SSD = 0, MAGNETIC = 1};

    CDisk ( TType type, int size ) : m_size ( size ),  m_type ( type ) {

    }

    virtual CDisk * clone ( ) const & override {
        return new CDisk ( * this );
    }

    virtual CDisk * clone ( ) && override {
        return new CDisk ( move ( * this ) );
    }

    /*
     * Assignment operator
     */
    CDisk & operator = ( const CDisk & other ) {
        if ( & other == this ) return * this;
        m_size = other.m_size;
        m_type = other.m_type;
        m_partitions = other.m_partitions;
        return * this;
    }

    /*
     * @return disk size
     */
    int getSize() const {
        return m_size;
    }

    /*
     * @return disk type
     */
    string getType () const{
        return m_type == SSD ? "SSD" : "HDD";
    }


    /*
     * add partiotion to the list
     * @param[in], size : size of the partition in GiB
     * @param[in], parition : type of the partition HDD/SSD
     * @return *this, where we add parition to the list
     */
    CDisk AddPartition ( int size, string partition ) {
        m_partitions.emplace_back( partition, size );
        return *this;
    }

    /*
     * display data: size of Disk and each parition
     * @param[in] os
     */
    virtual void print ( std::ostream & os ) const override {
        os << getType() << ", " << getSize() << " GiB" << endl;
        for(size_t i = 0; i < m_partitions.size(); ++i)
        {

            if(NETWORK)
            {
                if(!LASTHOST && !LASTCOMPONENT) os << "| | ";
                else if(!LASTHOST && LASTCOMPONENT) os << "|   ";
                else if(LASTHOST && !LASTCOMPONENT) os << "  | ";
                else if(LASTHOST && LASTCOMPONENT) os << "    ";
            }else{
                if(!LASTCOMPONENT) os << "| ";
                else os << "  ";
            }
            if(m_partitions.size() == 1 || m_partitions.size()-1 == i) os << "\\-[";
            else os << "+-[";
            os << i << "]: " << m_partitions[i].second << " GiB, " <<  m_partitions[i].first << endl;
        }
    }

private:
    int  m_size;
    TType m_type;
    vector < pair < string, int > > m_partitions;
};

//=================================================================================================

class CComputer {

    string m_name;
    vector<string> addresses;
    vector < shared_ptr < CComponent > > m_components;
public:

    CComputer ( string name ) : m_name ( name ) {

    }

    CComputer ( const CComputer & other );

    ~CComputer ( ) {

    }

    /*
     * Assignment operator
     */
    CComputer & operator = ( const CComputer & other );

    CComputer & AddAddress ( string address ) {
        addresses.push_back(address);
        return * this;
    }

    /*
     * function adds a newcomponemt to the list
     * @oarameter component, component to retrieve the data
     * @return *this, the modified object
     */
    CComputer AddComponent ( const CComponent & component ) {
        m_components.push_back( shared_ptr <CComponent> ( component.clone()));
        return *this;
    }

    /*
     *@oarameter component, component to retrieve the data
     * function adds a newcomponemt to the list
     * @return *this, the modified object
     */
    CComputer & AddComponent (  CComponent && component ) {
        m_components.push_back( shared_ptr <CComponent> ( move (component).clone() ));
        return *this;
    }

    /*
     * output operator which displays the computer in the format ostream
     * @param[in] os
     * @param[in] computer
     */
    friend ostream & operator << ( ostream & os, const CComputer & computer );

    /*
     * @return name of the computer/host
     */
    string getName () const {
        return m_name;
    }

    /*
     * @return list of addresses
     */
    vector<string> getAddresses () const {
        return addresses;
    };

    /*
     * @return list of addresses
     */
    vector < shared_ptr < CComponent > > getComponents ( ) const {
        return m_components;
    }

};

CComputer::CComputer ( const CComputer & other ) {
    m_name = other.m_name;
    addresses = other.addresses;
    m_components = other.m_components;
}

CComputer & CComputer:: operator = ( const CComputer & other ) {
    if ( &other == this ) return *this;
    m_name = other.m_name;
    addresses = other.addresses;
    m_components = other.m_components;
    return *this;
}

ostream & operator << ( ostream & os, const CComputer & computer ) {
    if(NETWORK){
        if(LASTHOST) os <<  "\\-";
        else os <<  "+-";
        os << "Host: " << computer.m_name << endl;
        size_t addr_size = computer.addresses.size();
        size_t comp_size = computer.m_components.size();
        for(size_t i = 0; i < addr_size; ++i)
        {
            if(LASTHOST) os  << "  ";
            else os << "| ";
            if(comp_size == 0 && addr_size - 1 == i) os << "\\-";
            else os << "+-";
            os << computer.addresses[i] << endl;
        }
        for(size_t i = 0; i < comp_size; ++i)
        {
            if(LASTHOST) os  << "  ";
            else os << "| ";
            if(comp_size > 1 && comp_size - 1 == i) {
                os << "\\-";
                LASTCOMPONENT = true;
            }
            else os << "+-";
            os << * computer.m_components[i];
        }
    }else{
        os << "Host: " <<computer.m_name << endl;
        size_t addr_size = computer.addresses.size();
        size_t comp_size = computer.m_components.size();
        for(size_t i = 0; i < addr_size; ++i)
        {
            if(comp_size == 0 && addr_size - 1 == i) os << "\\-";
            else os << "+-";
            os << computer.addresses[i] << endl;
        }
        for(size_t i = 0; i < comp_size; ++i)
        {
            if(comp_size > 1 && comp_size - 1 == i) {
                os << "\\-";
                LASTCOMPONENT = true;
            }
            else os << "+-";
            os << * computer.m_components[i];
        }
    }
    LASTCOMPONENT = false;
    return os;
}


//=================================================================================================

class CNetwork
{
    string m_name;
    vector < CComputer > network;

public:

    CNetwork ( string name ) : m_name ( name ) {

    }

    ~CNetwork() {

    }

    CNetwork ( const CNetwork & cnetwork ) : m_name ( cnetwork.m_name ) {
        network = cnetwork.network;
    }

    /*
     * Assignment operator
     */
    CNetwork & operator = ( const CNetwork & other );

    /*
     * searches computer in the list
     * @param[in]  name, name of the computer to be found in the list
     * @return  pointer to an object if found, else null pointer
     */
    CComputer * FindComputer ( string name );

    /*
     * adds computer in the list
     * @param[in]  computer, computer to be added in the list
     * @return  *this modified network
     */
    CNetwork & AddComputer ( const CComputer & computer ) {
        network.emplace_back(computer);
        return *this;
    }

    /*
     * output operator which displays the netwrok in the format ostream
     * @param[in] os
     * @param[in] src
     */

    friend ostream & operator << ( ostream & os, const CNetwork & src ) ;

};

CComputer * CNetwork::FindComputer ( string name ) {

    for(size_t i = 0; i < this->network.size(); ++i )
    {
        if ( this->network[i].getName() == name ) {
            return &network[i];
        }
    }
    return nullptr;
}

CNetwork & CNetwork::operator = ( const CNetwork & other ) {
    if( &other == this ) return * this;
    m_name = other.m_name;
    network.clear();
    network.assign(other.network.begin(), other.network.end());
    return *this;
}

ostream & operator << ( ostream & os, const CNetwork & src ) {
    os << "Network: " << src.m_name << endl;
    NETWORK = true;
    for (size_t i = 0; i < src.network.size(); ++i) {
        if( src.network.size() > 1 &&  src.network.size()-1 == i) {
            LASTHOST = true;
        }
        os <<  src.network[i];
    }
    LASTHOST = false;
    LASTCOMPONENT = false;
    NETWORK = false;
    return os;
}

//=================================================================================================

#ifndef __PROGTEST__
template<typename _T>
string toString ( const _T & x )
{
    ostringstream oss;
    oss << x;
    return oss . str ();
}




int main ( void )
{

    CNetwork n ( "FIT network" );
    n . AddComputer (
            CComputer ( "progtest.fit.cvut.cz" ) .
                    AddAddress ( "147.32.232.142" ) .
                    AddComponent ( CCPU ( 8, 2400 ) ) .
                    AddComponent ( CCPU ( 8, 1200 ) ) .
                    AddComponent ( CDisk ( CDisk::MAGNETIC, 1500 ) .
                    AddPartition ( 50, "/" ) .
                    AddPartition ( 5, "/boot" ).
                    AddPartition ( 1000, "/var" ) ) .
                    AddComponent ( CDisk ( CDisk::SSD, 60 ) .
                    AddPartition ( 60, "/data" )  ) .
                    AddComponent ( CMemory ( 2000 ) ).
                    AddComponent ( CMemory ( 2000 ) ) ) .
            AddComputer (
            CComputer ( "edux.fit.cvut.cz" ) .
                    AddAddress ( "147.32.232.158" ) .
                    AddComponent ( CCPU ( 4, 1600 ) ) .
                    AddComponent ( CMemory ( 4000 ) ).
                    AddComponent ( CDisk ( CDisk::MAGNETIC, 2000 ) .
                    AddPartition ( 100, "/" )   .
                    AddPartition ( 1900, "/data" ) ) ) .
            AddComputer (
            CComputer ( "imap.fit.cvut.cz" ) .
                    AddAddress ( "147.32.232.238" ) .
                    AddComponent ( CCPU ( 4, 2500 ) ) .
                    AddAddress ( "2001:718:2:2901::238" ) .
                    AddComponent ( CMemory ( 8000 ) ) );

    cout << n << endl;

    assert ( toString ( n ) ==
             "Network: FIT network\n"
             "+-Host: progtest.fit.cvut.cz\n"
             "| +-147.32.232.142\n"
             "| +-CPU, 8 cores @ 2400MHz\n"
             "| +-CPU, 8 cores @ 1200MHz\n"
             "| +-HDD, 1500 GiB\n"
             "| | +-[0]: 50 GiB, /\n"
             "| | +-[1]: 5 GiB, /boot\n"
             "| | \\-[2]: 1000 GiB, /var\n"
             "| +-SSD, 60 GiB\n"
             "| | \\-[0]: 60 GiB, /data\n"
             "| +-Memory, 2000 MiB\n"
             "| \\-Memory, 2000 MiB\n"
             "+-Host: edux.fit.cvut.cz\n"
             "| +-147.32.232.158\n"
             "| +-CPU, 4 cores @ 1600MHz\n"
             "| +-Memory, 4000 MiB\n"
             "| \\-HDD, 2000 GiB\n"
             "|   +-[0]: 100 GiB, /\n"
             "|   \\-[1]: 1900 GiB, /data\n"
             "\\-Host: imap.fit.cvut.cz\n"
             "  +-147.32.232.238\n"
             "  +-2001:718:2:2901::238\n"
             "  +-CPU, 4 cores @ 2500MHz\n"
             "  \\-Memory, 8000 MiB\n" );
    CNetwork x = n;
    auto c = x . FindComputer ( "imap.fit.cvut.cz" );
    cout << *c;
    assert ( toString ( *c ) ==
             "Host: imap.fit.cvut.cz\n"
             "+-147.32.232.238\n"
             "+-2001:718:2:2901::238\n"
             "+-CPU, 4 cores @ 2500MHz\n"
             "\\-Memory, 8000 MiB\n" );
    c -> AddComponent ( CDisk ( CDisk::MAGNETIC, 1000 ) .
            AddPartition ( 100, "system" ) .
            AddPartition ( 200, "WWW" ) .
            AddPartition ( 700, "mail" ) );
    cout << x;
    assert ( toString ( x ) ==
             "Network: FIT network\n"
             "+-Host: progtest.fit.cvut.cz\n"
             "| +-147.32.232.142\n"
             "| +-CPU, 8 cores @ 2400MHz\n"
             "| +-CPU, 8 cores @ 1200MHz\n"
             "| +-HDD, 1500 GiB\n"
             "| | +-[0]: 50 GiB, /\n"
             "| | +-[1]: 5 GiB, /boot\n"
             "| | \\-[2]: 1000 GiB, /var\n"
             "| +-SSD, 60 GiB\n"
             "| | \\-[0]: 60 GiB, /data\n"
             "| +-Memory, 2000 MiB\n"
             "| \\-Memory, 2000 MiB\n"
             "+-Host: edux.fit.cvut.cz\n"
             "| +-147.32.232.158\n"
             "| +-CPU, 4 cores @ 1600MHz\n"
             "| +-Memory, 4000 MiB\n"
             "| \\-HDD, 2000 GiB\n"
             "|   +-[0]: 100 GiB, /\n"
             "|   \\-[1]: 1900 GiB, /data\n"
             "\\-Host: imap.fit.cvut.cz\n"
             "  +-147.32.232.238\n"
             "  +-2001:718:2:2901::238\n"
             "  +-CPU, 4 cores @ 2500MHz\n"
             "  +-Memory, 8000 MiB\n"
             "  \\-HDD, 1000 GiB\n"
             "    +-[0]: 100 GiB, system\n"
             "    +-[1]: 200 GiB, WWW\n"
             "    \\-[2]: 700 GiB, mail\n" );
    assert ( toString ( n ) ==
             "Network: FIT network\n"
             "+-Host: progtest.fit.cvut.cz\n"
             "| +-147.32.232.142\n"
             "| +-CPU, 8 cores @ 2400MHz\n"
             "| +-CPU, 8 cores @ 1200MHz\n"
             "| +-HDD, 1500 GiB\n"
             "| | +-[0]: 50 GiB, /\n"
             "| | +-[1]: 5 GiB, /boot\n"
             "| | \\-[2]: 1000 GiB, /var\n"
             "| +-SSD, 60 GiB\n"
             "| | \\-[0]: 60 GiB, /data\n"
             "| +-Memory, 2000 MiB\n"
             "| \\-Memory, 2000 MiB\n"
             "+-Host: edux.fit.cvut.cz\n"
             "| +-147.32.232.158\n"
             "| +-CPU, 4 cores @ 1600MHz\n"
             "| +-Memory, 4000 MiB\n"
             "| \\-HDD, 2000 GiB\n"
             "|   +-[0]: 100 GiB, /\n"
             "|   \\-[1]: 1900 GiB, /data\n"
             "\\-Host: imap.fit.cvut.cz\n"
             "  +-147.32.232.238\n"
             "  +-2001:718:2:2901::238\n"
             "  +-CPU, 4 cores @ 2500MHz\n"
             "  \\-Memory, 8000 MiB\n" );
    return 0;
}
#endif /* __PROGTEST__ */
