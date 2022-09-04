#include <vld.h> // �����ڴ�й¶��顣����MSVC Debugģʽ�²����á�

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <iostream>

#include <thread> // ������֤���߳� MY_XADD �Ƿ�ȫ�� test �����д
#include <mutex> // ������֤�������� MY_XADD �Ƿ���Ч�ĳ���

class Array {
public:
    Array();
    explicit Array(uint32_t _len);
    Array(uint32_t _len, float* data);
    Array(const Array& arr); // �������캯��
    Array& operator=(const Array& arr); // ���Ƹ�ֵ���캯��
    ~Array();

public:
    float* data;
    int len;

private:
    int* refcount; // ���ü�������ʹ�� volatile ���Σ�ϣ���ܽ�����̷߳��ʳ�ͻ

    void addref();
    void release();
};

std::mutex g_mutex;

//static int MY_XADD(int* addr, int delta) {
//    g_mutex.lock();
//
//    int tmp = *addr;
//    *addr += delta;
//    
//    g_mutex.unlock();
//    return tmp;
//}

//static int MY_XADD(std::atomic<int>* addr, int delta) {
//    int tmp = *addr;
//    *addr += delta;
//
//    return tmp;
//}


#ifdef MY_XADD
  // allow to use user-defined macro
#elif defined __GNUC__ || defined __clang__
#  if defined __clang__ && __clang_major__ >= 3 && !defined __ANDROID__ && !defined __EMSCRIPTEN__ && !defined(__CUDACC__)  && !defined __INTEL_COMPILER
#    ifdef __ATOMIC_ACQ_REL
#      define MY_XADD(addr, delta) __c11_atomic_fetch_add((_Atomic(int)*)(addr), delta, __ATOMIC_ACQ_REL)
#    else
#      define MY_XADD(addr, delta) __atomic_fetch_add((_Atomic(int)*)(addr), delta, 4)
#    endif
#  else
#    if defined __ATOMIC_ACQ_REL && !defined __clang__
       // version for gcc >= 4.7
#      define MY_XADD(addr, delta) (int)__atomic_fetch_add((unsigned*)(addr), (unsigned)(delta), __ATOMIC_ACQ_REL)
#    else
#      define MY_XADD(addr, delta) (int)__sync_fetch_and_add((unsigned*)(addr), (unsigned)(delta))
#    endif
#  endif
#elif defined _MSC_VER && !defined RC_INVOKED
#  include <intrin.h>
#  define MY_XADD(addr, delta) (int)_InterlockedExchangeAdd((long volatile*)addr, delta)
#else
#ifdef OPENCV_FORCE_UNSAFE_XADD
CV_INLINE MY_XADD(int* addr, int delta) { int tmp = *addr; *addr += delta; return tmp; }
#else
#error "OpenCV: can't define safe MY_XADD macro for current platform (unsupported). Define MY_XADD macro through custom port header (see OPENCV_INCLUDE_PORT_FILE)"
#endif
#endif


// ���ü���������1��
void Array::addref() {
    if (refcount) {
        MY_XADD(refcount, 1);
    }
}

// ���ü���������1��
// �������������0�����ͷ�data��refcount������NULL
void Array::release() {
    // ��� refcount ��Ϊ NULL����ô��
    //    ���¼������� *refcount = *refcount - 1
    //    �ж����ԭ���ļ�����ֵΪ1����ô���ڼ�������ֵΪ0��Ӧ���ͷ�����ڴ�
    if (refcount && MY_XADD(refcount, -1) == 1) {
        // end for refcount
        delete(refcount);
        refcount = NULL;

        // end for Array object's shared heap memory variable(s)
        free(data);
        data = NULL;
    }
}

Array::Array() :
    len(0), data(NULL), refcount(NULL) // ���캯�����������ڴ棬refcount��ʼ��ΪNULL
{

}

Array::Array(uint32_t _len) :
    len(_len), refcount(new int(0))
{
    data = (float*)malloc(sizeof(float) * len);
    addref(); // ���캯���������ڴ棬refcount �����ڴ棬Ȼ��+1
}

Array::Array(uint32_t _len, float* data) :
    len(_len), data(data), refcount(NULL) // ���캯�����������ڴ棬refcount��ʼ��ΪNULL
{

}

// �������캯��
Array::Array(const Array& arr) :
    refcount(arr.refcount) // �� arr.refcount��ֵ��refcount
{
    addref();

    len = arr.len;
    data = arr.data;
}

Array& Array::operator=(const Array& arr)
{
    // ��ǰarray����������״̬��refcount��Ϊ�գ���Ϊ��
    // ����Ĳ�������arrҲ������״̬��refcount��Ϊ�գ���Ϊ��
    // ���һ����4��״̬
    if (this != &arr)
    {
        release();
        refcount = arr.refcount;
        addref();

        data = arr.data;
        len = arr.len;
    }

    // ��������arr�����Լ�����ʲô����Ӧ����
    // ���ǰ���refcount��������Ա�ĸ�ֵŪ���ˣ�Ҳ�����ﷵ��
    return *this;
}

Array::~Array()
{
    release();
}


// ���ü������� - �������캯��
void test_copy_ctor()
{
    {
        int len = 10;
        Array arr1(len);
        Array arr2 = arr1;
    }

    {
        int len = 10;
        Array arr1(len);
        Array arr2 = arr1;
        Array arr3 = arr1;
    }

    {
        Array arr1;
        Array arr2 = arr1;
        Array arr3 = arr2;
    }

    float* data = (float*)malloc(sizeof(float) * 10);
    {
        Array arr1(10, data);
        Array arr2 = arr1;
    }
    free(data);
}

