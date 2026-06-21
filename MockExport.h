/**
 * @file MockExport.h
 * @brief 用于 Java JNA 调用的 DLL 导出函数头文件
 *
 * 本文件定义了供 Java JNA (Java Native Access) 调用的 C/C++ 函数接口。
 * 涵盖以下五大类别：
 *   1. 基本函数调用 - 简单的数值运算和字符串处理
 *   2. 结构体操作   - 结构体的创建、修改和传递
 *   3. 回调函数     - Java 向 C 传递回调函数的实现
 *   4. 数组操作     - 整数数组和结构体数组的处理
 *   5. 内存管理     - 动态内存的分配和释放
 *
 * @note 所有导出函数使用 extern "C" 确保不进行 C++ 名称修饰(name mangling)，
 *       使 JNA 能通过函数名正确查找符号。
 *       - C++ 名称修饰示例：mock_add() -> ?mock_add@@YAHHH@Z
 *       - extern "C" 后保持原名：mock_add() -> mock_add
 *       - 注意：__mock_add@4 是 stdcall 调用约定的修饰，与 C++ 名称修饰无关
 *
 * @note TESTDLLEXPORT_API 宏用于控制函数的导出/导入：
 *       - 编译 DLL 时定义为 __declspec(dllexport)
 *       - 使用 DLL 时定义为 __declspec(dllimport)
 *
 * @see https://github.com/java-native-access/jna
 */

#pragma once  // 防止头文件被多次包含，确保编译时只处理一次

#include "TestDllExport.h"  // 引入导出宏 TESTDLLEXPORT_API 的定义

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================
// 0. 错误码定义与版本信息 (Error Codes & Version)
// ============================================================

/**
 * @enum MockErrorCode
 * @brief 统一的错误码定义
 *
 * JNA 映射示例 (Java):
 *   public enum MockErrorCode {
 *       MOCK_SUCCESS(0),
 *       MOCK_ERROR_INVALID_PARAM(-1),
 *       MOCK_ERROR_NULL_POINTER(-2),
 *       MOCK_ERROR_BUFFER_TOO_SMALL(-3),
 *       MOCK_ERROR_OUT_OF_MEMORY(-4),
 *       MOCK_ERROR_NOT_FOUND(-5);
 *       private final int value;
 *       MockErrorCode(int value) { this.value = value; }
 *       public int getValue() { return value; }
 *   }
 */
typedef enum {
    MOCK_SUCCESS             = 0,   ///< 操作成功
    MOCK_ERROR_INVALID_PARAM = -1, ///< 参数无效
    MOCK_ERROR_NULL_POINTER  = -2, ///< 空指针
    MOCK_ERROR_BUFFER_TOO_SMALL = -3, ///< 缓冲区太小
    MOCK_ERROR_OUT_OF_MEMORY = -4, ///< 内存不足
    MOCK_ERROR_NOT_FOUND     = -5  ///< 未找到
} MockErrorCode;

/**
 * @brief 获取 DLL 版本号
 *
 * JNA 调用示例 (Java):
 *   int version = library.mock_getVersion();  // 例如返回 100 表示 v1.0.0
 *
 * @return 版本号 (格式：主版本*10000 + 次版本*100 + 修订号，如 10001 = v1.0.1)
 */
TESTDLLEXPORT_API int mock_getVersion(void);

/**
 * @brief 获取 DLL 构建信息字符串
 *
 * JNA 调用示例 (Java):
 *   Pointer ptr = library.mock_getBuildInfo();
 *   String info = ptr.getString(0);  // 例如 "MockExport v1.0.0, Build: 2024-01-15"
 *
 * @return 构建信息字符串指针 (静态内存，无需释放)
 */
TESTDLLEXPORT_API const char* mock_getBuildInfo(void);

/**
 * @brief 获取最后一次错误码
 *
 * JNA 调用示例 (Java):
 *   int result = library.mock_someFunction();
 *   if (result < 0) {
 *       int lastError = library.mock_getLastError();
 *   }
 *
 * @return 最后一次操作的错误码
 */
