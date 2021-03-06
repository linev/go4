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

#include "TGo4AnalysisConfiguration.h"

#include <QFileDialog>
#include <QCloseEvent>

#include "TGo4ConfigStep.h"
#include "TGo4AnalysisStatus.h"
#include "TGo4AnalysisStepStatus.h"
#include "TGo4ServerProxy.h"


TGo4AnalysisConfiguration::TGo4AnalysisConfiguration(QWidget *parent, const char* name) :
   QGo4Widget(parent, name)
{
   setupUi(this);

   QObject::connect(SubmitPushButton, SIGNAL(pressed()), this, SLOT(SubmitConfiguration()));
   QObject::connect(AnalysisSaveConf, SIGNAL(clicked()), this, SLOT(SaveConfiguration()));
   QObject::connect(LoadConfigFileName, SIGNAL(clicked()), this, SLOT(FileDialog_ConfFile()));
   QObject::connect(LoadAutoSaveFileName, SIGNAL(clicked()), this, SLOT(FileDialog_AutoSave()));
   QObject::connect(AutoSaveFileName, SIGNAL(textChanged(QString)), this, SLOT(LineEdit_AutoSaveFile()));
   QObject::connect(GetActiveConfigButton, SIGNAL(clicked()), this, SLOT(RequestAnalysisStatus()));
   QObject::connect(CompLevel, SIGNAL(valueChanged(int)), this, SLOT(SetCompressionLevel(int)));
   QObject::connect(AnalysisLoadConf, SIGNAL(clicked()), this, SLOT(LoadConfiguration()));
   QObject::connect(AutoSaveInterval, SIGNAL(valueChanged(int)), this, SLOT(SetAutoSaveInterval(int)));
   QObject::connect(AutoSaveOverwrite, SIGNAL(toggled(bool)), this, SLOT(SetAutoSaveOverwrite(bool)));
   QObject::connect(SaveNowB, SIGNAL(clicked()), this, SLOT(WriteAutoSave()));
   QObject::connect(AutoSaveEnable, SIGNAL(toggled(bool)), this, SLOT(EnableAutoSaveSlot(bool)));
   QObject::connect(SubmitAndStartButton, SIGNAL(clicked()), this, SLOT(SubmitAndStart()));
   QObject::connect(ClosePushButton, SIGNAL(clicked()), this, SLOT(CloseAnalysis()));

   fSourcePath = ".";
   fStorePath = ".";
   fConfigPath = ".";
   fAutoSavePath = ".";
   fbTypingMode = true;
   setCanDestroyWidget(false);
}


bool TGo4AnalysisConfiguration::IsAcceptDrag(const char* itemname, TClass* cl, int kind)
{
   return false;
//   if (cl==0) return false;
//   return cl->InheritsFrom(TGo4AnalysisStatus::Class());
}

void TGo4AnalysisConfiguration::DropItem(const char* itemname, TClass* cl, int kind)
{
//   if (cl==0) return;

//   if (cl->InheritsFrom(TGo4AnalysisStatus::Class()))
//      WorkWithStatus(itemname);
}

void TGo4AnalysisConfiguration::linkedObjectUpdated(const char* linkname, TObject* obj)
{
   if (strcmp(linkname,"Status")==0) {
     parentWidget()->raise();
     parentWidget()->show();
     if (parentWidget()->isMinimized()) parentWidget()->showNormal();
     RefreshWidget();

     TGo4ServerProxy* anal =
        dynamic_cast<TGo4ServerProxy*>(GetLinked("Analysis", 0));
     if (anal!=0)
        anal->RefreshNamesList();
   }
}

void TGo4AnalysisConfiguration::linkedObjectRemoved(const char* linkname)
{
   ShootCloseWidget(true);
}

void TGo4AnalysisConfiguration::WorkWithAnalysis(TGo4ServerProxy* anal)
{
   ResetWidget();

   if (anal==0) {
      ShootCloseWidget(true);
      return;
   }

   AddLink(anal->ParentSlot(), "Analysis");
   AddLink(anal->SettingsSlot(), "Status");

   RefreshWidget();
}

void TGo4AnalysisConfiguration::ResetWidget()
{
   QGo4Widget::ResetWidget();

   QWidget* w = 0;
   do {
     w = TabSteps->currentWidget();
     if (w!=0) {
        TabSteps->removeTab(TabSteps->indexOf(w));
        delete w;
     }
   } while (w!=0);
}

