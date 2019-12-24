CXX=g++
CXXFLAGS=-std=c++11 -framework OpenCL

all: src/*.cpp src/*.hpp
	mkdir -p build
	$(CXX) $(CXXFLAGS) src/*.cpp -o build/pt

clean:
	rm -r build