TESTDLLEXPORT_API int mock_getLastError(void);

// ============================================================
// 1. 基本函数调用 (Basic Function Calls)
// ============================================================

/**
 * @brief 两数相加
 *
 * JNA 调用示例 (Java):
 *   int result = library.mock_add(10, 20);  // result = 30
 *
 * @param a 第一个加数
 * @param b 第二个加数
 * @return 两数之和
 */
TESTDLLEXPORT_API int mock_add(int a, int b);

/**
 * @brief 两数相减
 *
 * JNA 调用示例 (Java):
 *   int result = library.mock_subtract(20, 10);  // result = 10
 *
 * @param a 被减数
 * @param b 减数
 * @return 差值 (a - b)
 */
TESTDLLEXPORT_API int mock_subtract(int a, int b);

/**
 * @brief 计算字符串长度
 *
 * JNA 调用示例 (Java):
 *   String str = "Hello";
 *   int len = library.mock_stringLength(str);  // len = 5
 *
 * @param str 输入字符串指针 (可为 null)
 * @return 字符串长度，若 str 为 null 则返回 0
 */
TESTDLLEXPORT_API int mock_stringLength(const char* str);

/**
 * @brief 复制字符串到目标缓冲区
 *
 * JNA 调用示例 (Java):
 *   byte[] dest = new byte[64];
 *   library.mock_copyString("Source", dest, dest.length);
 *
 * @param src  源字符串指针
 * @param dest 目标缓冲区指针
 * @param destSize 目标缓冲区大小 (字节)
 * @return 成功返回 0，失败返回 -1 (参数无效)
 */
TESTDLLEXPORT_API int mock_copyString(const char* src, char* dest, int destSize);

// ============================================================
// 2. 结构体操作 (Structure Operations)
// ============================================================

/**
 * @struct MockPoint
 * @brief 表示二维坐标点的结构体
 *
 * JNA 映射示例 (Java):
 *   public static class MockPoint extends Structure {
 *       public int x;
 *       public int y;
 *       // @FieldOrder({"x", "y"})  // JNA 5.x 使用注解
 *       protected List<String> getFieldOrder() { return Arrays.asList("x", "y"); }
 *   }
 */
typedef struct {
    int x;  ///< X 坐标
    int y;  ///< Y 坐标
} MockPoint;

/**
 * @struct MockData
 * @brief 包含 ID、名称和数值的通用数据结构体
 *
 * JNA 映射示例 (Java):
 *   public static class MockData extends Structure {
 *       public int id;
 *       public byte[] name = new byte[64];  // 固定长度字符数组
 *       public double value;
 *   }
 */
typedef struct {
    int id;           ///< 数据标识 ID
    char name[64];    ///< 名称字符串 (固定 64 字节)
    double value;     ///< 数值
} MockData;

/**
 * @struct MockLine
 * @brief 表示线段的结构体 (嵌套结构体示例)
 *
 * JNA 映射示例 (Java):
 *   public static class MockLine extends Structure {
 *       public MockPoint start;  // 嵌套结构体
 *       public MockPoint end;
 *       public byte[] label = new byte[32];
 *   }
 */
typedef struct {
    MockPoint start;   ///< 起始点
    MockPoint end;     ///< 终止点
    char label[32];    ///< 线段标签
} MockLine;

/**
 * @brief 创建一个点结构体
 *
 * JNA 调用示例 (Java):
 *   MockPoint point = library.mock_createPoint(100, 200);
 *   System.out.println("x=" + point.x + ", y=" + point.y);
 *
 * @param x X 坐标
 * @param y Y 坐标
 * @return 初始化后的 MockPoint 结构体
 */
TESTDLLEXPORT_API MockPoint mock_createPoint(int x, int y);

