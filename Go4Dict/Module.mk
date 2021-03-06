# This module to produce all kinds of dictionaries, used in Go4
# It is in separate directory while files from different subfolders are used

GO4DICT_DIR         = Go4Dict


GO4BASE_LINKDEF     = $(GO4DICT_DIR)/Go4BaseLinkDef.h
GO4BASE_DICT        = $(GO4DICT_DIR)/$(DICT_PREFIX)Go4Base
GO4BASE_DH          = $(GO4BASE_DICT).$(HedSuf)
GO4BASE_DS          = $(GO4BASE_DICT).$(SrcSuf)
GO4BASE_DO          = $(GO4BASE_DICT).$(ObjSuf)
GO4BASE_DDEP       =  $(GO4BASE_DICT).$(DepSuf)

GO4BASE_H = $(LOCKGRD_H)  \
            $(GO4LOG_H)   \
            $(EXCEPT_H)   \
            $(COMBASE_H)  \
            $(STATBASE_H)

GO4BASE_O = $(LOCKGRD_O)  \
            $(GO4LOG_O)   \
            $(EXCEPT_O)   \
            $(COMBASE_O)  \
            $(STATBASE_O) \
            $(GO4BASE_DO)

#  task handle dictionary 

GO4TSKH_LINKDEF     = $(GO4DICT_DIR)/Go4TaskHandlerLinkDef.h
GO4TSKH_DICT        = $(GO4DICT_DIR)/$(DICT_PREFIX)Go4TaskHandler
GO4TSKH_DH          = $(GO4TSKH_DICT).$(HedSuf)
GO4TSKH_DS          = $(GO4TSKH_DICT).$(SrcSuf)
GO4TSKH_DO          = $(GO4TSKH_DICT).$(ObjSuf)
GO4TSKH_DDEP       =  $(GO4TSKH_DICT).$(DepSuf)

GO4TSKH_H = $(GO4SOCKET_H)   \
            $(GO4QUEUE_H)    \
            $(TASKHAND_H)    \
            $(CMDTASKHANDL_H)

GO4TSKH_O = $(GO4SOCKET_O)   \
            $(GO4QUEUE_O)    \
            $(TASKHAND_O)    \
            $(CMDTASKHANDL_O) \
            $(GO4TSKH_DO)

# base analysis library (used also in GUI)

GO4ANBASE_LINKDEF     = $(GO4DICT_DIR)/Go4AnalysisBaseLinkDef.h
GO4ANBASE_DICT        = $(GO4DICT_DIR)/$(DICT_PREFIX)Go4AnalysisBase
GO4ANBASE_DH          = $(GO4ANBASE_DICT).$(HedSuf)
GO4ANBASE_DS          = $(GO4ANBASE_DICT).$(SrcSuf)
GO4ANBASE_DO          = $(GO4ANBASE_DICT).$(ObjSuf)
GO4ANBASE_DDEP       =  $(GO4ANBASE_DICT).$(DepSuf)


GO4ANBASE_H+= $(GO4EVENTPAR_H)  \
              $(EVENTSERVPAR_H) \
              $(CONDBASE_H)     \
              $(DYNLIST_H)      \
              $(STATANAL_H)     \
              $(VERSION_H)

# exclude $(MBSAPIBASE_H) while CINT is not working              
              

GO4ANBASE_O+= $(MBSAPIBASE_O)  \
              $(GO4EVENTPAR_O) \
              $(EVENTSERVPAR_O) \
              $(CONDBASE_O) \
              $(DYNLIST_O) \
              $(STATANAL_O) \
              $(VERSION_O) \
              $(GO4ANBASE_DO)

ifdef USEHDF5
GO4ANBASE_H += $(HDFPAR_H)
GO4ANBASE_O += $(HDFPAR_O)
endif

# analysis library (not used in GUI)

GO4AN_LINKDEF     = $(GO4DICT_DIR)/Go4AnalysisLinkDef.h
GO4AN_DICT        = $(GO4DICT_DIR)/$(DICT_PREFIX)Go4Analysis
GO4AN_DH          = $(GO4AN_DICT).$(HedSuf)
GO4AN_DS          = $(GO4AN_DICT).$(SrcSuf)
GO4AN_DO          = $(GO4AN_DICT).$(ObjSuf)
GO4AN_DDEP       =  $(GO4AN_DICT).$(DepSuf)

