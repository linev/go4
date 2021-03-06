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

#include "QFitMinuitWidget.h"

#include "TGo4FitMinuit.h"

QFitMinuitWidget::QFitMinuitWidget(QWidget *parent, const char* name)
         : QFitNamedWidget(parent, name)
{
   setupUi(this);
   QObject::connect(CommandsEdit, SIGNAL(textChanged()), this, SLOT(CommandsEdit_textChanged()));
}

TGo4FitMinuit * QFitMinuitWidget::GetMinuit()
{
    return dynamic_cast<TGo4FitMinuit*> (GetObject());
}

void QFitMinuitWidget::FillSpecificData()
{
  QFitNamedWidget::FillSpecificData();
  TGo4FitMinuit* minuit = GetMinuit();
  if (minuit) {
      QString cmds;
      char s[3]; s[0] = 13; s[1] = 10; s[2]=0;
      for(int i=0;i<minuit->GetNumCommands();i++) {
         cmds+=minuit->GetCommand(i);
         if (i<minuit->GetNumCommands()-1) cmds+=s;
      }
      CommandsEdit->setText(cmds);
   } else {
      CommandsEdit->setText("");
   }
}


void QFitMinuitWidget::CommandsEdit_textChanged()
{
   TGo4FitMinuit* minuit = GetMinuit();
   if (!minuit) return;

   minuit->ClearCommands();

   QTextCursor curs(CommandsEdit->document());
   while (!curs.atEnd()) {
     curs.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
     minuit->AddCommand(curs.selectedText().toLatin1().constData());
     curs.movePosition(QTextCursor::NextWord, QTextCursor::MoveAnchor);
   }
}
