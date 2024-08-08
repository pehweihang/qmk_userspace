# User Pointing Device customization

To disable the customized pointing device code and implement it at the keymap, add `CUSTOM_POINTING_DEVICE = no` to your `rules.mk`.

## Automatic Mouse Layer

***Deprecated*** - Functionality moved to core

## Gaming

When the gamepad or diablo layers are enabled, the mouse layer is locked on, as well.

## Keycodes

The only custom keycode for Pointing devices here is `KC_ACCEL`.  This allow the mouse report to have an acceleration curve (exponential).
