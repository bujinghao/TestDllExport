/**
 * @file MockExportArray.cpp
 * @brief 数组操作实现
 *
 * 本文件实现整数数组和结构体数组的处理函数。
 */

#include "pch.h"          // 预编译头文件
#include "framework.h"    // 项目框架头文件
#include "MockExport.h"   // 导出函数声明

// ============================================================
// 标准库引用
// ============================================================
#include <cstddef>  // size_t

// ============================================================
// 数组操作函数实现
// ============================================================

/**
 * @brief 计算数组元素之和
 * @param arr    整数数组
 * @param length 数组长度
 * @return 元素之和
 */
TESTDLLEXPORT_API int mock_sumArray(const int* arr, size_t length) {
    if (arr == nullptr || length == 0) {
        return 0;
    }
    int sum = 0;
    for (size_t i = 0; i < length; i++) {
        sum += arr[i];
    }
    return sum;
}

/**
 * @brief 查找数组最大值
 * @param arr    整数数组
 * @param length 数组长度
 * @return 最大值
 */
TESTDLLEXPORT_API int mock_findMax(const int* arr, size_t length) {
    if (arr == nullptr || length == 0) {
        return 0;
    }
    int maxVal = arr[0];
    for (size_t i = 1; i < length; i++) {
        if (arr[i] > maxVal) {
            maxVal = arr[i];
        }
    }
    return maxVal;
}

/**
 * @brief 查找数组最小值
 * @param arr    整数数组
 * @param length 数组长度
 * @return 最小值
 */
TESTDLLEXPORT_API int mock_findMin(const int* arr, size_t length) {
    if (arr == nullptr || length == 0) {
        return 0;
    }
    int minVal = arr[0];
    for (size_t i = 1; i < length; i++) {
        if (arr[i] < minVal) {
            minVal = arr[i];
        }
    }
    return minVal;
}

/**
 * @brief 反转数组
 * @param arr    整数数组 (会被修改)
 * @param length 数组长度
 */
TESTDLLEXPORT_API void mock_reverseArray(int* arr, size_t length) {
    if (arr == nullptr || length <= 1) {
        return;
    }
    for (size_t i = 0; i < length / 2; i++) {
        int temp = arr[i];
        arr[i] = arr[length - 1 - i];
        arr[length - 1 - i] = temp;
    }
}

/**
 * @brief 填充数组
 * @param arr    整数数组 (会被修改)
 * @param length 数组长度
 * @param value  填充值
 */
TESTDLLEXPORT_API void mock_fillArray(int* arr, size_t length, int value) {
    if (arr == nullptr || length == 0) {
        return;
    }
    for (size_t i = 0; i < length; i++) {
        arr[i] = value;
    }
}

/**
 * @brief 复制数组
 * @param src    源数组
 * @param dest   目标数组
 * @param length 复制长度
 * @return 成功返回 MOCK_SUCCESS，失败返回错误码
 */
TESTDLLEXPORT_API int mock_copyArray(const int* src, int* dest, size_t length) {
    if (src == nullptr || dest == nullptr || length == 0) {
        return MOCK_ERROR_INVALID_PARAM;
    }
    for (size_t i = 0; i < length; i++) {
        dest[i] = src[i];
    }
    return MOCK_SUCCESS;
}

/**
 * @brief 计算点数组中心
 * @param points 点数组
 * @param count  点数量
 * @return 平均坐标点
 */
TESTDLLEXPORT_API MockPoint mock_calculateCenter(const MockPoint* points, size_t count) {
    MockPoint center = {0, 0};
    if (points == nullptr || count == 0) {
        return center;
    }
    long long sumX = 0, sumY = 0;
    for (size_t i = 0; i < count; i++) {
        sumX += points[i].x;
        sumY += points[i].y;
    }
    center.x = (int)(sumX / count);
    center.y = (int)(sumY / count);
    return center;
}

/**
 * @brief 批量移动点
 * @param points 点数组 (会被修改)
 * @param count  点数量
 * @param dx     X 偏移
 * @param dy     Y 偏移
 */
TESTDLLEXPORT_API void mock_moveAllPoints(MockPoint* points, size_t count, int dx, int dy) {
    if (points == nullptr || count == 0) {
        return;
    }
    for (size_t i = 0; i < count; i++) {
        points[i].x += dx;
        points[i].y += dy;
    }
}