#include "$LOWER_PROJECT_NAME$_Entry.h"
#include "$LOWER_PROJECT_NAME$_SharedState.h"
#include "$LOWER_PROJECT_NAME$_Engine.h"
#include "$LOWER_PROJECT_NAME$_Editor.h"

[[nodiscard]] static SharedState* asUserState(mostly_harmless::core::ISharedState* state) noexcept {
    return static_cast<SharedState*>(state);
}

std::unique_ptr<mostly_harmless::core::ISharedState> Entry::createState(mostly_harmless::core::SharedStateContext&& context) {
    return std::make_unique<SharedState>(std::move(context));
}

std::unique_ptr<mostly_harmless::core::IEngine> Entry::createEngine(mostly_harmless::core::ISharedState* sharedState) {
    return std::make_unique<Engine>(asUserState(sharedState));
}

bool Entry::hasGui() const noexcept {
    return true;
}

std::unique_ptr<mostly_harmless::core::IEditor> Entry::createEditor(mostly_harmless::core::ISharedState* sharedState) {
    return std::make_unique<Editor>(asUserState(sharedState));
}

MH_REGISTER_PLUGIN_ENTRY(Entry)
