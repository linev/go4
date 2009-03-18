EXUSERSRC_NAME        = Go4ExampleUserSource
MODULE_NAME         = $(EXUSERSRC_NAME)

## normally should be like this for every module, but can be specific

ifdef GO4PACKAGE
EXUSERSRC_DIR         = $(GO4SYS)/$(EXUSERSRC_NAME)
else
EXUSERSRC_DIR         = .
endif

EXUSERSRC_LINKDEF     = $(EXUSERSRC_DIR)/Go4UserAnalysisLinkDef.$(HedSuf)
EXUSERSRC_LIBNAME     = libGo4ExampleUserSource
EXUSERSRC_EXENAME     = MainUserAnalysis
EXUSERSRC_ONLYMAP     = $(EXUSERSRC_DIR)/.localmap
EXUSERSRC_MAP         = $(EXUSERSRC_DIR)/$(ROOTMAPNAME)

EXUSERSRC_NOTLIBF     = 

## must be similar for every module

EXUSERSRC_EXEO        = $(EXUSERSRC_DIR)/$(EXUSERSRC_EXENAME).$(ObjSuf)
#EXUSERSRC_EXEH        = $(EXUSERSRC_DIR)/$(EXUSERSRC_EXENAME).$(HedSuf)
EXUSERSRC_EXES        = $(EXUSERSRC_DIR)/$(EXUSERSRC_EXENAME).$(SrcSuf)
EXUSERSRC_EXE         = $(EXUSERSRC_DIR)/$(EXUSERSRC_EXENAME)$(ExeSuf)   

EXUSERSRC_DICT        = $(EXUSERSRC_DIR)/$(DICT_PREFIX)$(EXUSERSRC_NAME)
EXUSERSRC_DH          = $(EXUSERSRC_DICT).$(HedSuf)
EXUSERSRC_DS          = $(EXUSERSRC_DICT).$(SrcSuf)
EXUSERSRC_DO          = $(EXUSERSRC_DICT).$(ObjSuf)

EXUSERSRC_H           = $(filter-out $(EXUSERSRC_EXEH) $(EXUSERSRC_NOTLIBF) $(EXUSERSRC_DH) $(EXUSERSRC_LINKDEF), $(wildcard $(EXUSERSRC_DIR)/*.$(HedSuf)))
EXUSERSRC_S           = $(filter-out $(EXUSERSRC_EXES) $(EXUSERSRC_NOTLIBF) $(EXUSERSRC_DS), $(wildcard $(EXUSERSRC_DIR)/*.$(SrcSuf)))
EXUSERSRC_O           = $(EXUSERSRC_S:.$(SrcSuf)=.$(ObjSuf))

EXUSERSRC_DEP         =  $(EXUSERSRC_O:.$(ObjSuf)=.$(DepSuf))
EXUSERSRC_DDEP        =  $(EXUSERSRC_DO:.$(ObjSuf)=.$(DepSuf))
EXUSERSRC_EDEP        =  $(EXUSERSRC_EXEO:.$(ObjSuf)=.$(DepSuf))

EXUSERSRC_SLIB        =  $(EXUSERSRC_DIR)/$(EXUSERSRC_LIBNAME).$(DllSuf)
EXUSERSRC_LIB         =  $(EXUSERSRC_DIR)/$(EXUSERSRC_LIBNAME).$(DllSuf).$(VERSSUF)

# used in the main Makefile

EXAMPDEPENDENCS    += $(EXUSERSRC_DEP) $(EXUSERSRC_DDEP) $(EXUSERSRC_EDEP)

ifdef DOPACKAGE
DISTRFILES         += $(EXUSERSRC_S) $(EXUSERSRC_H) $(EXUSERSRC_LINKDEF) $(EXUSERSRC_EXEH) $(EXUSERSRC_EXES)
DISTRFILES         += $(EXUSERSRC_DIR)/AnalysisStart.sh $(EXUSERSRC_DIR)/rename.sh $(EXUSERSRC_DIR)/README.txt
DISTRFILES         += $(EXUSERSRC_DIR)/befoil50.scf $(EXUSERSRC_DIR)/tafoil50.scf
endif

##### local rules #####

$(EXUSERSRC_EXE):      $(BUILDGO4LIBS) $(EXUSERSRC_EXEO) $(EXUSERSRC_LIB)
	$(LD) $(LDFLAGS) $(EXUSERSRC_EXEO) $(LIBS_FULLSET) $(EXUSERSRC_LIB) $(OutPutOpt) $(EXUSERSRC_EXE)
	@echo "$@  done"      

$(EXUSERSRC_LIB):   $(EXUSERSRC_O) $(EXUSERSRC_DO)
	@$(MakeLib) $(EXUSERSRC_LIBNAME) "$(EXUSERSRC_O) $(EXUSERSRC_DO)" $(EXUSERSRC_DIR)

$(EXUSERSRC_DS): $(EXUSERSRC_H)  $(EXUSERSRC_LINKDEF)
	@$(ROOTCINTGO4) $(EXUSERSRC_H) $(EXUSERSRC_LINKDEF)

$(EXUSERSRC_ONLYMAP): $(EXUSERSRC_LINKDEF) $(EXUSERSRC_LIB)
	@rm -f $(EXUSERSRC_ONLYMAP)
	@$(MakeMap) $(EXUSERSRC_ONLYMAP) $(EXUSERSRC_SLIB) $(EXUSERSRC_LINKDEF) "$(ANAL_LIB_DEP)"

all-$(EXUSERSRC_NAME):     $(EXUSERSRC_LIB) $(EXUSERSRC_EXE) map-$(EXUSERSRC_NAME)

clean-obj-$(EXUSERSRC_NAME):
	@rm -f $(EXUSERSRC_O) $(EXUSERSRC_DO)
	@$(CleanLib) $(EXUSERSRC_LIBNAME) $(EXUSERSRC_DIR)
	@rm -f $(EXUSERSRC_EXEO) $(EXUSERSRC_EXE)

clean-$(EXUSERSRC_NAME): clean-obj-$(EXUSERSRC_NAME)
	@rm -f $(EXUSERSRC_DEP) $(EXUSERSRC_DDEP) $(EXUSERSRC_DS) $(EXUSERSRC_DH)
	@rm -f $(EXUSERSRC_EDEP)
	@rm -f $(EXUSERSRC_ONLYMAP) $(EXUSERSRC_MAP)

ifdef DOMAP
map-$(EXUSERSRC_NAME): $(GO4MAP) $(EXUSERSRC_ONLYMAP)
	@rm -f $(EXUSERSRC_MAP)
	@cat $(GO4MAP) $(EXUSERSRC_ONLYMAP) > $(EXUSERSRC_MAP)
else
map-$(EXUSERSRC_NAME):

endif
