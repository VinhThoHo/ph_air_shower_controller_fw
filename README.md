# stm32_biological_safety_class1_fw

# Thông số kỹ thuật biological_safety_class1 controller

## Tính năng:
- Điều khiển 3 thiết bị UV, Neon, FAN bằng nút nhấn
- Có chế độ tự động, nhấn nút Auto. Thời gian cài đặt tự động 00 - 99 phút
- Chế độ tự động bật đèn UV theo thời gian Auto Time (đèn UV bật, đèn Neon Tắt, Quạt tắt).
- Hiển thị màn hình lcd graphic (thời gian thực, trạng thái đèn UV, Neon, Fan, mode tự động, cảnh báo thay bộ lọc, thời gian hoạt động (UV, bộ lọc, hệ thống), đo chênh áp bộ lọc HEAP, nội suy tốc độ gió, hiển thị nhiệt độ cảm biến áp suất)
- Nhấn giữ nút đèn Neon hơn 15s để vào chế độ Menu Setting:
	- setup thời gian
	- setup thông số chênh áp cảnh báo bộ lọc
	- setup thời gian Auto
	- setup tốc độ quạt (Default, Fast, Medium, Slow), mặc định Default và Fast cùng relay (sử dụng cho loại quạt điều chỉnh tốc độ bằng Dimmer)
	- set buzzer on, off
	- setup độ sáng màn hình, backlight
	- xem thời gian UV, Filter, hệ thống đã hoạt động
	- Đặt lại thời gian hoạt động của UV, Neon, System khi thay đèn Neon, UV
	- Xem ngày sản xuất sản phẩm (làm lịch bảo hành khi lắp sản phẩm)
- Lưu data setup vào flash nội.
- Thời gian đếm sản phẩm lưu vào bộ nhớ RAM backup (đảm bảo lưu liên tục mỗi giây -> sai số thấp khi mất điện), sau mỗi lần vào menu setup, thời gian này sẽ được lưu vào flash, khi RAM backup hết pin, thì sẽ load thời gian từ flash lên (đảm bảo không bị mất nhiều thời gian khi hết pin RTC)
- Hỗ trợ mở rộng giao tiếp RS485 với PC baud 9600 - 1 stop bit - no-parity (Đang phát triến)

## Thông số kỹ thuật:
- 4 Button: Menu, UV (Down), Neon (Up), Auto (Exit), khi vào Menu thì phím chức năg trong ()
- Sử dụng VĐK: STM32F103RBT6 RAM: 20Kb, Flash: 128Kb (CodeBuild: RAM use: 9Kb, Flash use: 100Kb)
- LCD Graphic ST7920 (Xanh dương)
- 2 Relay output: (UV, Neon) (Relay 12V)
- Điện áp cung cấp 12V
- Sử dụng board 5 relay. (Neon, UV, Default/Fast, Medium, Slow)