// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include <stdbool.h>

static const char* required_function_names[] = {
    "vvctre_settings_set_audio_volume",
    "vvctre_settings_set_custom_textures",
    "vvctre_settings_set_resolution",
    "vvctre_settings_set_layout",
    "vvctre_set_cheat_enabled",
    "vvctre_button_device_new",
    "vvctre_set_custom_touch_state",
    "vvctre_use_real_touch_state",
    "vvctre_button_device_get_state"

};

typedef void (*vvctre_settings_set_audio_volume_t)(float value);
typedef void (*vvctre_settings_set_custom_textures_t)(bool value);
typedef void (*vvctre_settings_set_resolution_t)(int value);
typedef void (*vvctre_settings_set_layout_t)(int value);
typedef void (*vvctre_set_cheat_enabled_t)(void* core, int index, bool enabled);
typedef void (*vvctre_button_device_new_t)(void* plugin_manager, const char* params);
typedef void (*vvctre_set_custom_touch_state_t)(void* core, float x, float y, bool pressed);
typedef void (*vvctre_use_real_touch_state_t)(void * core);
typedef bool (*vvctre_button_device_get_state_t)(void * device);

static vvctre_settings_set_audio_volume_t vvctre_settings_set_audio_volume;
static vvctre_settings_set_custom_textures_t vvctre_settings_set_custom_textures;
static vvctre_settings_set_resolution_t vvctre_settings_set_resolution;
static vvctre_settings_set_layout_t vvctre_settings_set_layout;
static vvctre_set_cheat_enabled_t vvctre_set_cheat_enabled;
static vvctre_button_device_new_t vvctre_button_device_new;
static vvctre_set_custom_touch_state_t vvctre_set_custom_touch_state;
static vvctre_use_real_touch_state_t vvctre_use_real_touch_state;
static vvctre_button_device_get_state_t vvctre_button_device_get_state;
static void* vvctre_core;
static void* vvctre_devices[3];

#ifdef _WIN32
#define VVCTRE_PLUGIN_EXPORT __declspec(dllexport)
#else
#define VVCTRE_PLUGIN_EXPORT
#endif

VVCTRE_PLUGIN_EXPORT int GetRequiredFunctionCount() {
    return 9;
}

VVCTRE_PLUGIN_EXPORT const char** GetRequiredFunctionNames() {
    return required_function_names;
}

VVCTRE_PLUGIN_EXPORT void PluginLoaded(void* core, void* plugin_manager,
                                       void* required_functions[]) {
    vvctre_core = core;
    vvctre_settings_set_audio_volume = (vvctre_settings_set_audio_volume_t)required_functions[0];
    vvctre_settings_set_custom_textures = (vvctre_settings_set_custom_textures_t)required_functions[1];
    vvctre_settings_set_resolution = (vvctre_settings_set_resolution_t)required_functions[2];
    vvctre_settings_set_layout = (vvctre_settings_set_layout_t)required_functions[3];
    vvctre_set_cheat_enabled = (vvctre_set_cheat_enabled_t)required_functions[4];
    vvctre_button_device_new = (vvctre_button_device_new_t)required_functions[5];
    vvctre_set_custom_touch_state = (vvctre_set_custom_touch_state_t)required_functions[6];
    vvctre_use_real_touch_state = (vvctre_use_real_touch_state_t)required_functions[7];
    vvctre_button_device_get_state = (vvctre_button_device_get_state_t)required_functions[8];
    char* buttons[3] = {
        "engine:sdl,port:0,guid:030000007e0500000920000010026800,button:5",
        "engine:sdl,port:0,guid:030000007e0500000920000010026800,button:7",
        "engine:sdl,port:0,guid:030000007e0500000920000010026800,button:8"
    };
    for (int i = 0; i < 3; i++) {
        vvctre_devices[i] = vvctre_button_device_new(plugin_manager, buttons[i]);
    }
}


VVCTRE_PLUGIN_EXPORT void InitialSettingsOpening() {
    vvctre_settings_set_audio_volume(0.85f);
    vvctre_settings_set_custom_textures(true);
    vvctre_settings_set_resolution(5);
    vvctre_settings_set_layout(2);
}

VVCTRE_PLUGIN_EXPORT void EmulationStarting() {
    vvctre_set_cheat_enabled(vvctre_core, 0, true);
}

VVCTRE_PLUGIN_EXPORT void AfterSwapWindow() {
    float coords[3][2] = {
        {
            284.0f,
            61.0f,
        },
        {
            32.0f,
            49.0f,
        },
        {
            303.0f,
            6.0f
        }

    };
    static bool was_pressed = false;
    for (int i = 0; i < 3; i++) {
        const bool pressed = vvctre_button_device_get_state(vvctre_devices[i]);
        if (was_pressed && !pressed) {
            vvctre_use_real_touch_state(vvctre_core);
            was_pressed = false;
        } else if (!was_pressed && pressed) {
            vvctre_set_custom_touch_state(vvctre_core, coords[i][0], coords[i][1], true);
            was_pressed = true;
        }
    }
}
