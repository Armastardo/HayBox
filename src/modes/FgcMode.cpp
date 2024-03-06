#include "modes/FgcMode.hpp"

FgcMode::FgcMode(socd::SocdType socd_type) : ControllerMode(socd_type) {
    _socd_pair_count = 1;
    _socd_pairs = new socd::SocdPair[_socd_pair_count]{
        socd::SocdPair{&InputState::left, &InputState::right},
    };
}

void FgcMode::HandleSocd(InputState &inputs) {
    InputMode::HandleSocd(inputs);
}

void FgcMode::UpdateDigitalOutputs(InputState &inputs, OutputState &outputs) {
    // Directions
    outputs.dpadLeft = inputs.left;
    outputs.dpadRight = inputs.right;
    outputs.dpadDown = inputs.down;
    outputs.dpadUp = inputs.y;

    // Menu keys
    outputs.start = inputs.start;
    outputs.select = inputs.select;
    outputs.home = inputs.home;

    // Right hand bottom row
    outputs.a = inputs.c_left;
    outputs.b = inputs.c_right;
    outputs.triggerRDigital = inputs.c_up;
    outputs.triggerLDigital = inputs.z;

    // Right hand top row
    outputs.x = inputs.a;
    outputs.y = inputs.b;
    outputs.buttonR = inputs.l;
    outputs.buttonL = inputs.r;
    
    outputs.leftStickClick = inputs.mod_z;
    outputs.rightStickClick = inputs.x;
}

void FgcMode::UpdateAnalogOutputs(InputState &inputs, OutputState &outputs) {
    outputs.leftStickX = 128;
    outputs.leftStickY = 128;
    outputs.rightStickX = 128;
    outputs.rightStickY = 128;
    outputs.triggerLAnalog = outputs.triggerLDigital ? 255 : 0;
    outputs.triggerRAnalog = outputs.triggerRDigital ? 255 : 0;
}
