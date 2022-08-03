#source file
SOURCE  := $(wildcard src/*.cpp)
OBJS    := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCE)))
  
#target
TARGET  := CncVxlRenderText
  
#compile and lib parameter
CC      := g++
LIBS    :=
LDFLAGS := -std=c++20
DEFINES := 
INCLUDE := -I./inc
CFLAGS  := -g -Wall -O2 -m32 -fpermissive $(DEFINES) $(INCLUDE)
CXXFLAGS:= $(CFLAGS) -DHAVE_CONFIG_H -std=c++20

.PHONY : everything all objs clean veryclean rebuild

everything: $(TARGET)

all: $(TARGET)

objs: $(OBJS)

rebuild: veryclean everything

$(TARGET): $(OBJS)
	$(CC) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS) $(LIBS)

clean:
	rm -fr src/*.so
	rm -fr src/*.o
    
veryclean: clean
	rm -fr $(TARGET)