/**
 * @brief 计算两点之间的欧几里得距离
 *
 * JNA 调用示例 (Java):
 *   MockPoint p1 = library.mock_createPoint(0, 0);
 *   MockPoint p2 = library.mock_createPoint(3, 4);
 *   double dist = library.mock_distanceBetweenPoints(p1, p2);  // dist = 5.0
 *
 * @param p1 第一个点
 * @param p2 第二个点
 * @return 两点之间的距离 (sqrt((p2.x-p1.x)^2 + (p2.y-p1.y)^2))
 */
TESTDLLEXPORT_API double mock_distanceBetweenPoints(MockPoint p1, MockPoint p2);

/**
 * @brief 通过指针修改点结构体的坐标
 *
 * JNA 调用示例 (Java):
 *   MockPoint point = new MockPoint();
 *   library.mock_modifyPoint(point, 50, 60);
 *
 * @param point 要修改的点结构体指针
 * @param newX  新的 X 坐标
 * @param newY  新的 Y 坐标
 */
TESTDLLEXPORT_API void mock_modifyPoint(MockPoint* point, int newX, int newY);

/**
 * @brief 创建一个数据结构体
 *
 * JNA 调用示例 (Java):
 *   MockData data = library.mock_createData(1, "Test", 3.14);
 *
 * @param id    数据 ID
 * @param name  名称字符串
 * @param value 数值
 * @return 初始化后的 MockData 结构体
 */
TESTDLLEXPORT_API MockData mock_createData(int id, const char* name, double value);

/**
 * @brief 根据 ID 获取数据结构体 (模拟查询)
 *
 * JNA 调用示例 (Java):
 *   MockData data = library.mock_getDataById(100);
 *
 * @param id 要查询的数据 ID
 * @return MockData 结构体，value = id * 1.5，name = "Data_{id}"
 */
TESTDLLEXPORT_API MockData mock_getDataById(int id);

/**
 * @brief 通过指针填充数据结构体
 *
 * JNA 调用示例 (Java):
 *   MockData data = new MockData();
 *   library.mock_fillData(data, 10, "Example", 2.718);
 *
 * @param data  要填充的数据结构体指针
 * @param id    数据 ID
 * @param name  名称字符串
 * @param value 数值
 */
TESTDLLEXPORT_API void mock_fillData(MockData* data, int id, const char* name, double value);

// ============================================================
// 3. 回调函数 (Callback Functions)
// ============================================================

/**
 * @typedef MockCallbackInt
 * @brief 整数回调函数类型定义
 *
 * JNA 回调接口示例 (Java):
 *   public interface MockCallbackInt extends Callback {
 *       int invoke(int value);
 *   }
 *
 * @param value 回调时传入的整数
 * @return 回调函数处理后的返回值
 */
typedef int (*MockCallbackInt)(int value);

/**
 * @typedef MockCallbackString
 * @brief 字符串回调函数类型定义
 *
 * JNA 回调接口示例 (Java):
 *   public interface MockCallbackString extends Callback {
 *       void invoke(String message);
 *   }
 *
 * @param message 回调时传入的字符串消息
 */
typedef void (*MockCallbackString)(const char* message);

/**
 * @typedef MockCallbackPoint
 * @brief 结构体回调函数类型定义
 *
 * JNA 回调接口示例 (Java):
 *   public interface MockCallbackPoint extends Callback {
 *       void invoke(MockPoint point);
 *   }
 *
 * @param point 回调时传入的点结构体
 */
typedef void (*MockCallbackPoint)(MockPoint point);

/**
 * @typedef MockProgressCallback
 * @brief 进度回调函数类型定义 (用于长时间操作)
 *
 * JNA 回调接口示例 (Java):
 *   public interface MockProgressCallback extends Callback {
 *       void invoke(int current, int total, String status);
 *   }
 *
 * @param current 当前步骤
 * @param total   总步骤数
 * @param status  状态描述字符串
 */
