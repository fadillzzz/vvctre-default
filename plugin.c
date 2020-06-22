// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include <stdbool.h>

static const char* required_function_names[] = {
	"vvctre_settings_set_audio_volume",
	"vvctre_settings_set_custom_textures",
	"vvctre_settings_set_resolution",
	"vvctre_settings_set_layout",
    "vvctre_set_cheat_enabled"
};

typedef void (*vvctre_settings_set_audio_volume_t)(float value);
typedef void (*vvctre_settings_set_custom_textures_t)(bool value);
typedef void (*vvctre_settings_set_resolution_t)(int value);
typedef void (*vvctre_settings_set_layout_t)(int value);
typedef void (*vvctre_set_cheat_enabled_t)(void* core, int index, bool enabled);

static vvctre_settings_set_audio_volume_t vvctre_settings_set_audio_volume;
static vvctre_settings_set_custom_textures_t vvctre_settings_set_custom_textures;
static vvctre_settings_set_resolution_t vvctre_settings_set_resolution;
static vvctre_settings_set_layout_t vvctre_settings_set_layout;
static vvctre_set_cheat_enabled_t vvctre_set_cheat_enabled;
static void* vvctre_core;

#ifdef _WIN32
#define VVCTRE_PLUGIN_EXPORT __declspec(dllexport)
#else
#define VVCTRE_PLUGIN_EXPORT
#endif

VVCTRE_PLUGIN_EXPORT int GetRequiredFunctionCount() {
    return 5;
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