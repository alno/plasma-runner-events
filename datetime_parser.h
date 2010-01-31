#ifndef DATETIME_PARSER_H
#define DATETIME_PARSER_H

#include "datetime_range.h"

#include <QMap>

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
    QString now, today, tomorrow, yesterday, from, to;
    
    FormatMap timeFormats;
    FormatMap dateFormats;
};

#endif
