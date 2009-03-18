#include "TMeshFinalEvent.h"

#include "Riostream.h"

#include "TGo4FileSource.h"
#include "TMeshFinalProc.h"

//***********************************************************
TMeshFinalEvent::TMeshFinalEvent()
  :TGo4EventElement("FinalEvent"),fxFinalProc(0),fxFile(0)
{

}
//***********************************************************
TMeshFinalEvent::TMeshFinalEvent(const char* name)
  :TGo4EventElement(name),fxFinalProc(0),fxFile(0)
{
}
//***********************************************************
TMeshFinalEvent::~TMeshFinalEvent()
{
}
//***********************************************************


//-----------------------------------------------------------
Int_t TMeshFinalEvent::Init()
{
   // check for different source types
  Int_t rev = 0;
   Clear();
  if(CheckEventSource("TMeshFinalProc"))
   {
      fxFinalProc = dynamic_cast<TMeshFinalProc*>(GetEventSource());
      cout << "**** TMeshFinalEvent init for Final step"<< endl;
   }
  else if(CheckEventSource("TGo4FileSource"))
   {
      fxFile = dynamic_cast<TGo4FileSource*> (GetEventSource());
      cout << "**** TMeshFinalEvent init for file input"<< endl;
   }
  else
    rev=1;
  return rev;

}
//-----------------------------------------------------------
Int_t TMeshFinalEvent::Fill()
{
   // check for different source types
   Int_t rev = 0;
   Clear();
   if(fxFinalProc)
      fxFinalProc->Analysis(this);
   else if(fxFile)
      fxFile->BuildEvent(this);
   else
      rev=1;
   return rev;

}
//-----------------------------------------------------------
void TMeshFinalEvent::Clear(Option_t *t)
{
//-----------------------------------------------------------
fxB1.Clear(t);
fxB1.SetValid(kFALSE);
fxB2.Clear(t);
fxB1.SetValid(kFALSE);
fxB3.Clear(t);
fxB1.SetValid(kFALSE);
fxB12.Clear(t);
fxB1.SetValid(kFALSE);
}
