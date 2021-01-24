######################################
#
######################################
# Target
TARGET  := trigram

#source file
SOURCE  := main.c ../arduino/main/trigram.c ../arduino/main/lunar-date.c
OBJS    := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCE)))

#compile and lib parameter
CC      := clang
LIBS    :=
LDFLAGS :=
DEFINES :=
INCLUDE := -I. -I../arduino/main
CFLAGS  := -g -Wall -O3 $(DEFINES) $(INCLUDE)

.PHONY : everything objs clean veryclean build

everything : $(TARGET)

all : $(TARGET)

objs : $(OBJS)

build : veryclean everything

clean :
	-rm -rf $(TARGET)
	-rm -rf *.o ../arduino/main/*.o

$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS) $(LIBS)