typedef void (*MockProgressCallback)(int current, int total, const char* status);

/**
 * @brief 使用整数回调函数
 *
 * 将输入值翻倍后传递给回调函数，返回回调函数的结果。
 *
 * JNA 调用示例 (Java):
 *   MockCallbackInt callback = (value) -> { System.out.println("Received: " + value); return value + 1; };
 *   int result = library.mock_useIntCallback(10, callback);
 *   // C 将 10*2=20 传给回调，回调返回 21，最终 result = 21
 *
 * @param value    输入值
 * @param callback 回调函数指针
 * @return 回调函数的返回值，若 callback 为 null 则返回 -1
 */
TESTDLLEXPORT_API int mock_useIntCallback(int value, MockCallbackInt callback);

/**
 * @brief 使用字符串回调函数
 *
 * 将消息传递给回调函数处理。
 *
 * JNA 调用示例 (Java):
 *   MockCallbackString callback = (msg) -> System.out.println("Message: " + msg);
 *   library.mock_useStringCallback("Hello from C!", callback);
 *
 * @param message  要传递的消息字符串
 * @param callback 回调函数指针
 */
TESTDLLEXPORT_API void mock_useStringCallback(const char* message, MockCallbackString callback);

/**
 * @brief 使用结构体回调函数
 *
 * 创建点结构体并传递给回调函数。
 *
 * JNA 调用示例 (Java):
 *   MockCallbackPoint callback = (point) -> System.out.println("Point: " + point.x + "," + point.y);
 *   library.mock_usePointCallback(100, 200, callback);
 *
 * @param x        点的 X 坐标
 * @param y        点的 Y 坐标
 * @param callback 回调函数指针
 */
TESTDLLEXPORT_API void mock_usePointCallback(int x, int y, MockCallbackPoint callback);

/**
 * @brief 模拟长时间操作，使用进度回调
 *
 * 执行指定步数的模拟操作，每步间隔 100ms，并通过回调报告进度。
 *
 * JNA 调用示例 (Java):
 *   MockProgressCallback callback = (cur, total, status) -> {
 *       System.out.println(status + " (" + cur + "/" + total + ")");
 *   };
 *   library.mock_longOperation(10, callback);  // 执行 10 步，每步 100ms
 *
 * @param steps    总步骤数
 * @param callback 进度回调函数指针
 */
TESTDLLEXPORT_API void mock_longOperation(int steps, MockProgressCallback callback);

// ============================================================
// 4. 数组操作 (Array Operations)
// ============================================================

/**
 * @brief 计算整数数组的元素之和
 *
 * JNA 调用示例 (Java):
 *   int[] arr = {1, 2, 3, 4, 5};
 *   int sum = library.mock_sumArray(arr, arr.length);  // sum = 15
 *
 * @param arr   整数数组指针
 * @param length 数组长度
 * @return 数组元素之和，若参数无效则返回 0
 */
TESTDLLEXPORT_API int mock_sumArray(const int* arr, size_t length);

/**
 * @brief 查找整数数组中的最大值
 *
 * JNA 调用示例 (Java):
 *   int[] arr = {3, 7, 2, 9, 5};
 *   int max = library.mock_findMax(arr, arr.length);  // max = 9
 *
 * @param arr   整数数组指针
 * @param length 数组长度
 * @return 数组中的最大值，若参数无效则返回 0
 */
TESTDLLEXPORT_API int mock_findMax(const int* arr, size_t length);

/**
 * @brief 查找整数数组中的最小值
 *
 * JNA 调用示例 (Java):
 *   int[] arr = {3, 7, 2, 9, 5};
 *   int min = library.mock_findMin(arr, arr.length);  // min = 2
 *
 * @param arr   整数数组指针
 * @param length 数组长度
 * @return 数组中的最小值，若参数无效则返回 0
 */
TESTDLLEXPORT_API int mock_findMin(const int* arr, size_t length);

