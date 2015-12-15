/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medAbstractDiffusionModelEstimationProcess.h>
#include <medAbstractImageData.h>
#include <medAbstractDiffusionModelImageData.h>
#include <medMetaDataKeys.h>

class medAbstractDiffusionModelEstimationProcessPrivate
{
public:
    medAbstractImageData *input;
    medAbstractImageData *mask;
    medAbstractDiffusionModelImageData *output;
};

medAbstractDiffusionModelEstimationProcess::medAbstractDiffusionModelEstimationProcess(QObject *parent)
    : medAbstractProcess(parent), d(new medAbstractDiffusionModelEstimationProcessPrivate)
{
    d->input = NULL;
    d->mask = NULL;
    d->output = NULL;
}

medAbstractDiffusionModelEstimationProcess::~medAbstractDiffusionModelEstimationProcess()
{
}

void medAbstractDiffusionModelEstimationProcess::setInput(medAbstractImageData *data)
{
    if (!data)
        return;

    if (data->Dimension() != 4)
        return;

    d->input = data;
}

medAbstractImageData* medAbstractDiffusionModelEstimationProcess::input() const
{
    return d->input;
}

void medAbstractDiffusionModelEstimationProcess::setMask(medAbstractImageData *mask)
{
    if (mask->Dimension() != 3)
        return;

    d->mask = mask;
}

medAbstractImageData* medAbstractDiffusionModelEstimationProcess::mask() const
{
    return d->mask;
}

medAbstractDiffusionModelImageData* medAbstractDiffusionModelEstimationProcess::output() const
{
    return d->output;
}

void medAbstractDiffusionModelEstimationProcess::setOutput(medAbstractDiffusionModelImageData *data)
{
    d->output = data;

    QString newSeriesDescription = d->input->metadata ( medMetaDataKeys::SeriesDescription.key() );
    newSeriesDescription += " models";

    if (!d->output->hasMetaData(medMetaDataKeys::SeriesDescription.key()))
        d->output->setMetaData ( medMetaDataKeys::SeriesDescription.key(), newSeriesDescription );

    foreach ( QString metaData, d->input->metaDataList() )
    {
        if (!d->output->hasMetaData(metaData))
            d->output->addMetaData ( metaData, d->input->metaDataValues ( metaData ) );
    }

    foreach ( QString property, d->input->propertyList() )
        d->output->addProperty ( property,d->input->propertyValues ( property ) );
}