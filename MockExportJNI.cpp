/**
 * @file MockExportJNI.cpp
 * @brief Java JNI 调用封装层
 *
 * 本文件提供 Java JNI (Java Native Interface) 调用接口，
 * 封装 MockExport.h 中定义的底层 C 函数。
 *
 * Java 包名：com.example.jni.JniExample
 *
 * @note JNI 函数命名规则：Java_<包名>_<类名>_<方法名>
 *       包名中的 '.' 替换为 '_'
 *
 * @see https://docs.oracle.com/javase/8/docs/technotes/guides/jni/
 */

#include "pch.h"          // 预编译头文件
#include "framework.h"    // 项目框架头文件
#include "MockExport.h"   // 底层导出函数声明

// ============================================================
// JNI 头文件
// ============================================================
#include <jni.h>          // Java Native Interface

// ============================================================
// 标准库引用
// ============================================================
#include <cstring>        // strlen()
#include <cctype>         // toupper(), tolower()
#include <cstdio>         // sprintf_s()
#include <cstdlib>        // malloc(), free()
#include <string>         // std::string

// ============================================================
// 1. 基本函数调用 (Basic Function Calls)
// ============================================================

/**
 * @brief 两数相加 (JNI 封装)
 * @param env JNI 环境指针
 * @param obj Java 对象引用
 * @param a 第一个加数
 * @param b 第二个加数
 * @return a + b
 */
extern "C" JNIEXPORT jint JNICALL
Java_com_example_jni_JniExample_add(JNIEnv* env, jobject obj, jint a, jint b) {
    return mock_add(a, b);
}

/**
 * @brief 两数相减 (JNI 封装)
 * @param env JNI 环境指针
 * @param obj Java 对象引用
 * @param a 被减数
 * @param b 减数
 * @return a - b
 */
extern "C" JNIEXPORT jint JNICALL
Java_com_example_jni_JniExample_subtract(JNIEnv* env, jobject obj, jint a, jint b) {
    return mock_subtract(a, b);
}

/**
 * @brief 获取 DLL 版本号 (JNI 封装)
 * @return 版本号
 */
extern "C" JNIEXPORT jint JNICALL
Java_com_example_jni_JniExample_getVersion(JNIEnv* env, jobject obj) {
    return mock_getVersion();
}

/**
 * @brief 获取 DLL 构建信息 (JNI 封装)
 * @return 构建信息字符串
 */
extern "C" JNIEXPORT jstring JNICALL
Java_com_example_jni_JniExample_getBuildInfo(JNIEnv* env, jobject obj) {
    const char* info = mock_getBuildInfo();
    return env->NewStringUTF(info);
}

// ============================================================
// 2. 结构体操作 (Structure Operations)
// ============================================================

/**
 * @brief 创建点结构体 (JNI 封装)
 * @param x X 坐标
 * @param y Y 坐标
 * @return Java Point 对象
 *
 * @note Java 端需定义内部类：
 *   public static class Point {
 *       public int x, y;
 *       public Point(int x, int y) { this.x = x; this.y = y; }
 *   }
 */
extern "C" JNIEXPORT jobject JNICALL
Java_com_example_jni_JniExample_createPoint(JNIEnv* env, jobject obj, jint x, jint y) {
    // 调用底层函数
    MockPoint point = mock_createPoint(x, y);

    // 找到 Java 的 Point 类
    jclass pointClass = env->FindClass("com/example/jni/JniExample$Point");
    if (pointClass == nullptr) {
        return nullptr;  // 类未找到
    }

    // 获取构造函数 (参数为两个 int)
    jmethodID constructor = env->GetMethodID(pointClass, "<init>", "(II)V");
    if (constructor == nullptr) {
        return nullptr;  // 构造函数未找到
    }

    // 创建 Java 对象
    return env->NewObject(pointClass, constructor, point.x, point.y);
}

/**
 * @brief 计算两点距离 (JNI 封装)
 * @param p1 第一个点 (Java Point 对象)
 * @param p2 第二个点 (Java Point 对象)
 * @return 欧几里得距离
 */
