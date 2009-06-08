DYNLIST_NAME        = Go4DynamicList

## normally should be like this for every module, but can be specific

DYNLIST_DIR         = $(GO4SYS)/$(DYNLIST_NAME)
DYNLIST_LINKDEF     = $(DYNLIST_DIR)/$(DYNLIST_NAME)LinkDef.$(HedSuf)

DYNLIST_NOTLIBF     = $(DYNLIST_DIR)/TGo4HistoFillAction.h

## must be similar for every module

DYNLIST_DICT        = $(DYNLIST_DIR)/$(DICT_PREFIX)$(DYNLIST_NAME)
DYNLIST_DH          = $(DYNLIST_DICT).$(HedSuf)
DYNLIST_DS          = $(DYNLIST_DICT).$(SrcSuf)
DYNLIST_DO          = $(DYNLIST_DICT).$(ObjSuf)

DYNLIST_H           = $(filter-out $(DYNLIST_NOTLIBF) $(DYNLIST_DH) $(DYNLIST_LINKDEF) $(DYNLIST_OTHERF), $(wildcard $(DYNLIST_DIR)/*.$(HedSuf)))
DYNLIST_S           = $(filter-out $(DYNLIST_NOTLIBF) $(DYNLIST_DS), $(wildcard $(DYNLIST_DIR)/*.$(SrcSuf)))
DYNLIST_O           = $(DYNLIST_S:.$(SrcSuf)=.$(ObjSuf))

DYNLIST_DEP         =  $(DYNLIST_O:.$(ObjSuf)=.$(DepSuf))
DYNLIST_DDEP        =  $(DYNLIST_DO:.$(ObjSuf)=.$(DepSuf))

# used in the main Makefile

ALLHDRS +=  $(patsubst $(DYNLIST_DIR)/%.h, $(GO4SYS)/include/%.h, $(DYNLIST_H))

LIBDEPENDENC       += $(DYNLIST_DEP) $(DYNLIST_DDEP)

ifdef DOPACKAGE
DISTRFILES         += $(DYNLIST_S) $(DYNLIST_H) $(DYNLIST_LINKDEF) $(DYNLIST_NOTLIBF)
endif

##### local rules #####

$(GO4SYS)/include/%.h: $(DYNLIST_DIR)/%.h
	@cp -f $< $@

$(DYNLIST_DS): $(DYNLIST_H)  $(DYNLIST_LINKDEF)
		@$(ROOTCINTGO4) $(DYNLIST_H) $(DYNLIST_LINKDEF)

all-$(DYNLIST_NAME):  $(DYNLIST_O) $(DYNLIST_DO)

clean-obj-$(DYNLIST_NAME):
		@rm -f $(DYNLIST_O) $(DYNLIST_DO)

clean-$(DYNLIST_NAME): clean-obj-$(DYNLIST_NAME)
		@rm -f $(DYNLIST_DEP) $(DYNLIST_DDEP) $(DYNLIST_DS) $(DYNLIST_DH)

