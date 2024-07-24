CXX = g++
CXXFLAGS = -std=c++0x
CXXINCLUDES = .

main.exe: main.o
	$(CXX) $(CXXFLAGS) main.o -o main.exe

main.o: main.cpp BST.h unique_contraint_violation.h subtree_root_not_found.h subtree_extraction_failed.h sequence_to_bst_injection_error.h
	$(CXX) $(CXXFLAGS) -I$(CXXINCLUDES) -c main.cpp -o main.o

.PHONY: clear docs run_v
clear:
	rm *.o *.exe
docs:
	doxygen
run:
	make
	./main.exe
run_v:
	make
	valgrind --leak-check=full ./main.exe