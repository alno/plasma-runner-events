#include "datetime_parser_test.h"

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

QTEST_MAIN(DateTimeParserTest)

