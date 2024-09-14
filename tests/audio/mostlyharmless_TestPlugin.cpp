//
// Created by Syl Morrison on 11/09/2024.
//
#include "mostlyharmless_TestHelpers.h"
#include <mostly_harmless/mostlyharmless_Plugin.h>
#include <mostly_harmless/gui/mostlyharmless_WebviewEditor.h>
#include <marvin/library/marvin_Literals.h>
#include <catch2/catch_test_macros.hpp>
namespace mostly_harmless::testing {
    template <marvin::FloatType SampleType>
    class GenericPlugin : public mostly_harmless::Plugin<SampleType> {
    public:
        explicit GenericPlugin(const clap_host* host) : mostly_harmless::Plugin<SampleType>(host, {}) {
        }
        void initialise(double sampleRate, std::uint32_t minFrame, std::uint32_t maxFrame) noexcept override {
        }

        void process(marvin::containers::BufferView<SampleType> buffer, mostly_harmless::events::InputEventContext context) noexcept override {
        }

        void flushParams(mostly_harmless::events::InputEventContext context) noexcept override {
        }

        void reset() noexcept override {
        }

        void loadState(std::string_view data) noexcept override {
        }

        void saveState(std::ostringstream& outStream) noexcept override {
        }

        std::unique_ptr<mostly_harmless::gui::IEditor> createEditor() noexcept override {
            return nullptr;
        }

    private:
    };
    // ret name arg


    std::uint32_t size(const struct clap_input_events* list) {
        return 0;
    }

    template <marvin::FloatType SampleType, size_t NumChannels, size_t BufferSize> void testPlugin() {
        using namespace marvin::literals;
        auto host = std::make_unique<clap_host>();
        GenericPlugin<SampleType> plugin{ host.get() };
        plugin.activate(44100.0, BufferSize, BufferSize);
        clap_audio_buffer in;
        SampleType** buffer = generateImpulse<SampleType, NumChannels, BufferSize>();
        if constexpr (std::same_as<float, SampleType>) {
            in = {
                .data32 = buffer,
                .data64 = nullptr,
                .channel_count = NumChannels,
                .latency = 0,
                .constant_mask = 0x0
            };
        } else {
            in = {
                .data32 = nullptr,
                .data64 = buffer,
                .channel_count = NumChannels,
                .latency = 0,
                .constant_mask = 0x0
            };
        }

        auto* ctx = malloc(sizeof(SampleType) * 100);
        clap_input_events inputEvents{
            .ctx = ctx,
            .size = size
        };

        const clap_process process{
            .steady_time = -1,
            .frames_count = BufferSize,
            .transport = nullptr,
            .audio_inputs = &in,
            .audio_outputs = &in,
            .audio_inputs_count = NumChannels,
            .audio_outputs_count = NumChannels,
            .in_events = &inputEvents,
            .out_events = nullptr
        };
        const auto res = static_cast<mostly_harmless::Plugin<SampleType>*>(&plugin)->process(&process);
        marvin::containers::BufferView<SampleType> view{ buffer, NumChannels, BufferSize};
        const auto bufferOk = checkBuffer(view);
        REQUIRE(bufferOk);


        free(ctx);
        for (auto i = 0_sz; i < NumChannels; ++i) {
            delete[] buffer[i];
        }
        delete[] buffer;
    }

    TEST_CASE("Test Plugin") {
        testPlugin<float, 1, 64>();
        testPlugin<double, 1, 64>();
        testPlugin<float, 2, 128>();
        testPlugin<double, 2, 128>();
    }
}