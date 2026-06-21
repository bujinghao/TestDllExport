/**
 * @file MockExportJNA.java
 * @brief Java JNA 调用 MockExport DLL 的完整示例
 *
 * 本文件演示如何使用 JNA (Java Native Access) 调用 C/C++ DLL 导出函数。
 * 涵盖：函数调用、结构体操作、回调函数、数组操作、内存管理。
 *
 * 使用前请确保：
 *   1. 项目 pom.xml 添加 JNA 依赖
 *   2. TestDllExport.dll 放在 java.library.path 或项目根目录
 *
 * @see MockExport.h - C/C++ 导出函数头文件
 */

// ============================================================
// 1. Maven 依赖配置 (pom.xml)
// ============================================================

/*
<dependency>
    <groupId>net.java.dev.jna</groupId>
    <artifactId>jna</artifactId>
    <version>5.13.0</version>
</dependency>
<dependency>
    <groupId>net.java.dev.jna</groupId>
    <artifactId>jna-platform</artifactId>
    <version>5.13.0</version>
</dependency>
*/

// ============================================================
// 2. 结构体定义 (Java JNA 映射)
// ============================================================

import com.sun.jna.*;
import com.sun.jna.ptr.*;
import java.util.Arrays;
import java.util.List;

/**
 * MockPoint 结构体 - 二维坐标点
 */
public static class MockPoint extends Structure {
    public int x;
    public int y;

    public MockPoint() {
        super();
    }

    public MockPoint(int x, int y) {
        super();
        this.x = x;
        this.y = y;
    }

    @Override
    protected List<String> getFieldOrder() {
        return Arrays.asList("x", "y");
    }

    // ByReference 用于指针传递
    public static class ByReference extends MockPoint implements Structure.ByReference {}
    public static class ByValue extends MockPoint implements Structure.ByValue {}
}

/**
 * MockData 结构体 - 通用数据结构
 */
public static class MockData extends Structure {
    public int id;
    public byte[] name = new byte[64];  // 固定长度字符数组
    public double value;

    public MockData() {
        super();
    }

    @Override
    protected List<String> getFieldOrder() {
        return Arrays.asList("id", "name", "value");
    }

    public String getName() {
        return new String(name).trim();
    }

    public void setName(String str) {
        byte[] bytes = str.getBytes();
        int len = Math.min(bytes.length, 63);
        System.arraycopy(bytes, 0, name, 0, len);
        name[len] = 0;  // null 终止符
    }

    public static class ByReference extends MockData implements Structure.ByReference {}
}

/**
 * MockLine 结构体 - 线段 (嵌套结构体示例)
 */
public static class MockLine extends Structure {
    public MockPoint.ByValue start;  // 嵌套结构体，使用 ByValue
    public MockPoint.ByValue end;
    public byte[] label = new byte[32];

    public MockLine() {
        super();
    }

    @Override
    protected List<String> getFieldOrder() {
        return Arrays.asList("start", "end", "label");
    }
}

// ============================================================
// 3. 回调函数定义 (Java JNA Callback)
// ============================================================

/**
 * 整数回调函数接口
 */
public interface MockCallbackInt extends Callback {
    int invoke(int value);
}

/**
 * 字符串回调函数接口
 */
public interface MockCallbackString extends Callback {
    void invoke(String message);
}

/**
 * 结构体回调函数接口
 */
public interface MockCallbackPoint extends Callback {
    void invoke(MockPoint.ByValue point);
}

/**
 * 进度回调函数接口
 */
public interface MockProgressCallback extends Callback {
    void invoke(int current, int total, String status);
}

// ============================================================
// 4. DLL 接口定义
// ============================================================

/**
 * MockExport DLL 接口映射
 */
public interface MockExportLibrary extends Library {

    // 加载 DLL (自动搜索 java.library.path 和当前目录)
    MockExportLibrary INSTANCE = Native.load("TestDllExport", MockExportLibrary.class);

    // ============================================================
    // 0. 错误码与版本信息
    // ============================================================

    int mock_getVersion();
    String mock_getBuildInfo();  // const char* 自动映射为 String
    int mock_getLastError();

    // ============================================================
    // 1. 基本函数调用
    // ============================================================

    int mock_add(int a, int b);
    int mock_subtract(int a, int b);
    int mock_stringLength(String str);
    int mock_copyString(String src, byte[] dest, int destSize);

    // ============================================================
    // 2. 结构体操作
    // ============================================================

    MockPoint.ByValue mock_createPoint(int x, int y);
    double mock_distanceBetweenPoints(MockPoint.ByValue p1, MockPoint.ByValue p2);
    void mock_modifyPoint(MockPoint.ByReference point, int newX, int newY);
    MockData.ByValue mock_createData(int id, String name, double value);
    MockData.ByValue mock_getDataById(int id);
    void mock_fillData(MockData.ByReference data, int id, String name, double value);

