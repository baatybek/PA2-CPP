#ifndef __PROGTEST__
#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <algorithm>
#include <memory>
#include <functional>
using namespace std;

class CTimeStamp
{
public:
    CTimeStamp ( ) { }
    CTimeStamp (int year, int month, int day, int hour, int minute, double sec) :
            year (year), month (month), day (day), hour (hour), minute (minute), sec (sec) {}
    int Compare( const CTimeStamp & x ) const;
    friend ostream & operator << ( ostream & os, const CTimeStamp & x );
private:
    int    year;
    int    month;
    int    day;
    int    hour;
    int    minute;
    double sec;

    int compareYears  (int year1, int year2)     const;
    int compareMonths (int month1, int month2)   const;
    int compareDays   (int day1, int day2)       const;
    int compareHours  (int hour1, int hour2)     const;
    int compareMinutes(int minute1, int minute2) const;
    int compareSeconds(double sec1, double sec2) const;
};

ostream & operator << ( ostream & os, const CTimeStamp & x )
{
    if(x.year >= 1000)                       os << x.year;
    else if(x.year >= 100 && x.year <= 999)  os << "0" << x.year;
    else if(x.year >= 10 && x.year <= 99)    os << "00" << x.year;
    else if(x.year < 10)                     os << "000" << x.year;

    os << "-";

    if(x.month >= 10)                        os << x.month;
    else                                     os << "0" << x.month;

    os << "-";

    if(x.day >= 10)                          os << x.day;
    else                                     os << "0" << x.day;

    os << " ";

    if(x.hour >= 10)                         os << x.hour;
    else                                     os << "0" << x.hour;

    os << ":";

    if(x.minute >= 10)                       os << x.minute;
    else                                     os << "0" << x.minute;

    os << ":";

    if(x.sec >= 10)
    {
        os << fixed;
        os << setprecision(3) << x.sec;
    }else
    {
        os << "0";
        os << fixed;
        os << setprecision(3) << x.sec;
    }

    return os;
}

int CTimeStamp::Compare       (const CTimeStamp &x)      const
{
    if(compareYears(this->year, x.year) == 0)
    {
        if(compareMonths(this->month, x.month) == 0)
        {
            if(compareDays(this->day, x.day) == 0)
            {
                if(compareHours(this->hour, x.hour) == 0)
                {
                    if(compareMinutes(this->minute, x.minute) == 0)
                    {
                        if(compareSeconds(this->sec, x.sec) == 0) return 0;
                        return compareSeconds(this->sec, x.sec) == 1 ? 1 : -1;
                    }
                    return compareMinutes(this->minute, x.minute) == 1 ? 1 : -1;
                }
                return compareHours(this->hour, x.hour) == 1 ? 1 : -1;
            }
            return compareDays(this->day, x.day) == 1 ? 1 : -1;
        }
        return compareMonths(this->month, x.month) == 1 ? 1 : -1;
    }
    return compareYears(this->year, x.year) == 1 ? 1 : -1;
}

int CTimeStamp::compareYears  (int year1, int year2)     const
{
    if(year1 == year2) return 0;
    return year1 > year2 ? 1 : -1;
}

int CTimeStamp::compareMonths (int month1, int month2)   const
{
    if(month1 == month2) return 0;
    return month1 > month2 ? 1 : -1;
}

int CTimeStamp::compareDays   (int day1, int day2)       const
{
    if(day1 == day2) return 0;
    return day1 > day2 ? 1 : -1;
}

int CTimeStamp::compareHours  (int hour1, int hour2)     const
{
    if(hour1 == hour2) return 0;
    return hour1 > hour2 ? 1 : -1;
}

int CTimeStamp::compareMinutes(int minute1, int minute2) const
{
    if(minute1 == minute2) return 0;
    return minute1 > minute2 ? 1 : -1;
}

int CTimeStamp::compareSeconds(double sec1, double sec2) const
{
    if(sec1 == sec2) return 0;
    return sec1 > sec2  ? 1 : -1;
}


class CMail
{
public:

    CMail ( const CTimeStamp & timeStamp, const string & from, const string & to, const string & subject ) :
            from(from), to(to), subject(subject) {timeStampL = timeStamp; }
    int CompareByTime ( const CTimeStamp & x ) const;
    int CompareByTime ( const CMail      & x ) const;
    const string & From ( void ) const;
    const string & To ( void ) const;
    const string & Subject ( void ) const;
    const CTimeStamp & TimeStamp ( void ) const;
    friend ostream & operator << ( ostream  & os, const CMail & x );


private:
    CTimeStamp timeStampL;
    string     from;
    string     to;
    string     subject;
};

