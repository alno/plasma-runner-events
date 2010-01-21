#include "events.h"

#include <KDebug>
#include <KMimeType>
#include <KIconLoader>

#include <Akonadi/CollectionFetchJob>
#include <Akonadi/ItemCreateJob>
#include <Akonadi/Item>

#include <kcal/event.h>
#include <kcal/todo.h>

#include <boost/shared_ptr.hpp>

// This is the command that links your applet to the .desktop file
K_EXPORT_PLASMA_RUNNER(events_runner, EventsRunner)

// Mime types
QString eventMimeType( "text/calendar" );
QString todoMimeType( "text/calendar" );

QString eventKeyword( "event" );
QString todoKeyword( "todo" );

using namespace Akonadi;

enum IncidentType {
    Event,
    Todo
};

static KDateTime variantToDateTime( const QVariant & var ) {
    return var.type() == QVariant::Date ? KDateTime( var.toDate() ) : KDateTime( var.toDateTime() );
}

static QVariant dateTimeToVariant( const KDateTime & dt ) {
    return dt.isDateOnly() ? QVariant( dt.date() ) : QVariant( dt.dateTime() );
}

EventsRunner::EventsRunner(QObject *parent, const QVariantList& args)
    : Plasma::AbstractRunner(parent, args)
{
    Q_UNUSED(args);
    
    setObjectName("events_runner");
            
    icon = KIcon( KIconLoader().loadMimeTypeIcon( KMimeType::mimeType( "text/calendar" )->iconName(), KIconLoader::NoGroup ) );
    
    qDebug() << "Runner started";
    
    CollectionFetchJob *job = new CollectionFetchJob( Collection::root(), CollectionFetchJob::Recursive, this );

 //   connect( job, SIGNAL( collectionsReceived(Akonadi::Collection::List) ), this, SLOT( collectionsReceived(Akonadi::Collection::List) ) );
    connect( job, SIGNAL(result(KJob*)), this, SLOT(jobCompleted(KJob*)) );
}

EventsRunner::~EventsRunner() {
}

void EventsRunner::reloadConfiguration() {
}

void EventsRunner::jobCompleted( KJob * job ) {
    qDebug() << "Collections fetched" << endl;

    if ( job->error() ) {
        qDebug() << "Error occurred: " << job->errorText() << endl;
        exit( -1 );
        return;
    }

    const CollectionFetchJob * fetchJob = qobject_cast<CollectionFetchJob*>( job );

    qDebug() << "There are " << fetchJob->collections().size() << " collections";

    collectionsReceived( fetchJob->collections() );
}

void EventsRunner::collectionsReceived( const Collection::List & list ) {
    foreach ( const Collection & coll, list ) {
        qDebug() << coll.name() << ": " << coll.contentMimeTypes();

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
        data["dtStart"] = dateTimeToVariant( startDate );

        Plasma::QueryMatch match( this );

        match.setText( i18n( "Create event \"%1\" at %2", summary, startDate.toString( startDate.isDateOnly() ? "%d.%m.%Y" : "%H:%M %d.%m.%Y" ) ) );
        match.setData( data );
        match.setId( eventKeyword );
        match.setRelevance( 0.8 );
        match.setIcon( icon );

        context.addMatch( term, match );
     } else if ( term.startsWith( todoKeyword ) ) {
        QStringList args = term.mid( eventKeyword.length() ).split( ";" );
    
        if ( args.size() < 2 || args[0].length() < 3 || args[1].length() < 3 )
            return;
        
        QString summary = args[0].trimmed();
        KDateTime dueDate = dateTimeParser.parse( args[1].trimmed() );
        
        if ( !dueDate.isValid() )
            return;
        
        QMap<QString,QVariant> data;
        data["type"] = Todo;
        data["summary"] = summary;
        data["dtDue"] = dateTimeToVariant( dueDate );
        
        Plasma::QueryMatch match( this );
        
        match.setText( i18n( "Create todo \"%1\" due to %2", summary, dueDate.toString( dueDate.isDateOnly() ? "%d.%m.%Y" : "%H:%M %d.%m.%Y" ) ) );
        match.setData( data );
        match.setId( todoKeyword );  
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
        event->setDtStart( variantToDateTime( data["dtStart"] ) );
            
        Item item( eventMimeType );
        item.setPayload<KCal::Event::Ptr>( event );
            
        new Akonadi::ItemCreateJob( item, eventsCollection, this );
    } else if ( data["type"].toInt() == Todo ) {
        if ( !todoCollection.isValid() ) {
            qDebug() << "No valid collection for todos available";
            return;
        }
        
        KCal::Todo::Ptr todo( new KCal::Todo() );
        todo->setSummary( data["summary"].toString() );
        todo->setDtDue( variantToDateTime( data["dtDue"] ) );
        todo->setPercentComplete( 0 );
        todo->setHasStartDate( false );
        todo->setHasDueDate( true );
            
        Item item( todoMimeType );
        item.setPayload<KCal::Todo::Ptr>( todo );
            
        new Akonadi::ItemCreateJob( item, todoCollection, this );
    }
}