/**
 * @brief 反转整数数组
 *
 * JNA 调用示例 (Java):
 *   int[] arr = {1, 2, 3, 4, 5};
 *   library.mock_reverseArray(arr, arr.length);
 *   // arr 变为 {5, 4, 3, 2, 1}
 *
 * @param arr   整数数组指针 (会被修改)
 * @param length 数组长度
 */
TESTDLLEXPORT_API void mock_reverseArray(int* arr, size_t length);

/**
 * @brief 用指定值填充整数数组
 *
 * JNA 调用示例 (Java):
 *   int[] arr = new int[10];
 *   library.mock_fillArray(arr, arr.length, 42);
 *   // arr 所有元素变为 42
 *
 * @param arr   整数数组指针 (会被修改)
 * @param length 数组长度
 * @param value 要填充的值
 */
TESTDLLEXPORT_API void mock_fillArray(int* arr, size_t length, int value);

/**
 * @brief 复制整数数组
 *
 * JNA 调用示例 (Java):
 *   int[] src = {1, 2, 3};
 *   int[] dest = new int[3];
 *   library.mock_copyArray(src, dest, src.length);
 *
 * @param src    源数组指针
 * @param dest   目标数组指针
 * @param length 要复制的元素数量
 * @return 成功返回 0，失败返回 -1
 */
TESTDLLEXPORT_API int mock_copyArray(const int* src, int* dest, size_t length);

/**
 * @brief 计算点数组的几何中心
 *
 * JNA 调用示例 (Java):
 *   MockPoint[] points = new MockPoint[3];
 *   points[0] = library.mock_createPoint(0, 0);
 *   points[1] = library.mock_createPoint(10, 0);
 *   points[2] = library.mock_createPoint(5, 10);
 *   MockPoint center = library.mock_calculateCenter(points, 3);
 *   // center ≈ (5, 3)
 *
 * @param points 点结构体数组指针
 * @param count  点的数量
 * @return 所有点的平均坐标 (几何中心)
 */
TESTDLLEXPORT_API MockPoint mock_calculateCenter(const MockPoint* points, size_t count);

/**
 * @brief 批量移动点数组中的所有点
 *
 * JNA 调用示例 (Java):
 *   MockPoint[] points = new MockPoint[2];
 *   points[0] = library.mock_createPoint(0, 0);
 *   points[1] = library.mock_createPoint(10, 10);
 *   library.mock_moveAllPoints(points, 2, 5, -3);
 *   // points[0] 变为 (5, -3), points[1] 变为 (15, 7)
 *
 * @param points 点结构体数组指针 (会被修改)
 * @param count  点的数量
 * @param dx     X 方向偏移量
 * @param dy     Y 方向偏移量
 */
TESTDLLEXPORT_API void mock_moveAllPoints(MockPoint* points, size_t count, int dx, int dy);

// ============================================================
// 5. 内存管理 (Memory Management)
// ============================================================

/**
 * @brief 动态分配整数数组
 *
 * 分配指定长度的整数数组，并初始化为零。
 * 调用者需使用 mock_freeIntArray() 释放内存。
 *
 * JNA 调用示例 (Java):
 *   Pointer ptr = library.mock_allocateIntArray(100);
 *   IntArray arr = new IntArray(ptr);  // 使用 JNA 的 IntArray 包装
 *   // 使用完后释放
 *   library.mock_freeIntArray(ptr);
 *
 * @param length 要分配的数组长度
 * @return 分配的数组指针，若 length <= 0 则返回 null
 * @warning 返回的内存必须由调用者释放，否则会造成内存泄漏
 */
TESTDLLEXPORT_API int* mock_allocateIntArray(size_t length);

/**
 * @brief 释放动态分配的整数数组
 *
 * JNA 调用示例 (Java):
 *   Pointer ptr = library.mock_allocateIntArray(100);
 *   // 使用数组...
 *   library.mock_freeIntArray(ptr);  // 释放内存
 *
 * @param arr 要释放的数组指针
 */
