#include "CrashEngine.hpp"
#include <Geode/Geode.hpp>
#include "utils.hpp"

#ifdef GEODE_IS_WINDOWS
inline bool isWine() {
	HMODULE hModule = GetModuleHandleA("ntdll.dll");
	if (!hModule) return false;
	FARPROC func = GetProcAddress(hModule, "wine_get_version");
	return func != nullptr;
}

/* Cringedows stuff */
#include <winternl.h>
#include <psapi.h>
typedef NTSTATUS(NTAPI *pdef_NtRaiseHardError)(NTSTATUS ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask OPTIONAL, PULONG_PTR Parameters, ULONG ResponseOption, PULONG Response);
typedef NTSTATUS(NTAPI *pdef_RtlAdjustPrivilege)(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN Enabled);
/* ================ */
#endif

#ifdef GEODE_IS_WINDOWS
#define BRKPNT __debugbreak()
#define NO_INLINE __declspec(noinline)
#else
#define BRKPNT __builtin_trap()
#define NO_INLINE __attribute__((noinline))
#endif


#ifdef GEODE_IS_WINDOWS64
// declare extern functions, which are defined in Win64Utils.asm
extern "C" {
    void corruptTheStack();
    void jumpToPointer(uintptr_t pointer);
}
#endif

namespace CrashEngine {

#define TRIGGER_CASE(type, func) case CrashType::type: func(); break

    void NO_INLINE trigger(CrashType type) {
        switch (type) {
        TRIGGER_CASE(EXCEPTION, Payload::except);
        TRIGGER_CASE(B00B1E5, Payload::boobies);
        TRIGGER_CASE(STACK_OVERFLOW, Payload::stackOverflow);
        TRIGGER_CASE(DEADLOCK, Payload::deadlock);
        TRIGGER_CASE(ACCESS_VIOLATION, Payload::accessViolation);
        TRIGGER_CASE(COCOS2D_TEXTURE, Payload::cocos2dTexture);
        TRIGGER_CASE(BAD_ALLOC, Payload::badAlloc);
        TRIGGER_CASE(BREAKPOINT, Payload::breakpoint);
        TRIGGER_CASE(DOUBLE_FREE, Payload::doubleFree);
        TRIGGER_CASE(THREAD_CRASH, Payload::threadCrash);
        TRIGGER_CASE(GPU_CRASH, Payload::gpuCrash);
        TRIGGER_CASE(MEMORY_OVERFLOW, Payload::memoryOverflow);
        TRIGGER_CASE(DIVIDE_BY_ZERO, Payload::divideByZero);
#ifdef GEODE_IS_WINDOWS
        TRIGGER_CASE(STACK_CORRUPTION, Payload::stackCorruption);
        TRIGGER_CASE(EPIC_JUMP, Payload::epicJump);
        TRIGGER_CASE(BSOD, Payload::bsod);
        default: break;
#endif
        }
        geode::log::debug("Unknown crash triggered: {}", static_cast<int>(type));
    }

#undef TRIGGER_CASE

    namespace Payload {
        void NO_INLINE except() {
            throw std::runtime_error("Have a nice day!");
        }

        void NO_INLINE boobies() {
            (*(void (*)(void))0xB00B1E5)();
        }

        void NO_INLINE stackOverflow() {
            int megaStack[0x10000000];
            for (int i = 0; i < 0x10000000; i++) {
                megaStack[i] = i;
            }
            geode::log::debug("Huh? How did we get here? {}", megaStack[152125]);
        }

        void NO_INLINE deadlock() {
            std::mutex mutex;
            mutex.lock();
            mutex.lock();
        }

        void NO_INLINE accessViolation() {
            volatile int* ptr = nullptr;
            *ptr = 0;
        }

        void NO_INLINE cocos2dTexture() {
            auto something = CCMenuItemSpriteExtra::create(
                    cocos2d::CCSprite::createWithSpriteFrameName("I don't even know what I'm doing.png"),
                    nullptr, nullptr
            );
            something->setPosition(0, 0);
        }

        void NO_INLINE badAlloc() {
            // Allocate ~42535296000000000 YB of memory :trollface:
            for (size_t i = 0; i < std::numeric_limits<size_t>::max(); i++) {
                volatile auto ptr = new int[std::numeric_limits<size_t>::max() / 32];
            }
        }