ostream & operator << ( ostream  & os, const CMail & x )
{
    os  << x.TimeStamp() << " " << x.From() << " -> " << x.To() << ", subject: " << x.Subject();
    return os;
}

int CMail::CompareByTime ( const CTimeStamp & x ) const
{
    return this->timeStampL.Compare(x);
}

int CMail::CompareByTime ( const CMail      & x ) const
{
    return this->timeStampL.Compare(x.timeStampL);
}

const string & CMail::From ( void ) const
{
    return this->from;
}

const string & CMail::To ( void ) const
{
    return this->to;
}

const string & CMail::Subject ( void ) const
{
    return this->subject;
}

const CTimeStamp & CMail::TimeStamp ( void ) const
{
    return this->timeStampL;
}

// your code will be compiled in a separate namespace
namespace MysteriousNamespace {
#endif /* __PROGTEST__ */
//----------------------------------------------------------------------------------------
    class Compare
    {
    public:
        bool operator () (const CTimeStamp & lhs, const CTimeStamp & rhs) const { return lhs.Compare(rhs) < 0; }
    };

    class CMailLog
    {
    public:
        CMailLog  ( );
        ~CMailLog ( );
        int ParseLog ( istream & in );
        list<CMail> ListMail ( const CTimeStamp & from, const CTimeStamp & to ) const;
        set<string> ActiveUsers ( const CTimeStamp & from, const CTimeStamp & to ) const;
    private:
        bool isValidMessage   (string message) const;
        void fillmap          (string message, string id, map <string , pair < string, string > > & fsmap);

        int getMonth          (string month) const;
        int getDay            (string day)   const;
        int getYear           (string year)  const;
        int getHour           (string hour)  const;
        int getMinute         (string minute)  const;
        double getSeconds     (string second)  const;
        string Hour           (string time);
        string Minute         (string time);
        string Seconds        (string time);
        string getFrom        (string message) const;
        string getTo          (string message) const;
        string getSubject     (string message) const;
        multimap<CTimeStamp, CMail, Compare> mails_multimap;
    };
    CMailLog::CMailLog()  {}
    CMailLog::~CMailLog() {}