extern "C" JNIEXPORT jdouble JNICALL
Java_com_example_jni_JniExample_distance(JNIEnv* env, jobject obj, jobject p1, jobject p2) {
    // 获取 Point 类
    jclass pointClass = env->GetObjectClass(p1);

    // 获取 x, y 字段
    jfieldID xField = env->GetFieldID(pointClass, "x", "I");
    jfieldID yField = env->GetFieldID(pointClass, "y", "I");

    // 提取坐标值
    MockPoint point1, point2;
    point1.x = env->GetIntField(p1, xField);
    point1.y = env->GetIntField(p1, yField);
    point2.x = env->GetIntField(p2, xField);
    point2.y = env->GetIntField(p2, yField);

    // 调用底层函数
    return mock_distanceBetweenPoints(point1, point2);
}

/**
 * @brief 修改点坐标 (JNI 封装)
 * @param point Java Point 对象 (会被修改)
 * @param newX 新 X 坐标
 * @param newY 新 Y 坐标
 */
extern "C" JNIEXPORT void JNICALL
Java_com_example_jni_JniExample_modifyPoint(JNIEnv* env, jobject obj, jobject point, jint newX, jint newY) {
    // 获取 Point 类
    jclass pointClass = env->GetObjectClass(point);

    // 获取并设置 x, y 字段
    jfieldID xField = env->GetFieldID(pointClass, "x", "I");
    jfieldID yField = env->GetFieldID(pointClass, "y", "I");

    env->SetIntField(point, xField, newX);
    env->SetIntField(point, yField, newY);
}

/**
 * @brief 创建数据结构体 (JNI 封装)
 * @param id   数据 ID
 * @param name 名称
 * @param value 数值
 * @return Java Data 对象
 *
 * @note Java 端需定义内部类：
 *   public static class Data {
 *       public int id;
 *       public String name;
 *       public double value;
 *       public Data(int id, String name, double value) { ... }
 *   }
 */
extern "C" JNIEXPORT jobject JNICALL
Java_com_example_jni_JniExample_createData(JNIEnv* env, jobject obj, jint id, jstring name, jdouble value) {
    // 获取名称字符串
    const char* nameStr = env->GetStringUTFChars(name, nullptr);

    // 调用底层函数
    MockData data = mock_createData(id, nameStr, value);

    // 释放字符串
    env->ReleaseStringUTFChars(name, nameStr);

    // 找到 Java 的 Data 类
    jclass dataClass = env->FindClass("com/example/jni/JniExample$Data");
    if (dataClass == nullptr) {
        return nullptr;
    }

    // 获取构造函数
    jmethodID constructor = env->GetMethodID(dataClass, "<init>", "(ILjava/lang/String;D)V");
    if (constructor == nullptr) {
        return nullptr;
    }

    // 创建 Java 字符串
    jstring jname = env->NewStringUTF(data.name);

    // 创建 Java 对象
    return env->NewObject(dataClass, constructor, data.id, jname, data.value);
}

/**
 * @brief 根据 ID 获取数据 (JNI 封装)
 * @param id 数据 ID
 * @return Java Data 对象
 */
extern "C" JNIEXPORT jobject JNICALL
Java_com_example_jni_JniExample_getDataById(JNIEnv* env, jobject obj, jint id) {
    // 调用底层函数
    MockData data = mock_getDataById(id);

    // 找到 Java 的 Data 类
    jclass dataClass = env->FindClass("com/example/jni/JniExample$Data");
    if (dataClass == nullptr) {
        return nullptr;
    }

    // 获取构造函数
    jmethodID constructor = env->GetMethodID(dataClass, "<init>", "(ILjava/lang/String;D)V");
    if (constructor == nullptr) {
        return nullptr;
    }

    // 创建 Java 字符串
    jstring jname = env->NewStringUTF(data.name);

    // 创建 Java 对象
    return env->NewObject(dataClass, constructor, data.id, jname, data.value);
}

// ============================================================
// 3. 数组操作 (Array Operations)
// ============================================================

/**
 * @brief 数组求和 (JNI 封装)
 * @param arr Java int 数组
 * @return 元素之和
 */
extern "C" JNIEXPORT jint JNICALL
Java_com_example_jni_JniExample_sumArray(JNIEnv* env, jobject obj, jintArray arr) {
    // 获取数组长度
    jsize length = env->GetArrayLength(arr);

    // 获取数组元素指针
    jint* elements = env->GetIntArrayElements(arr, nullptr);

    // 调用底层函数
    jint result = mock_sumArray(elements, (size_t)length);

    // 释放数组
    env->ReleaseIntArrayElements(arr, elements, 0);

    return result;
}

