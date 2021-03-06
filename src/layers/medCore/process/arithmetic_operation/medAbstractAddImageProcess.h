#pragma once
/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2020. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medAbstractArithmeticOperationProcess.h>

#include <dtkCore>

#include <medCoreExport.h>

class MEDCORE_EXPORT  medAbstractAddImageProcess: public medAbstractArithmeticOperationProcess
{
    Q_OBJECT
public:
    medAbstractAddImageProcess(QObject *parent): medAbstractArithmeticOperationProcess(parent) {}

protected:
    virtual QString outputNameAddon() const {return "add image";}
};

DTK_DECLARE_PLUGIN_FACTORY(medAbstractAddImageProcess, MEDCORE_EXPORT)
