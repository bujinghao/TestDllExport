/**
 * @file MockExportMemory.cpp
 * @brief 内存管理实现
 *
 * 本文件实现动态内存的分配和释放函数。
 */

#include "pch.h"          // 预编译头文件
#include "framework.h"    // 项目框架头文件
#include "MockExport.h"   // 导出函数声明

// ============================================================
// 标准库引用
// ============================================================
#include <cstdlib>  // malloc(), free()
#include <cstring>  // strlen(), strncpy_s()
#include <cstdio>   // sprintf_s()
#include <cstddef>  // size_t

// ============================================================
// 内存管理函数实现
// ============================================================

/**
 * @brief 分配整数数组
 * @param length 数组长度
 * @return 分配的数组指针
 * @warning 调用者必须使用 mock_freeIntArray() 释放
 */
TESTDLLEXPORT_API int* mock_allocateIntArray(size_t length) {
    if (length == 0) {
        return nullptr;
    }
    int* arr = (int*)malloc(length * sizeof(int));
    if (arr != nullptr) {
        memset(arr, 0, length * sizeof(int));
    }
    return arr;
}

/**
 * @brief 释放整数数组
 * @param arr 要释放的数组指针
 */
TESTDLLEXPORT_API void mock_freeIntArray(int* arr) {
    if (arr != nullptr) {
        free(arr);
    }
}

/**
 * @brief 分配 MockData 结构体
 * @return 分配的结构体指针
 * @warning 调用者必须使用 mock_freeData() 释放
 */
TESTDLLEXPORT_API MockData* mock_allocateData() {
    MockData* data = (MockData*)malloc(sizeof(MockData));
    if (data != nullptr) {
        memset(data, 0, sizeof(MockData));
    }
    return data;
}

/**
 * @brief 释放 MockData 结构体
 * @param data 要释放的结构体指针
 */
TESTDLLEXPORT_API void mock_freeData(MockData* data) {
    if (data != nullptr) {
        free(data);
    }
}

/**
 * @brief 分配点数组
 * @param count 点数量
 * @return 分配的数组指针
 * @warning 调用者必须使用 mock_freePointArray() 释放
 */
TESTDLLEXPORT_API MockPoint* mock_allocatePointArray(size_t count) {
    if (count == 0) {
        return nullptr;
    }
    MockPoint* points = (MockPoint*)malloc(count * sizeof(MockPoint));
    if (points != nullptr) {
        memset(points, 0, count * sizeof(MockPoint));
    }
    return points;
}

/**
 * @brief 释放点数组
 * @param points 要释放的数组指针
 */
TESTDLLEXPORT_API void mock_freePointArray(MockPoint* points) {
    if (points != nullptr) {
        free(points);
    }
}

/**
 * @brief 分配字符串缓冲区
 * @param length 缓冲区长度
 * @return 分配的字符串指针
 * @warning 调用者必须使用 mock_freeString() 释放
 */
TESTDLLEXPORT_API char* mock_allocateString(size_t length) {
    if (length == 0) {
        return nullptr;
    }
    char* str = (char*)malloc(length * sizeof(char));
    if (str != nullptr) {
        str[0] = '\0';
    }
    return str;
}

/**
 * @brief 释放字符串
 * @param str 要释放的字符串指针
 */
TESTDLLEXPORT_API void mock_freeString(char* str) {
    if (str != nullptr) {
        free(str);
    }
}

/**
 * @brief 创建动态数据结构体
 * @param id    数据 ID
 * @param name  名称
 * @param value 数值
 * @return 动态分配并初始化的 MockData 指针
 * @warning 调用者必须使用 mock_freeData() 释放
 */
TESTDLLEXPORT_API MockData* mock_createDynamicData(int id, const char* name, double value) {
    MockData* data = mock_allocateData();
    if (data != nullptr) {
        mock_fillData(data, id, name, value);
    }
    return data;
}

/**
 * @brief 生成动态格式化字符串
 * @param prefix 字符串前缀
 * @param number 要拼接的数字
 * @return 格式化字符串指针
 * @warning 调用者必须使用 mock_freeString() 释放
 */
TESTDLLEXPORT_API char* mock_getDynamicString(const char* prefix, int number) {
    if (prefix == nullptr) {
        return nullptr;
    }
    size_t prefixLen = strlen(prefix);
    size_t numLen = 16;
    size_t totalLen = prefixLen + numLen + 1;

    char* str = mock_allocateString(totalLen);
    if (str != nullptr) {
        sprintf_s(str, totalLen, "%s%d", prefix, number);
    }
    return str;
}