/**
 * @brief 查找数组最大值 (JNI 封装)
 * @param arr Java int 数组
 * @return 最大值
 */
extern "C" JNIEXPORT jint JNICALL
Java_com_example_jni_JniExample_findMax(JNIEnv* env, jobject obj, jintArray arr) {
    jsize length = env->GetArrayLength(arr);
    jint* elements = env->GetIntArrayElements(arr, nullptr);

    jint result = mock_findMax(elements, (size_t)length);

    env->ReleaseIntArrayElements(arr, elements, 0);
    return result;
}

/**
 * @brief 查找数组最小值 (JNI 封装)
 * @param arr Java int 数组
 * @return 最小值
 */
extern "C" JNIEXPORT jint JNICALL
Java_com_example_jni_JniExample_findMin(JNIEnv* env, jobject obj, jintArray arr) {
    jsize length = env->GetArrayLength(arr);
    jint* elements = env->GetIntArrayElements(arr, nullptr);

    jint result = mock_findMin(elements, (size_t)length);

    env->ReleaseIntArrayElements(arr, elements, 0);
    return result;
}

/**
 * @brief 反转数组 (JNI 封装)
 * @param arr Java int 数组 (会被修改)
 */
extern "C" JNIEXPORT void JNICALL
Java_com_example_jni_JniExample_reverseArray(JNIEnv* env, jobject obj, jintArray arr) {
    jsize length = env->GetArrayLength(arr);
    jint* elements = env->GetIntArrayElements(arr, nullptr);

    mock_reverseArray(elements, (size_t)length);

    // 释放数组并提交修改 (JNI_ABORT = 0 表示提交修改)
    env->ReleaseIntArrayElements(arr, elements, 0);
}

/**
 * @brief 填充数组 (JNI 封装)
 * @param arr Java int 数组 (会被修改)
 * @param value 填充值
 */
extern "C" JNIEXPORT void JNICALL
Java_com_example_jni_JniExample_fillArray(JNIEnv* env, jobject obj, jintArray arr, jint value) {
    jsize length = env->GetArrayLength(arr);
    jint* elements = env->GetIntArrayElements(arr, nullptr);

    mock_fillArray(elements, (size_t)length, value);

    env->ReleaseIntArrayElements(arr, elements, 0);
}

/**
 * @brief 批量处理数组 - 每个元素乘以因子 (JNI 封装)
 * @param arr Java int 数组 (会被修改)
 * @param factor 乘数因子
 */
extern "C" JNIEXPORT void JNICALL
Java_com_example_jni_JniExample_multiplyArray(JNIEnv* env, jobject obj, jintArray arr, jint factor) {
    jsize length = env->GetArrayLength(arr);
    jint* elements = env->GetIntArrayElements(arr, nullptr);

    // 每个元素乘以因子
    for (jsize i = 0; i < length; i++) {
        elements[i] *= factor;
    }

    env->ReleaseIntArrayElements(arr, elements, 0);
}

/**
 * @brief 计算点数组中心 (JNI 封装)
 * @param points Java Point[] 数组
 * @return 中心点 (Java Point 对象)
 */
extern "C" JNIEXPORT jobject JNICALL
Java_com_example_jni_JniExample_calculateCenter(JNIEnv* env, jobject obj, jobjectArray points) {
    jsize count = env->GetArrayLength(points);
    if (count == 0) {
        return nullptr;
    }

    // 获取 Point 类
    jclass pointClass = env->GetObjectClass(env->GetObjectArrayElement(points, 0));
    jfieldID xField = env->GetFieldID(pointClass, "x", "I");
    jfieldID yField = env->GetFieldID(pointClass, "y", "I");

    // 创建 C 数组
    MockPoint* cPoints = (MockPoint*)malloc(count * sizeof(MockPoint));

    // 转换 Java 数组到 C 数组
    for (jsize i = 0; i < count; i++) {
        jobject jpoint = env->GetObjectArrayElement(points, i);
        cPoints[i].x = env->GetIntField(jpoint, xField);
        cPoints[i].y = env->GetIntField(jpoint, yField);
    }

    // 调用底层函数
    MockPoint center = mock_calculateCenter(cPoints, (size_t)count);

    // 释放 C 数组
    free(cPoints);

    // 创建返回的 Java Point 对象
    jmethodID constructor = env->GetMethodID(pointClass, "<init>", "(II)V");
    return env->NewObject(pointClass, constructor, center.x, center.y);
}

