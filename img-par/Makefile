CXX=g++

CPPFLAGS = -std=c++17 -fopenmp -Wall -Wextra -Werror -Wno-deprecated -pedantic -pedantic-errors
OPTIMIZE_FLAG = -O3 -DDEBUG

all: tool
	@echo "***************************"
	@echo "Compilation successfully!"
	@echo ""

tool: image-par.o
	$(CXX) $(CPPFLAGS) $(OPTIMIZE_FLAG) -o image-par image-par.cpp bmpImage.cpp

debug: image-par.o
	$(CXX) -g -o image-par image-par.cpp bmpImage.cpp
	@echo "***************************"
	@echo "Compilation successfully!"
	@echo ""

clean:
	rm -f image-par
	rm *.o
	@echo "***************************"
	@echo "Deleted files!"
	@echo  ""