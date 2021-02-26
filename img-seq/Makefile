CXX=g++

CPPFLAGS = -std=c++17 -Wall -Wextra -Werror -Wno-deprecated -pedantic -pedantic-errors
OPTIMIZE_FLAG = -O0

optimize: OPTIMIZE_FLAG = -O3 -DDEBUG

all: tool
	@echo "***************************"
	@echo "Compilation successfully!"
	@echo ""

tool: image-seq.o
	$(CXX) $(CPPFLAGS) $(OPTIMIZE_FLAG) -o image-seq image-seq.cpp bmpImage.cpp

debug: image-seq.o
	$(CXX) -g -o image-seq image-seq.cpp bmpImage.cpp
	@echo "***************************"
	@echo "Compilation successfully!"
	@echo ""

optimize: all

clean:
	rm -f image-seq
	rm *.o
	@echo "***************************"
	@echo "Deleted files!"
	@echo  ""