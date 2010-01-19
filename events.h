
#ifndef EVENTS_H
#define EVENTS_H

#include <plasma/abstractrunner.h>
#include <KIcon>

#include <Akonadi/Collection>

// Define our plasma Runner
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
    
    Akonadi::Collection eventsCollection;
    Akonadi::Collection todoCollection;
};

#endif