    // ============================================================
    // 3. 回调函数
    // ============================================================

    int mock_useIntCallback(int value, MockCallbackInt callback);
    void mock_useStringCallback(String message, MockCallbackString callback);
    void mock_usePointCallback(int x, int y, MockCallbackPoint callback);
    void mock_longOperation(int steps, MockProgressCallback callback);

    // ============================================================
    // 4. 数组操作
    // ============================================================

    int mock_sumArray(int[] arr, long length);  // size_t 映射为 long
    int mock_findMax(int[] arr, long length);
    int mock_findMin(int[] arr, long length);
    void mock_reverseArray(int[] arr, long length);
    void mock_fillArray(int[] arr, long length, int value);
    int mock_copyArray(int[] src, int[] dest, long length);
    MockPoint.ByValue mock_calculateCenter(MockPoint[] points, long count);
    void mock_moveAllPoints(MockPoint[] points, long count, int dx, int dy);

    // ============================================================
    // 5. 内存管理
    // ============================================================

    Pointer mock_allocateIntArray(long length);  // 返回指针
    void mock_freeIntArray(Pointer arr);
    Pointer mock_allocateData();
    void mock_freeData(Pointer data);
    Pointer mock_allocatePointArray(long count);
    void mock_freePointArray(Pointer points);
    Pointer mock_allocateString(long length);
    void mock_freeString(Pointer str);
    Pointer mock_createDynamicData(int id, String name, double value);
    Pointer mock_getDynamicString(String prefix, int number);
}

// ============================================================
// 5. 使用示例
// ============================================================

public class MockExportJNAExample {

