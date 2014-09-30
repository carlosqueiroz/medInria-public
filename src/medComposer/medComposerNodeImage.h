/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#ifndef COMPOSERNODEIMAGE_H
#define COMPOSERNODEIMAGE_H

#include <dtkComposer/dtkComposerNodeLeaf.h>
#include <dtkComposer/dtkComposerNodeLeafData.h>

#include <QtCore>

class medComposerNodeImagePrivate;

class medComposerNodeImage : public dtkComposerNodeLeafData
{
public:
     medComposerNodeImage(void);
    ~medComposerNodeImage(void);

public:
    bool isAbstractData(void) const;

    QString abstractDataType(void) const;

public:
    void run(void);

public:
    QString type(void);
    QString titleHint(void) ;

public:
    QString  inputLabelHint(int port);
    QString outputLabelHint(int port);

private:
    medComposerNodeImagePrivate *d;
};

#endif