# CXX Make variable for compiler
CXX=g++
# -std=c++11  C/C++ variant to use, e.g. C++ 2011
# -Wall       show the necessary warning files
# -g3         information for symbolic debugger e.g. gdb 
CXXFLAGS=-std=c++11 -Wall -g3 -c
LDFLAGS=-pthread
INCLUDE=-I./

# object files
OBJS = log_helpers.o main.o args_handling.o main_functionality.o page_functions.o page_node.o page_table.o vaddr_tracereader.o wsclock_algorithm.o  
HEADERS = log_helpers.h args_handling.h main_functionality.h page_functions.h page_node.h page_table.h vaddr_tracereader.h wsclock_algorithm.h 
# Program name
PROGRAM = demandpaging

# Rules format:
# target : dependency1 dependency2 ... dependencyN
#     Command to make target, uses default rules if not specified

# First target is the one executed if you just type make
# make target specifies a specific target
# $^ is an example of a special variable.  It substitutes all dependencies
$(PROGRAM) : $(OBJS) $(HEADERS)
	$(CXX) -o $(PROGRAM) $^ $(LDFLAGS)

log_helpers.o : log_helpers.h log_helpers.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) log_helpers.cpp
	
args_handling.o : args_handling.h args_handling.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) args_handling.cpp
	
main_functionality.o : main_functionality.h main_functionality.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) main_functionality.cpp
	
page_functions.o : page_functions.h page_functions.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) page_functions.cpp

page_node.o : page_node.h page_node.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) page_node.cpp
	
page_table.o : page_table.h page_table.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) page_table.cpp
	
vaddr_tracereader.o : vaddr_tracereader.h vaddr_tracereader.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) vaddr_tracereader.cpp

wsclock_algorithm.o : wsclock_algorithm.h wsclock_algorithm.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) wsclock_algorithm.cpp

main.o : log_helpers.cpp main_functionality.cpp page_functions.cpp args_handling.cpp main.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) main.cpp
	
clean :
	rm -f *.o $(PROGRAM)
