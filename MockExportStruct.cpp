/**
 * @file MockExportStruct.cpp
 * @brief 结构体操作实现
 *
 * 本文件实现结构体的创建、修改和传递函数。
 */

#include "pch.h"          // 预编译头文件
#include "framework.h"    // 项目框架头文件
#include "MockExport.h"   // 导出函数声明

// ============================================================
// 标准库引用
// ============================================================
#include <cmath>    // sqrt()
#include <cstdio>   // sprintf_s()
#include <cstring>  // strncpy_s()

// ============================================================
// 结构体操作函数实现
// ============================================================

/**
 * @brief 创建点结构体
 * @param x X 坐标
 * @param y Y 坐标
 * @return 初始化后的 MockPoint 结构体
 */
TESTDLLEXPORT_API MockPoint mock_createPoint(int x, int y) {
    MockPoint point;
    point.x = x;
    point.y = y;
    return point;
}

/**
 * @brief 计算两点距离
 * @param p1 第一个点
 * @param p2 第二个点
 * @return 欧几里得距离
 */
TESTDLLEXPORT_API double mock_distanceBetweenPoints(MockPoint p1, MockPoint p2) {
    int dx = p2.x - p1.x;
    int dy = p2.y - p1.y;
    return sqrt((double)(dx * dx + dy * dy));
}

/**
 * @brief 修改点结构体坐标
 * @param point 点结构体指针
 * @param newX  新 X 坐标
 * @param newY  新 Y 坐标
 */
TESTDLLEXPORT_API void mock_modifyPoint(MockPoint* point, int newX, int newY) {
    if (point != nullptr) {
        point->x = newX;
        point->y = newY;
    }
}

/**
 * @brief 创建数据结构体
 * @param id    数据 ID
 * @param name  名称
 * @param value 数值
 * @return 初始化后的 MockData 结构体
 */
TESTDLLEXPORT_API MockData mock_createData(int id, const char* name, double value) {
    MockData data;
    data.id = id;
    data.value = value;
    if (name != nullptr) {
        strncpy_s(data.name, 64, name, _TRUNCATE);
    } else {
        data.name[0] = '\0';
    }
    return data;
}

/**
 * @brief 根据 ID 获取数据 (模拟查询)
 * @param id 数据 ID
 * @return MockData 结构体
 */
TESTDLLEXPORT_API MockData mock_getDataById(int id) {
    MockData data;
    data.id = id;
    data.value = id * 1.5;
    sprintf_s(data.name, 64, "Data_%d", id);
    return data;
}

/**
 * @brief 填充数据结构体
 * @param data  数据结构体指针
 * @param id    数据 ID
 * @param name  名称
 * @param value 数值
 */
TESTDLLEXPORT_API void mock_fillData(MockData* data, int id, const char* name, double value) {
    if (data != nullptr) {
        data->id = id;
        data->value = value;
        if (name != nullptr) {
            strncpy_s(data->name, 64, name, _TRUNCATE);
        } else {
            data->name[0] = '\0';
        }
    }
}