# 🔍 Image Blur Filter — CPU & GPU (CUDA)

A simple C++ project that demonstrates how to apply a **box blur** to an image using both a **CPU-based implementation** and a **GPU-accelerated version via CUDA**.

Designed for educational and performance comparison purposes, this project shows how image processing tasks can benefit from GPU parallelism.

---

## 🛠 Features

- ✅ Load and save **PPM (P6)** images
- ✅ Apply a 3x3 **box blur** filter
- ✅ Compare **CPU vs GPU** performance
- ✅ Clean, readable C++ and CUDA code
- ✅ Cross-platform (Mac: CPU only; Linux w/ Nvidia GPU: CPU + GPU)

---

## 🚀 How to Run

### 💻 CPU Version (runs anywhere)

1. **Compile and run:**

```bash
make

./blur_cpu
