include $(GO4SYS)/Makefile.config

## normally should be like this for every module, but can be specific

ifdef GO4PACKAGE
USERGUI3_DIR         = qt3/Go4UserGUI
USERGUI3_QFLAGS      = "QTDIR=$(QTPATH)" "GO4SYS=../.."
USERGUI3_RULES       = | qt3-GUI qt3-heads
else
USERGUI3_DIR         = $(CURDIR)
endif

USERGUI3_LIBDIR      = $(USERGUI3_DIR)
USERGUI3_QTLIBNAME   = libGo4UserGui
USERGUI3_QTPRO       = Go4UserGui.pro
USERGUI3_QTMAKE      = Makefile.qt
USERGUI3_QGO4WIDG    = $(GO4SYS)/include/QGo4Widget.h

USERGUI3_NOTLIBF     =

## must be similar for every module

USERGUI3_FORMS      := $(wildcard $(USERGUI3_DIR)/*.ui)
USERGUI3_FORMSI     := $(wildcard $(USERGUI3_DIR)/*.ui.h)
USERGUI3_FH         := $(USERGUI3_FORMS:.ui=.h)
USERGUI3_FS         := $(USERGUI3_FORMS:.ui=.cpp)

USERGUI3_QTLIB      = $(USERGUI3_LIBDIR)/$(USERGUI3_QTLIBNAME).$(DllSuf)


# used in the main Makefile

ifdef DOPACKAGE
DISTRFILES         += $(USERGUI3_FORMS) $(USERGUI3_FORMSI)
DISTRFILES         += $(USERGUI3_DIR)/$(USERGUI3_QTPRO) $(USERGUI3_DIR)/README.md
DISTRFILES         += $(USERGUI3_DIR)/Makefile
endif

##### local rules #####

ifeq ($(GO4_QT), 3)

all:: all-qt3usergui

clean:: clean-qt3usergui

clean-bin:: clean-qt3usergui-bin

endif

$(USERGUI3_DIR)/$(USERGUI3_QTMAKE): $(USERGUI3_DIR)/$(USERGUI3_QTPRO) $(USERGUI3_FORMS) $(USERGUI3_RULES)
	@echo "Generating $(USERGUI3_QTMAKE)..."
	cd $(USERGUI3_DIR); $(QMAKE) $(USERGUI3_QTPRO) -o $(USERGUI3_QTMAKE) $(QMAKEOPTFLAG) $(QMAKEFLAGS)

$(USERGUI3_QTLIB): $(USERGUI3_DIR)/$(USERGUI3_QTMAKE) $(USERGUI3_FORMSI) $(USERGUI3_QGO4WIDG) $(USERGUI3_RULES)
	@echo "Generating Qt part of the User GUI..."
	+cd $(USERGUI3_DIR); $(MAKE) -f $(USERGUI3_QTMAKE) $(USERGUI3_QFLAGS)

all-qt3usergui:  $(USERGUI3_QTLIB)

clean-qt3usergui-bin:
	@$(RM) $(USERGUI3_DIR)/*.o
ifneq ($(wildcard $(USERGUI3_DIR)/$(USERGUI3_QTMAKE)),)
	cd $(USERGUI3_DIR); $(MAKE) -f $(USERGUI3_QTMAKE) clean $(USERGUI3_QFLAGS)
endif
	@$(RM) $(USERGUI3_DIR)/$(USERGUI3_QTMAKE)

clean-qt3usergui: clean-qt3usergui-bin
	@$(RM) $(USERGUI3_LIBDIR)/$(USERGUI3_QTLIBNAME).$(DllSuf)*

include $(GO4SYS)/Makefile.rules