void TGo4AnalysisConfiguration::RefreshWidget()
{
   TGo4AnalysisStatus* status =
      dynamic_cast<TGo4AnalysisStatus*> (GetLinked("Status",0));
   TGo4ServerProxy* anal =
      dynamic_cast<TGo4ServerProxy*>(GetLinked("Analysis", 0));

   if ((status==0) || (anal==0)) return;

   SubmitPushButton->setEnabled(anal->CanSubmitObjects());
   SubmitAndStartButton->setEnabled(anal->CanSubmitObjects());

   fbTypingMode = false;

   QWidget* w = 0;
   do {
     w = TabSteps->currentWidget();
     if (w!=0) {
        TabSteps->removeTab(TabSteps->indexOf(w));
        delete w;
     }
   } while (w!=0);

   for(int i=0; i<status->GetNumberOfSteps(); i++) {
      TGo4AnalysisStepStatus* stepstatus = status->GetStepStatus(i);
      if (stepstatus==0) continue;
      QString StepName = stepstatus->GetName();
      TGo4ConfigStep* NewStep = new TGo4ConfigStep(TabSteps, StepName.toLatin1().constData());
      NewStep->SetStepStatus(this, stepstatus, status->GetNumberOfSteps() > 1 ? i : -1);
      TabSteps->insertTab(i, NewStep, NewStep->GetTabTitle());
    }

    TabSteps->setCurrentIndex(0);
    TabSteps->adjustSize();

    SetAutoSaveConfig(status->GetAutoFileName(),
                      status->GetAutoSaveInterval(),
                      status->GetAutoSaveCompression(),
                      status->IsAutoSaveOn(),
                      status->IsAutoSaveOverwrite());

    SetAnalysisConfigFile(status->GetConfigFileName());

    parentWidget()->showNormal();
    parentWidget()->adjustSize();

    fbTypingMode = true;
}

void TGo4AnalysisConfiguration::ChangeTabTitle(TGo4ConfigStep* step, int number)
{
   if (number>=0)
      TabSteps->setTabText(number, step->GetTabTitle());
}


QString TGo4AnalysisConfiguration::GetSourcePath()
{
   return fSourcePath;
}

QString TGo4AnalysisConfiguration::GetStorePath()
{
   return fStorePath;
}

void TGo4AnalysisConfiguration::SetSourcePath(const QString & v)
{
   fSourcePath = v;
}

void TGo4AnalysisConfiguration::SetStorePath(const QString & v)
{
   fStorePath = v;
}

void TGo4AnalysisConfiguration::RequestAnalysisStatus()
{
   TGo4ServerProxy* anal =
      dynamic_cast<TGo4ServerProxy*>(GetLinked("Analysis", 0));
   if (anal!=0)
      anal->RequestAnalysisSettings();
}

void TGo4AnalysisConfiguration::FileDialog_ConfFile()
{
    QFileDialog fd(this,
                   "Select a configuration file",
                   fConfigPath,
                   " Configuration file (*.root)");
    fd.setFileMode( QFileDialog::AnyFile);
    if (fd.exec() != QDialog::Accepted) return;
    QStringList flst = fd.selectedFiles();
    QString fileName = flst.isEmpty() ? QString() : flst[0];
    fConfigPath = fd.directory().path();
    if(!fileName.endsWith(".root")) fileName.append(".root");
    ConfigFileName->setText(fileName);
}

void TGo4AnalysisConfiguration::FileDialog_AutoSave()
{
    QFileDialog fd( this,
          "Select file name for autosaving",
          fAutoSavePath,
          "Auto Save File (*.root)");
    fd.setFileMode(QFileDialog::AnyFile);
    fd.setAcceptMode(QFileDialog::AcceptSave);
    if (fd.exec() != QDialog::Accepted)  return;

    QStringList flst = fd.selectedFiles();
    if (flst.isEmpty()) return;

    QString fileName = flst[0];
    fAutoSavePath = fd.directory().path();
    if(!fileName.endsWith(".root")) fileName.append(".root");
    AutoSaveFileName->setText(fileName);
    LineEdit_AutoSaveFile();
}

void TGo4AnalysisConfiguration::LineEdit_AutoSaveFile()
{
   QString fname = AutoSaveFileName->text().trimmed();
   TGo4AnalysisStatus* status =
     dynamic_cast<TGo4AnalysisStatus*> (GetLinked("Status",0));
   if ((status!=0) && fbTypingMode)
     status->SetAutoFileName(fname.toLatin1().constData());
}

void TGo4AnalysisConfiguration::SetCompressionLevel( int t)
{
   TGo4AnalysisStatus* status =
     dynamic_cast<TGo4AnalysisStatus*> (GetLinked("Status",0));
   if ((status!=0) && fbTypingMode)
     status->SetAutoSaveCompression(t);
}

void TGo4AnalysisConfiguration::LoadConfiguration()
{
   QString fname = ConfigFileName->text().trimmed();
   TGo4ServerProxy* anal =
      dynamic_cast<TGo4ServerProxy*>(GetLinked("Analysis", 0));
   if (anal!=0)
      anal->LoadConfigFile(fname.toLatin1().constData());
   RequestAnalysisStatus();
}