GO4AN_H   = $(GO4EVENT_H)  \
            $(EVENTSERV_H) \
            $(HISTSERV_H)  \
            $(GO4ANAL_H)   \
            $(CMDANAL_H)   \
            $(ANALCL_H) 

GO4AN_O   = $(MBSAPI_O)    \
            $(RAWAPI_O)    \
            $(GO4EVENT_O)  \
            $(EVENTSERV_O) \
            $(HISTSERV_O)  \
            $(GO4ANAL_O)   \
            $(CMDANAL_O)   \
            $(ANALCL_O)    \
            $(GO4AN_DO)

ifdef USEHDF5
GO4AN_H += $(HDF_H)
GO4AN_O += $(HDF_O)
endif

# library used in the GUI

GO4BGUI_LINKDEF     = $(GO4DICT_DIR)/Go4GuiLinkDef.h
GO4BGUI_DICT        = $(GO4DICT_DIR)/$(DICT_PREFIX)Go4Gui
GO4BGUI_DH          = $(GO4BGUI_DICT).$(HedSuf)
GO4BGUI_DS          = $(GO4BGUI_DICT).$(SrcSuf)
GO4BGUI_DO          = $(GO4BGUI_DICT).$(ObjSuf)
GO4BGUI_DDEP       =  $(GO4BGUI_DICT).$(DepSuf)

GO4BGUI_H = $(GO4OBJM_H)  \
            $(GO4PROX_H) 

GO4BGUI_O = $(GO4OBJM_O)  \
            $(GO4DISPL_O) \
            $(GO4PROX_O)  \
            $(GO4BGUI_DO)


LIBDEPENDENC       += $(GO4BASE_DDEP) $(GO4TSKH_DDEP) $(GO4ANBASE_DDEP) $(GO4AN_DDEP) $(GO4BGUI_DDEP)


ifdef DOPACKAGE
THREADMGR_DICT_DISTR = $(GO4BASE_LINKDEF)
TASKHANLER_DICT_DISTR = $(GO4BASE_LINKDEF) $(GO4TSKH_LINKDEF)
DISTRFILES         += $(GO4BASE_LINKDEF) $(GO4TSKH_LINKDEF) $(GO4ANBASE_LINKDEF) $(GO4AN_LINKDEF) $(GO4BGUI_LINKDEF) 
endif

GO4BASE_DEPLIB = $(BASIC_LIB_DEP)

$(GO4BASE_LIB):   MAKELIB_SET = $(LIBS_BASESET)

GO4TSKH_DEPLIB = $(THRDMNGR_LIB) $(GO4BASE_LIB) $(BASIC_LIB_DEP)

$(GO4TSKH_LIB):   MAKELIB_SET = $(LIBS_BASESET) $(call go4ldname,Go4Base) $(call go4ldname,Go4ThreadManager)

GO4ANBASE_DEPLIB = $(GO4TSKH_LIB) $(THRDMNGR_LIB) $(GO4BASE_LIB) $(GO4FIT_LIB) $(BASIC_LIB_DEP)

$(GO4ANBASE_LIB): MAKELIB_SET = $(LIBS_BASESET) $(call go4ldname,Go4Fit) $(call go4ldname,Go4Base) $(call go4ldname,Go4ThreadManager) $(call go4ldname,Go4TaskHandler) 

GO4AN_DEPLIB = $(GO4ANBASE_LIB) $(GO4TSKH_LIB) $(THRDMNGR_LIB) $(GO4BASE_LIB) $(GO4FIT_LIB) $(BASIC_LIB_DEP)

$(GO4AN_LIB): MAKELIB_SET = $(LIBS_BASESET) $(call go4ldname,Go4Fit) $(call go4ldname,Go4Base) $(call go4ldname,Go4ThreadManager) $(call go4ldname,Go4TaskHandler) $(call go4ldname,Go4AnalBase)

GO4BGUI_DEPLIB = $(GO4ANBASE_LIB) $(GO4TSKH_LIB) $(THRDMNGR_LIB) $(GO4BASE_LIB) $(GO4FIT_LIB) $(BASIC_LIB_DEP)

$(GO4BGUI_LIB):   MAKELIB_SET = $(LIBS_BASESET) $(call go4ldname,Go4Fit) $(call go4ldname,Go4Base) $(call go4ldname,Go4ThreadManager) $(call go4ldname,Go4TaskHandler) $(call go4ldname,Go4AnalBase)