TESTDLLEXPORT_API void mock_freeIntArray(int* arr);

/**
 * @brief 动态分配单个 MockData 结构体
 *
 * 分配并初始化为零。调用者需使用 mock_freeData() 释放。
 *
 * JNA 调用示例 (Java):
 *   MockData data = new MockData(library.mock_allocateData());
 *   // 使用完后释放
 *   library.mock_freeData(data.getPointer());
 *
 * @return 分配的结构体指针，失败返回 null
 * @warning 返回的内存必须由调用者释放
 */
TESTDLLEXPORT_API MockData* mock_allocateData();

/**
 * @brief 释放动态分配的 MockData 结构体
 *
 * @param data 要释放的结构体指针
 */
TESTDLLEXPORT_API void mock_freeData(MockData* data);

/**
 * @brief 动态分配点结构体数组
 *
 * JNA 调用示例 (Java):
 *   Pointer ptr = library.mock_allocatePointArray(50);
 *   MockPoint[] points = new MockPoint(ptr).toArray(50);
 *   // 使用完后释放
 *   library.mock_freePointArray(ptr);
 *
 * @param count 要分配的点数量
 * @return 分配的数组指针，若 count <= 0 则返回 null
 * @warning 返回的内存必须由调用者释放
 */
TESTDLLEXPORT_API MockPoint* mock_allocatePointArray(size_t count);

/**
 * @brief 释放动态分配的点结构体数组
 *
 * @param points 要释放的数组指针
 */
TESTDLLEXPORT_API void mock_freePointArray(MockPoint* points);

/**
 * @brief 动态分配字符串缓冲区
 *
 * 分配指定长度的字符串缓冲区，并初始化为空字符串。
 * 调用者需使用 mock_freeString() 释放。
 *
 * JNA 调用示例 (Java):
 *   Pointer ptr = library.mock_allocateString(256);
 *   // 使用完后释放
 *   library.mock_freeString(ptr);
 *
 * @param length 要分配的缓冲区长度 (字节)
 * @return 分配的字符串指针，若 length <= 0 则返回 null
 * @warning 返回的内存必须由调用者释放
 */
TESTDLLEXPORT_API char* mock_allocateString(size_t length);

/**
 * @brief 释放动态分配的字符串
 *
 * @param str 要释放的字符串指针
 */
TESTDLLEXPORT_API void mock_freeString(char* str);

/**
 * @brief 创建并返回动态分配的数据结构体
 *
 * 分配内存并填充数据，返回指针。调用者需使用 mock_freeData() 释放。
 *
 * JNA 调用示例 (Java):
 *   MockData.ByReference data = new MockData.ByReference();
 *   data.setPointer(library.mock_createDynamicData(1, "Test", 1.23));
 *   // 使用完后释放
 *   library.mock_freeData(data.getPointer());
 *
 * @param id    数据 ID
 * @param name  名称字符串
 * @param value 数值
 * @return 动态分配并初始化的 MockData 指针
 * @warning 返回的内存必须由调用者释放
 */
TESTDLLEXPORT_API MockData* mock_createDynamicData(int id, const char* name, double value);

/**
 * @brief 生成动态分配的格式化字符串
 *
 * 将前缀和数字拼接成字符串，动态分配并返回。
 * 调用者需使用 mock_freeString() 释放。
 *
 * JNA 调用示例 (Java):
 *   Pointer ptr = library.mock_getDynamicString("ID_", 123);
 *   String str = ptr.getString(0);  // str = "ID_123"
 *   library.mock_freeString(ptr);
 *
 * @param prefix 字符串前缀
 * @param number 要拼接的数字
 * @return 格式化后的字符串指针，若 prefix 为 null 则返回 null
 * @warning 返回的内存必须由调用者释放
 */
TESTDLLEXPORT_API char* mock_getDynamicString(const char* prefix, int number);

#ifdef __cplusplus
}
#endif