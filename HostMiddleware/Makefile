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



CC=gcc
CXX=g++
PY=python3
BUILDDIR := out

LFLAGS += 
CFLAGS += -Wall -Werror -g -MMD
INCLUDES += 

SRC_DIR := .
SOURCES := $(wildcard *.c)
SOURCESCPP := $(wildcard *.cpp)
OBJECTS := $(SOURCES:%.c=%.o)
OBJECTS += $(SOURCESCPP:%.cpp=%.o)
OBJECTS := $(addprefix $(BUILDDIR)/,$(OBJECTS))

# compilers (at least gcc and clang) don't create the subdirectories automatically
# $(shell mkdir -p $(BUILDDIR) >/dev/null)

all: lib.so

.PHONY: ../Host\ Password\ Manager/host_lib.so
../Host\ Password\ Manager/host_lib.so: Makefile ../Host\ Password\ Manager/Makefile $(SOURCES) $(SOURCESCPP)
	@$(MAKE) -C ../Host\ Password\ Manager SRSC=$(CURDIR)

lib.so: ../Host\ Password\ Manager/host_lib.so Makefile 
	@echo " [R] ../Host\ Password\ Manager/host_lib.so -> lib.so"
	@cp ../Host\ Password\ Manager/host_lib.so lib.so

.PHONY: clean
clean:
	@$(MAKE) -C ../Host\ Password\ Manager clean
	@rm -f *.o *.d
	@rm -f lib.so

.PHONY: run
run: lib.so
	@$(PY) app.py

.PHONY: dist
dist: lib.so Makefile $(wildcard *.py)
	@$(ECHO) Installing PYARMOR
	@pip -q install pyarmor
	@$(ECHO) Installing PyInstaller
	@pip -q install pyinstaller
	@$(ECHO) Installing tinyaes
	@pip -q install tinyaes
	@$(ECHO) Generating a random encryption key
	$(eval AKEY := $(shell md5sum app.py | head -c 16))
	@$(ECHO) Invoking PYARMOR with Encryption
# pyarmor obfuscate app.py
# pyarmor pack --name BPMMiddleware --clean -e "--onefile " app.py
# pyarmor pack --name BPMMiddleware --clean -e "--add-data 'lib.so:.' " app.py
	@pyarmor pack --name BPMMiddleware --clean -e "--add-data 'lib.so:.' --onefile --key '$(AKEY)' " app.py
	@$(ECHO) Cleaning up
	@cp dist/BPMMiddleware .
	@rm -r build dist
	@$(ECHO) Done. Executable generated in $(shell realpath BPMMiddleware)


endif

# .PHONY: build
# build: lib.so

# lib.so: $(OBJECTS) 
# 	@echo [L] $@
# 	$(CXX) -shared -o $@ $(LFLAGS) $(OBJECTS)

# $(BUILDDIR)/%.o: %.c Makefile
# 	@echo [C] $<
# 	@$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

# $(BUILDDIR)/%.o: %.cpp Makefile
# 	@echo [C] $<
# 	@$(CXX) -c $(CFLAGS) $(INCLUDES) $< -o $@

# -include $(OBJECTS:.o=.d)

# clean:
# 	@rm -f *.o *.d *.so
# 	@rm -rf $(BUILDDIR)

# TODO: add include .d for header files

