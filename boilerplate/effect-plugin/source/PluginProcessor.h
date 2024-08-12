//
// Created by Syl on 11/08/2024.
//

#ifndef PLUGINPROCESSOR_H
#define PLUGINPROCESSOR_H
#include <mostly_harmless/mostlyharmless_Plugin.h>
class PluginProcessor : public mostly_harmless::Plugin<float> {
public:
    PluginProcessor(const clap_host* host);
    ~PluginProcessor() noexcept override = default;
    void initialise(double sampleRate, std::uint32_t minFrames, std::uint32_t maxFrames) noexcept override;
    void process(marvin::containers::BufferView<float> bufferView, mostly_harmless::EventContext eventContext) noexcept override;
    void flushParams(mostly_harmless::EventContext eventContext) noexcept override;
    void reset() noexcept override;
    std::unique_ptr<mostly_harmless::gui::IEditor> createEditor() noexcept override;
private:

};
#endif //PLUGINPROCESSOR_H
