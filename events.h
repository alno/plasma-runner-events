
#ifndef EVENTS_H
#define EVENTS_H

#include <Plasma/AbstractRunner>

#include <Akonadi/Collection>

#include <KIcon>

#include "datetime_parser.h"

/**
*/
class EventsRunner : public Plasma::AbstractRunner {
    Q_OBJECT

public:
    // Basic Create/Destroy
    EventsRunner( QObject *parent, const QVariantList& args );
    ~EventsRunner();

    void match(Plasma::RunnerContext &context);
    void run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match);
    
    void reloadConfiguration();
    
private slots:
    
    /**
      Called when Akonadi collections loaded
    */
    void collectionsReceived( const Akonadi::Collection::List & list );
    
private:

    DateTimeParser dateTimeParser;
    
    Akonadi::Collection eventsCollection;
    Akonadi::Collection todoCollection;
};

#endif
