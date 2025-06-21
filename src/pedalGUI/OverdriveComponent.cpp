#include "OverdriveComponent.h"
#include "CustomLookAndFeel.h"

static CustomLookAndFeel customKnobLook;

OverdriveComponent::OverdriveComponent()
    : PedalComponent("Overdrive", std::make_unique<Overdrive>(), "overdrive.png")
{
    overdrivePtr = dynamic_cast<Overdrive*>(effectProcessor.get());

    // === LEVEL KNOB ONLY ===
    levelKnob.setLookAndFeel(&customKnobLook);
    levelKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    levelKnob.setRotaryParameters(juce::MathConstants<float>::pi * 1.25f,
                               juce::MathConstants<float>::pi * 2.75f,
                               true);
    levelKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    levelKnob.setRange(0.0, 2.0, 0.01);
    levelKnob.setValue(1.0);
    levelKnob.setRotaryParameters(juce::MathConstants<float>::pi * 0.75f,  // ~7 o'clock
                                  juce::MathConstants<float>::pi * 2.75f,  // ~5 o'clock
                                  true);

    levelKnob.onValueChange = [this]() {
        level = levelKnob.getValue();
    };

    addAndMakeVisible(levelKnob);
}

void OverdriveComponent::resized()
{
    const int knobSize = 38;

    // Updated positions
    const int levelCenterX = 73;  // previous was 68 + 5 pixels right
    const int levelCenterY = 20;  // previous was 61 + 20 pixels down

    const int driveCenterX = 159; // unchanged for now
    const int driveCenterY = 64;

    const int toneCenterX = 115;
    const int toneCenterY = 96;

    levelKnob.setBounds(levelCenterX - knobSize / 2, levelCenterY - knobSize / 2, knobSize, knobSize);
    driveKnob.setBounds(driveCenterX - knobSize / 2, driveCenterY - knobSize / 2, knobSize, knobSize);
    toneKnob.setBounds(toneCenterX - knobSize / 2, toneCenterY - knobSize / 2, knobSize, knobSize);
}


float OverdriveComponent::processSample(float sample)
{
    if (effectProcessor)
        sample = effectProcessor->processSample(sample);
    return sample * level;
}
