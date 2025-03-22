#include "$LOWER_PROJECT_NAME$_Editor.h"

Editor::Editor(SharedState* sharedState) : m_sharedState(sharedState) {
}

void Editor::initialise() {
}

void Editor::destroy() {
}

bool Editor::allowResize() const noexcept {
    return true;
}

void Editor::setSize(std::uint32_t /*width*/, std::uint32_t /*height*/) {
}

void Editor::getSize(std::uint32_t* /*width*/, std::uint32_t* /*height*/) {
}

void Editor::onResizeRequested(std::uint32_t* /*width*/, std::uint32_t* /*height*/) {

}

void Editor::setParent(void* /*parentHandle*/) {
}

void Editor::show() {
}

void Editor::hide() {
}

void Editor::onParamEvent(mostly_harmless::events::ProcToGuiParamEvent /*event*/) {
}