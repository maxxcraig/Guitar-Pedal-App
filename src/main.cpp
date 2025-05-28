#include <JuceHeader.h>

class MainComponent  : public juce::AudioAppComponent
{
public:
    MainComponent()
    {
        setAudioChannels (0, 2);
        setSize (400, 200);
    }

    ~MainComponent() override
    {
        shutdownAudio();
    }

    void prepareToPlay (int /*samplesPerBlockExpected*/, double sampleRate) override
    {
        currentSampleRate = sampleRate;
    }

    void getNextAudioBlock (const juce::AudioSourceChannelInfo& info) override
    {
        auto* outL = info.buffer->getWritePointer (0, info.startSample);
        auto* outR = info.buffer->getWritePointer (1, info.startSample);

        static double phase = 0.0;
        double phaseInc = 2.0 * juce::MathConstants<double>::pi * 440.0 / currentSampleRate;

        for (int i = 0; i < info.numSamples; ++i)
        {
            float s = (float) std::sin (phase);
            phase += phaseInc;
            if (phase > 2.0 * juce::MathConstants<double>::pi)
                phase -= 2.0 * juce::MathConstants<double>::pi;

            outL[i] = s * 0.8f;
            outR[i] = s * 0.8f;
        }
    }

    void releaseResources() override {}

    void paint (juce::Graphics& g) override
    {
        g.fillAll (juce::Colours::darkgrey);
        g.setColour (juce::Colours::white);
        g.setFont (20.0f);
        g.drawFittedText ("GuitarPedalApp", getLocalBounds(), juce::Justification::centred, 1);
    }

private:
    double currentSampleRate { 44100.0 };
};

//==============================================================================

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
            juce::Colours::lightgrey,
            juce::DocumentWindow::allButtons
        ));

        mainWindow->setUsingNativeTitleBar (true);
        mainWindow->setContentOwned (new MainComponent(), true);
        mainWindow->centreWithSize (400, 200);
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

//==============================================================================

START_JUCE_APPLICATION (MainApp)
