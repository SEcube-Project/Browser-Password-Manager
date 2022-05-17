
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

