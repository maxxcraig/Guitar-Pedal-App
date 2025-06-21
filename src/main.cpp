#include <JuceHeader.h>
#include "pedalGUI/PedalComponent.h"
#include "pedalGUI/OverdriveComponent.h"

#include "pedalSoundEffects/Overdrive.h"
#include "pedalSoundEffects/CustomReverb.h"
#include "pedalSoundEffects/Chorus.h"
#include "pedalSoundEffects/Phaser.h"
#include "pedalSoundEffects/BluesDriver.h"
#include "pedalSoundEffects/Delay.h"
#include "pedalSoundEffects/Tremolo.h"
#include "pedalSoundEffects/Distortion.h"

class MainComponent : public juce::AudioAppComponent
{
public:
    MainComponent()
    {
        juce::RuntimePermissions::request(juce::RuntimePermissions::recordAudio,
            [](bool granted) {
                if (!granted)
                    juce::Logger::outputDebugString("Mic permission not granted!");
        });

        overdrivePedal = std::make_unique<OverdriveComponent>();
        reverbPedal         = std::make_unique<OverdriveComponent>();
        chorusPedal         = std::make_unique<OverdriveComponent>();
        bluesDriverPedal    = std::make_unique<OverdriveComponent>();
        distortionPedal     = std::make_unique<OverdriveComponent>();
        delayPedal          = std::make_unique<OverdriveComponent>();
        tremoloPedal        = std::make_unique<OverdriveComponent>();
        phaserPedal         = std::make_unique<OverdriveComponent>();


        addAndMakeVisible(*overdrivePedal);
        addAndMakeVisible(*reverbPedal);
        addAndMakeVisible(*chorusPedal);
        addAndMakeVisible(*bluesDriverPedal);
        addAndMakeVisible(*distortionPedal);
        addAndMakeVisible(*delayPedal);
        addAndMakeVisible(*tremoloPedal);
        addAndMakeVisible(*phaserPedal);

        setAudioChannels(2, 2);
        setSize(1000, 600);
    }

    ~MainComponent() override
    {
        shutdownAudio();
    }

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override
    {
        currentSampleRate = sampleRate;

        auto setSR = [=](auto& pedal) {
            if (pedal) pedal->setSampleRate(sampleRate);
        };

        setSR(overdrivePedal);
        setSR(reverbPedal);
        setSR(chorusPedal);
        setSR(bluesDriverPedal);
        setSR(distortionPedal);
        setSR(delayPedal);
        setSR(tremoloPedal);
        setSR(phaserPedal);
    }

    void getNextAudioBlock(const juce::AudioSourceChannelInfo& info) override
    {
        auto totalInputChannels  = deviceManager.getCurrentAudioDevice()->getActiveInputChannels().countNumberOfSetBits();
        auto totalOutputChannels = deviceManager.getCurrentAudioDevice()->getActiveOutputChannels().countNumberOfSetBits();

        for (int channel = 0; channel < totalOutputChannels; ++channel)
        {
            auto* outBuffer = info.buffer->getWritePointer(channel, info.startSample);

            if (channel < totalInputChannels)
            {
                const float* inBuffer = info.buffer->getReadPointer(channel, info.startSample);

                for (int i = 0; i < info.numSamples; ++i)
                {
                    float sample = inBuffer[i];

                    auto process = [&](auto& pedal) {
                        if (pedal && pedal->isEnabled())
                            sample = pedal->processSample(sample);
                    };

                    process(overdrivePedal);
                    process(reverbPedal);
                    process(distortionPedal);
                    process(bluesDriverPedal);
                    process(delayPedal);
                    process(tremoloPedal);
                    process(chorusPedal);
                    process(phaserPedal);

                    outBuffer[i] = sample;
                }
            }
            else
            {
                juce::FloatVectorOperations::clear(outBuffer, info.numSamples);
            }
        }
    }

    void releaseResources() override {}
    void paint(Graphics& g) override
        {
            g.fillAll(Colours::darkgrey);  // optional background

            g.setColour(Colours::white);
            g.setFont(Font(40.0f, Font::bold));
            g.drawText("Guitar Pedal Board", getLocalBounds().removeFromTop(80),
                    Justification::centred, true);
        }


    void resized() override
    {
        const int pedalWidth = 200;
        const int pedalHeight = 160;
        const int padding = 20;

        const int columns = 4;
        const int titleHeight = 100;  // space for title at the top


        auto totalWidth = columns * pedalWidth + (columns + 1) * padding;
        int startX = (getWidth() - totalWidth) / 2 + padding;  // removes left gap

        auto placePedal = [&](auto& pedal, int row, int col) {
            if (pedal)
                pedal->setBounds(startX + col * (pedalWidth + padding),
                                titleHeight + padding + row * (pedalHeight + padding),  // ⬅️ offset down
                                pedalWidth, pedalHeight);
        };


        placePedal(overdrivePedal, 0, 0);
        placePedal(reverbPedal, 0, 1);
        placePedal(distortionPedal, 0, 2);
        placePedal(bluesDriverPedal, 0, 3);
        placePedal(delayPedal, 1, 0);
        placePedal(tremoloPedal, 1, 1);
        placePedal(chorusPedal, 1, 2);
        placePedal(phaserPedal, 1, 3);
    }


private:
    double currentSampleRate = 44100.0;

    std::unique_ptr<OverdriveComponent> overdrivePedal;
    std::unique_ptr<OverdriveComponent> distortionPedal;
    std::unique_ptr<OverdriveComponent> reverbPedal;
    std::unique_ptr<OverdriveComponent> chorusPedal;
    std::unique_ptr<OverdriveComponent> bluesDriverPedal;
    std::unique_ptr<OverdriveComponent> delayPedal;
    std::unique_ptr<OverdriveComponent> tremoloPedal;
    std::unique_ptr<OverdriveComponent> phaserPedal;

};

class MainApp : public juce::JUCEApplication
{
public:
    const juce::String getApplicationName() override       { return "GuitarPedalApp"; }
    const juce::String getApplicationVersion() override    { return "0.1.0"; }
    bool moreThanOneInstanceAllowed() override             { return true; }

    void initialise(const juce::String&) override
    {
        mainWindow.reset(new juce::DocumentWindow(
            getApplicationName(),
            juce::Colours::blue,
            juce::DocumentWindow::allButtons
        ));

        mainWindow->setUsingNativeTitleBar(true);
        mainWindow->setContentOwned(new MainComponent(), true);
        mainWindow->setResizable(true, false);
        mainWindow->centreWithSize(1000, 600);
        mainWindow->setVisible(true);
    }

    void shutdown() override
    {
        mainWindow = nullptr;
    }

    void systemRequestedQuit() override
    {
        quit();
    }

private:
    std::unique_ptr<juce::DocumentWindow> mainWindow;
};

START_JUCE_APPLICATION(MainApp)
