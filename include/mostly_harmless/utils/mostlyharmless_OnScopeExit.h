//
// Created by Syl on 08/02/2025.
//

#ifndef MOSTLYHARMLESS_MOSTLYHARMLESS_ONSCOPEEXIT_H
#define MOSTLYHARMLESS_MOSTLYHARMLESS_ONSCOPEEXIT_H
#include <functional>
namespace mostly_harmless::utils {
    /**
     * \brief Functor to run some code when this variable goes out of scope.
     *
     * Useful for performing some action after a function returns, for example:
     * ```cpp
     * void sayHiOnExit() {
     *     OnScopeExit se { []() -> void {
     *          std::cout << "Hello world!\n";
     *     }};
     *     // function body etc etc etc
     * }
     * ```
     * Will print "Hello world" after the function body has run.
     *
     * Lifetime concerns apply here, be very very careful.
     */
    class OnScopeExit final {
    public:
        /**
         * Constructs an OnScopeExit.
         * \param action An rvalue ref to an action to perform when this object gets destroyed.
         */
        explicit OnScopeExit(std::function<void(void)>&& action);

        /**
         * Destructor - the lambda you passed to the constructor will be executed here.
         */
        ~OnScopeExit() noexcept;
    private:
        std::function<void(void)> m_action{ nullptr };
    };
}
#endif // MOSTLYHARMLESS_MOSTLYHARMLESS_ONSCOPEEXIT_H
