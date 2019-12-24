CXX=g++
CXXFLAGS=-std=c++11 -framework OpenCL

all: src/*.cpp src/*.hpp src/render.cl build-scripts/*.py
	mkdir -p build
	python build-scripts/cl-to-cpp-constant-converter.py src/render.cl renderKernelCode > src/render-cl-kernel.hpp
	$(CXX) $(CXXFLAGS) src/*.cpp -o build/pt
	rm src/render-cl-kernel.hpp

clean:
	rm -r build
