#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

struct Pixel {
    unsigned char r, g, b;
};

struct Image {
    int width, height;
    std::vector<Pixel> data;
};

Image readPPM(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    std:: string format;
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


Image boxBlur(const Image& input) {
    Image output = input;
    for (int y = 1; y < input.height - 1; ++y) {
        for (int x = 1; x < input.width - 1; ++x) {
            int r = 0, g = 0, b = 0;
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    Pixel p = input.data[(y + dy) * input.width + (x + dx)];
                    r += p.r;
                    g += p.g;
                    b += p.b;
                }
            }
            Pixel& out = output.data[y * input.width + x];
            out.r = r / 9;
            out.g = g / 9;
            out.b = b / 9;
        }
    }
    return output;
}



int main() {
    std::string inputFile = "sample.ppm";
    std::string outputFile = "blurred_output.ppm";
    auto start = std::chrono::high_resolution_clock::now();
    Image img = readPPM(inputFile);
    Image blurred = boxBlur(img);
    writePPM(outputFile, blurred);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "CPU blur took " << elapsed.count() << " seconds\n";
    return 0;
}