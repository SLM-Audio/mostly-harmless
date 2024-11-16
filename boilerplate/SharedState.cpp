#include "$LOWER_PROJECT_NAME$_SharedState.h"

std::vector<mostly_harmless::Parameter<float>> createParameters() {
    std::vector<mostly_harmless::Parameter<float>> params;
    /**
     * TODO: Create your params here...
     */
    return params;
}

SharedState::SharedState(mostly_harmless::core::SharedStateContext&& context) : mostly_harmless::core::ISharedState(std::move(context), createParameters()) {
}

void SharedState::loadState(std::string_view /*loadedData*/) {
}

void SharedState::saveState(std::ostringstream& /*dest*/) {
}