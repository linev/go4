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

#include "QFitModelFunctionWidget.h"

#include "TGo4FitModelFunction.h"
#include "TGo4FitGuiTypes.h"


QFitModelFunctionWidget::QFitModelFunctionWidget(QWidget *parent, const char* name)
         : QFitModelWidget(parent, name)
{
   setupUi(this);

   QObject::connect(LibNameEdt, SIGNAL(textChanged(QString)), this, SLOT(LibNameEdt_textChanged(QString)));
   QObject::connect(FuncNameEdt, SIGNAL(textChanged(QString)), this, SLOT(FuncNameEdt_textChanged(QString)));
   QObject::connect(NumParSpin, SIGNAL(valueChanged(int)), this, SLOT(NumParSpin_valueChanged(int)));
}

TGo4FitModelFunction * QFitModelFunctionWidget::GetFunction()
{
   return dynamic_cast<TGo4FitModelFunction*> (GetObject());
}

void QFitModelFunctionWidget::FillSpecificData()
{
    QFitNamedWidget::FillSpecificData();
    if (GetFunction()) {
       LibNameEdt->setText(GetFunction()->GetLibraryName());
       FuncNameEdt->setText(GetFunction()->GetFunctionName());
       NumParSpin->setValue(GetFunction()->GetNumberOfFuncPar());
    }
}

void QFitModelFunctionWidget::LibNameEdt_textChanged( const QString & name)
{
   if(!fbFillWidget && GetFunction())
     GetFunction()->SetLibraryName(name.toLatin1().constData());
}

void QFitModelFunctionWidget::FuncNameEdt_textChanged( const QString & name)
{
   if(!fbFillWidget && GetFunction())
      GetFunction()->SetFunctionName(name.toLatin1().constData());
}

void QFitModelFunctionWidget::NumParSpin_valueChanged( int num)
{
  if(!fbFillWidget && GetFunction())
     if(GetFunction()->SetNumberOfFuncPar(num))
       UpdateItemsOfType(FitGui::ot_parslist, false);
}