// ============================================================
// 4. 字符串操作 (String Operations)
// ============================================================

/**
 * @brief 计算字符串长度 (JNI 封装)
 * @param str Java 字符串
 * @return 字符串长度
 */
extern "C" JNIEXPORT jint JNICALL
Java_com_example_jni_JniExample_stringLength(JNIEnv* env, jobject obj, jstring str) {
    // 获取 UTF-8 字符串
    const char* utfStr = env->GetStringUTFChars(str, nullptr);

    // 调用底层函数
    jint result = mock_stringLength(utfStr);

    // 释放字符串
    env->ReleaseStringUTFChars(str, utfStr);

    return result;
}

/**
 * @brief 复制字符串 (JNI 封装)
 * @param src 源字符串
 * @return 复制后的新字符串
 */
extern "C" JNIEXPORT jstring JNICALL
Java_com_example_jni_JniExample_copyString(JNIEnv* env, jobject obj, jstring src) {
    const char* utfSrc = env->GetStringUTFChars(src, nullptr);

    // 分配目标缓冲区
    size_t len = strlen(utfSrc) + 1;
    char* dest = (char*)malloc(len);

    // 调用底层函数
    mock_copyString(utfSrc, dest, (int)len);

    // 创建 Java 字符串
    jstring result = env->NewStringUTF(dest);

    // 释放资源
    free(dest);
    env->ReleaseStringUTFChars(src, utfSrc);

    return result;
}

/**
 * @brief 字符串处理 - 转换为大写 (JNI 封装)
 * @param input 输入字符串
 * @return 大写字符串
 */
extern "C" JNIEXPORT jstring JNICALL
Java_com_example_jni_JniExample_toUpperCase(JNIEnv* env, jobject obj, jstring input) {
    const char* utfInput = env->GetStringUTFChars(input, nullptr);

    // 转换为大写
    std::string result(utfInput);
    for (char& c : result) {
        c = (char)toupper(c);
    }

    env->ReleaseStringUTFChars(input, utfInput);

    return env->NewStringUTF(result.c_str());
}

/**
 * @brief 字符串处理 - 转换为小写 (JNI 封装)
 * @param input 输入字符串
 * @return 小写字符串
 */
extern "C" JNIEXPORT jstring JNICALL
Java_com_example_jni_JniExample_toLowerCase(JNIEnv* env, jobject obj, jstring input) {
    const char* utfInput = env->GetStringUTFChars(input, nullptr);

    // 转换为小写
    std::string result(utfInput);
    for (char& c : result) {
        c = (char)tolower(c);
    }

    env->ReleaseStringUTFChars(input, utfInput);

    return env->NewStringUTF(result.c_str());
}

/**
 * @brief 生成动态格式化字符串 (JNI 封装)
 * @param prefix 前缀字符串
 * @param number 数字
 * @return 格式化字符串
 */
extern "C" JNIEXPORT jstring JNICALL
Java_com_example_jni_JniExample_formatString(JNIEnv* env, jobject obj, jstring prefix, jint number) {
    const char* prefixStr = env->GetStringUTFChars(prefix, nullptr);

    // 调用底层函数
    char* result = mock_getDynamicString(prefixStr, number);

    env->ReleaseStringUTFChars(prefix, prefixStr);

    if (result == nullptr) {
        return nullptr;
    }

    // 创建 Java 字符串
    jstring jresult = env->NewStringUTF(result);

    // 释放动态分配的字符串
    mock_freeString(result);

    return jresult;
}

// ============================================================
// 5. 内存管理 (Memory Management)
// ============================================================

/**
 * @brief 分配整数数组 (JNI 封装)
 * @param length 数组长度
 * @return Java int 数组
 */
