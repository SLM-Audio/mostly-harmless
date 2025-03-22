//
// Created by Syl Morrison on 21/03/2025.
//

#ifndef MOSTLYHARMLESS_NODENORMALS_H
#define MOSTLYHARMLESS_NODENORMALS_H
#include <memory>
#include <cstdint>
namespace mostly_harmless::utils {
#ifndef MH_SSE_AVAILABLE
#if defined(__SSE__) || defined(_M_X64) || defined(_M_AMD64)
#define MH_SSE_AVAILABLE true
#else
#define MH_SSE_AVAILABLE false
#endif
#endif
    /**
        \brief RAII mechanism to disable denormals while an instance of this class is alive.
        Supports SSE or NEON instruction sets, will have no effect if neither of those are present.
        Goes without saying, but doesn't allocate (I deliberately didn't use pImpl for this reason)
    */
    class NoDenormals final {
    public:
        /**
         * Disables denormals on construction, by setting FTZ (and DAZ in SSE)
         */
        NoDenormals();

        /**
         * Resets the FTZ / DAZ bits to their original states
         */
        ~NoDenormals() noexcept;

    private:
        using FlagType = std::conditional_t<MH_SSE_AVAILABLE, unsigned int, std::uintptr_t>;
        FlagType m_controlFlag;
    };
} // namespace mostly_harmless::utils
#endif // MOSTLYHARMLESS_NODENORMALS_H
