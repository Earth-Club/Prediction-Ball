######################################
#
######################################
# Target
TARGET  := trigram

#source file
SOURCE  := trigram.c lunar-date.c
OBJS    := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCE)))

#compile and lib parameter
CC      := clang
LIBS    :=
LDFLAGS :=
DEFINES :=
INCLUDE := -I.
CFLAGS  := -g -Wall -O3 $(DEFINES) $(INCLUDE)

.PHONY : everything objs clean veryclean build

everything : $(TARGET)

all : $(TARGET)

objs : $(OBJS)

build : veryclean everything

clean :
	-rm -rf $(TARGET)
	-rm -rf *.o

$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS) $(LIBS)
