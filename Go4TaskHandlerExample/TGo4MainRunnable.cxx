#include "TGo4MainRunnable.h"

#include "TGo4Log.h"
#include "TGo4LockGuard.h"
#include "TGo4Thread.h"

#include "TGo4CommandInvoker.h"
#include "TGo4LogicException.h"

TGo4MainRunnable::TGo4MainRunnable(const char* name, TGo4ExampleClient* cli)
: TGo4Runnable(name,cli)
{
   TRACE((15,"TGo4MainRunnable::TGo4MainRunnable(Text_t*,TGo4ExampleClient*) constructor",__LINE__, __FILE__));
   fxApplication=cli->GetApplication();
}

TGo4MainRunnable::~TGo4MainRunnable()
{
   TRACE((15,"TGo4MainRunnable::~TGo4MainRunnable() destructor",__LINE__, __FILE__));
}

Int_t TGo4MainRunnable::Run(void*)
{
   TRACE((12,"TGo4MainRunnable::Run()",__LINE__, __FILE__));
   // first test: simply wait for command, then execute it and send back an object
   TGo4Command* com=0;
   TGo4ExampleClient* cli= dynamic_cast<TGo4ExampleClient*> (fxManager);
   if(!cli)
     throw TGo4LogicException();
   com=cli->NextCommand();
   if(com!=0)
      {
      TGo4LockGuard mainlock; // protect command invocation!
         TGo4CommandInvoker::Instance()->Invoke(com); // execute command by invoker
         delete com;
      }
   else
      {        }
      // do "main event loop"...
      fxApplication->FillHistograms();
      TGo4Thread::Sleep(20);
        return 0;
}
