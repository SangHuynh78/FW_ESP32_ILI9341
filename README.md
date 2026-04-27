# 🚀 Hệ Thống Đồ Họa 3D trên ESP32 + TFT ILI9341

> **Mục tiêu:** Xây dựng hệ thống render khối lập phương 3D tương tác trên ESP32, cho phép người dùng xoay khối theo thời gian thực thông qua thao tác trên màn hình cảm ứng.

<p align="center">
  <img src="docs/images/final_project_expectation.png" width="600"/>
</p>


## 📋 Mục Lục

- [⚙️ Quy Trình Phát Triển Sản Phẩm (V-Model)](#-quy-trình-phát-triển-sản-phẩm-v-model)
- [📌 Tổng Quan](#-tổng-quan)
- [🧠 Kiến Trúc Hệ Thống](#-kiến-trúc-hệ-thống)
- [📁 Cấu Trúc Project](#-cấu-trúc-project)
- [⚙️ Yêu Cầu Hệ Thống](#-yêu-cầu-hệ-thống)
- [🔧 Hướng Dẫn Build](#-hướng-dẫn-build)
- [🧩 Thành Phần Chính](#-thành-phần-chính)
- [🎥 Ví Dụ và Demo](#-ví-dụ-và-demo)
- [📚 Tài Liệu Tham Khảo](#-tài-liệu-tham-khảo)
- [📄 License](#-license)

## [⚙️ Quy Trình Phát Triển Sản Phẩm (V-Model)](#-quy-trình-phát-triển-sản-phẩm-v-model)
<p align="center">
  <img src="docs/images/v_model.png" width="600"/>
</p>

### ⚙️ Cách làm việc: Phương pháp V-Model

Để chạy được 3D trên ESP32 mà không bị crash, chúng tôi sử dụng một phiên bản V-Model đơn giản hóa. Nguyên tắc cốt lõi: Nghĩ kỹ, code nhanh, test sớm.

### 1. Thiết kế trước, code sau (Nhánh trái)
* **Kiến Trúc:** Thống nhất cấu trúc thư mục, Sơ đồ Task FreeRTOS, Thiết kế Hardware Abstraction Layer (HAL), 
* **Tài Liệu:** Tạo thư mục docs/ quản lý tài liệu, Thống nhất API Template để comment code, mô tả input/output của mỗi hàm.
* **Thuật Toán và Logic:** Flowchart Render 3D, SPI Timing & Config

### 2. Code có kỷ luật (Phần đáy)
* **Thiết lập kho lưu trữ GitHub:** Khởi tạo Repo, tạo sẵn các nhánh main, develop, phân vùng phân quyền. (Quy định Pull Request)
* **Quy ước lập trình:** Chốt quy tắc đặt tên, định nghĩa các quy tắt, cách kiểm tra các quy tắc (Phần toán 3D được tách hoàn toàn khỏi driver hiển thị ILI9341)

### 3. Test từng bước (Nhánh phải)
* **Test thuật toán trên PC trước:** Setup môi trường PC, Viết kịch bản test ma trận.
* **Test tích hợp & Hiệu năng:** Viết Task Monitor, Test màn hình cơ bản, Tích hợp Touch - Render
* **Theo dõi hệ thống:** Log liên tục qua Serial để phát hiện tụt FPS và kiểm tra bộ nhớ heap (truy vết memory leak).
* **CI/CD (Tự động hóa):** Cấu hình GitHub Actions

### 🗣️ Quy tắc team:
* **Cách báo cáo tiến độ:**
* **Cách báo cáo vấn đề:**
* **Cách tạo nhiệm vụ:**
* **Định nghĩa nhiệm vụ đã hoàn thành:**
* **Quy tắc sử dụng Git:**



## [📌 Tổng Quan](#-tổng-quan)
## [🧠 Kiến Trúc Hệ Thống](#-kiến-trúc-hệ-thống)
```text
┌─────────────────────────────────────────────────────────────┐
│                    APPLICATION LAYER                        │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐       │
│  │ System Apps  │  │  User Apps   │  │   Examples   │       │
│  │ - Sys Monitor│  │ - 3D Cube    │  │ - Touch Test │       │
│  │   (FPS/Heap) │  │   Interact   │  │ - Wireframe  │       │
│  └──────────────┘  └──────────────┘  └──────────────┘       │
└─────────────────────────────────────────────────────────────┘
                            ▲
                            │ Uses Core Engine API & Queues
                            │
┌─────────────────────────────────────────────────────────────┐
│                    3D CORE ENGINE & RTOS                    │
│  ┌──────────────────────────────────────────────────┐       │
│  │  FreeRTOS Task Management                        │       │
│  │  - Task_Display (High Priority)                  │       │
│  │  - Task_Math3D  (Medium Priority)                │       │
│  │  - Task_Touch   (Low Priority)                   │       │
│  └──────────────────────────────────────────────────┘       │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐       │
│  │   3D Math    │  │Graphics Rndr │  │ Event/Queue  │       │
│  │(Matrix/Proj) │  │(Line/Buffer) │  │(Touch Data)  │       │
│  └──────────────┘  └──────────────┘  └──────────────┘       │
└─────────────────────────────────────────────────────────────┘
                            ▲
                            │ Uses HAL Interface (Hardware Agnostic)
                            │
┌─────────────────────────────────────────────────────────────┐
│              HARDWARE ABSTRACTION LAYER (HAL)               │
│  ┌──────────────────────────────────────────────────┐       │
│  │  Hardware Interfaces                             │       │
│  │  - IDisplay (PushFrame, DrawPixel, SetWindow)    │       │
│  │  - ITouch   (GetCoordinates, isTouched)          │       │
│  │  - ISystem  (GetTicks, Delay)                    │       │
│  └──────────────────────────────────────────────────┘       │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐       │
│  │   ILI9341    │  │   XPT2046    │  │   (Future)   │       │
│  │ (SPI & DMA)  │  │  (SPI / I2C) │  │ ST7789/FT... │       │
│  └──────────────┘  └──────────────┘  └──────────────┘       │
└─────────────────────────────────────────────────────────────┘
                            ▲
                            │
                    ┌───────┴────────┐
                    │ ESP-IDF / RTOS │
                    │ SPI, DMA, GPIO │
                    └────────────────┘
```
## [📁 Cấu Trúc Project](#-cấu-trúc-project)
```text
esp32_3dcube_platform/
│
├── docs/                           # Tài liệu dự án (Document-Driven)
│   ├── hardware/                   # Sơ đồ pinout, timing diagram SPI
│   ├── software/                   # Flowchart thuật toán, state machine
│   └── doxygen_config/             # Cấu hình sinh API docs tự động
│
├── hal/                            # Hardware Abstraction Layer (Phần cứng)
│   ├── include/hal/                # HAL Interfaces (Header files)
│   │   ├── display_hal.h           # Interface chuẩn cho mọi màn hình
│   │   ├── touch_hal.h             # Interface chuẩn cho mọi chip cảm ứng
│   │   └── spi_bus.h               # Interface quản lý bus SPI & DMA
│   ├── src/
│   │   ├── ili9341/                # Driver cụ thể cho ILI9341
│   │   │   ├── ili9341_init.c      # Khởi tạo thanh ghi (Landscape/Portrait)
│   │   │   └── ili9341_render.c    # Giao tiếp DMA đẩy Framebuffer
│   │   ├── xpt2046/                # Driver cụ thể cho cảm ứng Touch
│   │   │   └── xpt2046.c           # Đọc tọa độ, lọc nhiễu (Debounce)
│   │   └── board_config.c          # Mapping chân GPIO thực tế của ESP32
│   └── CMakeLists.txt
│
├── core_engine/                    # Logic cốt lõi (Không dính dáng phần cứng)
│   ├── include/engine/
│   │   ├── math_3d.h               # Khai báo cấu trúc Vector, Matrix
│   │   └── render_pipeline.h       # Logic Rasterization (Vẽ đoạn thẳng)
│   ├── src/
│   │   ├── math/
│   │   │   ├── matrix.c            # Các hàm nhân ma trận xoay
│   │   │   └── projection.c        # Ép tọa độ 3D (Z) xuống mặt phẳng 2D
│   │   └── graphics/
│   │       ├── wireframe.c         # Thuật toán vẽ khung dây
│   │       └── framebuffer.c       # Quản lý bộ đệm màu (Internal RAM)
│   └── CMakeLists.txt
│
├── main/                           # Application Layer (Luồng chạy chính)
│   ├── src/
│   │   ├── tasks/                  # Các FreeRTOS Tasks độc lập
│   │   │   ├── task_touch.c        # Task lấy mẫu cảm ứng liên tục
│   │   │   ├── task_math_3d.c      # Task xử lý toán học dựa trên tọa độ
│   │   │   └── task_display.c      # Task đẩy dữ liệu ra màn hình
│   │   └── main.c                  # Khởi tạo hệ thống, tạo Tasks
│   └── CMakeLists.txt
│
├── tests/                          # Nhánh phải của V-Model (Kiểm thử)
│   ├── unit_tests/                 # Chạy trực tiếp trên PC (Host)
│   │   ├── test_math_3d.c          # Đút số giả lập, kiểm tra ma trận xoay
│   │   └── test_projection.c       # Test thuật toán chiếu 2D
│   ├── target_tests/               # Chạy trên chip ESP32
│   │   ├── test_spi_dma.c          # Đo tốc độ đẩy data ra ILI9341
│   │   └── test_memory_leak.c      # Monitor RAM khi xoay 3D liên tục
│   └── CMakeLists.txt
│
├── tools/                          # Công cụ hỗ trợ
│   ├── pc_simulator/               # Dùng SDL2/OpenGL test code C trên PC
│   └── monitor_fps.py              # Script Python đọc Serial vẽ biểu đồ FPS
│
├── .gitignore
├── sdkconfig.defaults              # Cấu hình ESP-IDF (VD: Tăng clock SPI)
└── README.md
```
## [⚙️ Yêu Cầu Hệ Thống](#-yêu-cầu-hệ-thống)
### Phần Cứng
- **CPU**:
- **RAM**: 
- **Camera**:

### Phần Mềm
- **OS**:
- **Compiler**:
- **CMake**:
- **Libraries**:


### Cho Development
- Git
- Make / Ninja
- (Optional)

## [🔧 Hướng Dẫn Build](#-hướng-dẫn-build)
## [🧩 Thành Phần Chính](#-thành-phần-chính)
## [🎥 Ví Dụ và Demo](#-ví-dụ-và-demo)
## [📚 Tài Liệu Tham Khảo](#-tài-liệu-tham-khảo)
## [📄 License](#-license)