$(GO4BASE_DS): $(GO4BASE_H) $(GO4BASE_LINKDEF) $(GO4SYS)/build/dummy.d
	@$(ROOTCINTGO4) $(GO4BASE_LIB) $(GO4BASE_H) $(GO4BASE_LINKDEF)
	
$(GO4BASE_LIB):  $(GO4BASE_O) $(GO4BASE_DEPLIB)
	@$(MakeLibrary) $(GO4BASE_LIBNAME) "$(GO4BASE_O)" $(GO4DLLPATH) "$(GO4BASE_LINKDEF)" "$(GO4BASE_DEPLIB)" $(GO4BASE_DS) "$(GO4BASE_H)" 


$(GO4TSKH_DS): $(GO4TSKH_H) $(GO4TSKH_LINKDEF) $(GO4SYS)/build/dummy.d
	@$(ROOTCINTGO4) $(GO4TSKH_LIB) $(GO4TSKH_H) $(GO4TSKH_LINKDEF)
	
$(GO4TSKH_LIB):   $(GO4TSKH_O) $(GO4TSKH_DEPLIB)
	@$(MakeLibrary) $(GO4TSKH_LIBNAME) "$(GO4TSKH_O)" $(GO4DLLPATH) "$(GO4TSKH_LINKDEF)" "$(GO4TSKH_DEPLIB)" $(GO4TSKH_DS) "$(GO4TSKH_H)"


$(GO4ANBASE_DS): $(GO4ANBASE_H) $(GO4ANBASE_LINKDEF) $(GO4SYS)/build/dummy.d
	@$(ROOTCINTGO4) $(GO4ANBASE_LIB) $(GO4ANBASE_H) $(GO4ANBASE_LINKDEF)

$(GO4ANBASE_LIB): $(GO4ANBASE_O) $(GO4ANBASE_DEPLIB)
	@$(MakeLibrary) $(GO4ANBASE_LIBNAME) "$(GO4ANBASE_O)" $(GO4DLLPATH) "$(GO4ANBASE_LINKDEF)" "$(GO4ANBASE_DEPLIB)" $(GO4ANBASE_DS) "$(GO4ANBASE_H)"


$(GO4AN_DS): $(GO4AN_H) $(GO4AN_LINKDEF) $(GO4SYS)/build/dummy.d
	@$(ROOTCINTGO4) $(GO4AN_LIB) $(GO4AN_H) $(GO4AN_LINKDEF)

$(GO4AN_LIB): $(GO4AN_O) $(GO4AN_DEPLIB)
	@$(MakeLibrary) $(GO4AN_LIBNAME) "$(GO4AN_O)" $(GO4DLLPATH) "$(GO4AN_LINKDEF)" "$(GO4AN_DEPLIB)" $(GO4AN_DS) "$(GO4AN_H)"


$(GO4BGUI_DS): $(GO4BGUI_H) $(GO4BGUI_LINKDEF) $(GO4SYS)/build/dummy.d
	@$(ROOTCINTGO4) $(GO4BGUI_LIB) $(GO4BGUI_H) $(GO4BGUI_LINKDEF)

$(GO4BGUI_LIB): $(GO4BGUI_O) $(GO4BGUI_DEPLIB)
	@$(MakeLibrary) $(GO4BGUI_LIBNAME) "$(GO4BGUI_O)" $(GO4DLLPATH) "$(GO4BGUI_LINKDEF)" "$(GO4BGUI_DEPLIB)" $(GO4BGUI_DS) "$(GO4BGUI_H)"

clean-mainlibs::
	@$(CleanLib) $(GO4BASE_LIBNAME) $(GO4DLLPATH)
	@$(CleanLib) $(GO4TSKH_LIBNAME) $(GO4DLLPATH)
	@$(CleanLib) $(GO4ANBASE_LIBNAME) $(GO4DLLPATH)
	@$(CleanLib) $(GO4AN_LIBNAME) $(GO4DLLPATH)
	@$(CleanLib) $(GO4BGUI_LIBNAME) $(GO4DLLPATH)


clean-bin::
	@$(RM) $(GO4BASE_DICT).* $(GO4TSKH_DICT).* $(GO4ANBASE_DICT).* $(GO4AN_DICT).* $(GO4BGUI_DICT).* 
