// $Id$
//-----------------------------------------------------------------------
//       The GSI Online Offline Object Oriented (Go4) Project
//         Experiment Data Processing at EE department, GSI
//-----------------------------------------------------------------------
// Copyright (C) 2000- GSI Helmholtzzentrum fuer Schwerionenforschung GmbH
//                     Planckstr. 1, 64291 Darmstadt, Germany
// Contact:            http://go4.gsi.de
//-----------------------------------------------------------------------
// This software can be used under the license agreements as stated
// in Go4License.txt file which is part of the distribution.
//-----------------------------------------------------------------------

#include "QFitDataWidget.h"
#include "QFitItem.h"
#include "TGo4FitData.h"
#include "TGo4FitModel.h"
#include "TGo4Fitter.h"
#include "TGo4FitGuiTypes.h"

QFitDataWidget::QFitDataWidget(QWidget *parent, const char* name)
         : QFitNamedWidget(parent, name)
{
   setupUi(this);

   QObject::connect(NumCalibrSpin, SIGNAL(valueChanged(int)), this, SLOT(NumCalibrSpin_valueChanged(int)));
   QObject::connect(UseBinsChk, SIGNAL(toggled(bool)), this, SLOT(UseBinsChk_toggled(bool)));
   QObject::connect(SigmaCmb, SIGNAL(activated(int)), this, SLOT(SigmaCmb_activated(int)));
   QObject::connect(SigmaEdt, SIGNAL(textChanged(QString)), this, SLOT(SigmaEdt_textChanged(QString)));
   QObject::connect(BinsLimitEdt, SIGNAL(textChanged(QString)), this, SLOT(BinsLimitEdt_textChanged(QString)));
   QObject::connect(AmplChk, SIGNAL(toggled(bool)), this, SLOT(AmplChk_toggled(bool)));
   QObject::connect(UseBuffersChk, SIGNAL(toggled(bool)), this, SLOT(UseBuffersChk_toggled(bool)));
}

TGo4FitData* QFitDataWidget::GetData()
{
   return dynamic_cast<TGo4FitData*> (GetObject());
}

void QFitDataWidget::FillSpecificData()
{
  QFitNamedWidget::FillSpecificData();

  if(GetData()) {
     if(GetData()->GetDataType()==TGo4FitData::dtHistogram) DataInfoLbl->setText("Data type: histogramic");
                                                       else DataInfoLbl->setText("Data type: set of points");
     DataInfoLbl->adjustSize();

     AmplChk->setChecked(GetData()->GetAmplPar()!=0);
     AmplChk->setEnabled(GetData()->CanAmplTouch());

     NumCalibrSpin->setValue(GetData()->GetNumberOfTransSlots());
     UseBinsChk->setChecked(GetData()->GetUseBinScale());
     SigmaCmb->setCurrentIndex(GetData()->GetSigmaSource());
     SigmaEdt->setEnabled(GetData()->GetSigmaSource()==2);
     SigmaEdt->setText(QString::number(GetData()->GetSigmaValue()));
     BinsLimitEdt->setText(QString::number(GetData()->GetExcludeLessThen()));
     UseBuffersChk->setChecked(GetData()->GetUseBuffers());

     QString info("Models:");

     TGo4Fitter* fitter = dynamic_cast<TGo4Fitter*> (GetItem()->Parent()->Object());
     if (fitter)
      for(Int_t nmodel=0;nmodel<fitter->GetNumModel();nmodel++) {
         TGo4FitModel* model = fitter->GetModel(nmodel);
         if (model->IsAssignTo(GetData()->GetName())) {
            info+=" "; info+=model->GetName();
         }
      }
     ModelsLbl->setText(info);
     ModelsLbl->adjustSize();
   }
}

void QFitDataWidget::NumCalibrSpin_valueChanged( int num)
{
   if (!fbFillWidget && GetData())
      if (GetData()->SetNumberOfTransSlots(num)) {
         if (GetFitter())
            GetFitter()->SetUpdateSlotList();
         UpdateWidgetItem(true);
         UpdateItemsOfType(FitGui::ot_allslots, true);
      }
}

void QFitDataWidget::UseBinsChk_toggled( bool zn)
{
   if (!fbFillWidget && GetData())
      GetData()->SetUseBinScale(zn);
}

void QFitDataWidget::SigmaCmb_activated( int zn)
{
   if (!fbFillWidget && GetData()) {
      GetData()->SetSigmaSource(zn, -1.);
      SigmaEdt->setEnabled(zn == 2);
   }
}

void QFitDataWidget::SigmaEdt_textChanged( const QString & value)
{
  if(!fbFillWidget && GetData()) {
    bool ok = true;
    double zn = value.toDouble(&ok);
    if(ok) GetData()->SetSigmaValue(zn);
  }
}

void QFitDataWidget::BinsLimitEdt_textChanged( const QString & value)
{
   if (!fbFillWidget && GetData()) {
      bool ok = true;
      double zn = value.toDouble(&ok);
      if (ok)
         GetData()->SetExcludeLessThen(zn);
   }
}

void QFitDataWidget::AmplChk_toggled( bool chk)
{
   if (!fbFillWidget && GetData() && GetData()->CanAmplTouch()) {
      bool res = false;
      if (chk)
         res = GetData()->MakeAmpl();
      else
         res = GetData()->RemoveAmpl();
      if (res)
         UpdateItemsOfType(FitGui::ot_parslist, false);
   }
}

void QFitDataWidget::UseBuffersChk_toggled(bool zn)
{
   if (!fbFillWidget && GetData())
      GetData()->SetUseBuffers(zn);
}
