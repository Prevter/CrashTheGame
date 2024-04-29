#pragma once

namespace CrashEngine {

    enum class CrashType {
        EXCEPTION,
        B00B1E5,
        STACK_OVERFLOW,
        GEODE_WINDOWS(EPIC_JUMP,)
        DEADLOCK,
        ACCESS_VIOLATION,
        COCOS2D_TEXTURE,
        GEODE_WINDOWS(BSOD,)
        BAD_ALLOC,
        BREAKPOINT,
        DOUBLE_FREE,
        GEODE_WINDOWS(STACK_CORRUPTION,)
        THREAD_CRASH,
        GPU_CRASH,
        MEMORY_OVERFLOW,
        DIVIDE_BY_ZERO,
    };

    const char* const CrashNames[] = {
        "std::exception",
        "call 0xB00B1E5()",
        "Stack Overflow",
        GEODE_WINDOWS("jmp rand()",)
        "Mutex Deadlock",
        "Access Violation",
        "Missing Texture",
        GEODE_WINDOWS("BSOD",)
        "Bad Allocation",
        "Breakpoint",
        "Double free()",
        GEODE_WINDOWS("Stack Corruption",)
        "Thread Crash",
        "OpenGL Crash",
        "Memory Overflow",
        "Divide by Zero",
    };

    const size_t CrashCount = sizeof(CrashNames) / sizeof(CrashNames[0]);

    /// @brief Calls appropriate crash function based on type.
    void trigger(CrashType type);

    namespace Payload {
        /// @brief Throws std::runtime_error.
        void except();

        /// @brief Calls 0xB00B1E5.
        void boobies();

        /// @brief Causes stack overflow exception.
        void stackOverflow();

        /// @brief Causes deadlock using mutexes.
        void deadlock();

        /// @brief Accesses invalid memory.
        void accessViolation();

        /// @brief Loads missing texture and adds it to scene.
        void cocos2dTexture();

        /// @brief Allocates memory until bad_alloc is thrown.
        void badAlloc();

        /// @brief Triggers breakpoint (int 3).
        void breakpoint();

        /// @brief Calls free() on already freed memory.
        void doubleFree();

        /// @brief Creates new thread and crashes it.
        void threadCrash();

        /// @brief Makes some OpenGL calls to crash the GPU.
        void gpuCrash();

        /// @brief Allocates memory until memory overflow.
        void memoryOverflow();

        /// @brief Divides by zero.
        void divideByZero();

#ifdef GEODE_IS_WINDOWS

        /// @brief Causes stack guard corruption.
        void stackCorruption();

        /// @brief Causes Blue Screen of Death.
        void bsod();

        /// @brief Jumps to random address in the game module.
        void epicJump();

#endif
    }
}