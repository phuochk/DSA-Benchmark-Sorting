# Báo Cáo Đồ Án: Tối Ưu Thuật Toán Sắp Xếp

## Thông tin cá nhân
- **Tên môn học:** Cấu trúc dữ liệu và giải thuật
- **Thời gian học:** 2025-2026
- **Họ tên sinh viên:** Nguyễn Bá Phước
- **MSSV:** 25120422

---

## 1. Thuật toán cài đặt tốt nhất ở lần chạy đầu tiên (Phase 1)

Ở giai đoạn 1, các thuật toán được lựa chọn tập trung vào việc vượt qua giới hạn của các hàm sắp xếp truyền thống ($O(N \log N)$) bằng cách khai thác đặc trưng của dữ liệu.

* **Bài `int` (Sắp xếp số nguyên): Radix Sort (Cơ số 256)**
    * **Cài đặt & Tối ưu:** Thuật toán phân tách số nguyên 32-bit thành 4 byte để đếm phân phối (Counting Sort) 4 lần. Để xử lý số âm, mảng được XOR với `1 << 31` (đảo bit dấu) trước và sau khi sắp xếp.
    * **Lý do tốt nhất:** Radix Sort cho độ phức tạp thời gian $O(d \times N)$ với $d=4$ (rất nhỏ), vượt trội hoàn toàn so với Quick Sort hay Merge Sort. 
* **Bài `strlexi` (Sắp xếp chuỗi theo từ điển): MSD Radix Sort + Fallback Insertion Sort**
    * **Cài đặt & Tối ưu:** Thuật toán duyệt chuỗi từ trái sang phải. Chia dữ liệu vào 28 bucket (ký tự 'a'-'z' và `\0`). Khi kích thước bucket con $\le 16$, thuật toán tự động chuyển sang Insertion Sort để tránh overhead đệ quy. 
    * **Lý do tốt nhất:** Việc chia để trị theo từng ký tự giúp loại bỏ hoàn toàn các phép so sánh chuỗi lặp lại, hiệu quả cực cao khi có nhiều chuỗi trùng tiền tố.
* **Bài `strlenlexi` (Sắp xếp theo độ dài, rồi đến từ điển): Bucket Sort + Multi-key Quicksort (MKQS)**
    * **Cài đặt & Tối ưu:** Sử dụng mảng danh sách liên kết (`head` và `next_idx`) để đưa các chuỗi có cùng độ dài vào từng xô (Bucket Sort) tốn $O(N)$. Sau đó, bên trong mỗi xô, sử dụng MKQS (Quicksort 3 chiều phân nhánh) để sắp xếp theo từ điển.
    * **Lý do tốt nhất:** Giải quyết triệt để tiêu chí 1 (độ dài) trong thời gian tuyến tính. Ở tiêu chí 2, MKQS phân cụm các ký tự giống nhau rất nhanh, tránh được chi phí gọi hàm so sánh chậm chạp của `std::sort`.

---

## 2. Sinh Test Case (`test_gen.cpp`)

File `test_gen.cpp` cung cấp command-line interface nhận tham số đầu vào (`argc, argv`) để sinh bộ test tương ứng qua cú pháp: `test_gen.<ext> <int/strlexi/strlenlexi> <1/2/3/4/5>`.

**Chiến lược sinh test case nhằm làm chậm các thuật toán mục tiêu:**

* **Bộ test cho `int` (Nhắm vào Quicksort & Radix Sort lỗi bit):**
  * *Test 2 (Xen kẽ max dương và min âm):* Đánh lừa các phiên bản Radix Sort không xử lý tốt bit dấu (MSB).
  * *Test 3 & 4 (Trùng lặp nhiều `-1, 0, 1` & Mảng đảo ngược):* Đẩy Quicksort 2-way chuẩn về độ phức tạp $O(N^2)$.
  * *Test 5 (Gần như đã sắp xếp, chỉ swap 1000 phần tử):* Gây khó dễ cho các thuật toán phân hoạch ngẫu nhiên.
* **Bộ test cho `strlexi` (Nhắm vào String Sort thông thường & MSD Radix):**
  * *Test 1 (Tiền tố chung siêu dài):* 98 ký tự 'a' giống hệt nhau, chỉ khác ở 2 ký tự cuối. Ép MSD Radix Sort phải đệ quy gọi hàm cấp phát bucket tới 98 lần vô ích, tạo overhead khổng lồ.
  * *Test 2 (Chuỗi giống hệt nhau):* Kiểm tra khả năng xử lý trùng lặp, chặn đứng các thuật toán đệ quy không có điểm dừng.
  * *Test 3 (Bảng chữ cái nhị phân 'a', 'b'):* Làm chậm các thuật toán chia 26 xô vì chúng sẽ duyệt qua 24 xô trống vô ích ở mỗi tầng.
* **Bộ test cho `strlenlexi` (Nhắm vào sai lầm logic & Merge Sort):**
  * *Test 1 (Độ dài xen kẽ):* Sinh 10 ký tự 'z' và 100 ký tự 'a'. Kiểm tra nghiêm ngặt xem thuật toán có ưu tiên độ dài trước từ điển hay không (chuỗi ngắn toàn 'z' phải đứng trước chuỗi dài toàn 'a').
  * *Test 5 (Tập mẫu nhỏ lặp lại nhiều lần):* Ép thuật toán phải xử lý số lượng phần tử trùng lặp cực lớn, làm cạn kiệt bộ nhớ nếu dùng các phiên bản Merge Sort không tối ưu.

---

## 3. Thuật toán cài đặt tốt nhất ở lần thứ hai (Phase 2) & Phương pháp tối ưu tiếp nối

Ở Phase 2, thuật toán cốt lõi được giữ lại nhưng được nâng cấp bằng kỹ thuật quản lý bộ nhớ cục bộ và **Custom Fast I/O**, phá vỡ mọi giới hạn trễ của hệ điều hành.

* **Bài `int`:**
    * *Cải tiến:* Thay vì dùng `cin/cout`, đọc toàn bộ stdin vào buffer bằng `fread`, tự parsing chuỗi thành số nguyên. Output được ghi vào `out_buf` và xuất ra bằng 1 lệnh `fwrite`.
    * *Tối ưu:* Giảm thiểu triệt để thời gian chết do I/O, biến bài toán thành In-memory processing thuần túy.
* **Bài `strlexi`:**
    * *Cải tiến:* Kết hợp MSD Radix Sort với mảng con trỏ. Chuỗi không được copy mà chỉ lưu con trỏ trỏ thẳng vào `buf` đầu vào. Ký tự ngắt (space/newline) được thay thành `\0`. Hàm fallback Insertion Sort dùng con trỏ để duyệt thủ công thay vì dùng `strcmp`.
    * *Tối ưu:* Loại bỏ 100% chi phí sao chép dữ liệu và cấp phát động, thuật toán chỉ hoán đổi vị trí của các con trỏ nhẹ nhàng trên RAM.
* **Bài `strlenlexi`:**
    * *Cải tiến:* Chuyển từ mảng danh sách liên kết sang `std::vector<vector<string>>` để phân xô theo độ dài. Đột phá lớn nhất là thay thế MKQS bằng **Custom Merge Sort kết hợp `std::move()`**.
    * *Tối ưu:* Thay vì tốn kém tài nguyên copy chuỗi vào mảng tạm khi Merge, hàm `std::move()` trong C++11 chuyển giao quyền sở hữu bộ nhớ với độ phức tạp $O(1)$. Thuật toán sắp xếp mượt mà, ổn định (stable) và tận dụng được cache CPU tốt hơn rất nhiều so với lần 1.
