/* medToolBoxFactory.cpp ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Wed Nov 10 14:33:05 2010 (+0100)
 * Version: $Id$
 * Last-Updated: Wed Nov 10 16:06:00 2010 (+0100)
 *           By: Julien Wintz
 *     Update #: 32
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "medToolBox.h"
#include "medToolBoxFactory.h"

class medToolBoxFactoryPrivate
{
public:
    medToolBoxFactory::medToolBoxRegistrationCustomCreatorHash custom_registration_creators;
    medToolBoxFactory::medToolBoxDiffusionCustomCreatorHash custom_diffusion_creators;
    medToolBoxFactory::medToolBoxCompositeDataSetImporterCustomCreatorHash custom_compositedatasetimporter_creators;
    medToolBoxFactory::medToolBoxFilteringCustomCreatorHash custom_filtering_creators;
};

medToolBoxFactory *medToolBoxFactory::instance(void)
{
    if(!s_instance)
        s_instance = new medToolBoxFactory;

    return s_instance;
}

bool medToolBoxFactory::registerCustomRegistrationToolBox(QString type, medToolBoxRegistrationCustomCreator func)
{
    if(!d->custom_registration_creators.contains(type)) {
        d->custom_registration_creators.insert(type, func);
        return true;
    }

    return false;
}

QList<QString> medToolBoxFactory::registrationToolBoxes(void)
{
    return d->custom_registration_creators.keys();
}

medToolBoxRegistrationCustom *medToolBoxFactory::createCustomRegistrationToolBox(QString type, QWidget *parent)
{
    if(!d->custom_registration_creators.contains(type))
        return NULL;

    medToolBoxRegistrationCustom *toolbox = d->custom_registration_creators[type](parent);

    return toolbox;
}

bool medToolBoxFactory::registerCustomDiffusionToolBox(QString type, medToolBoxDiffusionCustomCreator func)
{
    if(!d->custom_diffusion_creators.contains(type)) {
        d->custom_diffusion_creators.insert(type, func);
        return true;
    }

    return false;
}

QList<QString> medToolBoxFactory::diffusionToolBoxes(void)
{
    return d->custom_diffusion_creators.keys();
}

medToolBoxDiffusionCustom *medToolBoxFactory::createCustomDiffusionToolBox(QString type, QWidget *parent)
{
    if(!d->custom_diffusion_creators.contains(type))
        return NULL;

    medToolBoxDiffusionCustom *toolbox = d->custom_diffusion_creators[type](parent);

    return toolbox;
}

bool medToolBoxFactory::registerCustomCompositeDataSetImporterToolBox(QString type, medToolBoxCompositeDataSetImporterCustomCreator func)
{
    if(!d->custom_compositedatasetimporter_creators.contains(type)) {
        d->custom_compositedatasetimporter_creators.insert(type, func);
        return true;
    }
    return false;
}

bool medToolBoxFactory::registerCustomFilteringToolBox(QString type, medToolBoxFilteringCustomCreator func)
{
    if(!d->custom_filtering_creators.contains(type)) {
        d->custom_filtering_creators.insert(type, func);
        return true;
    }

    return false;
}

QList<QString> medToolBoxFactory::compositeDataSetImporterToolBoxes(void)
{
    return d->custom_compositedatasetimporter_creators.keys();
}

medToolBoxCompositeDataSetImporterCustom *medToolBoxFactory::createCustomCompositeDataSetImporterToolBox(QString type, QWidget *parent)
{
    if(!d->custom_compositedatasetimporter_creators.contains(type))
        return NULL;

    medToolBoxCompositeDataSetImporterCustom *toolbox = d->custom_compositedatasetimporter_creators[type](parent);
    return toolbox;
}

QList<QString> medToolBoxFactory::filteringToolBoxes(void)
{
    return d->custom_filtering_creators.keys();
}

medToolBoxFilteringCustom *medToolBoxFactory::createCustomFilteringToolBox(QString type, QWidget *parent)
{
    if(!d->custom_filtering_creators.contains(type))
        return NULL;

    medToolBoxFilteringCustom *toolbox = d->custom_filtering_creators[type](parent);

    return toolbox;
}

medToolBoxFactory::medToolBoxFactory(void) : dtkAbstractFactory(), d(new medToolBoxFactoryPrivate)
{

}

medToolBoxFactory::~medToolBoxFactory(void)
{
    delete d;

    d = NULL;
}

medToolBoxFactory *medToolBoxFactory::s_instance = NULL;