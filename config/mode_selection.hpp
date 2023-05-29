#ifndef _CONFIG_MODE_SELECTION_HPP
#define _CONFIG_MODE_SELECTION_HPP

#include "core/state.hpp"
#include "modes/DefaultKeyboardMode.hpp"
#include "modes/FgcMode.hpp"
#include "modes/Melee20Button.hpp"
#include "modes/ProjectM.hpp"
#include "modes/RivalsOfAether.hpp"
#include "modes/Ultimate.hpp"

#define LED1 4
#define LED2 5

extern KeyboardMode *current_kb_mode;

void set_mode(CommunicationBackend *backend, ControllerMode *mode) {
    // Delete keyboard mode in case one is set, so we don't end up getting both controller and
    // keyboard inputs.
    delete current_kb_mode;
    current_kb_mode = nullptr;

    // Set new controller mode.
    backend->SetGameMode(mode);
}

void set_mode(CommunicationBackend *backend, KeyboardMode *mode) {
    // Delete and reassign current keyboard mode.
    delete current_kb_mode;
    current_kb_mode = mode;

    // Unset the current controller mode so backend only gives neutral inputs.
    backend->SetGameMode(nullptr);
}

void set_mode(CommunicationBackend *backend, ControllerMode *mode, bool ledPins[]) {
    // Delete keyboard mode in case one is set, so we don't end up getting both controller and
    // keyboard inputs.
    delete current_kb_mode;
    current_kb_mode = nullptr;

    // Set LEDs
    gpio_put(LED1, false);
    gpio_put(LED2, false);

    gpio_put(LED1, ledPins[0]);
    gpio_put(LED2, ledPins[1]);

    // Set new controller mode.
    backend->SetGameMode(mode);
}
    

void select_mode(CommunicationBackend *backend) {
    InputState &inputs = backend->GetInputs();
    if (inputs.mod_x && !inputs.mod_y && inputs.start) {
    bool ledPins[2] = {false, false};
        if (inputs.left) {
            ledPins[0] = 1;
            ledPins[1] = 1;
            set_mode(
                backend,
                new Melee20Button(socd::SOCD_2IP_NO_REAC, { .crouch_walk_os = false }),
                ledPins
            );
        } else if (inputs.l) {
            set_mode(
                backend,
                new ProjectM(
                    socd::SOCD_2IP_NO_REAC,
                    { .true_z_press = false, .ledgedash_max_jump_traj = true }
                )
            );
        } else if (inputs.down) {
            ledPins[0] = 0;
            ledPins[1] = 0;
            set_mode(backend, new Ultimate(socd::SOCD_2IP), ledPins);
        } else if (inputs.right) {
            ledPins[0] = 0;
            ledPins[1] = 1;
            set_mode(backend, new FgcMode(socd::SOCD_NEUTRAL), ledPins);
        } else if (inputs.up) {
            ledPins[0] = 1;
            ledPins[1] = 0;
            set_mode(backend, new RivalsOfAether(socd::SOCD_2IP), ledPins);
        }
    } else if (inputs.mod_y && !inputs.mod_x && inputs.start) {
        if (inputs.l) {
            set_mode(backend, new DefaultKeyboardMode(socd::SOCD_2IP));
        }
    }
}

#endif
