# Si4455 RF 通信演示

本演示在**发送板**和**接收板**之间建立无线通信链接，使用**Si4455 RF 收发器**。它可以用于**机器人控制**等应用中的无线命令传输。

## 需求

- **硬件**
  - **PIC24F16KA102** *(或其他兼容的 PIC24 微控制器)* - 开发平台。
  - **Si4455 RF 收发器** - 无线通信模块。
- **软件**
  - **MPLAB X** - PIC 微控制器的集成开发环境（IDE）。
  - **XC16 编译器** - 用于 PIC24 的 C 编译器。
  - **Wireless Development Suite 3** *(可选)* - 无线电配置工具。

## IO 端口

| PIC24 引脚 | 功能     | 连接设备 | 说明            |
| ---------- | -------- | -------- | --------------- |
| RA6        | GPIO     | LED      | 控制 LED 指示灯 |
| RB7        | UART TX  | RS232    | UART 发送数据   |
| RB2        | UART RX  | RS232    | UART 接收数据   |
| RB11       | SPI SCLK | Si4455   | SPI 时钟        |
| RB13       | SPI SDI  | Si4455   | SPI 数据输入    |
| RB10       | SPI SDO  | Si4455   | SPI 数据输出    |
| RB15       | SPI nSel | Si4455   | SPI 片选        |
| RB14       | GPIO     | Si4455   | 4455 中断请求   |
| RA1        | GPIO     | Si4455   | 4455 硬重启     |

## 源文件

- **调试与 LED 控制**
  - `debug.h` & `debug.c` - 调试工具，用于显示 Si4455 信息和状态。
  - `led.h` & `led.c` - LED 控制，调试时有用。
- **无线电通信**
  - `radio.h` & `radio.c` - **Si4455** 的核心功能。
  - `radio_comm.h` & `radio_comm.c` - 简化**Si4455 库**通信。
  - `radio_hal.h` & `radio_hal.c` - 硬件抽象层（HAL），用于不同微控制器的接口。
  - `si4455_api_lib.h` & `si4455_api_lib.c` - **Si4455** 命令的 API 函数。
  - `si4455_defs.h` - **Si4455 通信参数**定义（请勿修改）。
  - `si4455_patch.h` - 用于初始化 **Si4455** 的补丁文件（由 `radio_config.h` 使用，请勿修改）。
- **通信接口**
  - `spi.h` & `spi.c` - **SPI 驱动**，用于无线模块通信。
  - `uart.h` & `uart.c` - **UART 驱动**，用于 PC 端通信。
- **其他**
  - `adc.h` & `adc.c` - ADC 初始化。
  - `clock.h` & `clock.c` - 时钟初始化和时间延迟。
  - `eeprom.h` & `eeprom.c` - EEPROM 相关函数。

# 配置文件

本项目包含两个主要的无线电配置文件，用于接收和发送无线数据，**修改文件名后使用**。
- **`radio_config_recv_433MHz_variable_length`**：此文件用于接收端配置，主要设置了接收频率（433 MHz）、数据包的可变长度等参数。
- **`radio_config_trans_433MHz_variable_length`**：此文件用于发送端配置，设置了发送频率（433 MHz）、数据包的可变长度等相关参数。

## 数据包结构

每个发送的数据包包含以下部分：

| 字段       | 大小      | 描述                                     |
| ---------- | --------- | ---------------------------------------- |
| **前导码** | 8 字节    | 帮助接收方检测信号存在（101010...模式）  |
| **同步字** | 2 字节    | 用于同步，确保正确的包对齐               |
| **长度**   | 1 字节    | 载荷的字节数（0-47 字节）                |
| **载荷**   | 0-47 字节 | 实际传输的数据（最大 47 字节）           |
| **CRC**    | 2 字节    | CRC-16-IBM（初始值：0xFFFF，高字节在前） |

**TX 和 RX 的 FIFO 阈值均设置为 48 字节**，意味着长度字段允许每个包最多包含 **47 字节** 载荷数据。FIFO 总大小为 **64 字节**，因此可以根据需要调整阈值至 64 字节。

## 常见问题

### **Q1: 如何修改无线电频率设置？**

无线电设置（包括频率和调制）配置在 `radio_config.h` 中。建议使用 **Wireless Development Suite (WDS)** 生成新的配置文件并更新此文件。也可以手动修改相关参数。

### **Q2: 为什么信号传输不稳定？**

请确保 **PIC24 和 Si4455 能正常通信**，并且两个 Si4455 模块使用**相同的发射/接收电路**。此外，检查以下几点：

- 确保**天线连接正确**。
- 检查**是否受到周围电子设备的干扰**。
- 调整**发射功率设置**以优化信号强度。
- 如果使用面包板，尝试**缩短 RF 模块的连线**，以减少干扰。

### **Q3: 为什么生成的无线电配置文件无法使用？**

请修改**包含路径**，并确保 `RADIO_CONFIGURATION_DATA` 的定义与项目匹配。

- 如果问题仍然存在，请检查 `radio_config.h` 是否包含所有必要的参数，并确保 WDS 导出后不需要额外修改。

## 参考资料

- [PIC24F16KA102 数据手册](https://ww1.microchip.com/downloads/en/DeviceDoc/39927c.pdf)
- [Si4455 数据手册](https://www.silabs.com/documents/public/data-sheets/Si4455.pdf)
- [EZRadio API Rev C2A](https://www.silabs.com/documents/public/application-notes/EZRadio_REVC2_API.zip)
- [AN685: Si4455/435x 布局设计指南](https://www.silabs.com/documents/public/application-notes/AN685.pdf)
- [AN692: Si4455/4355 编程指南及示例代码](https://www.silabs.com/documents/public/application-notes/AN692.pdf)
- [AN796: WDS一般说明](https://www.silabs.com/documents/public/application-notes/AN796.pdf)
- [AN797: EZRadio 设备 WDS 用户指南](https://www.silabs.com/documents/public/application-notes/AN797.pdf)