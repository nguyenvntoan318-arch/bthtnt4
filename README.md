# btt4 - STM32F103 + FreeRTOS

## Yêu cầu
- STM32F103 (ví dụ STM32F103C8T6)
- ST-LINK V2
- Ubuntu/Linux
- arm-none-eabi-gcc
- make
- openocd

## Chức năng
Ba FreeRTOS task chạy độc lập:
- PA0: 0.1 Hz
- PA1: 1 Hz
- PA2: 10 Hz

Hàm chung:
`LED_Blink(pin, half_period_ms)`

Tần số được tính theo:
`f = 1 / T`
và LED đổi trạng thái mỗi `T/2`.

## Lấy FreeRTOS Kernel
Chạy:
```bash
./get_freertos.sh
```

Sau đó build:
```bash
make clean
make
```

Flash:
```bash
make flash
```

## Kết nối LED
Mỗi chân PA0, PA1, PA2 nối tới LED + điện trở hạn dòng. Có thể dùng LED active-high:
PAx -> resistor -> LED -> GND.

## Lưu ý
Project dùng GPIO thanh ghi trực tiếp để giảm phụ thuộc thư viện STM32. FreeRTOS sử dụng ARM Cortex-M3 GCC port.
