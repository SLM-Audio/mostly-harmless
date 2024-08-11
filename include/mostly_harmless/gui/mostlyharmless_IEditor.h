#ifndef MOSTLYHARMLESS_IEDITOR_H
#define MOSTLYHARMLESS_IEDITOR_H
#include <cstdint>
namespace mostly_harmless::gui {
    struct IEditor {
        virtual ~IEditor() noexcept = default;
        virtual void initialise() = 0;
        virtual void destroy() = 0;
        virtual void setSize(std::uint32_t width, std::uint32_t height) = 0;
        virtual void getSize(std::uint32_t* width, std::uint32_t* height) = 0;
        virtual void setParent(void* parentHandle) = 0;
        virtual void show() = 0;
        virtual void hide() = 0;
    };
} // namespace mostly_harmless::gui

#endif