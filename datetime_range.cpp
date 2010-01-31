
#include "datetime_range.h"

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
