
CPP     += 
LFLAGS  += 
CFLAGS  += 
INCLUDES+= 

SOURCES := $(wildcard *.c*)
OBJECTS := $(SOURCES:%.cpp=%.o)
BUILD_DIR:= $(if $(BUILD_DIR),$(BUILD_DIR),.)

all: bld

bld: $(BUILD_DIR)/$(OBJECTS)

$(BUILD_DIR)/%.o: %.cpp Makefile
	@echo [C] $<
	@$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

clean:
	@rm -f $(BUILD_DIR)/$(OBJECTS)
