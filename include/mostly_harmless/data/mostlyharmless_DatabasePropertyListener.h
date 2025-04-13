//
// Created by Syl Morrison on 13/04/2025.
//

#ifndef MOSTLYHARMLESS_DATABASEPROPERTYLISTENER_H
#define MOSTLYHARMLESS_DATABASEPROPERTYLISTENER_H
#include <mostly_harmless/data/mostlyharmless_DatabaseState.h>
#include <mostly_harmless/mostlyharmless_Concepts.h>
#include <mostly_harmless/utils/mostlyharmless_Timer.h>
#include <mostly_harmless/utils/mostlyharmless_Proxy.h>
namespace mostly_harmless::data {
    /**
     * \brief A convenience class for responding to changes in a databaseState, potentially fired from other processes.
     * Watches a given property's field corresponding to type T for changes, and fires a callback on change.
     * Note that callback will run on a background timer thread, so if you need thread safety, be sure to utilise `ISharedState::callOnMessageThread` from your callback lambda.
     * @tparam T Any type satisfying DatabaseStorageType
     */
    template <DatabaseStorageType T>
    class DatabasePropertyListener final {
    private:
        struct Private {};

    public:
        /**
         * @private
         */
        DatabasePropertyListener(Private, DatabaseState&& databaseState, std::string_view name, int pollFrequencyMs, std::function<void(const T&)>&& callback) : m_databaseState(std::move(databaseState)),
                                                                                                                                                                 m_propertyNameToWatch(name),
                                                                                                                                                                 m_callback(callback) {
            m_proxyThis = utils::Proxy<DatabasePropertyListener<T>>::create(this);
            auto proxyCopy = m_proxyThis;
            m_timer.action = [this, proxyCopy]() -> void {
                if (!proxyCopy->isValid()) {
                    return;
                }
                timerCallback();
            };
            m_timer.run(pollFrequencyMs);
        }

        /**
         * @private
         */
        ~DatabasePropertyListener() noexcept {
            m_proxyThis->null();
        }

        /**
         * Attempts to create a DatabasePropertyListener for the given property name.
         * As sqlite requires database access across different processes to each have their own unique connection, we first call `DatabaseState::duplicate` to create a new connection specifically for this thread.
         * @param databaseState A const reference to the database connection to duplicate for this PropertyListener.
         * @param propertyName The name of the property to watch.
         * @param pollFrequencyMs The interval in milliseconds to poll the database for changes at.
         * @param callback A lambda to be invoked from a background timer thread on property value change.
         * @return A DatabasePropertyListener for the given property name if successful, nullopt otherwise.
         */
        static auto tryCreate(const DatabaseState& databaseState, std::string_view propertyName, int pollFrequencyMs, std::function<void(const T&)>&& callback) -> std::optional<DatabasePropertyListener> {
            auto databaseCopyOpt = databaseState.duplicate();
            if (!databaseCopyOpt) {
                return {};
            }
            return std::make_optional<DatabasePropertyListener<T>>(Private{}, std::move(*databaseCopyOpt), propertyName, pollFrequencyMs, std::move(callback));
        }


    private:
        auto timerCallback() -> void {
            if (!m_callback) {
                return;
            }
            const auto getRes = m_databaseState.get<T>(m_propertyNameToWatch);
            if (!getRes) {
                assert(false);
                return;
            }
            auto hasChanged{ false };
            if (!m_previousValue.has_value()) {
                hasChanged = true;
            } else {
                hasChanged = *m_previousValue != *getRes;
            }
            m_previousValue = *getRes;
            if (!hasChanged) {
                return;
            }
            m_callback(*getRes);
        }

        DatabaseState m_databaseState;
        std::string m_propertyNameToWatch;
        std::function<void(const T&)> m_callback;
        std::shared_ptr<utils::Proxy<DatabasePropertyListener<T>>> m_proxyThis;
        utils::Timer m_timer;
        std::optional<T> m_previousValue{};
    };
} // namespace mostly_harmless::data
#endif // MOSTLYHARMLESS_DATABASEPROPERTYLISTENER_H
