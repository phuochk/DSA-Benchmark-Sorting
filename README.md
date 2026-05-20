# BÁO CÁO ĐỒ ÁN: BENCHMARK SORTING ALGORITHMS

## 1. Thông tin chung
* **Tên môn học:** Cấu trúc dữ liệu và giải thuật
* **Thời gian học:** Học kỳ 2, Năm học 2025 - 2026
* **Họ và tên sinh viên:** Nguyễn Bá Phước
* **MSSV:** 25120422

---

## 2. Thuật toán cài đặt tốt nhất ở Lần 1 (Phase 1)
Trong lần chạy đầu tiên, ưu tiên hàng đầu là tính đúng đắn và sự ổn định tổng quát. Các thuật toán được chọn:

* **Bài A (int - Integer Sort): Quick Sort với Phân hoạch Hoare.**
  * **Tối ưu hóa:** Sử dụng vòng lặp `while` thay vì phân hoạch Lomuto, kết hợp kỹ thuật chọn chốt Median-of-Three.
  * **Lý do tốt nhất:** Phân hoạch Hoare sử dụng hai con trỏ quét ngược chiều nhau, điều này cực kỳ thân thiện với bộ nhớ đệm (Cache L1/L2) của CPU. Thuật toán hoạt động In-place (O(1) không gian), không tốn thời gian cấp phát bộ nhớ phụ như Merge Sort.
* **Bài B (strlexi) & C (strlenlexi): Merge Sort thao tác trên mảng con trỏ.**
  * **Tối ưu hóa:** Thay vì hoán vị các chuỗi ký tự lớn, thuật toán chỉ hoán vị các con trỏ `char*` (hoặc chỉ số mảng) trỏ đến các chuỗi đó. 
  * **Lý do tốt nhất:** Các thuật toán cơ bản khác như Quick Sort chuỗi rất dễ bị suy biến thành O(N^2) khi gặp mảng chứa nhiều chuỗi trùng lặp hoặc mảng đã sắp xếp. Merge Sort luôn đảm bảo độ phức tạp thời gian O(N log N) trong mọi trường hợp (Worst-case), an toàn để qua toàn bộ các test cơ bản.

---

## 3. Chiến thuật sinh Test Case trong `test_gen.cpp`
File `test_gen.cpp` sử dụng thư viện `<random>` (chuẩn C++23) để tạo ra các bộ test bẫy (Killer Tests). Mục tiêu của các test này là đánh vào điểm yếu kiến trúc (trượt dự đoán rẽ nhánh, chi phí gọi hàm so sánh, suy biến độ phức tạp) của các thuật toán chuẩn như `std::sort` hoặc Quick Sort / Merge Sort cơ bản, từ đó làm tăng mạnh thời gian chạy của chúng.

### Bài A (int): Ép suy biến phân hoạch
* **Test 1 (Random):** Đánh giá tốc độ trần của thuật toán.
* **Test 2 (Zig-zag):** Giá trị Max và Min đan xen liên tục. Mục tiêu đánh lừa các thuật toán Quick Sort có bộ chọn chốt (pivot) cố định ở đầu/giữa/cuối, làm phân hoạch mất cân bằng nghiêm trọng.
* **Test 3 (Massive Duplicates):** Mảng 100.000 phần tử nhưng chỉ có 3 giá trị (-1, 0, 1). Ép Quick Sort không có cơ chế 3-way partition rơi vào vòng lặp vô hạn hoặc suy biến thành O(N^2).
* **Test 4 (Reverse) & Test 5 (Nearly Sorted):** Ép Insertion Sort và Quick Sort phải duyệt qua các mảng cực đoan, gây ra Branch Misprediction (Dự đoán sai rẽ nhánh) ngắt quãng CPU.

### Bài B (strlexi): Vắt kiệt hàm so sánh chuỗi
* **Test 1 (Deep Prefix):** Các chuỗi giống hệt nhau ở 98 ký tự đầu, chỉ khác 2 ký tự cuối. Mục đích: Ép hàm `strcmp` của Lần 1 phải duyệt 98 vòng lặp thừa thãi cho mỗi lần so sánh hai chuỗi.
* **Test 2 (Clone Bomb):** Toàn bộ chuỗi nhân bản giống nhau 100%. Gây tràn Stack cho các hàm đệ quy rẽ nhánh kém.
* **Test 3 (Binary Alphabet):** Chuỗi chỉ chứa ký tự 'a' và 'b'. Dồn cục dữ liệu, phá vỡ tính phân tán của Radix Sort thông thường.
* **Test 4 (Length Stairs) & Test 5 (Reverse):** Bẫy lỗi tràn viền (Out-of-bounds) và bắt thuật toán chia nhỏ dữ liệu liên tục.