    int CMailLog::ParseLog ( istream & in )
    {
        map <string , pair < string, string > > fsmap; // from, subject map
        string month, day, year, time, servername, emailID, message;
        char space;
        int counter = 0;
        while( in >> month >> day >> year >> time >> servername >> emailID && in.get(space) && getline(in, message))
        {
            if( !isValidMessage(message) ) goto NEXT;
            if( !getFrom(message).empty()|| !getSubject(message).empty() )
            {
                auto iterator =  fsmap.lower_bound(emailID);
                if(iterator != fsmap.end())
                {
                    if(getFrom(message) == " ")                 iterator->second.first  = "";
                    else if(!getFrom(message).empty() )           iterator->second.first  = getFrom(message);
                    else if( !getSubject(message).empty() )  iterator->second.second = getSubject(message);
                }else{
                    if(getFrom(message) == " ")  fsmap.emplace(make_pair(emailID, make_pair("", getSubject(message))));
                    else fsmap.emplace(make_pair(emailID, make_pair(getFrom(message), getSubject(message))));
                }
            }
            if( !getTo(message).empty() )
            {
                string hour = Hour(time);
                string minute = Minute(time);
                string seconds = Seconds(time);
                auto fs = fsmap.lower_bound(emailID);
                CTimeStamp tmps (getYear(year), getMonth(month), getDay(day), getHour(hour), getMinute(minute), getSeconds(seconds));
                if(getTo(message) == " ")
                {
                    CMail newmail (tmps, fs->second.first, "", fs->second.second);
                    mails_multimap.insert(make_pair(tmps, newmail));
                }else
                {
                    CMail newmail (tmps, fs->second.first, getTo(message), fs->second.second);
                    mails_multimap.insert(make_pair(tmps, newmail));
                }

                counter ++;
            }

            NEXT:
            space = 0;
        }

        return counter;
    }
    list<CMail> CMailLog::ListMail ( const CTimeStamp & from, const CTimeStamp & to ) const
    {
        list<CMail> cmaillist;
        if(mails_multimap.empty()) return cmaillist;
        if(mails_multimap.begin()->first.Compare(to) > 0) return cmaillist;
        if(mails_multimap.crbegin()->first.Compare(from) < 0) return cmaillist;

        auto lower = mails_multimap.lower_bound(from);
        auto upper = mails_multimap.upper_bound(to);

        if(lower != mails_multimap.end() && upper != mails_multimap.end())
        {
            while(lower != upper)
            {
                if(lower->first.Compare(from) >= 0)
                {
                    CMail newcmail(lower->second.TimeStamp(), lower->second.From(), lower->second.To(), lower->second.Subject());
                    cmaillist.emplace_back(newcmail);
                    if(lower->first.Compare(to) == 0) break;
                    lower++;
                }

            }
        }else if(lower != mails_multimap.end() && upper == mails_multimap.end())
        {
            while(lower != mails_multimap.end())
            {
                if(lower->first.Compare(from) >= 0)
                {
                    CMail newcmail(lower->second.TimeStamp(), lower->second.From(), lower->second.To(), lower->second.Subject());
                    cmaillist.emplace_back(newcmail);
                    if(lower->first.Compare(to) == 0) break;
                    lower++;
                }
            }
        }else
        {
            for(auto iterator = mails_multimap.begin(); iterator != mails_multimap.end(); iterator++)
            {
                if(iterator->first.Compare(to) >= 0)
                {
                    CMail newcmail(iterator->second.TimeStamp(), iterator->second.From(), iterator->second.To(), iterator->second.Subject());
                    cmaillist.emplace_back(newcmail);
                    if(iterator->first.Compare(to) == 0) break;
                }
            }
        }
        return cmaillist;
    }
    set<string> CMailLog::ActiveUsers(const CTimeStamp &from, const CTimeStamp &to) const
    {
        set<string> setusers;
        if(mails_multimap.empty()) return setusers;
        if(mails_multimap.begin()->first.Compare(to) > 0) return setusers;
        if(mails_multimap.crbegin()->first.Compare(from) < 0) return setusers;

        auto lower = mails_multimap.lower_bound(from);
        auto upper = mails_multimap.upper_bound(to);

        if(lower != mails_multimap.end() && upper != mails_multimap.end())
        {
            while(lower != upper)
            {
                if(lower->first.Compare(from) >= 0)
                {
                    string from_s = lower->second.From();
                    string to_s   = lower->second.To();
                    setusers.insert(from_s);
                    setusers.insert(to_s);
                    if(lower->first.Compare(to) == 0) break;
                    lower++;
                }

            }
        }else if(lower != mails_multimap.end() && upper == mails_multimap.end())
        {
            while(lower != mails_multimap.end())
            {
                if(lower->first.Compare(from) >= 0)
                {
                    string from_s = lower->second.From();
                    string to_s   = lower->second.To();
                    setusers.insert(from_s);
                    setusers.insert(to_s);
                    if(lower->first.Compare(to) == 0) break;
                    lower++;
                }
            }
        }else{
            for(auto iterator = mails_multimap.begin(); iterator != mails_multimap.end(); iterator++)
            {
                if(iterator->first.Compare(from) >= 0)
                {
                    string from_s = iterator->second.From();
                    string to_s = iterator->second.To();
                    setusers.insert(from_s);
                    setusers.insert(to_s);
                    if(iterator->first.Compare(to) == 0) break;
                }
            }
        }
        return setusers;
    }
//----------------------------------------------------------------------------------------
    bool  CMailLog::isValidMessage (string message) const
    {
        if(message.empty())
            return false;
        switch (message[0])
        {
            case 'f':
                if(message.size() >= 5 && message.substr(0, 5) == "from=") return true;
                return false;
            case 't':
                if(message.size() >= 3 && message.substr(0, 3) == "to=") return true;
                return false;
            case 's':
                if(message.size() >= 8 && message.substr(0, 8) == "subject=") return true;
                return false;
            default:
                break;
        }
        return false;
    }

//----------------------------------------------------------------------------------------
    int CMailLog::getDay          (string day)     const
    {
        std::string::size_type sz;
        return stoi(day, &sz);
    }
    int CMailLog::getMonth        (string month)   const
    {
        if(month == "Jan") return 1;
        else if(month == "Feb") return 2;
        else if(month == "Mar") return 3;
        else if(month == "Apr") return 4;
        else if(month == "May") return 5;
        else if(month == "Jun") return 6;
        else if(month == "Jul") return 7;
        else if(month == "Aug") return 8;
        else if(month == "Sep") return 9;
        else if(month == "Oct") return 10;
        else if(month == "Nov") return 11;
        else if(month == "Dec") return 12;
        return 0;
    }
    int CMailLog::getYear         (string year)    const
    {
        std::string::size_type sz;
        return stoi(year, &sz);
    }
    int CMailLog::getHour         (string hour)    const
    {
        std::string::size_type tsz;
        return stoi(hour, &tsz);
    }
    int CMailLog::getMinute       (string minute)    const
    {
        std::string::size_type tsz;
        return stoi(minute, &tsz);
    }
    double CMailLog::getSeconds   (string second)    const
    {
        std::string::size_type sz;
        double hey = stod(second, &sz);
        return hey;
    }
    string CMailLog::getFrom      (string message) const
    {
        if(message.length() == 5) return " ";
        return message.substr(0, 5) == "from=" ? message.substr(5, message.size()-1) : "";
    }
    string CMailLog::getTo        (string message) const
    {
        if(message.length() == 3) return " ";
        return message.substr(0, 3) == "to=" ? message.substr(3, message.size()-1) : "";
    }
    string CMailLog::getSubject   (string message) const
    {
        if(message.size() == 8 && message.substr(0, 8) == "subject=") return "";
        return message.substr(0, 8) == "subject=" ? message.substr(8, message.size()-1) : "";
    }

