#include "datetime_parser.h"

#include <KLocalizedString>
#include <QDebug>

DateTimeParser::DateTimeParser() {
    now = i18n("now");
    today = i18n("today");
    tomorrow = i18n("tomorrow");
    yesterday = i18n("yesterday");
}

KDateTime DateTimeParser::parse( const QString& s ) {
    if ( s.isEmpty() )
        return KDateTime();
    
    QRegExp time("\\d\\d?\\:\\d\\d");
    
    if ( s.startsWith( now ) ) {
        return KDateTime::currentLocalDateTime();
    } else if ( s.startsWith( today ) ) {
        return merge( QDate::currentDate(), s, today.length() );
    } else if ( s.startsWith( tomorrow ) ) {
        return merge( QDate::currentDate().addDays( 1 ), s, today.length() );
    } else if ( s.startsWith( yesterday ) ) {
        return merge( QDate::currentDate().addDays( -1 ), s, today.length() );
    } else if ( time.indexIn( s ) == 0 ) {
        return merge( QTime::fromString( s.left( time.matchedLength() ), "h:mm" ), s, time.matchedLength() );
    }
    
    return KDateTime::fromString( s );
}

KDateTime DateTimeParser::merge( const QDate & date, const QString & s, int offset ) {
    KDateTime dt = parse( s.mid( offset ).trimmed() );
    
    if ( dt.isValid() ) {
        dt.setDate( date );
        return dt;
    } else {
        return KDateTime( date );
    }
}

KDateTime DateTimeParser::merge( const QTime & time, const QString & s, int offset ) {
    KDateTime dt = parse( s.mid( offset ).trimmed() );
    
    if ( dt.isValid() ) {
        dt.setDateOnly( false );
        dt.setTime( time );
        return dt;
    } else {
        return KDateTime( QDate::currentDate(), time );
    }
}