#include <JuceHeader.h>
#include "Overdrive.h"
#include "CustomReverb.h"

class MainComponent : public juce::AudioAppComponent,
                      public juce::Button::Listener,
                      public juce::Slider::Listener
{
public:
    MainComponent()
    {
        addAndMakeVisible(overdriveToggle);
        overdriveToggle.setButtonText("Overdrive");
        overdriveToggle.addListener(this);


        addAndMakeVisible(reverbToggle);
        reverbToggle.setButtonText("Reverb");
        reverbToggle.addListener(this);

        addAndMakeVisible(gainSlider);
        gainSlider.setRange(0, 10.0, 0.1);
        gainSlider.setValue(5.0);
        gainSlider.addListener(this);
        gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);

        addAndMakeVisible(reverbSlider);
        reverbSlider.setRange(0.0, 10, 0.1);
        reverbSlider.setValue(5);
        reverbSlider.addListener(this);
        reverbSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);

        // Gain Label
        gainLabel.setText("Gain", juce::dontSendNotification);
        gainLabel.attachToComponent(&gainSlider, false); //false = label goes above

        // Reverb Label
        reverbLabel.setText("Reverb", juce::dontSendNotification);
        reverbLabel.attachToComponent(&reverbSlider, false);


        setAudioChannels(2, 2);
        setSize(400, 300);
    }

    ~MainComponent() override
    {
        shutdownAudio();
    }

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override
    {
        currentSampleRate = sampleRate;
        reverb.setSampleRate(sampleRate);
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

                if (overdriveEnabled)
                    sample = overdrive.processSample(sample);

                if (reverbEnabled)
                    sample = reverb.processSample(sample);

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
        overdriveToggle.setBounds(20, 20, getWidth() - 40, 30);
        reverbToggle.setBounds(110, 20, getWidth() - 40, 30);
        gainSlider.setBounds(20, 70, getWidth() - 40, 60);
        reverbSlider.setBounds(20, 150, getWidth() - 40, 60);
    }

    void buttonClicked(juce::Button* button) override
    {
        if (button == &overdriveToggle)
            overdriveEnabled = overdriveToggle.getToggleState();
        else if (button == &reverbToggle)
            reverbEnabled = reverbToggle.getToggleState();
    }

    void sliderValueChanged(juce::Slider* slider) override
    {
        if (slider == &gainSlider)
            overdrive.setGain(gainSlider.getValue());
        else if (slider == &reverbSlider)
            reverb.setMix(reverbSlider.getValue());
    }

private:
    double currentSampleRate = 44100.0;
    bool overdriveEnabled = false;
    bool reverbEnabled = false;

    Overdrive overdrive;
    CustomReverb reverb;


    juce::ToggleButton overdriveToggle;
    juce::ToggleButton reverbToggle;
    juce::Slider gainSlider;
    juce::Slider reverbSlider;
    juce::Label gainLabel;
    juce::Label reverbLabel;
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
