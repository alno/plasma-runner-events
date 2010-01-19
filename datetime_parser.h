#ifndef DATETIME_PARSER_H
#define DATETIME_PARSER_H

#include <KDateTime>

#include <QMap>

class DateTimeParser {
public:
    DateTimeParser();
    
    KDateTime parse( const QString & s );
    
    void addTimeFormat( const QString & s );
    void addDateFormat( const QString & s );
    
private:
    
    typedef QMap< QString, QRegExp > FormatMap;
    
private:
    
    KDateTime merge( const QDate & date, const QString & s, int offset );
    KDateTime merge( const QTime & time, const QString & s, int offset );
    
private:
    QString now, today, tomorrow, yesterday;
    
    FormatMap timeFormats;
    FormatMap dateFormats;
};

#endif