### Bài C (strlenlexi): Phá vỡ logic phân nhóm (Bucket)
* **Test 1 (Len Vs Lex Trap):** Chuỗi ngắn chứa ký tự lớn 'z', chuỗi dài chứa ký tự nhỏ 'a'. Đánh lừa thuật toán ưu tiên sai điều kiện (so sánh nội dung trước khi đo chiều dài).
* **Test 2 (All Max Length):** Toàn bộ chuỗi dài 100 ký tự. Trực tiếp vô hiệu hóa khả năng chia nhóm (Bucket Sort) theo chiều dài, ép bài toán quay về độ phức tạp so sánh từ điển khổng lồ.
* **Test 3, 4, 5 (Shifted, Periodic, Cross Duplicates):** Gây nhiễu loạn cho con trỏ bộ nhớ và làm chậm quá trình băm (hashing) chuỗi.

---

## 4. Thuật toán cài đặt tốt nhất ở Lần 2 (Phase 2)
Để đối phó với các Killer Tests tự sinh ra ở phần 3, các thuật toán ở Lần 2 được thay thế hoàn toàn bằng các cấu trúc không sử dụng phép so sánh trực tiếp, đẩy độ phức tạp về mức tuyến tính O(N).

* **Bài A (int): Radix Sort Base 65536 (Cơ số 16-bit).**
  * **Cách thức tối ưu:** Thay vì chia số nguyên 32-bit thành 4 byte (phải lặp 4 lần), thuật toán gom thành 2 khối 16-bit. Do đó, toàn bộ 100.000 phần tử được sắp xếp chỉ trong đúng **2 vòng lặp (2 passes)**.
  * **Vượt trội so với Lần 1:** Tránh hoàn toàn việc so sánh chéo O(N log N) của Quick Sort. Xử lý triệt để bẫy mảng trùng lặp (Massive Duplicates) và Zig-zag với thời gian cố định O(N). Tích hợp thủ thuật dịch bit (Bitwise XOR) để đổi số âm sang không dấu, loại bỏ lỗi điều kiện.
  
* **Bài B (strlexi): Multikey Quicksort (3-way String Quicksort).**
  * **Cách thức tối ưu:** Thay vì dùng `strcmp` so sánh toàn bộ chuỗi nhiều lần, Multikey Quicksort chỉ so sánh từng ký tự từ trái qua phải. Nếu ký tự đầu đã khác nhau, thuật toán chia nhánh và **bỏ qua vĩnh viễn** phần đuôi của chuỗi đó. 
  * **Vượt trội so với Lần 1:** Tiêu diệt triệt để bẫy "Tiền tố chung sâu" (Deep Prefix). Merge Sort ở Lần 1 phải chạy O(L * N log N), trong khi thuật toán này chạm ngưỡng O(N). Hoạt động hoàn toàn In-place trên mảng con trỏ, tối ưu hóa Cache Miss.

* **Bài C (strlenlexi): Bucket Sort + LSD Radix Sort/Multikey.**
  * **Cách thức tối ưu:** Đọc độ dài chuỗi một lần duy nhất bằng $O(N)$ và phân bổ vào các Bucket (head/next_idx) theo chiều dài (từ 10 đến 100). Sau đó, chỉ sắp xếp từ điển nội bộ trong các Bucket có nhiều hơn 1 phần tử.
  * **Vượt trội so với Lần 1:** Gỡ hoàn toàn nút thắt phải so sánh chiều dài liên tục ở Lần 1. Khi kết hợp cùng Custom Fast I/O (sử dụng `getchar` cấp thấp thay cho `std::cin` và vô hiệu hóa `std::vector`), thuật toán Lần 2 lướt qua các bẫy All Max Length và Length Entropy với thời gian gần như tuyệt đối do loại bỏ 100% chi phí overhead của thư viện chuẩn.
