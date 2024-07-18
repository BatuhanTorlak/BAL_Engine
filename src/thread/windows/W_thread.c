#include "../../../BALE/thread/thread.h"
#include <windows.h>
#define WIN_THREAD(thread) ((WinThread*)thread)

typedef struct WinThread_t
{
    HANDLE threadHandle;
    DWORD threadID;
    int isPaused;
    int isAlive;
    int exitCode;
} WinThread, *PWinThread;

typedef struct BalThreadParams_t
{
    BALThreadStart start;
    void* params;
    PWinThread balThread;
} BalThreadParams;

typedef struct BalWaitParams_t
{
    HANDLE handle;
    PWinThread thread;
    int miliseconds;
} BalWaitParams;

WinThread* _processThread = 0;

DWORD BalThreadStart(BalThreadParams* params);
DWORD BalWait(BalWaitParams* params);

DWORD BalThreadStart(BalThreadParams* params)
{
    BalThreadParams _params = *params;
    free(params);
    PWinThread _thrd = _params.balThread;
    _thrd->exitCode = _params.start(params->params);
    _thrd->isAlive = 0;
    return 0;
}

DWORD BalWait(BalWaitParams* params)
{
    BalWaitParams _params = *params;
    free(params);
    Sleep(_params.miliseconds);
    ThreadResume(_params.thread);
    CloseHandle(_params.handle);
}

int ThreadCreate(Thread* thread, BALThreadStart startPoint, void* parameters)
{
    *thread = 0;
    PWinThread _thrd = malloc(sizeof(WinThread));
    if (_thrd == 0)
        return BAL_THREAD_ERROR_MEMORY;
    _thrd->isAlive = 1;
    _thrd->isPaused = 0;

    BalThreadParams* _params = malloc(sizeof(BalThreadParams));
    if (_params == 0)
    {
        free(_thrd);
        return BAL_THREAD_ERROR_MEMORY;
    }
    *_params = (BalThreadParams){
        .balThread = _thrd,
        .params = parameters,
        .start = startPoint
    };

    _thrd->threadHandle = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)BalThreadStart, _params, 0, &_thrd->threadID);
    if (_thrd->threadHandle == 0)
    {
        free(_thrd);
        free(_params);
        return BAL_THREAD_ERROR_CREATION;
    }
    *thread = _thrd;

    return BAL_THREAD_SUCCESS;
}

int ThreadSleep(Thread thread, int miliseconds)
{
    if (WIN_THREAD(thread)->threadHandle != GetCurrentThread())
    {
        int _code = ThreadPause(thread);
        if (_code != BAL_THREAD_SUCCESS)
            return _code;
    }
    BalWaitParams* _params = malloc(sizeof(BalWaitParams));
    if (_params == 0)
    {
        ThreadResume(thread);
        return BAL_THREAD_ERROR_MEMORY;
    }
    _params->thread = thread;
    _params->miliseconds = miliseconds;
    HANDLE _h = CreateThread(0, 0, BalWait, _params, CREATE_SUSPENDED, 0);
    if (_h == 0)
    {
        free(_params);
        ThreadResume(thread);
        return BAL_THREAD_ERROR_CREATION;
    }
    _params->handle = _h;
    DWORD _c = ResumeThread(_h);
    if (_c == -1)
        return BAL_THREAD_ERROR_PAUSE;
    return BAL_THREAD_SUCCESS;
}

int ThreadPause(Thread thread)
{
    if (thread == 0)
        return BAL_THREAD_DOESNT_EXIST;
    if (WIN_THREAD(thread)->isAlive == 0)
        return BAL_THREAD_FINISHED;
    if (WIN_THREAD(thread)->isPaused == 1)
        return BAL_THREAD_ALREADY_PAUSED;
    if (WIN_THREAD(thread)->isPaused == 2)
        return BAL_THREAD_SLEEPING;
    DWORD _code = SuspendThread(WIN_THREAD(thread)->threadHandle);
    if (_code == -1)
        return BAL_THREAD_ERROR_PAUSE;
    WIN_THREAD(thread)->isPaused = 1;
    return BAL_THREAD_SUCCESS;
}

int ThreadResume(Thread thread)
{
    if (thread == 0)
        return BAL_THREAD_DOESNT_EXIST;
    if (WIN_THREAD(thread)->isAlive == 0)
        return BAL_THREAD_FINISHED;
    if (WIN_THREAD(thread)->isPaused == 0)
        return BAL_THREAD_ALREADY_RUNNING;
    if (WIN_THREAD(thread)->isPaused == 2)
        return BAL_THREAD_SLEEPING;
    DWORD _code = ResumeThread(WIN_THREAD(thread)->threadHandle);
    if (_code == -1)
        return GetLastError();
    WIN_THREAD(thread)->isPaused = 0;
    return BAL_THREAD_SUCCESS;
}

Thread ThreadGetCurrent()
{
}

int ThreadWaitForExit(Thread thread, int* exitCode)
{
    if (thread == 0)
        return BAL_THREAD_DOESNT_EXIST;
    if (WIN_THREAD(thread)->isAlive == 0)
        return BAL_THREAD_FINISHED;
    WaitForSingleObject(WIN_THREAD(thread)->threadHandle, INFINITE);
    if (exitCode != 0)
        *exitCode = WIN_THREAD(thread)->exitCode;
    return BAL_THREAD_SUCCESS;
}

int ThreadDestroy(Thread* thread)
{
    if (thread == 0 || *thread == 0)
        return BAL_THREAD_DOESNT_EXIST;
    HANDLE _threadHandle = WIN_THREAD(*thread)->threadHandle;

    if (WIN_THREAD(*thread)->isAlive)
    {
        TerminateThread(_threadHandle, 0);
        CloseHandle(_threadHandle);
        free(*thread);
        *thread = 0;
        return BAL_THREAD_FORCED_TO_EXIT;
    }
    CloseHandle(_threadHandle);
    free(*thread);
    *thread = 0;
    return BAL_THREAD_SUCCESS;
}