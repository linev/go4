// $Id: TXXXProc.h 996 2013-07-17 11:30:03Z linev $
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

#ifndef TDABCPROCESSOR_H
#define TDABCPROCESSOR_H

#include "TGo4EventProcessor.h"

#ifndef __CINT__
#include "mbs/api.h"
#endif

class TXXXProc : public TGo4EventProcessor {
   protected:

#ifndef __CINT__
   mbs::MonitorHandle  fHandle;   //!  hide from CINT
#endif

   public:
      TXXXProc() ;
      TXXXProc(const char* name);
      virtual ~TXXXProc() ;

      Bool_t BuildEvent(TGo4EventElement*); // event processing function

   private:
      TH1           *fCr1Ch[8];
      TH1           *fCr2Ch[8];
      TH2           *fCr1Ch1x2;
      TH1           *fHis1;
      TH1           *fHis1gate;
      TH1           *fHis2;
      TH1           *fHis2gate;
      TH1           *fHis3;
      TGo4WinCond   *fconHis1;
      TGo4WinCond   *fconHis2;
      TGo4PolyCond  *fPolyCon;
      TGo4Picture   *fcondSet;

   ClassDef(TXXXProc,1)
};

#endif //TDABCPROCESSOR_H
