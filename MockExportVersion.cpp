/**
 * @file MockExportVersion.cpp
 * @brief DLL 版本信息与错误码管理
 *
 * 本文件实现 DLL 的版本查询和错误码管理功能。
 */

#include "pch.h"          // 预编译头文件
#include "framework.h"    // 项目框架头文件
#include "MockExport.h"   // 导出函数声明

// ============================================================
// 全局变量 (Global Variables)
// ============================================================

/// 最后一次操作的错误码 (线程安全考虑：实际项目应使用 thread_local)
static int g_lastError = MOCK_SUCCESS;

/// DLL 版本号：主版本*10000 + 次版本*100 + 修订号
static const int MOCK_VERSION = 10000;  // v1.0.0

/// DLL 构建信息 (静态字符串，无需释放)
static const char* MOCK_BUILD_INFO = "MockExport v1.0.0, Build: 2024-06-18, Platform: x64";

// ============================================================
// 版本与错误码函数实现
// ============================================================

/**
 * @brief 获取 DLL 版本号
 * @return 版本号 (格式：主版本*10000 + 次版本*100 + 修订号)
 */
TESTDLLEXPORT_API int mock_getVersion(void) {
    return MOCK_VERSION;
}

/**
 * @brief 获取 DLL 构建信息
 * @return 构建信息字符串指针 (静态内存，无需释放)
 * @note 返回的是静态字符串，调用者不应修改或释放
 */
TESTDLLEXPORT_API const char* mock_getBuildInfo(void) {
    return MOCK_BUILD_INFO;
}

/**
 * @brief 获取最后一次错误码
 * @return 最后一次操作的错误码
 * @note 注意：当前实现非线程安全，多线程环境应使用 thread_local
 */
TESTDLLEXPORT_API int mock_getLastError(void) {
    return g_lastError;
}