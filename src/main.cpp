#include <JuceHeader.h>
#include "pedalGUI/PedalComponent.h"
#include "pedalSoundEffects/Overdrive.h"
#include "pedalSoundEffects/CustomReverb.h"



class MainComponent : public juce::AudioAppComponent
                      
{
public:
    MainComponent()
    {
        overdrivePedal = std::make_unique<PedalComponent>("Overdrive", std::make_unique<Overdrive>());
        addAndMakeVisible(*overdrivePedal);

        
        reverbPedal = std::make_unique<PedalComponent>("Reverb", std::make_unique<CustomReverb>());
        addAndMakeVisible(*reverbPedal);





        setAudioChannels(2, 2);
        setSize(400, 300);
    }

    ~MainComponent() override
    {
        shutdownAudio();
    }

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override {
        currentSampleRate = sampleRate;
        if (overdrivePedal) overdrivePedal->setSampleRate(sampleRate);
        if (reverbPedal) reverbPedal->setSampleRate(sampleRate);
    }


    void getNextAudioBlock(const juce::AudioSourceChannelInfo& info) override

{
    juce::Logger::outputDebugString("Processing audio block");

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

                sample = overdrivePedal->isEnabled() ? overdrivePedal->processSample(sample) : sample;
                sample = reverbPedal->isEnabled() ? reverbPedal->processSample(sample) : sample;

                outBuffer[i] = sample;
            }
        }
        else
        {
            // Clear any output channels with no corresponding input
            juce::FloatVectorOperations::clear(outBuffer, info.numSamples);
        }
    }
}


    void releaseResources() override {}

    void resized() override
{
        const int pedalWidth = 150;
        const int pedalHeight = 120;
        const int padding = 10;

        auto placePedal = [&](auto& pedal, int row, int col) {
            if (pedal)
                pedal->setBounds(padding + col * (pedalWidth + padding),
                                padding + row * (pedalHeight + padding),
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
    
    std::unique_ptr<PedalComponent> overdrivePedal;
    std::unique_ptr<PedalComponent> distortionPedal;
    std::unique_ptr<PedalComponent> reverbPedal;
    std::unique_ptr<PedalComponent> bluesDriverPedal;
    std::unique_ptr<PedalComponent> delayPedal;
    std::unique_ptr<PedalComponent> tremoloPedal;
    std::unique_ptr<PedalComponent> chorusPedal;
    std::unique_ptr<PedalComponent> phaserPedal;

};

class MainApp  : public juce::JUCEApplication
{
public:
    const juce::String getApplicationName() override       { return "GuitarPedalApp"; }
    const juce::String getApplicationVersion() override    { return "0.1.0"; }
    bool moreThanOneInstanceAllowed() override             { return true; }

    void initialise (const juce::String&) override
    {
        mainWindow.reset (new juce::DocumentWindow (
            getApplicationName(),
            juce::Colours::blue,
            juce::DocumentWindow::allButtons
        ));

        mainWindow->setUsingNativeTitleBar (true);
        mainWindow->setContentOwned (new MainComponent(), true);
        mainWindow->centreWithSize (400, 300);
        mainWindow->setVisible (true);
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

START_JUCE_APPLICATION (MainApp)
