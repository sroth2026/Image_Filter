#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <cuda_runtime.h>

struct Pixel {
    unsigned char r, g, b;
};

struct Image {
    int width, height;
    std::vector<Pixel> data;
};

Image readPPM(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    std::string format;
    int width, height, maxval;

    file >> format >> width >> height >> maxval;
    file.ignore(1);

    std::vector<Pixel> pixels(width * height);
    file.read(reinterpret_cast<char*>(pixels.data()), pixels.size() * 3);

    return {width, height, pixels};
}

void writePPM(const std::string& filename, const Image& img) {
    std::ofstream file(filename, std::ios::binary);
    file << "P6\n" << img.width << " " << img.height << "\n255\n";
    file.write(reinterpret_cast<const char*>(img.data.data()), img.data.size() * 3);
}

__global__ void boxBlurKernel(Pixel* input, Pixel* output, int width, int height) {
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;

    if (x < 1 || x >= width - 1 || y < 1 || y >= height - 1)
        return;

    int r = 0, g = 0, b = 0;

    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            int idx = (y + dy) * width + (x + dx);
            r += input[idx].r;
            g += input[idx].g;
            b += input[idx].b;
        }
    }

    int outIdx = y * width + x;
    output[outIdx].r = r / 9;
    output[outIdx].g = g / 9;
    output[outIdx].b = b / 9;
}

Image boxBlurCUDA(const Image& input) {
    int imgSize = input.width * input.height * sizeof(Pixel);

    Pixel *d_input, *d_output;
    cudaMalloc(&d_input, imgSize);
    cudaMalloc(&d_output, imgSize);

    cudaMemcpy(d_input, input.data.data(), imgSize, cudaMemcpyHostToDevice);

    dim3 blockSize(16, 16);
    dim3 gridSize((input.width + 15) / 16, (input.height + 15) / 16);

    boxBlurKernel<<<gridSize, blockSize>>>(d_input, d_output, input.width, input.height);
    cudaDeviceSynchronize();

    Image output = input;
    cudaMemcpy(output.data.data(), d_output, imgSize, cudaMemcpyDeviceToHost);

    cudaFree(d_input);
    cudaFree(d_output);

    return output;
}

int main() {
    std::string inputFile = "sample.ppm";
    std::string outputFile = "blurred_output_cuda.ppm";

    Image img = readPPM(inputFile);

    auto start = std::chrono::high_resolution_clock::now();
    Image blurred = boxBlurCUDA(img);
    auto end = std::chrono::high_resolution_clock::now();

    writePPM(outputFile, blurred);

    std::chrono::duration<double> elapsed = end - start;
    std::cout << "GPU blur took " << elapsed.count() << " seconds\n";

    return 0;
}