        void NO_INLINE breakpoint() {
            BRKPNT;
        }

        void NO_INLINE doubleFree() {
            // Simplified version of https://stackoverflow.com/questions/60248976/c-double-free-or-corruptionout
            std::vector<int> vals = { 1, 2, 3, 4, 5, 6 };
            auto it = vals.begin();
            vals.insert(it, 100);
            it += 4;
            vals.insert(it, 200);
        }

        void NO_INLINE threadCrash() {
            auto t = std::thread([] {
                geode::log::debug("Thread started");
                BRKPNT;
                geode::log::debug("Thread ended");
            });
            // Set thread name
            GEODE_WINDOWS(SetThreadDescription(t.native_handle(), L"CrashEngine Thread");)
            t.detach();
        }

        void NO_INLINE gpuCrash() {
            // Got this from https://stackoverflow.com/questions/64828827/whenever-i-run-my-opengl-code-i-get-a-white-screen-and-a-crash
            // But cut out the unnecessary stuff
            unsigned int VAO4, VBO4, EBO4;
            unsigned int vertices[] = { 0, 1, 3, 3, 1, 2 };
            float vertex[] = {
                    -0.15f, -0.15f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
                    -0.15f, 0.15f, 0.0f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
                    0.15f, 0.15f, 0.0f,    0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
                    -0.15f, 0.15f, 0.0f,    1.0f, 0.0f, 0.0f,   0.0f, 1.0f
            };
            glGenVertexArrays(1, &VAO4);
            glBindVertexArray(VAO4);
            glGenBuffers(1, &VBO4);
            glBindBuffer(GL_ARRAY_BUFFER, VBO4);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
            glGenBuffers(GL_ELEMENT_ARRAY_BUFFER, &EBO4);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO4);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)nullptr);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
            glEnableVertexAttribArray(2);
        }

        void NO_INLINE memoryOverflow() {
            while (true) {
                malloc(0x1000);
            }
        }

        void NO_INLINE divideByZero() {
            // Compilers love to optimize this out
            volatile int a = 1;
            volatile int b = 0;
            volatile int c = a / b;
            geode::log::debug("1 / 0 = {}", c);
        }

#ifdef GEODE_IS_WINDOWS

        void NO_INLINE stackCorruption() {
#ifdef GEODE_IS_WINDOWS32
            __asm {
                // Pop some registers from stack
                pop eax
                pop ebx
                pop ecx
                pop edx
                pop esi
                pop edi
                pop ebp
            }
#else
            corruptTheStack();
#endif
        }

        void NO_INLINE bsod() {
            if (isWine()) {
                system("cmd /c start /unix echo c > /proc/sysrq-trigger");
            } else {
                BOOLEAN bEnabled;
                ULONG uResp;
                LPVOID lpFuncAddress = GetProcAddress(LoadLibraryA("ntdll.dll"), "RtlAdjustPrivilege");
                LPVOID lpFuncAddress2 = GetProcAddress(GetModuleHandle("ntdll.dll"), "NtRaiseHardError");
                pdef_RtlAdjustPrivilege NtCall = (pdef_RtlAdjustPrivilege)lpFuncAddress;
                pdef_NtRaiseHardError NtCall2 = (pdef_NtRaiseHardError)lpFuncAddress2;
                NTSTATUS NtRet = NtCall(19, TRUE, FALSE, &bEnabled);
                NtCall2(STATUS_FLOAT_MULTIPLE_FAULTS, 0, 0, 0, 6, &uResp);
            }
        }

        void NO_INLINE epicJump() {
            auto module = GetModuleHandle(nullptr);
            MODULEINFO moduleInfo;
            GetModuleInformation(GetCurrentProcess(), module, &moduleInfo, sizeof(moduleInfo));
            auto base = reinterpret_cast<uintptr_t>(moduleInfo.lpBaseOfDll);
            auto size = static_cast<uintptr_t>(moduleInfo.SizeOfImage);

            auto random = util::randPtr(base, base + size);
            GEODE_WINDOWS32(__asm { jmp random })
            GEODE_WINDOWS64(jumpToPointer(random);)
        }

#endif
    }

}

