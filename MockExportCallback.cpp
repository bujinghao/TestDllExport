/**
 * @file MockExportCallback.cpp
 * @brief 回调函数实现
 *
 * 本文件实现 Java 向 C 传递回调函数的功能。
 */

#include "pch.h"          // 预编译头文件
#include "framework.h"    // 项目框架头文件
#include "MockExport.h"   // 导出函数声明

// ============================================================
// 标准库引用
// ============================================================
#include <cstdio>   // sprintf_s()

// ============================================================
// 回调函数实现
// ============================================================

/**
 * @brief 使用整数回调
 * @param value    输入值
 * @param callback 回调函数指针
 * @return 回调函数处理后的返回值
 * @note 将输入值翻倍后传递给回调函数
 */
TESTDLLEXPORT_API int mock_useIntCallback(int value, MockCallbackInt callback) {
    if (callback == nullptr) {
        return MOCK_ERROR_NULL_POINTER;
    }
    // 调用 Java 传入的回调函数，将值翻倍后传递
    return callback(value * 2);
}

/**
 * @brief 使用字符串回调
 * @param message  消息字符串
 * @param callback 回调函数指针
 */
TESTDLLEXPORT_API void mock_useStringCallback(const char* message, MockCallbackString callback) {
    if (callback != nullptr && message != nullptr) {
        callback(message);
    }
}

/**
 * @brief 使用结构体回调
 * @param x        X 坐标
 * @param y        Y 坐标
 * @param callback 回调函数指针
 */
TESTDLLEXPORT_API void mock_usePointCallback(int x, int y, MockCallbackPoint callback) {
    if (callback != nullptr) {
        MockPoint point = mock_createPoint(x, y);
        callback(point);
    }
}

/**
 * @brief 模拟长时间操作
 * @param steps    总步骤数
 * @param callback 进度回调函数指针
 * @note 使用 Sleep() 模拟耗时操作，每步暂停 100ms
 */
TESTDLLEXPORT_API void mock_longOperation(int steps, MockProgressCallback callback) {
    if (callback == nullptr || steps <= 0) {
        return;
    }

    for (int i = 1; i <= steps; i++) {
        // 模拟工作 - 每步暂停 100 毫秒
        Sleep(100);

        // 报告进度到 Java 回调函数
        char status[32];
        sprintf_s(status, 32, "Step %d/%d", i, steps);
        callback(i, steps, status);
    }
}