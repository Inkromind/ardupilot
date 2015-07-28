#---------
#
# CppUTest Makefile
#
#----------

#Set this to @ to keep the makefile quiet
ifndef SILENCE
	SILENCE = @
endif

#--- Inputs ----#
COMPONENT_NAME = ArduPilotTest
CPPUTEST_HOME = /home/vagrant/cpputest

CPPUTEST_USE_EXTENSIONS = Y
CPP_PLATFORM = Gcc
LIBDIR  := ../libraries/

# This line is overriding the default new macros.  This is helpful
# when using std library includes like <list> and other containers
# so that memory leak detection does not conflict with stl.
#CPPUTEST_MEMLEAK_DETECTOR_NEW_MACRO_FILE = -include ApplicationLib/ExamplesNewOverrides.h

SRC_DIRS = \
	$(LIBDIR)AC_Facade\
	$(LIBDIR)AC_CommunicationFacade\
	$(LIBDIR)AC_ReactiveFacade\
	$(LIBDIR)AC_SafetyControl\
	$(LIBDIR)AMW_Corridors\
	$(LIBDIR)AMW_Facade\
	$(LIBDIR)AMW_List\
	$(LIBDIR)AMW_Modules\
	$(LIBDIR)AMW_Planner\
	$(LIBDIR)AC_Facade\
	$(LIBDIR)AMW_Queue\

TEST_SRC_DIRS = \
	test

INCLUDE_DIRS =\
  .\
  stubs\
  mocks\
  $(shell find $(LIBDIR) -type d)\
  $(CPPUTEST_HOME)/include\
  
MOCKS_SRC_DIRS =\
  stubs\
  mocks\
  
CPPUTEST_CPPFLAGS += -DTESTENV -std=c++0x
CPPUTEST_WARNINGFLAGS = -Wall

include $(CPPUTEST_HOME)/build/MakefileWorker.mk