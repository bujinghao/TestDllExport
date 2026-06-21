// TestDllExport.cpp : 定义 DLL 的导出函数。
//

#include "pch.h"
#include "framework.h"
#include "TestDllExport.h"
#include "ios"

#include "iostream" // 为了使用 std命名空间， 可以 using namespace std;
//#include "jni.h"  //  
#include <jni.h>  // 使用 <> 语法


// 这是导出变量的一个示例
TESTDLLEXPORT_API int nTestDllExport=0;

// 这是导出函数的一个示例。
TESTDLLEXPORT_API int fnTestDllExport(void)
{
    return 0;
}

// 这是已导出类的构造函数。
CTestDllExport::CTestDllExport()
{
    return;
}

TESTDLLEXPORT_API int getVersion(char* buffer, int size) {
    const char* ver = "v1.0.0";
    strncpy_s(buffer, size, ver, _TRUNCATE);
    return 0;
}

TESTDLLEXPORT_API int addNumbers(int a, int b) {

    int count = a + b;
    printf("JNA——C程序里面的计算结果：%d\n", count);

    std::cout << "JNA——C程序里面的计算结果：" << count;
    printf("\n");

    return count;
}

JNIEXPORT jint JNICALL 
Java_com_example_SimgleDllCall_getTestVal(
    JNIEnv* env,
        jobject obj) {
    return 999;
}

/*  确保整个项目中（包括 .h 和 .cpp ），对于 extern "C" 函数，
    只能有一个声明/定义 ，且签名必须完全匹配。如果 .h 里已经有了，
    .cpp 里直接写实现即可，
    不要重复写 extern "C" ... 函数名(...); 这种声明语句。
    DLL/C++ 端 ：函数名必须是 Java_包名_类名_方法名
*/
//JNIEXPORT jint JNICALL
//Java_com_example_SimgleDllCall_addNumbers(

extern "C" JNIEXPORT jint JNICALL
    Java_com_example_SimgleDllCall_addNumbers(
    JNIEnv* env,    // 必须有, 用于调用 JNI 函数
    jobject obj,    // 必须有, 用于获取 Java 对象的指针
    int a, int b) {

    int count = a + b;
    printf("JNI——C程序里面的计算结果：%d\n", count);

    std::cout << "JNI——C程序里面的计算结果："<<count;
    printf("\n");

    return count;
}
