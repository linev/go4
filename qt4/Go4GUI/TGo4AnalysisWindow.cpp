#include "TGo4AnalysisWindow.h"

#include "TSystem.h"
#include "Riostream.h"

#include "qimage.h"
#include "qlabel.h"
#include "qpushbutton.h"

#include <QtCore/QTimer>
#include <QtCore/QTextStream>
#include <QtCore/QProcess>

#include <QtGui/QHBoxLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QTextEdit>
#include <QtGui/QToolTip>
#include <QtGui/QMessageBox>
#include <QtGui/QFileDialog>
#include <QtGui/QToolButton>

#include "TGo4QSettings.h"
#include "TGo4AnalysisProxy.h"
#include "TGo4BrowserProxy.h"
#include "TGo4AnalysisObjectResult.h"
#include "QGo4CommandsHistory.h"



TGo4AnalysisWindow::TGo4AnalysisWindow(QWidget* parent, const char* name, bool needoutput, bool needkillbtn)
    : QGo4Widget( parent, name)
{

    setCanDestroyWidget(false);
    setAcceptDrops(FALSE);

    fAnalysisProcess = 0;
    fxOutput = 0;
    outputBuffer = "";
    fiMaxOuputSize = 0;
    fxCmdHist = 0;

    fNewObjectForEditor = true;

    setWindowTitle("Analysis Terminal");

    if (needoutput) {

       resize(700, 400);
       setWindowIcon(QIcon(":/icons/analysiswin.png"));
       QGridLayout* layout = new QGridLayout( this );
       layout->setMargin(11);
       layout->setSpacing(6);

       fxOutput = new QTextEdit(this);
       fxOutput->setUndoRedoEnabled(FALSE);
       fxOutput->setAutoFormatting(QTextEdit::AutoNone);
       fxOutput->setWordWrapMode(QTextOption::NoWrap);
       fxOutput->setReadOnly(TRUE);
       layout->addWidget( fxOutput, 0, 0 );

       fiMaxOuputSize = go4sett->getTermHistorySize();

       QHBoxLayout *box1 = new QHBoxLayout(this);
       box1->addWidget(new QLabel("Press enter to execute.", this), 1);
       CreateCmdLine(box1);
       layout->addLayout(box1, 1, 0);

       QHBoxLayout *box2 = new QHBoxLayout(this);
       CreateButtons(box2, needkillbtn);
       layout->addLayout(box2, 2, 0);

       updateTerminalOutput();
    } else {

       QHBoxLayout *box = new QHBoxLayout(this);

       CreateButtons(box, needkillbtn);

       CreateCmdLine(box);

       adjustSize();
    }
}

void TGo4AnalysisWindow::CreateCmdLine(QHBoxLayout* box)
{
   fxCmdHist = new QGo4CommandsHistory(this, "commandslist");
   fxCmdHist->setToolTip("CINT command for analysis process. Note: '@' means 'TGo4Analysis::Instance()->' .");
   connect(fxCmdHist, SIGNAL(activated(const QString&)),  this, SLOT(HistActivated(const QString&)));
   fxCmdHist->setMinimumSize( QSize( 220, 25 ) );

   box->addWidget(fxCmdHist, HasOutput() ? 3 : 1);

   QToolButton* MacroSearch = new QToolButton( this );
   MacroSearch->setMinimumSize( QSize( 30, 25 ) );
   MacroSearch->setMaximumSize( QSize( 30, 25 ) );
   MacroSearch->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
   MacroSearch->setIcon( QIcon(":/icons/findfile.png" ) );
   MacroSearch->setToolTip("Search root macro on disk.");
   connect(MacroSearch, SIGNAL(clicked()), this, SLOT(FileDialog_Macro()));
   box->addWidget(MacroSearch,1);
}

