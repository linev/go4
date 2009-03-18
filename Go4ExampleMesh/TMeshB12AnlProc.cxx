#include "TMeshB12AnlProc.h"

#include "Riostream.h"

#include "TGo4UserException.h"

#include "TMeshB12OutputEvent.h"
#include "TMeshB1OutputEvent.h"
#include "TMeshB2OutputEvent.h"

//***********************************************************
// this one is used in TXXXAnlFact
TMeshB12AnlProc::TMeshB12AnlProc(const char* name)
  :TGo4EventProcessor(name),fxInput1(0),fxInput2(0)
{
  cout << "**** TMeshB12AnlProc: Create" << endl;
}
//***********************************************************
TMeshB12AnlProc::TMeshB12AnlProc()
  : TGo4EventProcessor("Processor12"),fxInput1(0),fxInput2(0)
{
}
//***********************************************************
TMeshB12AnlProc::~TMeshB12AnlProc()
{
}
//***********************************************************

//-----------------------------------------------------------
void TMeshB12AnlProc::Analysis(TMeshB12OutputEvent* poutevt)
{
 if(fxInput1==0)
      {
         // lazy init for input event from framework
         TGo4EventElement* providerinput=GetInputEvent("Output1Provider");
         fxInput1=dynamic_cast<TMeshB1OutputEvent*>(providerinput);
      }
 if(fxInput2==0)
      {
         // lazy init for input event from framework
         TGo4EventElement* providerinput=GetInputEvent("Output2Provider");
         fxInput2=dynamic_cast<TMeshB2OutputEvent*>(providerinput);
      }


   if(fxInput1 && fxInput2)
      {
         // do the processing here:
               for(Int_t i=0;i<4;i++)
               {
                  Int_t j=i+1;
                  if(j>3) j=0;
                  Float_t val=0;
                  if(fxInput2->frData[j])
                     val=TMath::Abs(fxInput1->frData[i]/fxInput2->frData[j]);
                  //cout <<"val["<<i<<"]="<<val << endl;
                  poutevt->frData[i]=val;
               }
      }
   else
      {
         throw TGo4UserException(3,"Error: not all input events available for processor %s",GetName());
      }






} // BuildCalEvent
