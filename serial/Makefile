.SUFIXES:
.SUFIXES: .o .cpp

TARGET = bfs_serial
C_SOURCES = bfs_serial.cpp bfs_main.cpp
C_OBJS = bfs_serial.o bfs_main.o
MY_INCLUDES = bfs_serial.h bfs_main.cpp

CCX = g++
CXXFLAGS = -g -Wall -lrt -std=c++0x -O3

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

