#include "datetime_parser.h"

#include <KLocalizedString>
#include <QDebug>

QRegExp inMinutes("in\\s*([+-]?\\d+)\\s*minutes\\s*(after\\s+)?");
QRegExp inHours("in\\s*([+-]?\\d+)\\s*hours\\s*(after\\s+)?");
QRegExp inDays("in\\s*([+-]?\\d+)\\s*days\\s*(after\\s+)?");
QRegExp inWeeks("in\\s*([+-]?\\d+)\\s*weeks\\s*(after\\s+)?");
QRegExp inMonths("in\\s*([+-]?\\d+)\\s*months\\s*(after\\s+)?");
QRegExp inYears("in\\s*([+-]?\\d+)\\s*years\\s*(after\\s+)?");

void DateTimeRange::setDate( const QDate & date, Elements elems ) {
    if ( !date.isValid() )
        return;

    if ( elems & Start ) {
        if ( start.isValid() )
            start.setDate( date );
        else
            start = KDateTime( date );
    }

    if ( elems & Finish ) {
        if ( finish.isValid() )
            finish.setDate( date );
        else
            finish = KDateTime( date );
    }
}

void DateTimeRange::setTime( const QTime & time, Elements elems ) {
    if ( !time.isValid() )
        return;

    if ( elems & Start ) {
        if ( !start.isValid() )
            start = KDateTime( QDate::currentDate() );

        start.setDateOnly( false );
        start.setTime( time );
    }

    if ( elems & Finish ) {
        if ( !finish.isValid() )
            finish = KDateTime( QDate::currentDate() );

        finish.setDateOnly( false );
        finish.setTime( time );
    }
}

void DateTimeRange::addSecs( int secs, Elements elems ) {
    if ( elems & Start ) start = start.addSecs( secs );
    if ( elems & Finish ) finish = finish.addSecs( secs );
}

void DateTimeRange::addDays( int days, Elements elems ) {
    if ( elems & Start ) start = start.addDays( days );
    if ( elems & Finish ) finish = finish.addDays( days );
}

void DateTimeRange::addMonths( int months, Elements elems ) {
    if ( elems & Start ) start = start.addMonths( months );
    if ( elems & Finish ) finish = finish.addMonths( months );
}

void DateTimeRange::addYears( int years, Elements elems ) {
    if ( elems & Start ) start = start.addYears( years );
    if ( elems & Finish ) finish = finish.addYears( years );
}

DateTimeParser::DateTimeParser() {
    now = i18n("now");
    today = i18n("today");
    tomorrow = i18n("tomorrow");
    yesterday = i18n("yesterday");
    
    addTimeFormat( "h:mm" );
    
    addDateFormat( "d.M.yyyy" );
}

void DateTimeParser::addTimeFormat( const QString & s ) {
    if ( timeFormats.contains( s ) )
        return;
    
    QString formatRegexp = QRegExp::escape( s );
    
    formatRegexp.replace( QRegExp("hh|mm|ss"), "\\d\\d" );
    formatRegexp.replace( QRegExp("h|m|s"), "\\d\\d?" );
    formatRegexp.replace( "zzz", "\\d\\d\\d" );
    formatRegexp.replace( "z", "\\d\\d?\\d?" );
    formatRegexp.replace( "AP", "(AM|PM)" );
    formatRegexp.replace( "ap", "(am|pm)" );
    
    timeFormats.insert( s, QRegExp( formatRegexp ) );
}

void DateTimeParser::addDateFormat( const QString & s ) {
    if ( dateFormats.contains( s ) )
        return;
    
    QString formatRegexp = QRegExp::escape( s );
        
    formatRegexp.replace( 'd', 'D' );
    formatRegexp.replace( "yyyy", "\\d\\d\\d\\d" );
    formatRegexp.replace( QRegExp("DDDD|MMMM"), "\\w+" );
    formatRegexp.replace( "DDD", "(Mon|Tue|Wed|Thu|Fri|Sat|Sun)" );
    formatRegexp.replace( "MMM", "(Jan|Feb|Mar|Apr|May|Jun|Jul|Aug|Sep|Oct|Nov|Dec)" );
    formatRegexp.replace( QRegExp("DD|MM|yy"), "\\d\\d" );
    formatRegexp.replace( QRegExp("D|M"), "\\d\\d?" );
    
    dateFormats.insert( s, QRegExp( formatRegexp ) );
}

