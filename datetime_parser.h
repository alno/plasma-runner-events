#ifndef DATETIME_PARSER_H
#define DATETIME_PARSER_H

#include <KDateTime>

#include <QMap>
#include <QPair>

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

class DateTimeParser {
public:
    DateTimeParser();
    
    KDateTime parse( const QString & s );
    DateTimeRange parseRange( const QString & s );
    
    void addTimeFormat( const QString & s );
    void addDateFormat( const QString & s );
    
private:
    
    typedef QMap< QString, QRegExp > FormatMap;
    
private:
    
    QString parseElement( const QString & s, DateTimeRange & range, DateTimeRange::Elements elems, const QDate & defaultDate = QDate(), const QTime & defaultTime = QTime() );

private:
    QString now, today, tomorrow, yesterday;
    
    FormatMap timeFormats;
    FormatMap dateFormats;
};

#endif