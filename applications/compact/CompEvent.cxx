//---------------------------------------------
// Go4 Comp analysis
// Author: Hans G. Essel
//         H.Essel@gsi.de
// GSI, Experiment Electronics, Data Processing
//---------------------------------------------

#include "CompEvent.h"

#include "Riostream.h"

//***********************************************************
CompEvent::CompEvent() {}
//***********************************************************
CompEvent::CompEvent(const char* name)
{
    cout << "Comp> CompEvent: " << name << " created" << endl;
}
//***********************************************************
CompEvent::~CompEvent(){
    //cout << "Comp> CompEvent: " << GetName() << " deleted" << endl;
}
//***********************************************************

//-----------------------------------------------------------
void CompEvent::PrintFission(Bool_t full, UInt_t nof){
if(full)PrintEvent();else
printf(" --SF    %9d MevH: %6.2f  L: %6.2f SF#:%8d X %3d Y %3d Spill %d\n",
		fiEventNumber,
		ffStopXHhitV/1000.,
		ffStopXLhitV/1000.,
		nof,
		fiStopXHhitI,
		fiStopYHhitI,
		fisMacro);
}
void CompEvent::PrintAlpha(Bool_t full, Float_t timeD){
if(full)PrintEvent();else
printf(" --Alpha %9d MevL: %6.2f [ms] %10u toSF [s] %8.3f X %3d Y %3d Spill %d\n",
		fiEventNumber,
		ffStopXLhitV/1000.,
		fiSystemmsec,timeD,
		fiStopXLhitI,
		fiStopYLhitI,
		fisMacro);
}
void CompEvent::PrintEvr(Bool_t full, Float_t timeD){
if(full)PrintEvent();else
printf("Evr ---- %9d MevH: %6.2f     [ms] %10u toSF [s] %8.3f X %3d Y %3d Spill %d\n",
		fiEventNumber,
		ffStopXHhitV/1000.,
		fiSystemmsec,timeD,
		fiStopXHhitI,
		fiStopYHhitI,
		fisMacro);
}
void CompEvent::PrintEvent(Bool_t full){
	if(full)
	{
		printf("Event Chain %4d Run %3d File %4d Evt %9d Tof:%d Off:%d EVR:%d Al:%d SF:%d\n",
				fiChainNumber,fiFileNumber>>16,fiFileNumber&0xffff,fiEventNumber,
				fisTof,!fisMacro,fisEvr,fisAlpha,fisFission);
		printf("  [ms] %9u [mysec] Sys %9u Adc %9u, d [msec] Sys %6.3f Adc %6.3f\n",
				fiSystemmsec,fiSystemMysec,fiTimeStamp,
				(Float_t)fiDeltaSystemTime,
				(Float_t)fiDeltaTime/1000);
		printf("  StopXL(H) i %3d (%3d), [MeV] %7.3f (%7.3f)",
				fiStopXLhitI,fiStopXHhitI,ffStopXLhitV/1000.,ffStopXHhitV/1000.);
		printf("  StopYL(H) i %3d (%3d), [MeV] %7.3f (%7.3f)\n",
				fiStopYLhitI,fiStopYHhitI,ffStopYLhitV/1000.,ffStopYHhitV/1000.);
	}
	else {
		if(fisAlpha)PrintAlpha(kFALSE,0);
		if(fisFission)PrintFission(kFALSE,0);
		if(fisEvr)PrintEvr(kFALSE,0);
	}
  return;
}
