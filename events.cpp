#include "events.h"

#include <KDebug>
#include <KIcon>

#include <Akonadi/CollectionFetchJob>
#include <Akonadi/ItemCreateJob>
#include <Akonadi/Item>

#include <kcal/event.h>

#include <boost/shared_ptr.hpp>

// This is the command that links your applet to the .desktop file
K_EXPORT_PLASMA_RUNNER(events_runner, EventsRunner)

// Mime types
QString eventMimeType = "application/x-vnd.akonadi.calendar.event";
QString todoMimeType = "application/x-vnd.akonadi.calendar.todo";

QString eventKeyword( "event" );
QString todoKeyword( "todo" );

using namespace Akonadi;

enum IncidentType {
    Event,
    Todo
};

EventsRunner::EventsRunner(QObject *parent, const QVariantList& args)
    : Plasma::AbstractRunner(parent, args)
{
    Q_UNUSED(args);
    
    setObjectName("events_runner");
    
    // fetching all collections containing emails recursively, starting at the root collection
    CollectionFetchJob *job = new CollectionFetchJob( Collection::root(), CollectionFetchJob::Recursive, this );
    
    connect( job, SIGNAL( collectionsReceived(Collection::List) ), this, SLOT( myCollectionsReceived(Collection::List) ) );
}

EventsRunner::~EventsRunner() {
}

void EventsRunner::reloadConfiguration() {
}

void EventsRunner::collectionsReceived( const Collection::List & list ) {
    foreach ( const Collection & coll, list ) {
        if ( !eventsCollection.isValid() && coll.contentMimeTypes().contains( eventMimeType ) ) {
            eventsCollection = coll;
        }
        
        if ( !todoCollection.isValid() && coll.contentMimeTypes().contains( todoMimeType ) ) {
            todoCollection = coll;
        }
    }
}

void EventsRunner::match( Plasma::RunnerContext &context ) {
    const QString term = context.query();
    
    if ( term.length() < 8 )
        return;
    
    
    if ( term.startsWith( eventKeyword ) ) {
        QString summary = term.mid( eventKeyword.length() ).trimmed();
        
        if ( summary.length() < 3 )
            return;
        
        QMap<QString,QVariant> data;
        data["type"] = Event;
        data["summary"] = summary;
        data["start"] = KDateTime::currentLocalDateTime().toString();
        
        Plasma::QueryMatch match( this );
        
        match.setText(i18n( "Create event \"%1\"", summary ));        
        match.setData( data );
        match.setId( summary );  
        match.setRelevance( 0.8 );
        
        context.addMatch( term, match );
     }
}

void EventsRunner::run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match)
{    
    Q_UNUSED(context)
    
    QMap<QString,QVariant> data = match.data().toMap();
    
    if ( data["type"] == Event ) {
        if ( !eventsCollection.isValid() ) {
            qDebug() << "No valid collection for events available";
            return;
        }
        
        KCal::Event::Ptr event( new KCal::Event() );
        event->setSummary( data["summary"].toString() );
        event->setDtStart( KDateTime::fromString( data["start"].toString() ) );
            
        Item item( eventMimeType );
        item.setPayload<KCal::Event::Ptr>( event );
            
        new Akonadi::ItemCreateJob( item, eventsCollection, this );
    }
}

