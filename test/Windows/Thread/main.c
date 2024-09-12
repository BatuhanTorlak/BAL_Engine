#include <stdio.h>

#include <thread/thread.h>

int BalThreadTest(void* params)
{
    printf("Param: %i!\n", params);
    for (int x = 0; x < 5; x++)
    {
        printf("Thread test!\n");
        ThreadSleepA(2000);
    }
    return 0;
}

int main()
{
    Thread _thread;
    int _c = ThreadCreate(&_thread, (BALThreadStart)BalThreadTest, (void*)20);

    ThreadSleep(_thread, 5000);

    ThreadWaitForExit(_thread, 0);

    ThreadDestroy(&_thread);
}