#include "CommonHead.h"
#include "shm_obj_pool.h"
#include "shm_stl.h"

class A:public CXObj
{
public:
    A() {}
    ~A() {}
    bool CheckMem() { return true; }
    void Init() { /*printf("第一次使用对象内存\n");*/ }
    void Resume() { /*printf("恢复对象内存\n");*/ }
    static A* CreateObject(void* p)
    {
        A* pa = (A*)p;


        printf("val = %d\n", pa->val);


        return pa;
    }
    void Reclaim()
    {
        val = 0;
        printf("回收对象内存\n");
    }
    int val;
};

const int a_size = 4;

int TestShm()
{

    ShmVector<A> shmvec;
    ShmMap<int,A> shmvmap;

    // 第二个参数是数量
    if (ESSM_Resume == shmvec.Init(0x123456, 100, 0))
    {
        printf("恢复内存\n");
    }
    else
    {
        printf("第一次使用内存\n");
    }
    A a,b;
    a.val = 11111;
    b.val = 22222;
    shmvec.PushBack(a);

    int ret = shmvmap.Init(0x123457, 200, 0);
    if (ESSM_Resume == ret)
    {
        printf("恢复内存\n");
    }
    else if(ESSM_Init == ret)
    {
        printf("第一次使用内存\n");
    }
    else
    {
        printf("出错\n");
        return -1;
    }
    shmvmap[7] = a;
    shmvmap[3] = b;


    int index = 0;
    A* p = NULL;
    CShmObjPool<A> myobj;

    srand((unsigned int)time(0));

    myobj.CreateObjPool(1234, 1234, sizeof(A), a_size, 0, __FILE__, __LINE__);

    ENMShmStartMode starttype = myobj.GetStartFlag();
    if (starttype == ESSM_Resume)
    {
        printf("恢复内存\n");
    }
    else
    {
        printf("第一次使用内存\n");
    }


   /* for (int i = 0; i < 4; i++)
    {
        myobj.FreeObj(0, i);
    }

    printf("剩余数量：%d\n", myobj.GetUseNum());
    for (int i = 0; i < a_size; i++)
    {
        A* pa = myobj.GetObj(i, 0);
        if (pa)
        {
            printf("%d\n", pa->val);
        }
        else
        {
            printf("0\n");
        }

    }*/

    int iCount = rand() % 2 + 2;
    for (int i = 0; i < iCount; i++)
    {
        p = myobj.GetFreeObj(0, &index);
        if (p)
        {
            if (p->val != 0)
            {
                printf("===========出错=======\n");
            }
            p->val = rand();
        }
    }


    printf("使用数量：%d\n", myobj.GetUseNum());
    for (int i = 0; i < a_size; i++)
    {
        A* pa = myobj.GetObj(i, 0);
        if (pa)
        {
            printf("%d\n", pa->val);
        }
        else
        {
            printf("0\n");
        }
    }

    /* for (int i = 0; i < a_size;i++)
     {
         int index = -1;
         A* p = myobj.GetFreeObj(0, &index);
         if (p == NULL)
         {
             printf("共享内存分配完毕\n");
         }
         else
         {
             p->val = 125;
         }
     }*/


    printf("hello from TestShm!\n");
    return 0;
}