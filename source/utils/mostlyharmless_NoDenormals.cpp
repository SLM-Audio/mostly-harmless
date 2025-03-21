//
// Created by Syl Morrison on 21/03/2025.
//
#include <mostly_harmless/utils/mostlyharmless_NoDenormals.h>
#include <cstdint>
namespace mostly_harmless::utils {
#if defined(__SSE__) || defined(_M_X64)
#include <xmmintrin.h>
#endif
    NoDenormals::NoDenormals() {
#if defined(__SSE__) || defined(_M_X64)
            m_controlFlag = _mm_getcsr();
            constexpr static auto FTZ_AND_DAZ = 0x8040;
            _mm_setcsr(std::get<std::uint8_t>(m_controlFlag) | FTZ_AND_DAZ);
#elif defined(__ARM_NEON) || defined(__ARM_NEON__)
            constexpr static auto FTZ = 0x01000000U;
            uintptr_t asmStatus;
            asm volatile("mrs %0, fpcr" : "=r"(asmStatus));
            m_controlFlag = asmStatus = asmStatus | FTZ;
            asm volatile("msr fpcr, %0" : : "ri"(asmStatus));
#endif
    }

    NoDenormals::~NoDenormals() noexcept  {
#if defined(__SSE__) || defined(_M_X64)
            _mm_setcsr(std::get<std::uint8_t>(m_controlFlag));
#elif defined(__ARM_NEON) || defined(__ARM_NEON__)
            uintptr_t asmStatus = std::get<std::uintptr_t>(m_controlFlag);
            asm volatile("msr fpcr, %0" : : "ri"(asmStatus));
#endif
    }
}