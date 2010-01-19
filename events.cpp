#include "events.h"

#include <KDebug>
#include <KMimeType>
#include <KIconLoader>

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
            
    icon = KIcon( KIconLoader().loadMimeTypeIcon( KMimeType::mimeType( "text/calendar" )->iconName(), KIconLoader::NoGroup ) );
    
    CollectionFetchJob *job = new CollectionFetchJob( Collection::root(), CollectionFetchJob::Recursive, this );
    
    connect( job, SIGNAL( collectionsReceived(Akonadi::Collection::List) ), this, SLOT( collectionsReceived(Akonadi::Collection::List) ) );
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
        QStringList args = term.mid( eventKeyword.length() ).split( ";" );
                
        if ( args.size() < 2 || args[0].length() < 3 || args[1].length() < 3 )
            return;
        
        QString summary = args[0].trimmed();
        KDateTime startDate = dateTimeParser.parse( args[1].trimmed() );
        
        if ( !startDate.isValid() )
            return;
        
        QMap<QString,QVariant> data;
        data["type"] = Event;
        data["summary"] = summary;
        data["start"] = startDate.toString();
        
        Plasma::QueryMatch match( this );
        
        match.setText( i18n( "Create event \"%1\" at %2 %3", summary, startDate.isDateOnly() ? "" : startDate.time().toString("hh:mm") , startDate.date().toString("dd.MM.yyyy") ) );        
        match.setData( data );
        match.setId( "event" );  
        match.setRelevance( 0.8 );
        match.setIcon( icon );
        
        context.addMatch( term, match );
     }
}

void EventsRunner::run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match) {    
    Q_UNUSED(context)
    
    QMap<QString,QVariant> data = match.data().toMap();
    
    if ( data["type"].toInt() == Event ) {
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

