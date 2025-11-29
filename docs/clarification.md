# Hardware Design Clarifications

## All Pin Conflicts Resolved

All previously identified pin conflicts have been resolved based on the latest `hw_description.md`.

### GPIO4 Conflict (Resolved)
- **`btn_mode.plus`** was moved from `gpio4` to `gpio12`.
- **`temp.a0`** remains on `gpio4`.
- This conflict is resolved.

### GPIO12 Conflict (Resolved)
- **`btn_mode.plus`** was assigned to `gpio12`.
- A temporary conflict arose as `SW_LASER_PIN` was also using `gpio12` as a placeholder.
- **`sw_laser.plus`** has now been assigned to `gpio21` in `hw_description.md`.
- This conflict is resolved.

All component pins are now uniquely assigned.
