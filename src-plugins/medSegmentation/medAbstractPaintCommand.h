/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <QUndoCommand>
#include <QtCore>
#include <QVector>
#include <QVector3D>

#include <itkImage.h>
#include <itkLabelObject.h>
#include <itkAttributeLabelObject.h>
#include <itkLabelMap.h>
#include <itkLabelImageToLabelMapFilter.h>
#include <itkSmartPointer.h>

class medAbstractPaintCommandPrivate;

class medClickAndMoveEventFilter;
class medAbstractImageView;
class medAbstractData;
class medAbstractImageData;


typedef itk::Image<unsigned char, 3> MaskType;

typedef itk::AttributeLabelObject<unsigned char, 3, unsigned char> LabelObjectType;
typedef itk::LabelMap< LabelObjectType > LabelMapType;


typedef itk::SmartPointer<itk::Image<unsigned char, 3> >MaskTypePointer;
typedef itk::SmartPointer<itk::LabelMap< LabelObjectType > > LabelMapTypePointer;


class medPaintCommandManager : public QObject
{
    Q_OBJECT

public:
    static medPaintCommandManager *instance();
    LabelMapType::Pointer labelMap(MaskType* mask);

protected:
    medPaintCommandManager(){}
    ~medPaintCommandManager(){}

protected:
    static medPaintCommandManager *s_instance;
    QHash<MaskType*, LabelMapType::Pointer> labelMaps;
};



struct medPaintCommandOptions
{
    medAbstractImageView *view;
    medAbstractData* data;
    medAbstractData *maskData;
    //LabelMapType::Pointer labelMap;
    MaskType::Pointer itkMask;
    QVector<QVector3D> points;
    unsigned int maskValue;
    double radius;
};


class medAbstractPaintCommand : public QUndoCommand
{
public:
    medAbstractPaintCommand(medPaintCommandOptions *options, QUndoCommand *parent = 0);

    virtual void undo() = 0;
    virtual void redo() = 0;

    medPaintCommandOptions *options() const;

//protected:
//    virtual void paint() = 0;
//    virtual void unpaint() = 0;

private:
    medAbstractPaintCommandPrivate *d;
};
