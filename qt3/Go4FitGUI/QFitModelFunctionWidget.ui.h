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

/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename slots use Qt Designer which will
** update this file, preserving your code. Create an init() slot in place of
** a constructor, and a destroy() slot in place of a destructor.
*****************************************************************************/

TGo4FitModelFunction * QFitModelFunctionWidget::GetFunction() {
   return dynamic_cast<TGo4FitModelFunction*> (GetObject());
}

void QFitModelFunctionWidget::FillSpecificData() {
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
     GetFunction()->SetLibraryName(name);
}

void QFitModelFunctionWidget::FuncNameEdt_textChanged( const QString & name)
{
   if(!fbFillWidget && GetFunction())
      GetFunction()->SetFunctionName(name);
}

void QFitModelFunctionWidget::NumParSpin_valueChanged( int num)
{
  if(!fbFillWidget && GetFunction())
     if(GetFunction()->SetNumberOfFuncPar(num))
       UpdateItemsOfType(FitGui::ot_parslist, false);
}
