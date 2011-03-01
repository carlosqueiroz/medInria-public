/* medDataManager.h --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Mon Dec 21 08:34:45 2009 (+0100)
 * Version: $Id$
 * Last-Updated: Wed Mar 17 18:25:19 2010 (+0100)
 *           By: Julien Wintz
 *     Update #: 5
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#ifndef MEDDATAMANAGER_H
#define MEDDATAMANAGER_H

#include <QtCore/QObject>

#include "medCoreExport.h"
#include "medDataIndex.h"

class dtkAbstractData;
class dtkAbstractDataFactory;

class medDataManagerPrivate;

/**
 * This class is the global access point to data stored in the database. 
 * It tries to use several database-controllers to provide data
 */
class MEDCORE_EXPORT medDataManager : public QObject
{
    Q_OBJECT

public:
    static medDataManager *instance(void);

    void insert(const medDataIndex& index, dtkAbstractData *data);
    void remove(const medDataIndex& index);

          dtkAbstractData *  data(const medDataIndex& index);
    QList<dtkAbstractData *> dataForPatient(int id);
    QList<dtkAbstractData *> dataForStudy  (int id);
    QList<dtkAbstractData *> dataForSeries (int id);
    QList<dtkAbstractData *> dataForImage  (int id);

protected:
     medDataManager(void);
    ~medDataManager(void);

protected:
    static medDataManager *s_instance;

private:
    medDataManagerPrivate *d;
};

#endif // MEDDATAMANAGER_H