DateTimeRange DateTimeParser::parseRange( const QString & s ) {
    DateTimeRange range;
    QString remaining = s;

    while ( !remaining.isEmpty() )
        remaining = parseElement( remaining, range, DateTimeRange::Both );

    return range;
}

QString DateTimeParser::parseElement( const QString & s, DateTimeRange & range, DateTimeRange::Elements elems, const QDate & defaultDate, const QTime & defaultTime ) {
    if ( s.startsWith( now ) ) {
        range.setDate( QDate::currentDate(), elems );
        range.setTime( QTime::currentTime(), elems );

        return s.mid( now.length() ).trimmed();
    } else if ( s.startsWith( today ) ) {
        range.setDate( QDate::currentDate(), elems );

        return s.mid( today.length() ).trimmed();
    } else if ( s.startsWith( tomorrow ) ) {
        range.setDate( QDate::currentDate().addDays( 1 ), elems );

        return s.mid( tomorrow.length() ).trimmed();
    } else if ( s.startsWith( yesterday ) ) {
        range.setDate( QDate::currentDate().addDays( -1 ), elems );

        return s.mid( yesterday.length() ).trimmed();
    }

    if ( inMinutes.indexIn( s ) == 0 ) {
        int value = inMinutes.cap( 1 ).toInt();
        QString rem = s.mid( inMinutes.matchedLength() ).trimmed();
        QString res = parseElement( rem, range, elems, QDate(), QTime::currentTime() );

        range.addSecs( value * 60, elems );

        return res;
    }

    if ( inHours.indexIn( s ) == 0 ) {
        int value = inHours.cap( 1 ).toInt();
        QString rem = s.mid( inHours.matchedLength() ).trimmed();
        QString res = parseElement( rem, range, elems, QDate(), QTime::currentTime() );

        range.addSecs( value * 3600, elems );

        return res;
    }

    if ( inDays.indexIn( s ) == 0 ) {
        int value = inDays.cap( 1 ).toInt();
        QString rem = s.mid( inDays.matchedLength() ).trimmed();
        QString res = parseElement( rem, range, elems, QDate::currentDate() );

        range.addDays( value, elems );

        return res;
    }

    if ( inWeeks.indexIn( s ) == 0 ) {
        int value = inWeeks.cap( 1 ).toInt();
        QString rem = s.mid( inWeeks.matchedLength() ).trimmed();
        QString res = parseElement( rem, range, elems, QDate::currentDate() );

        range.addDays( value * 7, elems );

        return res;
    }

    if ( inMonths.indexIn( s ) == 0 ) {
        int value = inMonths.cap( 1 ).toInt();
        QString rem = s.mid( inMonths.matchedLength() ).trimmed();
        QString res = parseElement( rem, range, elems, QDate::currentDate() );

        range.addMonths( value, elems );

        return res;
    }

    if ( inYears.indexIn( s ) == 0 ) {
        int value = inYears.cap( 1 ).toInt();
        QString rem = s.mid( inYears.matchedLength() ).trimmed();
        QString res = parseElement( rem, range, elems, QDate::currentDate() );

        range.addYears( value, elems );

        return res;
    }

    for ( FormatMap::iterator it = timeFormats.begin(); it != timeFormats.end(); ++ it ) {
        if ( it.value().indexIn( s ) == 0 ) {
            range.setTime( QTime::fromString( s.left( it.value().matchedLength() ), it.key() ), elems );

            return s.mid( it.value().matchedLength() ).trimmed();
        }
    }

    for ( FormatMap::iterator it = dateFormats.begin(); it != dateFormats.end(); ++ it ) {
        if ( it.value().indexIn( s ) == 0 ) {
            range.setDate( QDate::fromString( s.left( it.value().matchedLength() ), it.key() ), elems );

            return s.mid( it.value().matchedLength() ).trimmed();
        }
    }

    range.setDate( defaultDate, elems );
    range.setTime( defaultTime, elems );

    return "";
}

KDateTime DateTimeParser::parse( const QString& s ) {
    return parseRange( s ).start;
}