    string CMailLog::Hour    (string time)
    {
        if(time[0] == ':') return "00";
        size_t index = 0;
        for(size_t i = 0; i < time.length(); i++)
        {
            if(time[i] == ':')
            {
                index = i;
                break;
            }
        }
        return time.substr(0, index);
    }
    string CMailLog::Minute  (string time)
    {
        if(time[0] == ':' && time[1] == ':') return "00";
        size_t index = 0;
        for(size_t i = 0; i < time.length(); i++)
        {
            if(time[i] == ':')
            {
                index = i;
                break;
            }
        }
        time.erase(0, index + 1);
        if(time[0] == ':') return "00";
        index = 0;
        for(size_t i = 0; i < time.length(); i++)
        {
            if(time[i] == ':')
            {
                index = i;
                break;
            }
        }
        return time.substr(0, index);
    }
    string CMailLog::Seconds (string time)
    {
        if(time[0] == ':' && time[1] == ':') return "00";
        size_t index = 0;
        for(size_t i = 0; i < time.length(); i++)
        {
            if(time[i] == ':')
            {
                index = i;
                break;
            }
        }
        time.erase(0, index + 1);
        index = 0;
        for(size_t i = 0; i < time.length(); i++)
        {
            if(time[i] == ':')
            {
                index = i;
                break;
            }
        }
        time.erase(0, index + 1);
        if(time.empty()) return "0.000";
        if(time.length() == 1 && time[0] == '.') return "0.000";
        if(time[0] == '.') return "0" + time;
        if(time[time.length()-1] == '.') return  time + "000";
        return time;
    }

//----------------------------------------------------------------------------------------
#ifndef __PROGTEST__
} // namespace
string             printMail                               ( const list<CMail> & all )
{
    ostringstream oss;
    for ( const auto & mail : all )
        oss << mail << endl;
     return oss . str ();
}
string             printUsers                              ( const set<string> & all )
{
    ostringstream oss;
    bool first = true;
    for ( const auto & name : all )
    {
        if ( ! first )
            oss << ", ";
        else
            first = false;
        oss << name;
    }
    return oss . str ();
}