void TGo4AnalysisWindow::CreateButtons(QHBoxLayout* box, bool needkillbtn)
{
   if (needkillbtn) {
      QToolButton* KillProcess = new QToolButton( this );
      KillProcess->setMinimumSize( QSize( 30, 25 ) );
      KillProcess->setMaximumSize( QSize( 30, 25 ) );
      KillProcess->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
      KillProcess->setIcon( QIcon( ":/icons/killanal.png" ) );
      KillProcess->setToolTip("Apply Ctrl+C in the analysis terminal.");
      connect(KillProcess, SIGNAL(clicked()), this, SLOT(RequestTerminate()));
      box->addWidget(KillProcess);
   }

   if (HasOutput()) {
      QToolButton* ClearButton = new QToolButton( this );
      ClearButton->setMinimumSize( QSize( 30, 25 ) );
      ClearButton->setMaximumSize( QSize( 30, 25 ) );
      ClearButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
      ClearButton->setIcon( QIcon( ":/icons/clear.png" ) );
      ClearButton->setToolTip("Clear Terminal Window.");
      connect(ClearButton, SIGNAL(clicked()), this, SLOT(ClearAnalysisOutput()));
      box->addItem(new QSpacerItem(1,1));
      box->addWidget(ClearButton,1);
   }

   QToolButton* PrintHistoButton = new QToolButton( this );
   PrintHistoButton->setMinimumSize( QSize( 30, 25 ) );
   PrintHistoButton->setMaximumSize( QSize( 30, 25 ) );
   PrintHistoButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
   PrintHistoButton->setIcon( QIcon( ":/icons/hislist.png" ) );
   PrintHistoButton->setToolTip("Print list of all histograms.");
   connect(PrintHistoButton, SIGNAL(clicked()), this, SLOT(PrintHistograms()));
   box->addWidget(PrintHistoButton,1);

   QToolButton* PrintConnyButton = new QToolButton( this );
   PrintConnyButton->setMinimumSize( QSize( 30, 25 ) );
   PrintConnyButton->setMaximumSize( QSize( 30, 25 ) );
   PrintConnyButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
   PrintConnyButton->setIcon( QIcon( ":/icons/condlist.png" ) );
   PrintConnyButton->setToolTip("Print list of all conditions.");
   connect(PrintConnyButton, SIGNAL(clicked()), this, SLOT(PrintConditions()));
   box->addWidget(PrintConnyButton,1);

   QToolButton*  PrintEventButton = new QToolButton( this );
   PrintEventButton->setMinimumSize( QSize( 30, 25 ) );
   PrintEventButton->setMaximumSize( QSize( 30, 25 ) );
   PrintEventButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
   PrintEventButton->setIcon( QIcon( ":/icons/zoom.png" ) );
   PrintEventButton->setToolTip("Start Event Inspection panel");
   connect(PrintEventButton, SIGNAL(clicked()), this, SLOT(PrintEvent()));
   box->addWidget(PrintEventButton,1);
}

TGo4AnalysisWindow::~TGo4AnalysisWindow()
{
//   TerminateAnalysisProcess();
   if (Browser()!=0)
      Browser()->ToggleMonitoring(0);
}

bool TGo4AnalysisWindow::HasOutput()
{
   return fxOutput!=0;
}


void TGo4AnalysisWindow::SetHistorySize(int sz)
{
   fiMaxOuputSize = sz;
}

void TGo4AnalysisWindow::updateTerminalOutput()
{
    if (fxOutput==0) return;

    unsigned int buflen = outputBuffer.length();

    if (fiMaxOuputSize>0) {

      // size remaining after cut of text
      unsigned int cutlength = fiMaxOuputSize / 2;

      if (buflen > 0) {
         unsigned int outlen = fxOutput->toPlainText().length();
         if (buflen + outlen < fiMaxOuputSize)
           fxOutput->append(outputBuffer);
         else
         if (buflen>=cutlength) {
            outputBuffer.remove(0, buflen-cutlength);
            fxOutput->setText(outputBuffer);
            fxOutput->moveCursor(QTextCursor::End);
         } else {
           QString curr = fxOutput->toPlainText();
           curr.remove(0, cutlength - buflen);
           curr+=outputBuffer;
           fxOutput->setText(curr);
           fxOutput->moveCursor(QTextCursor::End);
         }
      }
    } else
      if (buflen>0)
         fxOutput->append(outputBuffer);

    outputBuffer = "";
    QTimer::singleShot(100, this, SLOT(updateTerminalOutput()));
}

void TGo4AnalysisWindow::readFromStdout()
{
   if (fAnalysisProcess!=0) {
      QByteArray ba = fAnalysisProcess->readAllStandardOutput();
      QString buf(ba);
      AppendOutputBuffer(buf);
   }
}


void TGo4AnalysisWindow::readFromStderr()
{
    if (fAnalysisProcess!=0) {
       QByteArray ba = fAnalysisProcess->readAllStandardError();
       QString buf(ba);
       AppendOutputBuffer(buf);
    }
}

void TGo4AnalysisWindow::AppendOutputBuffer(const QString& value)
{
    outputBuffer.append(value);
}

void TGo4AnalysisWindow::StartAnalysisShell(const char* text)
{
    if (fAnalysisProcess!=0) delete fAnalysisProcess;

    fAnalysisProcess = new QProcess();
    connect(fAnalysisProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readFromStdout()));
    connect(fAnalysisProcess, SIGNAL(readyReadStandardError()), this, SLOT(readFromStderr()));

    fAnalysisProcess->start(text);

    if (fAnalysisProcess->state() == QProcess::NotRunning) {
       cerr << "Fatal error. Could not start the Analysis" << endl;
       TerminateAnalysisProcess();
    }
}

