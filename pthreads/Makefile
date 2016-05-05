.SUFIXES:
.SUFIXES: .o .cpp

TARGET = bfs_pthread
C_SOURCES = bfs_pthread.cpp
C_OBJS = bfs_pthread.o
MY_INCLUDES = bfs_pthread.h

CCX = g++
CXXFLAGS = -g -lpthread -Wall -lrt -std=c++0x -O3

all: $(TARGET)
.o:.cpp $(MY_INCLUDES) 
	$(CCX) -c $(CXXFLAGS) $<
$(TARGET): $(C_OBJS) 
	$(CCX) $(CXXFLAGS) $^ $(LIBDIRS) -o $@

ALL_SOURCES = Makefile $(C_SOURCES) $(MY_INCLUDES)

clean:
	rm -rf $(TARGET) $(C_OBJS) core
tar: $(ALL_SOURCES) $(NOTES)
	tar cvf $(TARGET).tar $(ALL_SOURCES) $(NOTES)
$(TARGET).ps: $(ALL_SOURCES)
	enscript -pcode.ps $(ALL_SOURCES)

