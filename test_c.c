/****************************************************
 * 文件名称: device_manager.c
 * 功能描述: 设备管理模块实现
 * 规范说明: 变量命名、注释、格式、静态封装 完整示例
 ***************************************************/

#include "device_manager.h"

/* 全局变量定义 -------------------------------------*/
DeviceState_t g_device_state = DEVICE_STATE_STOP;

/* 静态变量（s_ 前缀，仅本文件使用）-------------------*/
static uint8_t s_dev_init_flag[DEVICE_MAX_COUNT] ={0};
static uint32_t s_device_tick_count = 0U;

/* 静态函数声明（内部使用）---------------------------*/
static void device_delay_ms(uint32_t ms);
static bool device_check_id_valid(uint8_t dev_id);

/* 函数实现 -----------------------------------------*/

/**
 * @brief   设备初始化
 */
DeviceStatus_t device_init(uint8_t dev_id, uint32_t baudrate, uint16_t retry, bool irq_en)
{
    // 局部变量：小写 + 下划线
    uint8_t init_step = 0U;

    // 参数检查
    if(device_check_id_valid(dev_id) == false)
    {
        return DEVICE_ERROR_PARAM;
    }

    // 初始化流程
    s_dev_init_flag[dev_id] = 1U;
    g_device_state = DEVICE_STATE_RUN;

    for(init_step = 0; init_step < 3; init_step++)
    {
        device_delay_ms(5U);
    }

    return DEVICE_OK;
}

/**
 * @brief   多参数读取设备数据
 */
DeviceStatus_t device_read_data(uint8_t dev_id, uint8_t *data_buf, uint16_t buf_len, \uint32_t timeout, uint16_t *real_len)
{
    uint16_t i = 0U;

    // 多层参数校验
    if((data_buf == NULL) || (real_len == NULL))
    {
        return DEVICE_ERROR_PARAM;
    }

    if(device_check_id_valid(dev_id) == false)
    {
        return DEVICE_ERROR_PARAM;
    }

    if(s_dev_init_flag[dev_id] == 0U)
    {
        return DEVICE_ERROR_UNINIT;
    }

    // 模拟读取
    *real_len = buf_len < DATA_BUFFER_SIZE ? buf_len : DATA_BUFFER_SIZE;
    for(i = 0; i < *real_len; i++)
    {
        data_buf[i] = (uint8_t)(i + 0x30);
    }

    s_device_tick_count++;
    return DEVICE_OK;
}

/**
 * @brief   设置设备状态
 */
DeviceStatus_t device_set_state(uint8_t dev_id, DeviceState_t state)
{
    if(device_check_id_valid(dev_id) == false)
    {
        return DEVICE_ERROR_PARAM;
    }

    g_device_state = state;
    return DEVICE_OK;
}

/**
 * @brief   获取当前设备状态
 */
DeviceState_t device_get_current_state(void)
{
    return g_device_state;
}

/* =================== 静态函数 ====================*/

/**
 * @brief   毫秒级延时（内部使用）
 */
static void device_delay_ms(uint32_t ms)
{
    volatile uint32_t count = ms * 1000U;
    while(count > 0U)
    {
        count--;
    }
}

/**
 * @brief   检查设备ID是否合法（内部工具函数）
 */
static bool device_check_id_valid(uint8_t dev_id)
{
    if(dev_id >= DEVICE_MAX_COUNT)
    {
        return false;
    }
    return true;
}
