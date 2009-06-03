//---------------------------------------------
// Go4 Tasca analysis
// Author: Hans G. Essel
//         H.Essel@gsi.de
// GSI, Experiment Electronics, Data Processing
//---------------------------------------------

#ifndef TascaEvent_H
#define TascaEvent_H
#include "TascaCheckEvent.h"


class TascaEvent : public TObject {
   public:
      TascaEvent();
      TascaEvent(const char* name);
      virtual ~TascaEvent();

      Int_t PrintEvent();
      Bool_t Copy(TascaCheckEvent *);

      Float_t ffSystemSec;
      Float_t ffSystemMysec;
      Float_t ffTimeStamp;
      // index of maximum hit, if we had more than one hit
      UInt_t fiStopXLhitI;
      UInt_t fiStopXHhitI;
      UInt_t fiStopYLhitI;
      UInt_t fiStopYHhitI;
      UInt_t fiBackHhitI;
      UInt_t fiBackLhitI;
      UInt_t fiVetoHhitI;
      UInt_t fiVetoLhitI;
      // value of maximum hit, if we had more than one hit
      Float_t ffStopXLhitV;
      Float_t ffStopXHhitV;
      Float_t ffStopYLhitV;
      Float_t ffStopYHhitV;
      Float_t ffBackHhitV;
      Float_t ffBackLhitV;
      Float_t ffVetoHhitV;
      Float_t ffVetoLhitV;

      Float_t ffGammaMysec[8];
      Float_t ffGammaKev[8];

      UInt_t fiEventNumber;
      Bool_t fisTof;
      Bool_t fisChopper;
      Bool_t fisMacro;
      Bool_t fisMicro;

   ClassDef(TascaEvent,1)
};

#endif