    public static void main(String[] args) {
        MockExportLibrary lib = MockExportLibrary.INSTANCE;

        // ----------------------------------------------------
        // 0. 版本信息
        // ----------------------------------------------------
        System.out.println("=== 版本信息 ===");
        int version = lib.mock_getVersion();
        String buildInfo = lib.mock_getBuildInfo();
        System.out.println("Version: " + version);
        System.out.println("Build Info: " + buildInfo);

        // ----------------------------------------------------
        // 1. 基本函数调用
        // ----------------------------------------------------
        System.out.println("\n=== 基本函数调用 ===");
        int sum = lib.mock_add(10, 20);
        System.out.println("mock_add(10, 20) = " + sum);

        int diff = lib.mock_subtract(20, 10);
        System.out.println("mock_subtract(20, 10) = " + diff);

        int len = lib.mock_stringLength("Hello JNA");
        System.out.println("mock_stringLength(\"Hello JNA\") = " + len);

        byte[] dest = new byte[64];
        lib.mock_copyString("Source String", dest, dest.length);
        System.out.println("mock_copyString result: " + new String(dest).trim());

        // ----------------------------------------------------
        // 2. 结构体操作
        // ----------------------------------------------------
        System.out.println("\n=== 结构体操作 ===");
        MockPoint.ByValue p1 = lib.mock_createPoint(0, 0);
        MockPoint.ByValue p2 = lib.mock_createPoint(3, 4);
        double dist = lib.mock_distanceBetweenPoints(p1, p2);
        System.out.println("Distance between (0,0) and (3,4) = " + dist);

        MockPoint.ByReference pRef = new MockPoint.ByReference();
        pRef.x = 10;
        pRef.y = 20;
        lib.mock_modifyPoint(pRef, 50, 60);
        System.out.println("After modifyPoint: (" + pRef.x + ", " + pRef.y + ")");

        MockData.ByValue data = lib.mock_createData(1, "Test Data", 3.14);
        System.out.println("MockData: id=" + data.id + ", name=" + data.getName() + ", value=" + data.value);

        MockData.ByValue queryData = lib.mock_getDataById(100);
        System.out.println("getDataById(100): id=" + queryData.id + ", name=" + queryData.getName());

        // ----------------------------------------------------
        // 3. 回调函数
        // ----------------------------------------------------
        System.out.println("\n=== 回调函数 ===");

        // 整数回调
        MockCallbackInt intCallback = (value) -> {
            System.out.println("Callback received: " + value);
            return value + 1;
        };
        int callbackResult = lib.mock_useIntCallback(10, intCallback);
        System.out.println("mock_useIntCallback(10) returned: " + callbackResult);

        // 字符串回调
        MockCallbackString strCallback = (msg) -> {
            System.out.println("String callback: " + msg);
        };
        lib.mock_useStringCallback("Hello from Java!", strCallback);

        // 进度回调
        MockProgressCallback progressCallback = (current, total, status) -> {
            System.out.println("Progress: " + status + " (" + current + "/" + total + ")");
        };
        System.out.println("Starting long operation (5 steps)...");
        lib.mock_longOperation(5, progressCallback);
        System.out.println("Long operation completed.");

        // ----------------------------------------------------
        // 4. 数组操作
        // ----------------------------------------------------
        System.out.println("\n=== 数组操作 ===");
        int[] arr = {1, 2, 3, 4, 5};
        int arrSum = lib.mock_sumArray(arr, arr.length);
        System.out.println("sumArray([1,2,3,4,5]) = " + arrSum);

        int maxVal = lib.mock_findMax(arr, arr.length);
        int minVal = lib.mock_findMin(arr, arr.length);
        System.out.println("findMax = " + maxVal + ", findMin = " + minVal);

        int[] arrToReverse = {1, 2, 3, 4, 5};
        lib.mock_reverseArray(arrToReverse, arrToReverse.length);
        System.out.println("reverseArray result: " + Arrays.toString(arrToReverse));

        int[] arrToFill = new int[5];
        lib.mock_fillArray(arrToFill, arrToFill.length, 42);
        System.out.println("fillArray(42): " + Arrays.toString(arrToFill));

        // 结构体数组
        MockPoint[] points = new MockPoint[3];
        for (int i = 0; i < 3; i++) {
            points[i] = new MockPoint();
            points[i].x = i * 10;
            points[i].y = i * 5;
        }
        MockPoint.ByValue center = lib.mock_calculateCenter(points, points.length);
        System.out.println("Center of points: (" + center.x + ", " + center.y + ")");

        lib.mock_moveAllPoints(points, points.length, 100, -50);
        System.out.println("After moveAllPoints(100, -50):");
        for (MockPoint pt : points) {
            System.out.println("  Point: (" + pt.x + ", " + pt.y + ")");
        }

        // ----------------------------------------------------
        // 5. 内存管理
        // ----------------------------------------------------
        System.out.println("\n=== 内存管理 ===");

        // 分配整数数组
        Pointer intArrayPtr = lib.mock_allocateIntArray(10);
        IntArray intArray = new IntArray(intArrayPtr);
        intArray.setValue(0, 100);
        intArray.setValue(1, 200);
        System.out.println("allocateIntArray: element[0]=" + intArray.getValue(0));
        lib.mock_freeIntArray(intArrayPtr);

        // 分配结构体
        Pointer dataPtr = lib.mock_allocateData();
        MockData.ByReference dynData = new MockData.ByReference(dataPtr);
        lib.mock_fillData(dynData, 99, "Dynamic Data", 2.718);
        System.out.println("Dynamic MockData: id=" + dynData.id + ", name=" + dynData.getName());
        lib.mock_freeData(dataPtr);

        // 动态字符串
        Pointer strPtr = lib.mock_getDynamicString("ID_", 123);
        String dynamicStr = strPtr.getString(0);
        System.out.println("getDynamicString(\"ID_\", 123) = " + dynamicStr);
        lib.mock_freeString(strPtr);

        System.out.println("\n=== 所有测试完成 ===");
    }
}

// ============================================================
// 6. 注意事项
// ============================================================

/*
【重要注意事项】

1. 结构体对齐
   - JNA 默认使用平台对齐，与 C/C++ 默认对齐一致
   - 如需精确控制，使用 @Structure.FieldOrder 注解或 getFieldOrder() 方法
   - 嵌套结构体使用 ByValue (值传递) 或 ByReference (指针传递)

2. 字符串处理
   - C 的 char* 自动映射为 Java String (UTF-8)
   - 固定长度字符数组使用 byte[] 映射
   - 动态分配的字符串返回 Pointer，需调用 free 释放

3. 数组处理
   - Java 数组自动映射为 C 数组指针
   - size_t 类型映射为 Java long (64位)
   - 修改数组会直接影响 Java 端数组内容

4. 回调函数
   - 回调接口必须继承 com.sun.jna.Callback
   - 回调方法名必须是 invoke
   - 回调在 C 端执行，注意线程安全

5. 内存管理
   - 动态分配的内存必须手动释放
   - 使用 Pointer 包装返回的指针
   - 避免内存泄漏：分配后必须释放

6. 错误处理
   - 检查返回值是否为错误码 (负值)
   - 使用 mock_getLastError() 获取详细错误信息
   - 空指针检查：C 端返回 null 时 Pointer 为 null

7. 调用约定
   - JNA 默认使用 cdecl 调用约定
   - 如 DLL 使用 stdcall，需在 Library 接口添加 @Options(convention=Convention.STDCALL)

8. DLL 加载
   - DLL 必须放在 java.library.path 指定的路径
   - 或放在项目根目录、src/main/resources
   - Windows 下需要同时有 .dll 和 .lib 文件
*/