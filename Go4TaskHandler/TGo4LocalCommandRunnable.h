#ifndef TGO4LOCALCOMMANDRUNNABLE_H
#define TGO4LOCALCOMMANDRUNNABLE_H

#include "TGo4Runnable.h"

class TGo4Task;

/**
 * Runnable which executes all local commands on the server side to
 * prevent gui from blocking. Will listen to a server side command queue
 * and invokes command by server command invoker.
 * Method SubmitCommand will either send all Callback commands
 * to current client (remote command), or put it into the local queue.
 * Note that local commands would also be possible on the client side now,
 * since local command queue and this thread is owned by TGo4Task baseclass
 * @stereotype thread
 */

class TGo4LocalCommandRunnable : public TGo4Runnable {

   public:

      TGo4LocalCommandRunnable(const char* name, TGo4Task* task);

      virtual ~TGo4LocalCommandRunnable();

      virtual Int_t Run(void* ptr);

   private:
      TGo4LocalCommandRunnable();

      /**
       * @supplierCardinality 1
       * @directed
       */
      TGo4Task* fxLocalTask; //!
};

#endif //TGO4LOCALCOMMANDRUNNABLE_H
