# Hardware Design Clarifications

## Pin Conflict on GPIO4

A pin conflict has been identified in `hw_description.md`.

- **`btn_mode.plus`** is assigned to `gpio4`.
- **`temp.a0`** (for the DHT11 sensor) is also assigned to `gpio4`.

**Resolution:**
This conflict must be resolved before the DHT11 temperature sensor functionality can be implemented. The button functionality will be implemented first using `GPIO4`. The temperature sensor implementation is on hold pending a hardware pin reassignment.
