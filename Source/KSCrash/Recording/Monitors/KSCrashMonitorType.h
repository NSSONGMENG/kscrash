//
//  KSCrashMonitorType.h
//
//  Copyright (c) 2012 Karl Stenerud. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall remain in place
// in this source code.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//


#ifndef HDR_KSCrashMonitorType_h
#define HDR_KSCrashMonitorType_h

#ifdef __cplusplus
extern "C" {
#endif


/** Various aspects of the system that can be monitored:
 * - Mach kernel exception
 * - Fatal signal
 * - Uncaught C++ exception
 * - Uncaught Objective-C NSException
 * - Deadlock on the main thread
 * - User reported custom exception
 */
typedef enum
{
    /* Captures and reports Mach exceptions. */
    KSCrashMonitorTypeMachException      = 0x01,    // mach异常
    
    /* Captures and reports POSIX signals. */
    KSCrashMonitorTypeSignal             = 0x02,    // POSIX异常信号
    
    /* Captures and reports C++ exceptions.
     * Note: This will slightly slow down exception processing.
     */
    KSCrashMonitorTypeCPPException       = 0x04,    // C++异常
    
    /* Captures and reports NSExceptions. */
    KSCrashMonitorTypeNSException        = 0x08,    // NSExceptions异常
    
    /* Detects and reports a deadlock in the main thread. */
    KSCrashMonitorTypeMainThreadDeadlock = 0x10,    // 主线程假死
    
    /* Accepts and reports user-generated exceptions. */
    KSCrashMonitorTypeUserReported       = 0x20,    // 用户退出
    
    /* Keeps track of and injects system information. */
    KSCrashMonitorTypeSystem             = 0x40,    // 跟踪并注入系统信息 ？
    
    /* Keeps track of and injects application state. */
    KSCrashMonitorTypeApplicationState   = 0x80,    // 跟踪并注入应用程序信息 ？
    
    /* Keeps track of zombies, and injects the last zombie NSException. */
    KSCrashMonitorTypeZombie             = 0x100,   // 跟踪僵尸对象并注入僵尸异常捕获
} KSCrashMonitorType;

// 全部type
#define KSCrashMonitorTypeAll              \
(                                          \
    KSCrashMonitorTypeMachException      | \
    KSCrashMonitorTypeSignal             | \
    KSCrashMonitorTypeCPPException       | \
    KSCrashMonitorTypeNSException        | \
    KSCrashMonitorTypeMainThreadDeadlock | \
    KSCrashMonitorTypeUserReported       | \
    KSCrashMonitorTypeSystem             | \
    KSCrashMonitorTypeApplicationState   | \
    KSCrashMonitorTypeZombie               \
)

// 假死检测 - 实验阶段
#define KSCrashMonitorTypeExperimental     \
(                                          \
    KSCrashMonitorTypeMainThreadDeadlock   \
)

// mach异常，异常信号，c++异常，OC异常 - debug环境不可用
#define KSCrashMonitorTypeDebuggerUnsafe   \
(                                          \
    KSCrashMonitorTypeMachException      | \
    KSCrashMonitorTypeSignal             | \
    KSCrashMonitorTypeCPPException       | \
    KSCrashMonitorTypeNSException          \
)

#define KSCrashMonitorTypeAsyncSafe        \
(                                          \
    KSCrashMonitorTypeMachException      | \
    KSCrashMonitorTypeSignal               \
)

// 僵尸对象异常，可选选项
#define KSCrashMonitorTypeOptional         \
(                                          \
    KSCrashMonitorTypeZombie               \
)
    
#define KSCrashMonitorTypeAsyncUnsafe (KSCrashMonitorTypeAll & (~KSCrashMonitorTypeAsyncSafe))

/** Monitors that are safe to enable in a debugger. 可在调试时安全使用 */
#define KSCrashMonitorTypeDebuggerSafe (KSCrashMonitorTypeAll & (~KSCrashMonitorTypeDebuggerUnsafe))

/** Monitors that are safe to use in a production environment.
 * All other monitors should be considered experimental.
 *
 * 生产环境可安全使用， 去除主线程假死检测
 */
#define KSCrashMonitorTypeProductionSafe (KSCrashMonitorTypeAll & (~KSCrashMonitorTypeExperimental))

/** Production safe monitors, minus the optional ones. 生产环境安全，去除主线程假死检测和僵尸异常检测*/
#define KSCrashMonitorTypeProductionSafeMinimal (KSCrashMonitorTypeProductionSafe & (~KSCrashMonitorTypeOptional))

/** Monitors that are required for proper operation.
 * These add essential information to the reports, but do not trigger reporting.
 *
 * 向报表中添加基本信息，但不会触发上报
 */
#define KSCrashMonitorTypeRequired (KSCrashMonitorTypeSystem | KSCrashMonitorTypeApplicationState)

/** Effectively disables automatica reporting. The only way to generate a report
 * in this mode is by manually calling kscrash_reportUserException().
 *
 * 禁用自动上报，在这种模式下生成报告的唯一方法是手动调用kscrash_reportUserException()。
 */
#define KSCrashMonitorTypeManual (KSCrashMonitorTypeRequired | KSCrashMonitorTypeUserReported)

#define KSCrashMonitorTypeNone 0

const char* kscrashmonitortype_name(KSCrashMonitorType monitorType);


#ifdef __cplusplus
}
#endif

#endif // HDR_KSCrashMonitorType_h