// ���ü������� - ���Ƹ�ֵ����,case1
// ��ǰ����Ϊ0����������ҲΪ0
void test_assign_op_case1()
{
    {
        int len = 10;
        float* data = (float*)malloc(sizeof(float) * len);
        {
            Array arr1(len, data);
            Array arr2;

            arr2 = arr1;
        }
        free(data);
    }

    {
        Array arr1;
        Array arr2;
        arr2 = arr1;
    }

    {
        Array arr1;
        arr1 = arr1;
    }
}

// ���ü������� - ���Ƹ�ֵ����,case2
// ��ǰ����Ϊ0������������Ϊ0
void test_assign_op_case2()
{
    {
        int len = 10;
        Array arr1(len);
        Array arr2;

        arr2 = arr1;
    }

    {
        int len = 10;
        Array arr1(len);

        Array arr2;
        Array arr3;

        arr2 = arr1;
        arr3 = arr1;
    }
}

// ���ü������� - ���Ƹ�ֵ����,case2
// ��ǰ������Ϊ0����������Ϊ0
void test_assign_op_case3()
{
    {
        Array arr1;

        int len = 10;
        Array arr2(len);

        arr2 = arr1;
    }

    {
        Array arr1;

        int len = 10;
        Array arr2(len);

        Array arr3 = arr2;

        arr2 = arr1;
    }
}

// ���ü������� - ���Ƹ�ֵ����,case2
// ��ǰ������Ϊ0������������Ϊ0
void test_assign_op_case4()
{
    {
        int len = 10;
        Array arr1(len);
        Array arr2(len);

        arr2 = arr1;
    }

    {
        int len = 10;
        Array arr1(len);

        Array arr2 = arr1;

        Array arr3 = Array(len);

        arr2 = arr3;
    }

    {
        Array arr1(10);
        arr1 = arr1;
    }
}

Array g_arr;

void do_work()
{
    int loop_counter = 2000000; // ѭ�����������������״����̲߳���ȫ���⣬���Ա�1000�ٴ�һЩ
    // ���������������֣�����VS2019 x64 Debugģʽ�²��Է��֣���
    // ��һ�֣��ڴ�й¶��vld�ᱨ�����(loop_counter=1000��2000ʱ����)���Ǳ��֣������ʺܸ�
    // �ڶ��֣����б���Ϊ��ѭ������ʱ�䲻����� (loop_counter����2000�����ֵʱ���֣����еȸ���
    // �����֣��Ƿ��ڴ���ʣ�����ΪVS���� Debug Assertion Failed. (loop_counter=2000ʱ���֣�
    //
    // ��Release ģʽ�£���Ը��Ѹ������⣬��Ҫ����loop_counter��200000������󣬲��ܷ��֡��쳣�����
    //   (����ֵ������0������-1073740940)
    while (loop_counter--) {
        Array tmp_arr = g_arr;
    }
}

void test_multithread()
{
    std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();//��ʼʱ��

    std::thread th1(do_work);
    std::thread th2(do_work);
    std::thread th3(do_work);
    std::thread th4(do_work);
    std::thread th5(do_work);
    std::thread th6(do_work);
    std::thread th7(do_work);
    std::thread th8(do_work);
    std::thread th9(do_work);
    std::thread th10(do_work);

    th1.join();
    th2.join();
    th3.join();
    th4.join();
    th5.join();
    th6.join();
    th7.join();
    th8.join();
    th9.join();
    th10.join();

    std::chrono::steady_clock::time_point stop_time = std::chrono::steady_clock::now();//����ʱ��
    std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::microseconds>(stop_time - start_time);
    std::cout << "����ʱ��" << time_span.count() << " s" << std::endl; // ��ʱ
}


int  i = 0;
//std::atomic<int> i = 0;
//volatile int i = 0;
std::mutex mut;

void iplusplus() {

    //int c = 10000000;  //ѭ������
    //while (c--) {
    //    i++;
    //}

    //int c = 10000000;  //ѭ������
    //int c = 10;  //ѭ������
    int c = 10000000;  //ѭ������
    while (c--) {
        // mutex for lock.  0.95 ms
        //mut.lock();
        //i++;
        //mut.unlock();

        MY_XADD(&i, 1);  //0.25~0.34 ms

        //atomic
        //i++; // 0.34 ms
    }
}


void test_thread_safe()
{
    std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();//��ʼʱ��

    std::thread thread1(iplusplus);  //�����������߳�1
    std::thread thread2(iplusplus);  //�����������߳�2
    thread1.join();  // �ȴ��߳�1�������
    thread2.join();  // �ȴ��߳�2�������
    std::cout << "i = " << i << std::endl;

    std::chrono::steady_clock::time_point stop_time = std::chrono::steady_clock::now();//����ʱ��
    std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::microseconds>(stop_time - start_time);
    std::cout << "����ʱ��" << time_span.count() << " ms" << std::endl; // ��ʱ

}


int main()
{

    //test_copy_ctor();
    //test_assign_op_case1();
    //test_assign_op_case2();
    //test_assign_op_case3();
    //test_assign_op_case4();

    g_arr = Array(10);
    test_multithread();

    //test_thread_safe();

    return 0;
}
