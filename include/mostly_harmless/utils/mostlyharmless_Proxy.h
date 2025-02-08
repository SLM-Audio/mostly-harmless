//
// Created by Syl Morrison on 08/02/2025.
//

#ifndef MOSTLYHARMLESS_PROXY_H
#define MOSTLYHARMLESS_PROXY_H
#include <memory>
namespace mostly_harmless::utils {

    /**
     * \brief Helper class for managing lifetimes of captured references in a lambda.
     *
     * Take a scenario where you have a timer thread, which relies on some member variable being valid - for example,
     * ```cpp
     * class Something final {
     * public:
     *     Something() {
     *         auto timerCallback = [this]() -> void {
     *             ++m_callCount;
     *         };
     *         m_timer.action = std::move(timerCallback);
     *         m_timer.run(100);
     *     }
     *
     * private:
     *     std::atomic<int> m_callCount{ 0 };
     *     mostly_harmless::utils::Timer m_timer;
     * };
     * ```
     *
     * Now, lets say between calls to our timer action, our instance of `Something` gets destroyed - but the timer callback is still running.
     * In this case, we have a segfault, as `this == nullptr`. This comes up fairly often with timers, and is a massive PITA.
     *
     * The use case for Proxy then, is to be able to reliably check if the captured variables are still valid.
     * This works by constructing a shared_ptr to some "Proxy" class with a pointer to the data you want to check, and ensuring its lifecycle matches that of the wrapped pointer.
     * When your wrapped object is destroyed, make sure to call `null()` on the proxy, to invalidate its pointer member.
     * You can then capture the shared_ptr via copy to your timer callback, which will ensure that while the generated closure for the lambda is alive, so is our proxy.
     * Finally, in your callback, you can simply call `theProxyInstance.isValid()` to check whether the wrapped pointer is still valid, and bail if it isn't.
     * Putting that together for our earlier example then:
     *
     * ```cpp
     * class Something final {
     * public:
     *     Something() {
     *         m_proxy = mostly_harmless::utils::Proxy<Something>::create(this);
     *         auto proxyCopy = m_proxy;
     *         auto timerCallback = [this, proxyCopy]() -> void {
     *             if(!proxyCopy->isValid()) {
     *                 return;
     *             }
     *             ++m_callCount;
     *         };
     *         m_timer.action = std::move(timerCallback);
     *         m_timer.run(100);
     *     }
     *
     *     ~Something() {
     *         m_proxy->null();
     *     }
     *
     * private:
     *     std::shared_ptr<mostly_harmless::utils::Proxy<Something>> m_proxy{ nullptr };
     *     std::atomic<int> m_callCount{ 0 };
     *     mostly_harmless::utils::Timer m_timer;
     * };
     * ```
     *
     *
     */
    template <typename T>
    class Proxy final {
    private:
        struct Private {};

    public:
        /**
         * @internal
         */
        explicit Proxy([[maybe_unused]] Private, T* toWrap) : m_wrapped(toWrap) {
        }

        /**
         * Create a std::shared_ptr<Proxy<T>>, wrapping whatever you pass to toWrap.
         * \param toWrap a raw pointer to the data you want to wrap.
         */
        [[nodiscard]] static std::shared_ptr<Proxy<T>> create(T* toWrap) noexcept {
            return std::make_shared<Proxy<T>>(Private{}, toWrap);
        }

        /**
         * Retrieve the pointer this object is wrapping.
         * \return The wrapped pointer. Make sure to check this for `null`.
         */
        [[nodiscard]] T* getWrapped() noexcept {
            return m_wrapped;
        }

        /**
         * Reassigns the wrapped data.
         * \param wrapped A raw pointer to the data you want to wrap.
         */
        void setWrapped(T* wrapped) noexcept {
            m_wrapped = wrapped;
        }

        /**
         * nulls the wrapped pointer - make SURE you call this when whatever data your proxy is wrapping gets destroyed!
         */
        void null() noexcept {
            m_wrapped = nullptr;
        }

        /**
         * Checks whether the wrapped data is still allocated.
         * \return true if the data is valid, false otherwise.
         */
        [[nodiscard]] bool isValid() const noexcept {
            return m_wrapped;
        }

    private:
        T* m_wrapped{ nullptr };
    };

} // namespace mostly_harmless::utils
#endif // MOSTLYHARMLESS_PROXY_H
