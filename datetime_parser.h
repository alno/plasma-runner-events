#ifndef DATETIME_PARSER_H
#define DATETIME_PARSER_H

#include <KDateTime>

class DateTimeParser {
public:
    DateTimeParser();
    
    KDateTime parse( const QString & s );
    
private:
    
    KDateTime merge( const QDate & date, const QString & s, int offset );
    KDateTime merge( const QTime & time, const QString & s, int offset );
    
private:
    QString now, today, tomorrow, yesterday;
};

#endif