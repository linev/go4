FITGUI_NAME         = Go4FitGUI
MODULE_NAME         = $(FITGUI_NAME)

## normally should be like this for every module, but can be specific

FITGUI_DIR         = $(GO4SYS)/qt4/$(FITGUI_NAME)
FITGUI_QTLIBNAME   = libGo4FitGUI
FITGUI_QTPRO       = $(FITGUI_DIR)/Go4FitGUI.pro
FITGUI_QTMAKE      = $(FITGUI_DIR)/Makefile.qt

FITGUI_NOTLIBF     =

## must be similar for every module

FITGUI_FORMS       = $(wildcard $(FITGUI_DIR)/*.ui)
#FITGUI_FORMSI      = $(wildcard $(FITGUI_DIR)/*.ui.h)

FITGUI_UI_H        = $(patsubst $(FITGUI_FORMS)/%.ui,$(FITGUI_FORMS)/ui_%.h,$(FITGUI_FORMS))

FITGUI_FH          = $(FITGUI_FORMS:.ui=.h)
FITGUI_FS          = $(FITGUI_FORMS:.ui=.cpp)

FITGUI_QTS         = $(filter-out $(FITGUI_NOTLIBF), $(wildcard $(FITGUI_DIR)/*.cpp))
FITGUI_QTH         = $(FITGUI_QTS:.cpp=.h)


#FITGUI_QTH         = $(FITGUI_DIR)/QFitItem.h \
#                     $(FITGUI_DIR)/QFitWidget.h \
#                     $(FITGUI_DIR)/QFitNamedWidget.h \
#                     $(FITGUI_DIR)/QFitModelWidget.h
                     
                     
#FITGUI_QTS         = $(FITGUI_QTH:.h=.cpp)

# used in the main Makefile

ALLHDRS +=  $(patsubst $(FITGUI_DIR)/%.h, $(GO4SYS)/include/%.h, $(FITGUI_QTH))

ifdef DOPACKAGE

DISTRFILES         += $(FITGUI_FORMS)  $(FITGUI_QTPRO)
DISTRFILES         += $(FITGUI_QTH) $(FITGUI_QTS) $(FITGUI_DIR)/TGo4FitGuiTypes.h
endif

GO4QTHEADS         += $(FITGUI_UI_H)
GO4QTTAGS          += qt-$(FITGUI_NAME)

##### local rules #####

$(GO4SYS)/include/%.h: $(FITGUI_DIR)/%.h
	@cp -f $< $@

$(FITGUI_DIR)/ui_%.h: $(FITGUI_DIR)/%.ui
	@echo "Producing file $@ ..." 
	@$(UIC) $< -o $@

# generate makefile only if project is exists
ifneq ($(wildcard $(FITGUI_QTPRO)),)
$(FITGUI_QTMAKE): $(FITGUI_QTPRO) $(FITGUI_FORMS)
	@echo "Generating Makefile.qt of the Fit GUI..."
	cd $(FITGUI_DIR); $(QMAKE) $(FITGUI_QTPRO) -o $(FITGUI_QTMAKE) $(QMAKEOPTFLAG) $(QMAKEFLAGS) "DESTDIR=$(GO4DLLPATH)"
#	cd $(FITGUI_DIR); $(QMAKE) $(FITGUI_QTPRO) -o $(FITGUI_QTMAKE) "DESTDIR=$(GO4DLLPATH)"
endif

qt-$(FITGUI_NAME): $(FITGUI_QTMAKE) $(FITGUI_UI_H)
	@echo "Generating Qt part of the Fit GUI..."
	+cd $(FITGUI_DIR); $(MAKE) -f $(FITGUI_QTMAKE)

all-$(FITGUI_NAME):  $(FITGUI_FH) qt-$(FITGUI_NAME)

clean-obj-$(FITGUI_NAME):
	@rm -f $(GO4DLLPATH)/$(FITGUI_QTLIBNAME).$(DllSuf)*
	@rm -f $(FITGUI_DIR)/*.o

clean-$(FITGUI_NAME): clean-obj-$(FITGUI_NAME)
ifneq ($(wildcard $(FITGUI_QTMAKE)),)
	cd $(FITGUI_DIR); $(MAKE) -f $(FITGUI_QTMAKE) clean
endif
	@rm -f $(FITGUI_QTMAKE)
