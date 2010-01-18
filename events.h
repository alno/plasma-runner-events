
#ifndef EVENTS_H
#define EVENTS_H

#include <plasma/abstractrunner.h>
#include <KIcon>

// Define our plasma Runner
class events : public Plasma::AbstractRunner {
    Q_OBJECT

public:
    // Basic Create/Destroy
    events( QObject *parent, const QVariantList& args );
    ~events();

    void match(Plasma::RunnerContext &context);
    void run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match);
};
// This is the command that links your applet to the .desktop file
K_EXPORT_PLASMA_RUNNER(events, events)

#endif
