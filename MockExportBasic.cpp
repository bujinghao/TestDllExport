/**
 * @file MockExportBasic.cpp
 * @brief 基本函数调用实现
 *
 * 本文件实现简单的数值运算和字符串处理函数。
 */

#include "pch.h"          // 预编译头文件
#include "framework.h"    // 项目框架头文件
#include "MockExport.h"   // 导出函数声明

// ============================================================
// 标准库引用
// ============================================================
#include <cstring>  // strlen()

// ============================================================
// 基本函数实现
// ============================================================

/**
 * @brief 两数相加
 * @param a 第一个加数
 * @param b 第二个加数
 * @return a + b
 */
TESTDLLEXPORT_API int mock_add(int a, int b) {
    return a + b;
}

/**
 * @brief 两数相减
 * @param a 被减数
 * @param b 减数
 * @return a - b
 */
TESTDLLEXPORT_API int mock_subtract(int a, int b) {
    return a - b;
}

/**
 * @brief 计算字符串长度
 * @param str 输入字符串
 * @return 字符串长度，null 时返回 0
 */
TESTDLLEXPORT_API int mock_stringLength(const char* str) {
    if (str == nullptr) {
        return 0;
    }
    return (int)strlen(str);
}

/**
 * @brief 复制字符串
 * @param src      源字符串
 * @param dest     目标缓冲区
 * @param destSize 目标缓冲区大小
 * @return 成功返回 0，失败返回 -1
 * @note 使用 strncpy_s() 安全版本防止缓冲区溢出
 */
TESTDLLEXPORT_API int mock_copyString(const char* src, char* dest, int destSize) {
    if (src == nullptr || dest == nullptr || destSize <= 0) {
        return MOCK_ERROR_INVALID_PARAM;
    }
    strncpy_s(dest, destSize, src, _TRUNCATE);
    return MOCK_SUCCESS;
}