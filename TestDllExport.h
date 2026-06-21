// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 TESTDLLEXPORT_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// TESTDLLEXPORT_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef TESTDLLEXPORT_EXPORTS
#define TESTDLLEXPORT_API __declspec(dllexport)
#else
#define TESTDLLEXPORT_API __declspec(dllimport)
#endif

// 此类是从 dll 导出的
class TESTDLLEXPORT_API CTestDllExport {
public:
	CTestDllExport(void);
	// TODO: 在此处添加方法。
};

extern TESTDLLEXPORT_API int nTestDllExport;

TESTDLLEXPORT_API int fnTestDllExport(void);

/* 1. 简单函数 JNA调用, extern "C"  被JNA查询到。

	-头文件声明 ：加上 extern "C" 告诉其他 C++ 文件以 C 链接方式引用此函数
	- cpp 实现 ：加上 extern "C" 确保编译时不进行 C++ name mangling，JVM 能正确找到符号
*/
extern "C" TESTDLLEXPORT_API int addNumbers(int a, int b);

/* 1. 简单函数 JNI调用
JNI 的函数名映射规则是：

- DLL / C++ 端 ：函数名必须是 Java_包名_类名_方法名
- Java 端 ：方法名保持原始名称（如 addNumbers ），JVM 会自动查找对应的 Java_包名_类名_方法名


*/
//extern "C" TESTDLLEXPORT_API int Java_com_example_NativeDllCall_addNumbers(int a, int b);

// 1. 简单函数
extern "C" TESTDLLEXPORT_API int getVersion(char* buffer, int size);

// 1. 简单函数 JNI调用
extern "C" TESTDLLEXPORT_API int Java_com_example_SimgleDllCall_getTestVal();


