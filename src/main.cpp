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

        addAndMakeVisible(gainSlider);
        gainSlider.setRange(1.0, 10.0, 0.1);
        gainSlider.setValue(5.0);
        gainSlider.addListener(this);
        gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);

        addAndMakeVisible(reverbSlider);
        reverbSlider.setRange(0.0, 1.0, 0.01);
        reverbSlider.setValue(0.3);
        reverbSlider.addListener(this);
        reverbSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);

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
        for (int channel = 0; channel < info.buffer->getNumChannels(); ++channel)
        {
            auto* buffer = info.buffer->getWritePointer(channel, info.startSample);
            for (int i = 0; i < info.numSamples; ++i)
            {
                float sample = std::sin(phase);
                phase += 2.0 * juce::MathConstants<float>::pi * 440.0f / (float) currentSampleRate;
                if (phase > 2.0f * juce::MathConstants<float>::pi)
                    phase -= 2.0f * juce::MathConstants<float>::pi;

                if (overdriveEnabled)
                    sample = overdrive.processSample(sample);

                sample = reverb.processSample(sample);

                buffer[i] = sample;
            }
        }
    }

    void releaseResources() override {}

    void resized() override
    {
        overdriveToggle.setBounds(20, 20, getWidth() - 40, 30);
        gainSlider.setBounds(20, 70, getWidth() - 40, 60);
        reverbSlider.setBounds(20, 150, getWidth() - 40, 60);
    }

    void buttonClicked(juce::Button* button) override
    {
        if (button == &overdriveToggle)
            overdriveEnabled = overdriveToggle.getToggleState();
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
    float phase = 0.0f;
    bool overdriveEnabled = true;

    Overdrive overdrive;
    CustomReverb reverb;


    juce::ToggleButton overdriveToggle;
    juce::Slider gainSlider;
    juce::Slider reverbSlider;
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
            juce::Colours::lightgrey,
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
