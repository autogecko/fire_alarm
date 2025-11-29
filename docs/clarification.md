# Hardware Design Clarifications

## Pin Conflict on GPIO4 (Resolved)

The previous pin conflict on `GPIO4` has been resolved with the update to `hw_description.md`.

- **`btn_mode.plus`** has been reassigned from `gpio4` to `gpio12`.
- **`temp.a0`** remains on `gpio4`.

This allows both the button and temperature sensor to be implemented on their dedicated pins.

## New Potential Pin Conflict on GPIO12

A new potential conflict has been created by reassigning `btn_mode.plus` to `gpio12`.

- **`BTN_PIN`** is now `12`.
- **`SW_LASER_PIN`** is currently a placeholder also set to `12` in `include/config.h`.

**Resolution:**
The pin for `sw_laser` is not yet specified in `hw_description.md`. `SW_LASER_PIN` needs a new pin assignment in `include/config.h` before the laser can be used. The button will use `GPIO12` as specified.
