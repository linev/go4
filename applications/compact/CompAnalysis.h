//---------------------------------------------
// Go4 Comp analysis
// Author: Hans G. Essel
//         H.Essel@gsi.de
// GSI, Experiment Electronics, Data Processing
//---------------------------------------------

#ifndef CompANALYSIS_H
#define CompANALYSIS_H

#include "TGo4Analysis.h"
#include "TGo4Picture.h"

class TFile;
class TH1;
class TCutG;
class CompParameter;
class TGo4Picture;
class TGo4Condition;
class TGo4WinCond;
class TGo4PolyCond;
class TGo4CondArray;

class CompAnalysis : public TGo4Analysis  {
   public:
            CompAnalysis();
           ~CompAnalysis() ;
      Int_t UserPreLoop();
      Int_t UserEventFunc();
      Int_t UserPostLoop();
      TNamed* CreateObject(const Text_t* type, const Text_t* name, Int_t par1=0);
      TGo4Parameter* CreateParameter(const Text_t* type, const Text_t* name);
      void SetPicture(TGo4Picture *p, TH1 *h, UInt_t ir, UInt_t ic, UInt_t mode);
      TGo4Picture* CreatePicture(const Text_t* folder, const Text_t* name, const Text_t* title, UInt_t ir, UInt_t ic);
      TGo4Condition* CreateCondition(const Text_t* folder, const Text_t* name, Int_t dim, Bool_t reset, Double_t low, Double_t high);
      TGo4Condition* CreateCondition(const Text_t* folder, const Text_t* name, Int_t dim, Bool_t reset, TCutG* poly);
      TH1I* CreateTH1I(const Text_t* folder, const Text_t* name,
    		  const Text_t* title,
    		  UInt_t channels, Double_t low, Double_t high);
      TH1D* CreateTH1D(const Text_t* folder, const Text_t* name,
    		  const Text_t* title,
    		  UInt_t channels, Double_t low, Double_t high);
      TH2I* CreateTH2I(const Text_t* folder, const Text_t* name,
    		  const Text_t* title,
    		  UInt_t channels1, Double_t low1, Double_t high1, UInt_t channels2, Double_t low2, Double_t high2);
      TH2D* CreateTH2D(const Text_t* folder, const Text_t* name,
    		  const Text_t* title,
    		  UInt_t channels1, Double_t low1, Double_t high1, UInt_t channels2, Double_t low2, Double_t high2);

      TH1I* CreateTH1I(const Text_t* folder, const Text_t* name,
    		  const Text_t* title, const Text_t* xlett, const Text_t* ylett,
    		  UInt_t channels, Double_t low, Double_t high);
      TH1D* CreateTH1D(const Text_t* folder, const Text_t* name,
    		  const Text_t* title,  const Text_t* xlett, const Text_t* ylett,
    		  UInt_t channels, Double_t low, Double_t high);
      TH2I* CreateTH2I(const Text_t* folder, const Text_t* name,
    		  const Text_t* title,  const Text_t* xlett, const Text_t* ylett, const Text_t* zlett,
    		  UInt_t channels1, Double_t low1, Double_t high1, UInt_t channels2, Double_t low2, Double_t high2);
      TH2D* CreateTH2D(const Text_t* folder, const Text_t* name,  const Text_t* zlett,
    		  const Text_t* title,  const Text_t* xlett, const Text_t* ylett,
    		  UInt_t channels1, Double_t low1, Double_t high1, UInt_t channels2, Double_t low2, Double_t high2);
   private:
	      CompParameter      *fParam;
	      UInt_t events;

   ClassDef(CompAnalysis,1)
};

#endif //CompANALYSIS_H