void TGo4AnalysisConfiguration::SaveConfiguration()
{
   QString fname = ConfigFileName->text().trimmed();
   TGo4ServerProxy* anal =
      dynamic_cast<TGo4ServerProxy*>(GetLinked("Analysis", 0));
   if (anal!=0)
      anal->SaveConfigFile(fname.toLatin1().constData());
}

void TGo4AnalysisConfiguration::SubmitConfiguration()
{
   ServiceCall("SubmitAnalysisSettings");
}

void TGo4AnalysisConfiguration::SubmitAndStart()
{
   ServiceCall("SubmitStartAnalysis");
   parentWidget()->hide();
}

void TGo4AnalysisConfiguration::CloseAnalysis()
{
   ServiceCall("CloseAnalysisSettings");
}

void TGo4AnalysisConfiguration::SetAutoSaveInterval(int t)
{
   TGo4AnalysisStatus* status =
     dynamic_cast<TGo4AnalysisStatus*> (GetLinked("Status",0));
   if ((status!=0) && fbTypingMode)
     status->SetAutoSaveInterval(t);
}

void TGo4AnalysisConfiguration::SetAutoSaveOverwrite(bool overwrite)
{
   TGo4AnalysisStatus* status =
      dynamic_cast<TGo4AnalysisStatus*> (GetLinked("Status",0));
   if ((status!=0) && fbTypingMode)
      status->SetAutoSaveOverwrite(overwrite);
}

void TGo4AnalysisConfiguration::WriteAutoSave()
{
   TGo4AnalysisStatus* status =
      dynamic_cast<TGo4AnalysisStatus*> (GetLinked("Status",0));
   TGo4ServerProxy* anal =
      dynamic_cast<TGo4ServerProxy*>(GetLinked("Analysis", 0));

   if ((anal!=0) && (status!=0))
     anal->WriteAutoSave(status->GetAutoFileName(),
                         status->GetAutoSaveCompression(),
                         status->IsAutoSaveOverwrite());
}

void TGo4AnalysisConfiguration::EnableAutoSaveSlot(bool enabled)
{
   TGo4AnalysisStatus* status =
     dynamic_cast<TGo4AnalysisStatus*> (GetLinked("Status",0));
   if ((status!=0) && fbTypingMode)
      status->SetAutoSaveOn(enabled);
    AutoSaveInterval->setEnabled(enabled);
}

int TGo4AnalysisConfiguration::GetNumSteps()
{
   return TabSteps->count();
}

TGo4ConfigStep* TGo4AnalysisConfiguration::GetStepConfig(int n)
{
   if ((n<0) || (n>=TabSteps->count())) return 0;
   return dynamic_cast<TGo4ConfigStep*> (TabSteps->widget(n));
}

TGo4ConfigStep* TGo4AnalysisConfiguration::FindStepConfig(QString name)
{
   for (int n=0;n<GetNumSteps();n++) {
      TGo4ConfigStep* conf = GetStepConfig(n);
      if (conf!=0)
        if (conf->GetStepName()==name) return conf;
   }
   return 0;
}

void TGo4AnalysisConfiguration::SetAutoSaveConfig(QString filename,
                                                  int interval,
                                                  int compression,
                                                  bool enabled,
                                                  bool overwrite)
{
  AutoSaveFileName->setText(filename);
  AutoSaveInterval->setValue(interval);
  CompLevel->setValue(compression);

  AutoSaveOverwrite->setChecked(overwrite);
  AutoSaveEnable->setChecked(enabled);

  AutoSaveInterval->setEnabled(enabled);
}

void TGo4AnalysisConfiguration::GetAutoSaveConfig(QString& filename,
                                                  int& interval,
                                                  int& compression,
                                                  bool& enabled,
                                                  bool& overwrite)
{
  filename = AutoSaveFileName->text();
  interval = AutoSaveInterval->value();
  compression = CompLevel->value();

  overwrite = AutoSaveOverwrite->isChecked();
  enabled = AutoSaveEnable->isChecked();
}

void TGo4AnalysisConfiguration::SetAnalysisConfigFile(QString filename)
{
   ConfigFileName->setText(filename);
}

void TGo4AnalysisConfiguration::GetAnalysisConfigFile(QString& filename)
{
   filename = ConfigFileName->text();
}

void TGo4AnalysisConfiguration::DisplayMbsMonitor(const QString& mbsname )
{
   ServiceCall("DisplayMbsMonitor", (void*) mbsname.toLatin1().constData());
}

void TGo4AnalysisConfiguration::closeEvent(QCloseEvent* e)
{
  e->ignore(); // destroying this would mix up the upper level management
  QWidget* mdi = parentWidget();
  if (mdi==0) return;
  mdi->hide(); // instead of destroying, we just hide it when X is clicked. JAM
}

