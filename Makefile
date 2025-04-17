CXX = clang++
CXXFLAGS = -std=c++17 -O2 -Wall
CPU_TARGET = blur_cpu
CPU_SRCS = blur_cpu.cpp

NVCC = nvcc
NVCCFLAGS = -std=c++17
GPU_TARGET = blur_cuda
GPU_SRCS = blur_cuda.cu

all: $(CPU_TARGET)

$(CPU_TARGET): $(CPU_SRCS)
	$(CXX) $(CXXFLAGS) -o $(CPU_TARGET) $(CPU_SRCS)

gpu: $(GPU_TARGET)

$(GPU_TARGET): $(GPU_SRCS)
	$(NVCC) $(NVCCFLAGS) -o $(GPU_TARGET) $(GPU_SRCS)

clean:
	rm -f $(CPU_TARGET) $(GPU_TARGET) *.ppm
