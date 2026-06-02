// Copyright 2025 Manna Harbour
// https://github.com/manna-harbour/miryoku

// This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version. This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.

#include QMK_KEYBOARD_H

#include "manna-harbour_miryoku.h"


// Both shifts -> Caps Lock

const uint16_t PROGMEM capslock_combo[] = {LSFT_T(KC_F), LSFT_T(KC_J), COMBO_END};

combo_t key_combos[] = {
    COMBO(capslock_combo, KC_CAPS),
};


// Per-layer RGB indicator: highlight the active layer's functional keys

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    uint8_t layer = get_highest_layer(layer_state | default_layer_state);
    if (layer == U_BASE) {
        return false;
    }

    hsv_t hsv;
    switch (layer) {
        case U_EXTRA:  hsv = (hsv_t){128, 200, 255}; break;
        case U_TAP:    hsv = (hsv_t){170, 180, 255}; break;
        case U_BUTTON: hsv = (hsv_t){21,  255, 255}; break;
        case U_NAV:    hsv = (hsv_t){85,  255, 255}; break;
        case U_MOUSE:  hsv = (hsv_t){170, 255, 255}; break;
        case U_MEDIA:  hsv = (hsv_t){43,  255, 255}; break;
        case U_NUM:    hsv = (hsv_t){0,   255, 255}; break;
        case U_SYM:    hsv = (hsv_t){213, 255, 255}; break;
        case U_FUN:    hsv = (hsv_t){150, 255, 255}; break;
        default:       return false;
    }

    if (hsv.v > rgb_matrix_get_val()) {
        hsv.v = rgb_matrix_get_val();
    }
    rgb_t rgb = hsv_to_rgb(hsv);

    for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
        for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
            uint8_t index = g_led_config.matrix_co[row][col];
            if (index >= led_min && index < led_max && index != NO_LED &&
                keymap_key_to_keycode(layer, (keypos_t){col, row}) > KC_TRNS) {
                rgb_matrix_set_color(index, rgb.r, rgb.g, rgb.b);
            }
        }
    }
    return false;
}
