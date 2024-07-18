#ifndef THREAD_H
#define THREAD_H
#define BAL_THREAD_SUCCESS 0x0
#define BAL_THREAD_FORCED_TO_EXIT 0xFF
#define BAL_THREAD_ALREADY_DESTROYED 0x1
#define BAL_THREAD_ALREADY_PAUSED 0x2
#define BAL_THREAD_SLEEPING 0x3
#define BAL_THREAD_ALREADY_RUNNING 0x4
#define BAL_THREAD_CANNOT_STOP 0x5
#define BAL_THREAD_DOESNT_EXIST 0x6
#define BAL_THREAD_FINISHED 0x7
#define BAL_THREAD_ERROR_MEMORY 0x8
#define BAL_THREAD_ERROR_CREATION 0x9
#define BAL_THREAD_ERROR_ACCESS 0xA
#define BAL_THREAD_ERROR_PAUSE 0xB
#define BAL_THREAD_ERROR_RESUME 0xC

/// @brief Generic thread manager
typedef void* Thread;

typedef struct ThreadOptions_t
{
    int empty;
} ThreadOptions;

typedef struct ThreadStatus_t
{
    int isAlive;
    int isPaused;
} ThreadInformation;

typedef int(*BALThreadStart)(void* params);

/// @brief Creates and runs a thread on current process
/// @param thread [OUT] Thread's handler
/// @param startFunction [IN] Thread's start position
/// @param startFunction [IN] Thread's start position
/// @return Generic thread manager
int ThreadCreate(Thread* thread, BALThreadStart startPoint, void* parameters);

/// @brief [MADE IN PROGRESS!] Creates and runs a thread on current process
/// @param startFunction [IN] Thread's start position
/// @param threadOptions [IN] Thread's options
/// @return Generic thread manager
int ThreadCreateA(Thread* thread, BALThreadStart startPoint, ThreadOptions threadOptions);

int ThreadSleep(Thread thread, int miliseconds);

int ThreadPause(Thread thread);
int ThreadResume(Thread thread);

int ThreadStatus(Thread thread, ThreadInformation* status);

Thread ThreadGetCurrent();

int ThreadWaitForExit(Thread thread, int* exitCode);

/// @brief Stops and destroys the thread
/// @param [IN, OUT] A fully working thread manager
int ThreadDestroy(Thread* thread);

#endif