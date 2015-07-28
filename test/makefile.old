LIBDIR  := ../libraries/
OBJDIR  := ./obj/
BINDIR  := ./bin/
TESTDIR := ./test/
STUBDIR := ./stubs/
NAME    := testAll
EXE     := $(BINDIR)$(NAME)
LIBRARIES := $(shell find $(LIBDIR) -type d)
TESTLIBS := AC_Facade AC_ReactiveFacade AC_SafetyControl AMW_Facade AMW_List AMW_Planner AMW_Queue
TESTFILES := AllTests.cpp CircularBufferTest.cpp HelloTest.cpp 
STUBFILES := 
CPPUTEST_HOME = /home/vagrant/cpputest

SFILES  := cpp
OFILES  := o

TESTLIBSREL := $(foreach dir, $(TESTLIBS), $(LIBDIR)$(dir))
TESTSOURCES := $(shell find $(TESTDIR) -name "*.$(SFILES)")
TESTOBJECTS := $(patsubst $(TESTDIR)%.$(SFILES), $(OBJDIR)$(TESTDIR)%.$(OFILES), $(TESTSOURCES))
STUBSOURCES := $(shell find $(STUBDIR) -name "*.$(SFILES)")
STUBOBJECTS := $(patsubst $(TESTDIR)%.$(SFILES), $(OBJDIR)$(STUBDIR)%.$(OFILES), $(STUBSOURCES))
LIBSOURCES := $(foreach dir,$(TESTLIBSREL),$(shell find $(DIR) -name "*.$(SFILES)"))
LIBOBJECTS := $(patsubst $(LIBDIR)%.$(SFILES), $(OBJDIR)libraries/%.$(OFILES), $(LIBSOURCES))

OBJS = $(TESTOBJECTS)

CXX = g++
INCS = -I$(STUBDIR) -I/usr/local/include/ $(addprefix -I,$(TESTLIBSREL))
CXXFLAGS = -g -DTESTENV -Wall
CXXFLAGS += -lstdc++ -I$(CPPUTEST_HOME)/include/CppUTest/MemoryLeakDetectorNewMacros.h -L$(CPPUTEST_HOME)/lib -lCppUTest -lCppUTestExt
CPPFLAGS += -I$(CPPUTEST_HOME)/include
LDFLAGS = -L$(CPPUTEST_HOME)/lib -lCppUTest -lCppUTestExt -lstdc++


testAll: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(EXE) $(OBJS)
	
$(OBJDIR)$(TESTDIR)%$(OFILES): $(TESTDIR)%$(SFILES)
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCS) -c $< -o $@ 
	
$(OBJDIR)$(STUBDIR)%$(OFILES): $(STUBDIR)%$(SFILES)
	mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) $(INCS) -c $< -o $@
	
$(OBJDIR)libraries/%$(OFILES):    $(LIBDIR)%$(SFILES)
	mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) $(INCS) -c $< -o $@

clean:
	rm $(EXE) $(OBJDIR)/*.o $(EXE).xml