void TGo4AnalysisWindow::RequestTerminate()
{
   ServiceCall("TerminateAnalysis");
}

void TGo4AnalysisWindow::TerminateAnalysisProcess()
{
   if (fAnalysisProcess==0) return;
   AppendOutputBuffer("\nTerminate process ... \n\n");
   fAnalysisProcess->terminate();
   if (fAnalysisProcess->state() == QProcess::Running) fAnalysisProcess->kill();
   delete fAnalysisProcess;
   fAnalysisProcess = 0;
}

void TGo4AnalysisWindow::ClearAnalysisOutput()
{
   if (fxOutput!=0)
      fxOutput->clear();
}

void TGo4AnalysisWindow::SaveAnalysisOutput()
{
   if (fxOutput==0) return;
   QFileDialog fd(this,
                  "Save analysis terminal output",
                  "", "Plain text (*.txt)");
   fd.setFileMode( QFileDialog::AnyFile );
   fd.setAcceptMode(QFileDialog::AcceptSave);

   if (fd.exec() != QDialog::Accepted) return;

   QStringList flst = fd.selectedFiles();
   if (flst.isEmpty()) return;
   QString fileName = flst[0];

   if(!fileName.endsWith(".txt")) fileName.append(".txt");
   QFile NewFile(fileName);
   NewFile.open( QIODevice::ReadWrite | QIODevice::Append );
   QTextStream t( &NewFile );
   t << fxOutput->toPlainText() << endl;
   NewFile.close();
}

void TGo4AnalysisWindow::HistActivated(const QString& str)
{
   if (fxCmdHist->EnterPressed() && (str!="")) {
      fxCmdHist->ResetEnterPressed();
      int pos = -1;
      for (int i=0;i<fxCmdHist->count();i++)
        if (fxCmdHist->itemText(i)=="") pos = i;

      if (pos>0) fxCmdHist->removeItem(pos);
      if (pos!=0) fxCmdHist->insertItem(0, "");

      if (fxCmdHist->currentIndex()!=0)
        fxCmdHist->setCurrentIndex(0);

      TGo4AnalysisProxy* anal = GetAnalysis();
      if (anal!=0)
        anal->ExecuteLine(str.toAscii());
   }
}

void TGo4AnalysisWindow::FileDialog_Macro()
{
   QFileDialog fd( this,
                  "Select ROOT macro for analysis task"
                  "", "CINT Macro  (*.C)");
   fd.setFileMode( QFileDialog::ExistingFile);

   if (fd.exec() != QDialog::Accepted) return;

   QStringList flst = fd.selectedFiles();
   if (flst.isEmpty()) return;

   QString cmd = QString(".x ") + flst[0];
   if(!cmd.endsWith(".C")) cmd.append(".C");
   fxCmdHist->addItem(cmd);
}

void TGo4AnalysisWindow::PrintHistograms()
{
   const QString com="@PrintHistograms()";
   fxCmdHist->addItem(com);
   fxCmdHist->SetEnterPressed(1);
   HistActivated(com);
   fxCmdHist->SetEnterPressed(0);
}

void TGo4AnalysisWindow::PrintConditions()
{
   const QString com="@PrintConditions()";
   fxCmdHist->addItem(com);
   fxCmdHist->SetEnterPressed(1);
   HistActivated(com);
   fxCmdHist->SetEnterPressed(0);
}

void TGo4AnalysisWindow::PrintEvent()
{
   ServiceCall("StartEventInfo");
}

void TGo4AnalysisWindow::WorkWithUpdateObjectCmd(TGo4Slot* slot)
{
   // can be called only once when window is created
   AddLink(slot, "ObjectUpdateCmd");
}

void TGo4AnalysisWindow::WaitForNewObject(bool isobjectforeditor)
{
   fNewObjectForEditor = isobjectforeditor;
}

void TGo4AnalysisWindow::linkedObjectUpdated(const char* linkname, TObject* obj)
{
   if (strcmp(linkname, "ObjectUpdateCmd")!=0) return;
   TGo4AnalysisObjectResult* res = dynamic_cast<TGo4AnalysisObjectResult*>(obj);
   if (res==0) return;
   Browser()->SyncBrowserSlots();
   const char* itemname = res->GetObjectFullName();
   TClass* cl = Browser()->ItemClass(itemname);
   if (cl!=0) InformThatObjectCreated(itemname, cl);
   if (!fNewObjectForEditor) EditItem(itemname);
   fNewObjectForEditor = true;
}

void TGo4AnalysisWindow::linkedObjectRemoved(const char* linkname)
{
   if (!HasOutput())
      ServiceCall("CloseAnalysisWindow");
}
