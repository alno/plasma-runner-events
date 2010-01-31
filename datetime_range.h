#ifndef DATETIME_RANGE_H
#define DATETIME_RANGE_H

#include <KDateTime>

class DateTimeRange {
public:
    enum Elements {
        Start = 1,
        Finish = 2,
        Both = 3
    };
public:
    DateTimeRange() {}
    DateTimeRange( const KDateTime & dateTime ) : start( dateTime ), finish( dateTime ) {}
    DateTimeRange( const KDateTime & start, const KDateTime & finish ) : start( start ), finish( finish ) {}

    bool isPoint() const {
        return start == finish;
    }

    void setDate( const QDate & date, Elements elems );
    void setTime( const QTime & time, Elements elems );

    void addSecs( int secs, Elements elems );
    void addDays( int days, Elements elems );
    void addMonths( int months, Elements elems );
    void addYears( int years, Elements elems );
public:
    KDateTime start;
    KDateTime finish;
};

#endif
