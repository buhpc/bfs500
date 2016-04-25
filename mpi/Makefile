.SUFFIXES:
.SUFFIXES: .o .cpp
#============================================================
TARGET	=  bfs_omp

C_SOURCES =  bfs_omp.cpp 
C_OBJS     =  bfs_omp.o
MY_INCLUDES =  bfs_omp.h

CCX = g++
CXXFLAGS = -g -Wall -std=c++0x -lrt -fopenmp

#============================================================
all: $(TARGET)

.o:.cpp	$(MY_INCLUDES)
	$(CCX)  -c  $(CXXFLAGS) $<  

$(TARGET) :   $(C_OBJS)
	$(CCX) $(CXXFLAGS)  $^ $(LIBDIRS)  -o $@

# Implicit rules: $@ = target name, $< = first prerequisite name, $^ = name of all prerequisites
#============================================================

ALL_SOURCES = Makefile $(C_SOURCES) $(MY_INCLUDES)


clean:
	rm -f $(TARGET) $(C_OBJS) core 

tar: $(ALL_SOURCES) $(NOTES)
	tar cvf $(TARGET).tar $(ALL_SOURCES)  $(NOTES)

$(TARGET).ps: $(ALL SOURCES)
	enscript -pcode.ps $(ALL_SOURCES)