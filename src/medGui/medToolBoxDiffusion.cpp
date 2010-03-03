/* medToolBoxDiffusion.cpp --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Fri Feb 19 09:06:02 2010 (+0100)
 * Version: $Id$
 * Last-Updated: Mon Feb 22 21:35:18 2010 (+0100)
 *           By: Julien Wintz
 *     Update #: 105
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#include <medCore/medPluginManager.h>

#include "medDropSite.h"
#include "medToolBoxDiffusion.h"

#include "dtkCore/dtkAbstractData.h"
#include "dtkCore/dtkAbstractDataFactory.h"
#include "dtkCore/dtkAbstractView.h"
#include "dtkCore/dtkAbstractViewInteractor.h"
#include "dtkCore/dtkAbstractProcess.h"
#include "dtkCore/dtkAbstractProcessFactory.h"
#include <medSql/medDatabaseController.h>
#include <medCore/medDataManager.h>

#include <medGui/medProgressionStack.h>

class medToolBoxDiffusionPrivate
{
public:
  medDropSite         *tractographyDropSite;
  medProgressionStack *progression_stack;

  QComboBox *colorCombo;
  QCheckBox *displayCheckBox;
  QRadioButton *displayRadioPolylines;
  QRadioButton *displayRadioRibbons;
  QRadioButton *displayRadioTubes;
  QSlider      *radiusSlider;

  QListWidget *bundlingList;
  QPushButton *bundlingButtonVdt;
  QPushButton *bundlingButtonTag;
  QPushButton *bundlingButtonRst;
  
  QList<dtkAbstractProcess *> methods;
  dtkAbstractProcess         *activeMethod;

  dtkAbstractView *view;
  
  
};

medToolBoxDiffusion::medToolBoxDiffusion(QWidget *parent) : medToolBox(parent), d(new medToolBoxDiffusionPrivate)
{
    d->activeMethod = 0;
    d->view = 0;
  
    // /////////////////////////////////////////////////////////////////
    // Display page
    // /////////////////////////////////////////////////////////////////

    QWidget *displayPage = new QWidget(this);

    QLabel *colorLabel = new QLabel("Color fibers by:", displayPage);

    d->colorCombo = new QComboBox(displayPage);
    d->colorCombo->addItem("Local orientation");
    d->colorCombo->addItem("Global orientation");
    d->colorCombo->addItem("Fractional anisotropy");

    QHBoxLayout *colorLayout = new QHBoxLayout;
    colorLayout->addWidget(colorLabel);
    colorLayout->addWidget(d->colorCombo);

    d->displayCheckBox = new QCheckBox("Use hardware acceleration", displayPage);

    d->displayRadioPolylines = new QRadioButton("Display fibers as polylines", displayPage);
    d->displayRadioRibbons = new QRadioButton("Display fibers as ribbons", displayPage);
    d->displayRadioTubes = new QRadioButton("Display fibers as tubes", displayPage);

    QButtonGroup *displayRadioGroup = new QButtonGroup(this);
    displayRadioGroup->addButton(d->displayRadioPolylines);
    displayRadioGroup->addButton(d->displayRadioRibbons);
    displayRadioGroup->addButton(d->displayRadioTubes);
    displayRadioGroup->setExclusive(true);

    QVBoxLayout *displayGroupLayout = new QVBoxLayout;
    displayGroupLayout->addWidget(d->displayRadioPolylines);
    displayGroupLayout->addWidget(d->displayRadioRibbons);
    displayGroupLayout->addWidget(d->displayRadioTubes);
    displayGroupLayout->addStretch(1);

    d->displayRadioPolylines->setChecked(true);

    QLabel *radiusLabel = new QLabel("Fibers radius:", displayPage);

    d->radiusSlider = new QSlider(Qt::Horizontal, displayPage);
    d->radiusSlider->setMinimum(1);
    d->radiusSlider->setMaximum(10);

    QHBoxLayout *radiusLayout = new QHBoxLayout;
    radiusLayout->addWidget(radiusLabel);
    radiusLayout->addWidget(d->radiusSlider);

    QVBoxLayout *displayLayout = new QVBoxLayout(displayPage);
    displayLayout->addLayout(colorLayout);
    displayLayout->addWidget(d->displayCheckBox);
    displayLayout->addLayout(displayGroupLayout);
    displayLayout->addLayout(radiusLayout);

    // /////////////////////////////////////////////////////////////////
    // Bundling page
    // /////////////////////////////////////////////////////////////////

    QWidget *bundlingPage = new QWidget(this);

    QCheckBox *bundleBoxCheckBox = new QCheckBox("Activate bundling box", bundlingPage);
    
    d->bundlingButtonTag = new QPushButton("Tag", bundlingPage);
    QPushButton *bundlingButtonAdd = new QPushButton("Add", bundlingPage);
    d->bundlingButtonRst = new QPushButton("Reset", bundlingPage);
    d->bundlingButtonVdt = new QPushButton("Validate", bundlingPage);

    QHBoxLayout *bundlingButtonsLayout = new QHBoxLayout;
    bundlingButtonsLayout->addWidget(d->bundlingButtonTag);
    bundlingButtonsLayout->addWidget(bundlingButtonAdd);
    bundlingButtonsLayout->addWidget(d->bundlingButtonRst);
    bundlingButtonsLayout->addWidget(d->bundlingButtonVdt);

    d->bundlingList = new QListWidget(bundlingPage);
    d->bundlingList->setAlternatingRowColors(true);

    QCheckBox *bundlingShowCheckBox = new QCheckBox("Show all bundles", bundlingPage);

    QVBoxLayout *bundlingLayout = new QVBoxLayout(bundlingPage);
    bundlingLayout->addWidget(bundleBoxCheckBox);
    bundlingLayout->addLayout(bundlingButtonsLayout);
    bundlingLayout->addWidget(d->bundlingList);
    bundlingLayout->addWidget(bundlingShowCheckBox);

    // /////////////////////////////////////////////////////////////////
    // Tractography page
    // /////////////////////////////////////////////////////////////////

    QWidget *tractographyPage = new QWidget(this);

    QLabel *tractographyMethodLabel = new QLabel("Use:", tractographyPage);

    QComboBox *tractographyMethodCombo = new QComboBox(tractographyPage);

    foreach(QString handler, medPluginManager::instance()->handlers("tractography"))
        tractographyMethodCombo->addItem(handler);

    d->progression_stack = new medProgressionStack(tractographyPage);
    QLabel *progressLabel = new QLabel("Progress:", tractographyPage);
    QHBoxLayout *progressStackLayout = new QHBoxLayout;
    progressStackLayout->addWidget(progressLabel);
    progressStackLayout->addWidget(d->progression_stack);
    
    QHBoxLayout *tractographyMethodLayout = new QHBoxLayout;
    tractographyMethodLayout->addWidget(tractographyMethodLabel);
    tractographyMethodLayout->addWidget(tractographyMethodCombo);
    
    // QTextEdit *tractographyMethodEdit = new QTextEdit(tractographyPage);
    // tractographyMethodEdit->setReadOnly(true);
    // tractographyMethodEdit->setMaximumHeight(100);

    d->tractographyDropSite = new medDropSite(tractographyPage);

    QPushButton *tractographyRunButton = new QPushButton("Run", tractographyPage);

    QVBoxLayout *tractographyLayout = new QVBoxLayout(tractographyPage);
    tractographyLayout->addLayout(tractographyMethodLayout);
    // tractographyLayout->addWidget(tractographyMethodEdit);
    tractographyLayout->addWidget(d->tractographyDropSite);
    tractographyLayout->addWidget(tractographyRunButton);
    tractographyLayout->addLayout(progressStackLayout);
    tractographyLayout->setAlignment(d->tractographyDropSite, Qt::AlignHCenter);

    // /////////////////////////////////////////////////////////////////
    // Setup
    // /////////////////////////////////////////////////////////////////

    medToolBoxTab *tab = new medToolBoxTab(this);
    tab->addTab(displayPage, "Display");
    tab->addTab(bundlingPage, "Bundling");
    tab->addTab(tractographyPage, "Tractography"); 

    if (dtkAbstractProcess *proc = dtkAbstractProcessFactory::instance()->create ("itkProcessTensorDTITrackPipeline")) {
      tractographyMethodCombo->addItem( proc->description() );
      d->methods.append ( proc );
      d->activeMethod = proc;
      connect (d->activeMethod, SIGNAL (progressed (int)), d->progression_stack,  SLOT (setProgress (int)));
    }
    
    connect (tractographyRunButton,    SIGNAL (clicked()),               this, SLOT (run()) );
    connect (d->colorCombo,            SIGNAL(currentIndexChanged(int)), this, SLOT (onColorModeChanged (int)));
    connect (d->displayCheckBox,       SIGNAL(stateChanged(int)),        this, SLOT (onGPUActivated (int)));
    connect (d->displayRadioPolylines, SIGNAL(toggled(bool)),            this, SLOT (onLinesRenderingModeSelected (bool)));
    connect (d->displayRadioRibbons,   SIGNAL(toggled(bool)),            this, SLOT (onRibbonsRenderingModeSelected (bool)));
    connect (d->displayRadioTubes,     SIGNAL(toggled(bool)),            this, SLOT (onTubesRenderingModeSelected (bool)));

    connect (bundleBoxCheckBox, SIGNAL (stateChanged(int)), this, SLOT (onBundlingBoxActivated (int)));

    this->setTitle("Diffusion");
    this->setWidget(tab);
}

medToolBoxDiffusion::~medToolBoxDiffusion(void)
{
    delete d;

    d = NULL;
}


void medToolBoxDiffusion::run (void)
{

  medDataIndex index = d->tractographyDropSite->index();
  
  if (!index.isValid())
      return;
  
  if (d->activeMethod) {
    
    dtkAbstractData *data = medDataManager::instance()->data (index);
    
    if (!data) {
        data = medDatabaseController::instance()->read(index);
	if (data)
	    medDataManager::instance()->insert(index, data);
    }
    
    if (data) {

        if (!data->hasMetaData ("DiffusionGradientList")) {
	  QMessageBox msgBox;
	  msgBox.setText("No diffusion gradient attached to the image.");
	  msgBox.setInformativeText("Do you want to specify them manually?");
	  msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
	  msgBox.setDefaultButton(QMessageBox::Ok);
	  int ret = msgBox.exec();

	  switch (ret) {
	      case QMessageBox::Ok:
		{
		  QString fileName = QFileDialog::getOpenFileName(this, tr("Gradient File"), "", tr("Gradient files (*.*)"));

		  if (fileName.isEmpty())
		    return;

		  dtkAbstractData *gradients=dtkAbstractDataFactory::instance()->create ("itkDataDiffusionGradientList");
		  if (!gradients){
		    // error
		    return;
		  }
		  if (!gradients->read(fileName)) {
		    // error
		    return;
		  }

		  double *grad = (double *)(gradients->data(0));
		  int i=0;
		  QStringList gradientList;
		  while (double *grad=(double *)(gradients->data(i))){
		    QString s_gx, s_gy, s_gz;
		    s_gx = QString::number (grad[0], 'g', 10);
		    s_gy = QString::number (grad[1], 'g', 10);
		    s_gz = QString::number (grad[2], 'g', 10);
		    gradientList.append (s_gx);
		    gradientList.append (s_gy);
		    gradientList.append (s_gz);
		    i++;
		  }
		  data->addMetaData ("DiffusionGradientList", gradientList);
		  
		}
		
		break;
	      case QMessageBox::Cancel:
		return;
	      default:
		return;
	  }
        }
	
        d->activeMethod->setInput (data);
	if (d->activeMethod->run()==0 )
	    if (d->view)
	        if (dtkAbstractViewInteractor *interactor = d->view->interactor ("v3dViewFiberInteractor")) {
		    d->view->setData ( d->activeMethod->output() );
		    d->view->update();
		}
    }
  }
}


void medToolBoxDiffusion::update (dtkAbstractView *view)
{
  if (!view || view==d->view)
    return;

  if (d->view)
    if (dtkAbstractViewInteractor *interactor = d->view->interactor ("v3dViewFiberInteractor")) {
      disconnect (d->radiusSlider,      SIGNAL(valueChanged(int)),           interactor, SLOT (onRadiusSet(int)));
      disconnect (d->bundlingButtonVdt, SIGNAL (clicked(void)),              interactor, SLOT (onSelectionValidated (void)));
      disconnect (d->bundlingButtonTag, SIGNAL(clicked(void)),               interactor, SLOT (onSelectionTagged(void)));
      disconnect (d->bundlingButtonRst, SIGNAL(clicked(void)),               interactor, SLOT (onSelectionReset(void)));
      disconnect (interactor,           SIGNAL(selectionValidated(QString)), this,       SLOT (onBundleValidated(QString)));
    }

  view->enableInteractor ("v3dViewFiberInteractor");
  dtkAbstractViewInteractor *interactor = view->interactor ("v3dViewFiberInteractor");

  if (!interactor) {
    qDebug() << "Cannot enable interactor: v3dViewFiberInteractor";
    return;
  }
  
  d->view = view;

  connect (d->radiusSlider,      SIGNAL(valueChanged(int)),           interactor, SLOT (onRadiusSet(int)));
  connect (d->bundlingButtonVdt, SIGNAL (clicked(void)),              interactor, SLOT (onSelectionValidated (void)));
  connect (d->bundlingButtonTag, SIGNAL(clicked(void)),               interactor, SLOT (onSelectionTagged(void)));
  connect (d->bundlingButtonRst, SIGNAL(clicked(void)),               interactor, SLOT (onSelectionReset(void)));
  connect (interactor,           SIGNAL(selectionValidated(QString)), this,       SLOT (onBundleValidated(QString)));
  
  if (d->activeMethod) {
    d->view->setData ( d->activeMethod->output() );
    d->view->update();
  }
}

void medToolBoxDiffusion::onColorModeChanged (int index)
{
  if (!d->view)
    return;

  if (dtkAbstractViewInteractor *interactor = d->view->interactor ("v3dViewFiberInteractor")) {
    if (index==0)
      interactor->setProperty("ColorMode","local");
    if (index==1)
      interactor->setProperty("ColorMode","global");
    if (index==2)
      interactor->setProperty("ColorMode","fa");

    d->view->update();
  }
}

void medToolBoxDiffusion::onGPUActivated (int value)
{
  if (!d->view)
    return;

  if (dtkAbstractViewInteractor *interactor = d->view->interactor ("v3dViewFiberInteractor")) {
    if (value)
      interactor->setProperty ("GPUMode", "true");
    else
      interactor->setProperty ("GPUMode", "false");

    d->view->update();
  }
}

void medToolBoxDiffusion::onLinesRenderingModeSelected (bool value)
{
  if (!d->view)
    return;
  
  if (value)
    if(dtkAbstractViewInteractor *interactor = d->view->interactor ("v3dViewFiberInteractor")) {
      interactor->setProperty ("RenderingMode", "lines");
      
      d->view->update();
    }
}

void medToolBoxDiffusion::onRibbonsRenderingModeSelected (bool value)
{
    if (!d->view)
    return;
  
    if (value)
      if(dtkAbstractViewInteractor *interactor = d->view->interactor ("v3dViewFiberInteractor")) {
	interactor->setProperty ("RenderingMode", "ribbons");
	
	d->view->update();
      }
}

void medToolBoxDiffusion::onTubesRenderingModeSelected (bool value)
{
  if (!d->view)
    return;
  
  if (value)
    if(dtkAbstractViewInteractor *interactor = d->view->interactor ("v3dViewFiberInteractor")) {
      interactor->setProperty ("RenderingMode", "tubes");

      d->view->update();
    }
}

void medToolBoxDiffusion::onBundlingBoxActivated (int value)
{
  if (!d->view)
    return;

  if(dtkAbstractViewInteractor *interactor = d->view->interactor ("v3dViewFiberInteractor")) {
    if (value)
      interactor->setProperty ("BoxVisibility", "true");
    else
      interactor->setProperty ("BoxVisibility", "false");

      d->view->update();
    }
}

void medToolBoxDiffusion::onBundleValidated (QString name)
{
    d->bundlingList->addItem (name);
}