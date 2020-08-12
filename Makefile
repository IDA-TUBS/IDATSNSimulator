#
# OMNeT++/OMNEST Makefile for idaTSNSimulator
#
# This file was generated with the command:
#  opp_makemake -f --deep -O out -I.
#

# Name of target to be created (-o option)
TARGET = idaTSNSimulator$(D)$(EXE_SUFFIX)
TARGET_DIR = .

# User interface (uncomment one) (-u option)
USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(TKENV_LIBS) $(QTENV_LIBS) $(CMDENV_LIBS)
#USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(TKENV_LIBS)
#USERIF_LIBS = $(QTENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = -I.

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS =

# Output directory
PROJECT_OUTPUT_DIR = out
PROJECTRELATIVE_PATH =
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc, .msg and .sm files
OBJS = \
    $O/src/common/ConfigurationReader.o \
    $O/src/endnode/generator/msgcreators/EthMsgCreator.o \
    $O/src/endnode/generator/msgcreators/SampleMsgCreator.o \
    $O/src/endnode/generator/msgcreators/TraceEthMsgCreator.o \
    $O/src/endnode/generator/trafficsrcs/TraceSource.o \
    $O/src/endnode/generator/trafficsrcs/TrafficSource.o \
    $O/src/endnode/middleware/rtps/Rtps.o \
    $O/src/endnode/middleware/rtps/adapters/JoinReaders.o \
    $O/src/endnode/middleware/rtps/adapters/RtpsToEthernetAdapter.o \
    $O/src/endnode/middleware/rtps/delayunit/DelayUnit.o \
    $O/src/endnode/middleware/rtps/entities/Endpoint.o \
    $O/src/endnode/middleware/rtps/entities/Reader.o \
    $O/src/endnode/middleware/rtps/entities/Writer.o \
    $O/src/endnode/middleware/rtps/participant/Participant.o \
    $O/src/endnode/middleware/rtps/shaper/Shaper.o \
    $O/src/endnode/sink/SampleSink.o \
    $O/src/endnode/sink/Sink.o \
    $O/src/endnode/udpipstack/UdpIpStack.o \
    $O/src/statistics/ddsSampleLatencyStatistics.o \
    $O/src/statistics/E2ELatencyStatistics.o \
    $O/src/statistics/FramesLostStatistics.o \
    $O/src/statistics/LinkTransmissionStatistics.o \
    $O/src/statistics/StatisticManager.o \
    $O/src/statistics/TimeAwareShaperSegmentStatistic.o \
    $O/src/statistics/helperModulesStatistics/HistogramStatistics2DMap.o \
    $O/src/statistics/helperModulesStatistics/HistogramStatistics3DMap.o \
    $O/src/statistics/helperModulesStatistics/VectorStatistics2DMap.o \
    $O/src/statistics/helperModulesStatistics/VectorStatistics3DMap.o \
    $O/src/switch/egressport/ArbiterAVB.o \
    $O/src/switch/egressport/ArbiterIEEE802_1Q.o \
    $O/src/switch/egressport/ArbiterIEEE802_1Qbv.o \
    $O/src/switch/egressport/ArbiterIEEE802_3.o \
    $O/src/switch/egressport/BuffersModule.o \
    $O/src/switch/egressport/ClassifierPriority.o \
    $O/src/switch/egressport/EgressPort.o \
    $O/src/switch/egressport/FrameBuffer.o \
    $O/src/switch/forwardinglogic/ForwardingLogic.o \
    $O/src/switch/ingressport/IngressPort.o \
    $O/src/endnode/middleware/rtps/messages/RegisterEntity_m.o \
    $O/src/endnode/middleware/rtps/messages/RTPSEthMessage_m.o \
    $O/src/endnode/middleware/rtps/messages/Sample_m.o \
    $O/src/messagetype/BufferControlMessage_m.o \
    $O/src/messagetype/EthernetFrame_m.o \
    $O/src/messagetype/TraceGenerateMessage_m.o

# Message files
MSGFILES = \
    src/endnode/middleware/rtps/messages/RegisterEntity.msg \
    src/endnode/middleware/rtps/messages/RTPSEthMessage.msg \
    src/endnode/middleware/rtps/messages/Sample.msg \
    src/messagetype/BufferControlMessage.msg \
    src/messagetype/EthernetFrame.msg \
    src/messagetype/TraceGenerateMessage.msg

# SM files
SMFILES =

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
ifneq ("$(OMNETPP_ROOT)","")
CONFIGFILE = $(OMNETPP_ROOT)/Makefile.inc
else
CONFIGFILE = $(shell opp_configfilepath)
endif
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# Simulation kernel and user interface libraries
OMNETPP_LIBS = $(OPPMAIN_LIB) $(USERIF_LIBS) $(KERNEL_LIBS) $(SYS_LIBS)

COPTS = $(CFLAGS) $(IMPORT_DEFINES)  $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)
SMCOPTS =

# we want to recompile everything if COPTS changes,
# so we store COPTS into $COPTS_FILE and have object
# files depend on it (except when "make depend" was called)
COPTS_FILE = $O/.last-copts
ifneq ("$(COPTS)","$(shell cat $(COPTS_FILE) 2>/dev/null || echo '')")
$(shell $(MKPATH) "$O" && echo "$(COPTS)" >$(COPTS_FILE))
endif

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
# >>>
# <<<
#------------------------------------------------------------------------------

# Main target
all: $(TARGET_DIR)/$(TARGET)

$(TARGET_DIR)/% :: $O/%
	@mkdir -p $(TARGET_DIR)
	$(Q)$(LN) $< $@
ifeq ($(TOOLCHAIN_NAME),clangc2)
	$(Q)-$(LN) $(<:%.dll=%.lib) $(@:%.dll=%.lib)
endif

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile $(CONFIGFILE)
	@$(MKPATH) $O
	@echo Creating executable: $@
	$(Q)$(CXX) $(LDFLAGS) -o $O/$(TARGET) $(OBJS) $(EXTRA_OBJS) $(AS_NEEDED_OFF) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS)

.PHONY: all clean cleanall depend msgheaders smheaders

.SUFFIXES: .cc

$O/%.o: %.cc $(COPTS_FILE) | msgheaders smheaders
	@$(MKPATH) $(dir $@)
	$(qecho) "$<"
	$(Q)$(CXX) -c $(CXXFLAGS) $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(qecho) MSGC: $<
	$(Q)$(MSGC) -s _m.cc -MD -MP -MF $O/$(basename $<)_m.h.d $(MSGCOPTS) $?

%_sm.cc %_sm.h: %.sm
	$(qecho) SMC: $<
	$(Q)$(SMC) -c++ -suffix cc $(SMCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

smheaders: $(SMFILES:.sm=_sm.h)

clean:
	$(qecho) Cleaning $(TARGET)
	$(Q)-rm -rf $O
	$(Q)-rm -f $(TARGET_DIR)/$(TARGET)
	$(Q)-rm -f $(TARGET_DIR)/$(TARGET:%.dll=%.lib)
	$(Q)-rm -f $(call opp_rwildcard, . , *_m.cc *_m.h *_sm.cc *_sm.h)

cleanall:
	$(Q)$(MAKE) -s clean MODE=release
	$(Q)$(MAKE) -s clean MODE=debug
	$(Q)-rm -rf $(PROJECT_OUTPUT_DIR)

# include all dependencies
-include $(OBJS:%=%.d) $(MSGFILES:%.msg=$O/%_m.h.d)