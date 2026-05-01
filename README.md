# 🚀 Hệ Thống Đồ Họa 3D trên ESP32 + TFT ILI9341

> **Mục tiêu:** Xây dựng hệ thống render khối lập phương 3D tương tác trên ESP32, cho phép người dùng xoay khối theo thời gian thực thông qua thao tác trên màn hình cảm ứng.

<p align="center">
  <img src="docs/images/final_project_expectation.png" width="600"/>
</p>


## 📋 Mục Lục

- [⚙️ Quy Trình Phát Triển Sản Phẩm (V-Model)](#-quy-trình-phát-triển-sản-phẩm-v-model)


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