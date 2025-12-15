# 宿舍看门狗 v2.0 代码
**本仓库仅供存档**

## 代码内容说明

**代码是UTF-8编码，如果用别的编码保存，控制器显示的中文会变成乱码。**

代码改自之前开源的[NRF24L01驱动示例工程](https://github.com/MrWei95/NRF24L01-STM32-Driver/tree/main/Standard%20Peripheral%20Library/Example%20Project)，对代码进行了一些优化。

工程为发送 + 接收二合一代码，使用```NRF24L01_MODE```控制模式，具体实现：

```C
// 控制NRF24L01收发模式
#define		NRF24L01_MODE		0
#if NRF24L01_MODE == 0		// NRF24L01收发模式判断
#define		NRF24L01_TX_MODE		// 发送模式
#else
#define		NRF24L01_RX_MODE		// 接收模式
#endif	// NRF24L01收发模式判断
```