int                main                                    ( void )
{



    MysteriousNamespace::CMailLog m;
    list<CMail> mailList;
    set<string> users;
    istringstream iss;

    iss . clear ();
    iss . str (
            "Mar 29 2019 12:35:32.233 relay.fit.cvut.cz ADFger72343D: from=user1@fit.cvut.cz\n"
            "Mar 29 2019 12:37:16.234 relay.fit.cvut.cz JlMSRW4232Df: from=person3@fit.cvut.cz\n"
            "Mar 29 2019 12:55:13.023 relay.fit.cvut.cz JlMSRW4232Df: subject=New progtest homework!\n"
            "Mar 29 2019 13:38:45.043 relay.fit.cvut.cz Kbced342sdgA: from=office13@fit.cvut.cz\n"
            "Mar 29 2019 13:36:13.023 relay.fit.cvut.cz JlMSRW4232Df: to=user76@fit.cvut.cz\n"
            "Mar 29 2019 13:55:31.456 relay.fit.cvut.cz KhdfEjkl247D: from=PR-department@fit.cvut.cz\n"
            "Mar 29 2019 14:18:12.654 relay.fit.cvut.cz Kbced342sdgA: to=boss13@fit.cvut.cz\n"
            "Mar 29 2019 14:48:32.563 relay.fit.cvut.cz KhdfEjkl247D: subject=Business partner\n"
            "Mar 29 2019 14:58:32.000 relay.fit.cvut.cz KhdfEjkl247D: to=HR-department@fit.cvut.cz\n"
            "Mar 29 2019 14:25:23.233 relay.fit.cvut.cz ADFger72343D: mail undeliverable\n"
            "Mar 29 2019 15:02:34.231 relay.fit.cvut.cz KhdfEjkl247D: to=CIO@fit.cvut.cz\n"
            "Mar 29 2019 15:02:34.230 relay.fit.cvut.cz KhdfEjkl247D: to=CEO@fit.cvut.cz\n"
            "Mar 29 2019 15:02:34.230 relay.fit.cvut.cz KhdfEjkl247D: to=dean@fit.cvut.cz\n"
            "Mar 29 2019 15:02:34.230 relay.fit.cvut.cz KhdfEjkl247D: to=vice-dean@fit.cvut.cz\n"
            "Mar 29 2019 15:02:34.230 relay.fit.cvut.cz KhdfEjkl247D: to=archive@fit.cvut.cz\n" );
    assert ( m . ParseLog ( iss ) == 8 );
    mailList = m . ListMail ( CTimeStamp ( 2019, 3, 28, 0, 0, 0 ),
                              CTimeStamp ( 2019, 3, 29, 23, 59, 59 ) );
    assert ( printMail ( mailList ) ==
             "2019-03-29 13:36:13.023 person3@fit.cvut.cz -> user76@fit.cvut.cz, subject: New progtest homework!\n"
             "2019-03-29 14:18:12.654 office13@fit.cvut.cz -> boss13@fit.cvut.cz, subject: \n"
             "2019-03-29 14:58:32.000 PR-department@fit.cvut.cz -> HR-department@fit.cvut.cz, subject: Business partner\n"
             "2019-03-29 15:02:34.230 PR-department@fit.cvut.cz -> CEO@fit.cvut.cz, subject: Business partner\n"
             "2019-03-29 15:02:34.230 PR-department@fit.cvut.cz -> dean@fit.cvut.cz, subject: Business partner\n"
             "2019-03-29 15:02:34.230 PR-department@fit.cvut.cz -> vice-dean@fit.cvut.cz, subject: Business partner\n"
             "2019-03-29 15:02:34.230 PR-department@fit.cvut.cz -> archive@fit.cvut.cz, subject: Business partner\n"
             "2019-03-29 15:02:34.231 PR-department@fit.cvut.cz -> CIO@fit.cvut.cz, subject: Business partner\n" );
    mailList = m . ListMail ( CTimeStamp ( 2019, 3, 28, 0, 0, 0 ),
                              CTimeStamp ( 2019, 3, 29, 14, 58, 32 ) );
    assert ( printMail ( mailList ) ==
             "2019-03-29 13:36:13.023 person3@fit.cvut.cz -> user76@fit.cvut.cz, subject: New progtest homework!\n"
             "2019-03-29 14:18:12.654 office13@fit.cvut.cz -> boss13@fit.cvut.cz, subject: \n"
             "2019-03-29 14:58:32.000 PR-department@fit.cvut.cz -> HR-department@fit.cvut.cz, subject: Business partner\n" );
    mailList = m . ListMail ( CTimeStamp ( 2019, 3, 30, 0, 0, 0 ),
                              CTimeStamp ( 2019, 3, 30, 23, 59, 59 ) );
    assert ( printMail ( mailList ) == "" );


    users = m . ActiveUsers ( CTimeStamp ( 2019, 3, 28, 0, 0, 0 ),
                              CTimeStamp ( 2019, 3, 29, 23, 59, 59 ) );
    assert ( printUsers ( users ) == "CEO@fit.cvut.cz, CIO@fit.cvut.cz, HR-department@fit.cvut.cz, PR-department@fit.cvut.cz, archive@fit.cvut.cz, boss13@fit.cvut.cz, dean@fit.cvut.cz, office13@fit.cvut.cz, person3@fit.cvut.cz, user76@fit.cvut.cz, vice-dean@fit.cvut.cz" );
    users = m . ActiveUsers ( CTimeStamp ( 2019, 3, 28, 0, 0, 0 ),
                              CTimeStamp ( 2019, 3, 29, 13, 59, 59 ) );
    assert ( printUsers ( users ) == "person3@fit.cvut.cz, user76@fit.cvut.cz" );

    return 0;
}
#endif /* __PROGTEST__ */
