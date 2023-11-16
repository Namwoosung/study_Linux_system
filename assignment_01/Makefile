TARGET = hello	#Define execution file name
SRCS = test.c korea.c world.c usa.c #Define required source file 
OBJS = $(SRCS:.c=.o) #define required object file
CC = gcc #Specify compiler as gcc

#Define variables for FLAG
L_CPPFLAGS =
L_INCFLAGS = -I./
L_CFLAGS = 
L_LDFLAGS =
L_LDLIBS =

CPPFLAGS += $(L_CPPFLAGS) $(L_INCFLAGS) #Define preprocessor flags
CFLAGS += $(L_CFLAGS) -D_REENTRANT #define compiler flags
LDFLAGS += $(L_LDFLAGS) $(L_LDLIBS) #define linker flags

#.PHONY used to resolve name dispatches and improve performance
.PHONY: all
all: build #'all' target depends on build

.PHONY: build
build: $(TARGET) #'build' target depends on TARGET

$(TARGET): $(OBJS) #'TARGET' targer depends on OBJS
	$(CC) -o $@ $^ $(LDFLAGS) #Create TARGET with OBJS based on linking flag

.PHONY: clean
clean: 
	rm -f $(TARGET) $(OBJS) $(DEPEND_FILE) core* #Delete files created during compile

.PHONY: distclean
distclean: clean #distclean to clean until configure

.PHONY: depend
depend: $(SRCS)
	$(CC) -M $(CFLAGS) $(CPPFLAGS) $^ > $(DEPEND_FILE)

-include $(DEPEND_FILE)
