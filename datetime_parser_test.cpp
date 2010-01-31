#include "datetime_parser_test.h"

static void assertRangeEquals( const DateTimeRange & range, const KDateTime & dateTime ) {
    QVERIFY( range.start == dateTime );
    QVERIFY( range.finish == dateTime );
}

void DateTimeParserTest::testSimpleKeywords() {
    QVERIFY( KDateTime::currentLocalDateTime() == parser.parse("now") );
    QVERIFY( KDateTime( KDateTime::currentLocalDate() ) == parser.parse("today") );
    QVERIFY( KDateTime( KDateTime::currentLocalDate().addDays( 1 ) ) == parser.parse("tomorrow") );
    QVERIFY( KDateTime( KDateTime::currentLocalDate().addDays( -1 ) ) == parser.parse("yesterday") );
}

void DateTimeParserTest::testRelativeKeywords() {
    QVERIFY( KDateTime( KDateTime::currentLocalDate().addDays( 5 ) ) == parser.parse("in 5 days") );
    QVERIFY( KDateTime( KDateTime::currentLocalDate().addMonths( 2 ) ) == parser.parse("in 2 months") );
    QVERIFY( KDateTime( KDateTime::currentLocalDate().addYears( 3 ) ) == parser.parse("in 3 years") );
    QVERIFY( KDateTime( KDateTime::currentLocalDate().addDays( -1 ).addYears( 3 ) ) == parser.parse("in 3 years from yesterday") );
}

void DateTimeParserTest::testPreciseSpecs() {
    QVERIFY( KDateTime( QDate::fromString( "21.10.2009", "d.M.yyyy" ) ) == parser.parse("21.10.2009") );
}

void DateTimeParserTest::testPointRanges() {
    DateTimeRange r1 = parser.parseRange("21.10.2009");
    QVERIFY( r1.isPoint() );
    QVERIFY( r1.start == KDateTime( QDate::fromString( "21.10.2009", "d.M.yyyy" ) ) );

    DateTimeRange r2 = parser.parseRange("21.10.2009");
    QVERIFY( r2.isPoint() );
    QVERIFY( r2.start == KDateTime( QDate::fromString( "21.10.2009", "d.M.yyyy" ) ) );
}

void DateTimeParserTest::testNonPointRanges() {
}

QTEST_MAIN(DateTimeParserTest)

