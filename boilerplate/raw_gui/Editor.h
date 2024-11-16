#ifndef $PREPRO_NAME$_EDITOR_H
#define $PREPRO_NAME$_EDITOR_H

#include "$LOWER_PROJECT_NAME$_SharedState.h"
#include <mostly_harmless/core/mostlyharmless_IEditor.h>
class Editor final : public mostly_harmless::core::IEditor {
public:
    Editor(SharedState* sharedState);
    void initialise() override;
    void destroy() override;
    void setSize(std::uint32_t width, std::uint32_t height) override;
    void getSize(std::uint32_t* width, std::uint32_t* height) override;
    void setParent(void* parentHandle) override;
    void show() override;
    void hide() override;
    void onParamEvent(mostly_harmless::events::ProcToGuiParamEvent event) override;

private:
    SharedState* m_sharedState{ nullptr };
};
#endif // $PREPRO_NAME$_EDITOR_H