extern "C" JNIEXPORT jintArray JNICALL
Java_com_example_jni_JniExample_allocateIntArray(JNIEnv* env, jobject obj, jint length) {
    // 调用底层函数
    int* arr = mock_allocateIntArray((size_t)length);
    if (arr == nullptr) {
        return nullptr;
    }

    // 创建 Java 数组
    jintArray jarr = env->NewIntArray(length);
    env->SetIntArrayRegion(jarr, 0, length, arr);

    // 释放 C 数组
    mock_freeIntArray(arr);

    return jarr;
}

/**
 * @brief 分配点数组 (JNI 封装)
 * @param count 点数量
 * @return Java Point[] 数组
 */
extern "C" JNIEXPORT jobjectArray JNICALL
Java_com_example_jni_JniExample_allocatePointArray(JNIEnv* env, jobject obj, jint count) {
    // 找到 Point 类
    jclass pointClass = env->FindClass("com/example/jni/JniExample$Point");
    if (pointClass == nullptr) {
        return nullptr;
    }

    // 创建 Java 对象数组
    jobjectArray jarr = env->NewObjectArray(count, pointClass, nullptr);
    jmethodID constructor = env->GetMethodID(pointClass, "<init>", "(II)V");

    // 调用底层函数
    MockPoint* points = mock_allocatePointArray((size_t)count);
    if (points == nullptr) {
        return nullptr;
    }

    // 转换为 Java 对象
    for (jsize i = 0; i < count; i++) {
        jobject jpoint = env->NewObject(pointClass, constructor, points[i].x, points[i].y);
        env->SetObjectArrayElement(jarr, i, jpoint);
    }

    // 释放 C 数组
    mock_freePointArray(points);

    return jarr;
}

// ============================================================
// 6. 回调函数支持 (Callback Support)
// ============================================================

/**
 * @brief 全局引用 Java 回调对象 (用于异步回调)
 * @note 实际项目中应使用更复杂的回调机制
 */
static jobject g_callbackObject = nullptr;
static jclass g_callbackClass = nullptr;
static jmethodID g_callbackMethod = nullptr;

/**
 * @brief 注册回调对象 (JNI 封装)
 * @param callback Java 回调对象
 * @return 成功返回 0，失败返回 -1
 */
extern "C" JNIEXPORT jint JNICALL
Java_com_example_jni_JniExample_registerCallback(JNIEnv* env, jobject obj, jobject callback) {
    if (callback == nullptr) {
        return MOCK_ERROR_NULL_POINTER;
    }

    // 创建全局引用
    g_callbackObject = env->NewGlobalRef(callback);
    g_callbackClass = env->GetObjectClass(callback);
    g_callbackMethod = env->GetMethodID(g_callbackClass, "onProgress", "(IILjava/lang/String;)V");

    if (g_callbackMethod == nullptr) {
        return MOCK_ERROR_NOT_FOUND;
    }

    return MOCK_SUCCESS;
}

/**
 * @brief 执行长时间操作并回调 (JNI 封装)
 * @param steps 步骤数
 * @param callback Java 回调对象
 *
 * @note Java 端回调接口：
 *   public interface ProgressCallback {
 *       void onProgress(int current, int total, String status);
 *   }
 */
extern "C" JNIEXPORT void JNICALL
Java_com_example_jni_JniExample_longOperationWithCallback(JNIEnv* env, jobject obj, jint steps, jobject callback) {
    if (callback == nullptr || steps <= 0) {
        return;
    }

    // 获取回调类和方法
    jclass callbackClass = env->GetObjectClass(callback);
    jmethodID callbackMethod = env->GetMethodID(callbackClass, "onProgress", "(IILjava/lang/String;)V");

    if (callbackMethod == nullptr) {
        return;
    }

    // 执行操作并回调
    for (int i = 1; i <= steps; i++) {
        Sleep(100);  // 模拟工作

        // 构造状态字符串
        char status[32];
        sprintf_s(status, 32, "Step %d/%d", i, steps);
        jstring jstatus = env->NewStringUTF(status);

        // 调用 Java 回调
        env->CallVoidMethod(callback, callbackMethod, i, steps, jstatus);
    }
}

/**
 * @brief 释放回调对象 (JNI 封装)
 */
extern "C" JNIEXPORT void JNICALL
Java_com_example_jni_JniExample_releaseCallback(JNIEnv* env, jobject obj) {
    if (g_callbackObject != nullptr) {
        env->DeleteGlobalRef(g_callbackObject);
        g_callbackObject = nullptr;
    }
}