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
/// @return Function status
int ThreadCreate(Thread* thread, BALThreadStart startPoint, void* parameters);

/// @brief [MADE IN PROGRESS!] Creates and runs a thread on current process
/// @param thread [OUT] Thread's handler
/// @param startFunction [IN] Thread's start position
/// @param threadOptions [IN] Thread's options
/// @return Function status
int ThreadCreateA(Thread* thread, BALThreadStart startPoint, ThreadOptions threadOptions);

/// @brief Makes choosen thread sleep for choosen miliseconds
/// @param thread [IN] Choosen thread
/// @param miliseconds [IN] Miliseconds to wait
/// @return Function status
int ThreadSleep(Thread thread, int miliseconds);

/// @brief Makes current thread wait
/// @param miliseconds [IN] Miliseconds for wait
/// @return Always returns 1 :)
int ThreadSleepA(int miliseconds);

/// @brief Makes choosen thread pause
/// @param thread [IN] Choosen thread
/// @return Function status
int ThreadPause(Thread thread);

/// @brief Makes choosen thread resume
/// @param thread [IN] Choosen thread
/// @return Function status
int ThreadResume(Thread thread);

/// @brief Gets Thread's information
/// @param thread [IN] Thread
/// @param info [OUT] Thread's information
/// @return Function status
int ThreadInfo(Thread thread, ThreadInformation* info);

/// @brief Get current thread's manager
/// @return Thread manager
Thread ThreadGetCurrent();

int ThreadWaitForExit(Thread thread, int* exitCode);

/// @brief Stops and destroys the thread
/// @param [IN, OUT] A fully working thread manager
int ThreadDestroy(Thread* thread);

#endif