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


// Per-layer RGB indicator.
//
// NAV layer: per-key colors (arrows green, home/end yellow, copy/paste blue,
// undo/redo red, NAV thumb green); every other key is unlit.
// Other non-BASE layers: uniform highlight on the layer's functional keys.

static void apply_hsv(uint8_t index, hsv_t hsv) {
    if (hsv.v > rgb_matrix_get_val()) {
        hsv.v = rgb_matrix_get_val();
    }
    rgb_t rgb = hsv_to_rgb(hsv);
    rgb_matrix_set_color(index, rgb.r, rgb.g, rgb.b);
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    uint8_t layer = get_highest_layer(layer_state | default_layer_state);
    if (layer == U_BASE) {
        return false;
    }

    if (layer == U_NAV) {
        for (uint8_t i = led_min; i < led_max; i++) {
            rgb_matrix_set_color(i, 0, 0, 0);
        }

        uint8_t base_layer = get_highest_layer(default_layer_state);
        for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
            for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
                uint8_t index = g_led_config.matrix_co[row][col];
                if (index == NO_LED || index < led_min || index >= led_max) {
                    continue;
                }
                uint16_t kc      = keymap_key_to_keycode(layer,      (keypos_t){col, row});
                uint16_t base_kc = keymap_key_to_keycode(base_layer, (keypos_t){col, row});
                hsv_t hsv;
                switch (kc) {
                    case KC_LEFT:
                    case KC_DOWN:
                    case KC_UP:
                    case KC_RGHT:
                        hsv = (hsv_t){85, 255, 255}; break;
                    case KC_HOME:
                    case KC_END:
                        hsv = (hsv_t){43, 255, 255}; break;
                    case U_CPY:
                    case U_PST:
                        hsv = (hsv_t){170, 255, 255}; break;
                    case U_UND:
                    case U_RDO:
                        hsv = (hsv_t){0, 255, 255}; break;
                    default:
                        if (base_kc != LT(U_NAV, KC_SPC)) {
                            continue;
                        }
                        hsv = (hsv_t){85, 255, 255};
                        break;
                }
                apply_hsv(index, hsv);
            }
        }
        return false;
    }

    hsv_t hsv;
    switch (layer) {
        case U_EXTRA:  hsv = (hsv_t){128, 200, 255}; break;
        case U_TAP:    hsv = (hsv_t){170, 180, 255}; break;
        case U_BUTTON: hsv = (hsv_t){21,  255, 255}; break;
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
