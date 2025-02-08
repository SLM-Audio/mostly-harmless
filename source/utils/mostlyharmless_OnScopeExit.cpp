//
// Created by Syl on 08/02/2025.
//
#include <mostly_harmless/utils/mostlyharmless_OnScopeExit.h>
namespace mostly_harmless::utils {
    OnScopeExit::OnScopeExit(std::function<void()>&& action) : m_action(std::move(action)) {

    }

    OnScopeExit::~OnScopeExit() noexcept {
        m_action();
    }
}