#ifndef TGO4ANALYSISPROXY_H
#define TGO4ANALYSISPROXY_H

#include "TGo4Proxy.h"

#include "TObjArray.h"

class TGo4AnalysisObjectNames;
class TGo4AnalysisDummySlot;
class TFolder;
class TGo4AnalysisObjectAccess;
class TGo4Status;
class TGo4Display;

class TGo4AnalysisProxy : public TGo4Proxy {
   public:
      TGo4AnalysisProxy(Bool_t isserver = kFALSE);
      virtual ~TGo4AnalysisProxy();

      // proxy functionality

      virtual void Initialize(TGo4Slot* slot);
      virtual void Finalize(TGo4Slot* slot);

      virtual Bool_t HasSublevels() const;

      virtual TGo4LevelIter* MakeIter();

      virtual TGo4Access* MakeProxy(const char* name);

      virtual void WriteData(TGo4Slot* slot, TDirectory* dir, Bool_t onlyobjs);
      virtual void ReadData(TGo4Slot* slot, TDirectory* dir);

      virtual const char* GetContainedObjectInfo();
      virtual Int_t GetObjectKind() { return TGo4Access::kndFolder; }
      virtual const char* GetContainedClassName() { return ClassName(); }
      virtual TObject* GetAssignedObject() { return this; }

      virtual void Update(TGo4Slot* slot, Bool_t strong);

      TGo4Slot* ParentSlot() { return fxParentSlot; }
      TGo4Slot* SettingsSlot();
      TGo4Slot* RatemeterSlot();
      TGo4Slot* LoginfoSlot();
      TGo4Slot* UpdateObjectSlot();

      // communication with analysis functionality

      Bool_t IsAnalysisServer() const { return fIsServer; }
      Bool_t IsConnected();
      Int_t ConnectorPort();
      Int_t GetRole();
      Bool_t IsViewer();
      Bool_t IsController();
      Bool_t IsAdministrator();

      void SetAnalysisReady(Bool_t on = kTRUE) { fbAnalysisReady = on; }
      Bool_t IsAnalysisReady() const { return fbAnalysisReady; }

      void SetAnalysisSettingsReady(Bool_t on = kTRUE) { fbAnalysisSettingsReady = on ; }
      Bool_t IsAnalysisSettingsReady() const { return fbAnalysisSettingsReady; }

      Bool_t IsAnalysisRunning() const { return fbAnalysisRunning; }

      void RefreshNamesList();
      void DelayedRefreshNamesList(Int_t delay_sec);
      Bool_t NamesListReceived();

      void ReceiveObject(TNamed* obj);
      void ReceiveStatus(TGo4Status* status);
      Bool_t SubmitProxy(TGo4AnalysisObjectAccess* proxy);

      void LoadConfigFile(const char* fname);
      void SaveConfigFile(const char* fname);
      void WriteAutoSave(const char* fname,
                         Int_t complevel,
                         Bool_t overwrite);
      void StartAnalysis();
      void StopAnalysis();
      void ExecuteLine(const char* line);

      void RequestAnalysisSettings();
      void SubmitAnalysisSettings();

      void RequestObjectStatus(const char* objectname, TGo4Slot* tgtslot);
      void RequestEventStatus(const char* evname, Bool_t astree, TGo4Slot* tgtslot);
      void RemoteTreeDraw(const char* treename,
                          const char* varexp,
                          const char* cutcond,
                          const char* hname);
      void RemotePrintEvent(const char* evname,
                            Int_t evnumber,
                            Int_t subid,
                            Bool_t ishex,
                            Bool_t islong);

      Bool_t UpdateAnalysisObject(const char* fullpath, TObject* obj);

      void ClearAnalysisObject(const char* fullpath);
      void ChageObjectProtection(const char* fullpath, const char* flags);
      void RemoveObjectFromAnalysis(const char* fullpath, TClass* cl = 0);

      void PrintDynListEntry(const char* fullpath);

      static Int_t NumberOfWaitingProxyes() { return fNumberOfWaitingProxyes; }

      Bool_t LaunchAsClient(TString& launchcmd,
                            TString& killcmd,
                            Int_t shellkind, // 0 - exec, 1 - rsh, 2 - ssh
                            Int_t konsole,
                            const char* name,
                            const char* remotehost,
                            const char* remotedir,
                            const char* remoteexe);

      static Bool_t LaunchAsServer(TString& launchcmd,
                                   TString& killcmd,
                                   Int_t shellkind, // 0 - exec, 1 - rsh, 2 - ssh
                                   Int_t konsole,
                                   const char* name,
                                   const char* remotehost,
                                   const char* remotedir,
                                   const char* remoteexe);

      Bool_t ConnectToServer(const char* remotehost,
                             Int_t remoteport,
                             Int_t ascontroller, // 0 - observer, 1 - controller, 2 - administrator
                             const char* accesspass);

      Bool_t WaitForConnection(Int_t seconds);

      /**  Close connection to analysis and destroys proxy with
        *  correspondent slot. Wait waittime (in sec) for safe
        *  disconnection of the analysis.
        *  if servershutdown = kTRUE, shutdown command will be sent to analysis */
      void DisconnectAnalysis(Int_t waittime = 30, Bool_t servershutdown = kFALSE);

      void DisplayDeleted(TGo4Display* displ);

      void DisplayDisconnected(TGo4Display* displ);

      virtual Bool_t HandleTimer(TTimer* timer);

   protected:

      static Bool_t GetLaunchString(TString& launchcmd,
                                    TString& killcmd,
                                    Bool_t server,
                                    Int_t shellkind, // 0 - exec, 1 - rsh, 2 - ssh
                                    Int_t konsole,
                                    const char* name,
                                    const char* remotehost,
                                    const char* remotedir,
                                    const char* remoteexe,
                                    Int_t guiport);

      TGo4AnalysisObjectAccess* FindSubmittedProxy(const char* pathname, const char* objname);
      void DeleteSubmittedProxy(TGo4AnalysisObjectAccess* proxy);

      void AssignNewNamesList(TGo4AnalysisObjectNames* objnames);

      void CallSlotUpdate();

      Bool_t                    fIsServer;  //!
      TGo4AnalysisDummySlot*    fDummySlot;        //!
      TGo4AnalysisObjectNames*  fAnalysisNames;    //!
      TGo4Slot*                 fxParentSlot;      //!
      TObjArray                 fxSubmittedProxy;  //!
      Bool_t                    fbNamesListReceived; //!
      Bool_t                    fbAnalysisReady;     //!
      Bool_t                    fbAnalysisSettingsReady; //!
      Bool_t                    fbAnalysisRunning;    //!
      Int_t                     fDisconectCounter; //!
      TGo4Display*              fxDisplay;          //!
      TString                   fInfoStr;           //!
      Int_t                     fActualRole;        //!
      TTimer*                   fxRefreshTimer;     //!
      TTimer*                   fxConnectionTimer;     //!
      static Int_t              fNumberOfWaitingProxyes;  //!

   ClassDef(TGo4AnalysisProxy, 1);
};



#endif
