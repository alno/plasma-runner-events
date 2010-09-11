/*
 *   Copyright (C) 2010 Alexey Noskov <alexey.noskov@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License as
 *   published by the Free Software Foundation; either version 2 of
 *   the License or (at your option) version 3 or any later version
 *   accepted by the membership of KDE e.V. (or its successor approved
 *   by the membership of KDE e.V.), which shall act as a proxy
 *   defined in Section 14 of version 3 of the license.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

//Project-Includes
#include "events_config.h"
//KDE-Includes
#include <Plasma/AbstractRunner>

K_EXPORT_RUNNER_CONFIG(events, EventsRunnerConfig)

EventsRunnerConfigForm::EventsRunnerConfigForm(QWidget* parent) : QWidget(parent)
{
    setupUi( this );
}

EventsRunnerConfig::EventsRunnerConfig(QWidget* parent, const QVariantList& args): KCModule(ConfigFactory::componentData(), parent, args)
{
    ui = new EventsRunnerConfigForm(this);

    QGridLayout* layout = new QGridLayout(this);
    layout->addWidget(ui, 0, 0);
}


void EventsRunnerConfig::defaults()
{
    KCModule::defaults();
}

void EventsRunnerConfig::load()
{
    KCModule::load();
}

void EventsRunnerConfig::save()
{
    KCModule::save();
}
