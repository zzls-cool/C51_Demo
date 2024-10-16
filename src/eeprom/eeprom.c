#include "../i2c/i2c.h"

/* 读取 EEPROM 中的一个字节，addr-字节地址 */
unsigned char E2ReadByte(unsigned char addr)
{
    unsigned char dat;
    I2CStart();
    I2CWrite(0x50 << 1);          // 寻址器件，后续为写操作
    I2CWrite(addr);               // 写入存储地址
    I2CStart();                   // 发送重复启动信号
    I2CWrite((0x50 << 1) | 0x01); // 寻址器件，后续为读操作
    dat = I2CReadNAK();           // 读取一个字节数据
    I2CStop();

    return dat;
}

void E2WriteByte(unsigned char addr, unsigned char dat)
{
    I2CStart();
    I2CWrite(0x50 << 1); // 寻址器件，后续为写操作
    I2CWrite(addr);      // 写入存储地址
    I2CWrite(dat);       // 写入一个字节数据
    I2CStop();
}

/* 将一段内存数据转换为十六进制格式的字符串，
 str-字符串指针，src-源数据地址，len-数据长度 */
static void MemToStr(unsigned char *str, unsigned char *src, unsigned char len)
{
    unsigned char tmp;
    while (len--)
    {
        tmp = *src >> 4; // 先取高 4 位
        if (tmp <= 9)    // 转换为 0-9 或 A-F
            *str++ = tmp + '0';
        else
            *str++ = tmp - 10 + 'A';
        tmp = *src & 0x0F; // 再取低 4 位
        if (tmp <= 9)      // 转换为 0-9 或 A-F
            *str++ = tmp + '0';
        else
            *str++ = tmp - 10 + 'A';
        *str++ = ' '; // 转换完一个字节添加一个空格
        src++;
    }
    *str = '\0'; // 添加字符串结束符
}
/* E2 读取函数，buf-数据接收指针，addr-E2 中的起始地址，len-读取长度 */
void E2Read(unsigned char *buf, unsigned char addr, unsigned char len)
{
    do
    { // 用寻址操作查询当前是否可进行读写操作
        I2CStart();
        if (I2CWrite(0x50 << 1)) // 应答则跳出循环，非应答则进行下一次查询
        {
            break;
        }
        I2CStop();
    } while (1);
    I2CWrite(addr);               // 写入起始地址
    I2CStart();                   // 发送重复启动信号
    I2CWrite((0x50 << 1) | 0x01); // 寻址器件，后续为读操作
    while (len > 1)               // 连续读取 len-1 个字节
    {
        *buf++ = I2CReadACK(); // 最后字节之前为读取操作+应答
        len--;
    }
    *buf = I2CReadNAK(); // 最后一个字节为读取操作+非应答
    I2CStop();
}
/* E2 写入函数，buf-源数据指针，addr-E2 中的起始地址，len-写入长度 */
void E2Write(unsigned char *buf, unsigned char addr, unsigned char len)
{
    while (len--)
    {
        do
        { // 用寻址操作查询当前是否可进行读写操作
            I2CStart();
            if (I2CWrite(0x50 << 1)) // 应答则跳出循环，非应答则进行下一次查询
            {
                break;
            }
            I2CStop();
        } while (1);
        I2CWrite(addr++); // 写入起始地址
        I2CWrite(*buf++); // 写入一个字节数据
        I2CStop();        // 结束写操作，以等待写入完成
    }
}

/* E2 写入函数，buf-源数据指针，addr-E2 中的起始地址，len-写入长度 */
void E2WritePage(unsigned char *buf, unsigned char addr, unsigned char len)
{
    while (len > 0)
    {
        // 等待上次写入操作完成
        do
        { // 用寻址操作查询当前是否可进行读写操作
            I2CStart();
            if (I2CWrite(0x50 << 1)) // 应答则跳出循环，非应答则进行下一次查询
            {
                break;
            }
            I2CStop();
        } while (1);
        // 按页写模式连续写入字节
        I2CWrite(addr); // 写入起始地址
        while (len > 0)
        {
            I2CWrite(*buf++);       // 写入一个字节数据
            len--;                  // 待写入长度计数递减
            addr++;                 // E2 地址递增
            if ((addr & 0x07) == 0) // 检查地址是否到达页边界，24C02 每页 8 字节，
            {                       // 所以检测低 3 位是否为零即可
                break;              // 到达页边界时，跳出循环，结束本次写操作
            }
        }
        I2CStop();
    }
}