#ifndef DATETIME_PARSER_TEST_H
#define DATETIME_PARSER_TEST_H

#include <QtTest/QtTest>

#include "datetime_parser.h"

class DateTimeParserTest: public QObject {
    Q_OBJECT
private slots:
    void testSimpleKeywords();
    void testRelativeKeywords();
    void testPreciseSpecs();
    void testPointRanges();
    void testNonPointRanges();
private:
    DateTimeParser parser;
};

#endif
