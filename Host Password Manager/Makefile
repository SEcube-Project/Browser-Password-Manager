
ifneq ($(words $(MAKECMDGOALS)),1)
.DEFAULT_GOAL = all
%:
	@$(MAKE) $@ --no-print-directory -rRf $(firstword $(MAKEFILE_LIST))
else
ifndef ECHO
T := $(shell $(MAKE) $(MAKECMDGOALS) --no-print-directory \
      -nrRf $(firstword $(MAKEFILE_LIST)) \
      ECHO="COUNTTHIS" | grep -c "COUNTTHIS")

N := x
C = $(words $N)$(eval N := x $N)
ECHO = echo "`expr " [\`expr $C '*' 100 / $T\`" : '.*\(....\)$$'`%]"
endif

TARGETS+=host_lib.so
override SRSC+=sources

build: $(TARGETS)

all: build

clean:
	@rm -rf $(ALL_OBJ_FILES)
	@rm -rf $(ALL_CPP_FILES:%.cpp=%.d)
	@rm -rf $(ALL_C_FILES:%.c=%.d)
	@rm -f $(TARGETS)

##############################
# definition
CC=gcc
CXX=g++

CPPFLAGS+=-MMD    # automatic .d dependency file generation
CPPFLAGS+=-g
CPPFLAGS+=-fPIC

# CPPFLAGS+=-Wall -Wextra 
#CPPFLAGS+=-Werror

LDFLAGS+=

override ALL_CPP_FILES+=$(shell find $(SRSC) -name '*.cpp' | sort | uniq | sed 's/ /\\\\ /g')
ALL_C_FILES+=$(shell find $(SRSC) -name '*.c' | sort | uniq | sed 's/ /\\\\ /g')
ALL_OBJ_FILES=$(patsubst %.cpp,%.o,$(ALL_CPP_FILES))
ALL_OBJ_FILES+=$(patsubst %.c,%.o,$(ALL_C_FILES))

##############################
%.o: %.cpp Makefile
	@$(ECHO) " [C] $<"
	@$(CXX) -c $(CPPFLAGS) $< -o $@

%.o: %.c Makefile
	@$(ECHO) " [C] $<"
	@$(CC) -c $(CPPFLAGS) $< -o $@

examples/device_init.o: examples/device_init.cpp Makefile
	@$(ECHO) " [C] $<"
	@$(CXX) -c $(CPPFLAGS) $< -o $@

##############################

# tool targets
host_lib.so: $(ALL_OBJ_FILES)
	@$(ECHO) " [L] $@"
	@$(CXX) $^ -shared $(LDFLAGS) -o $@

init: $(ALL_OBJ_FILES) examples/device_init.o
	@$(ECHO) " [L] $@"
	@$(CXX) $^ $(LDFLAGS) -o $@

.PRECIOUS: S(ALL_OBJ_FILES)
.PHONY: clean all build

-include $(ALL_CPP_FILES:%.cpp=%.d)

endif