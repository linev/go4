#include "TGo4RevServParameter.h"

#include "Riostream.h"

#include "TGo4Status.h"
#include "TGo4Log.h"
#include "Go4EventServerTypes.h"


TGo4RevServParameter::TGo4RevServParameter(const char* name)
: TGo4EventSourceParameter(name, GO4EV_MBS_REVSERV), fiPort(0)
{
TRACE((14,"TGo4MbsRevServParameter::TGo4MbsRevServParameter(Text_t*,...)", __LINE__, __FILE__));
}

TGo4RevServParameter::TGo4RevServParameter()
: TGo4EventSourceParameter("default mbs remote event server", GO4EV_MBS_REVSERV), fiPort(0)
{
TRACE((14,"TGo4MbsRevServParameter::TGo4MbsRevServParameter()", __LINE__, __FILE__));
}

TGo4RevServParameter::~TGo4RevServParameter()
{
TRACE((14,"TGo4MbsRevServParameter::~TGo4MbsRevServParameter()", __LINE__, __FILE__));
}


Int_t TGo4RevServParameter::PrintParameter(Text_t* buffer, Int_t buflen)
{
 TRACE((12,"TGo4RevServParameter::PrintParameter()",__LINE__, __FILE__));
   Int_t locallen=128000;
   Text_t localbuf[128000];
   if(buflen<0 && buffer!=0)
      return 0;
   Int_t size=0;
   Text_t* current=localbuf;
   Int_t restlen=locallen;
   Int_t delta=TGo4EventSourceParameter::PrintParameter(current,restlen);
   restlen-=delta;
   current+=delta;
   current=TGo4Status::PrintIndent(current,restlen);
   current=TGo4Status::PrintBuffer(current,restlen, "  Revserv port: \t%d \n",GetPort());
   if(buffer==0)
      {
          cout << localbuf << endl;
      }
   else
      {
         size=locallen-restlen;
         if(size>buflen-1)
            size=buflen-1;
         strncpy(buffer,localbuf,size);
      }
   return size;
}
