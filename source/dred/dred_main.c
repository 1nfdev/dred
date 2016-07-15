
// These #defines enable us to load large files on Linux platforms. They need to be placed before including any headers.
#ifndef _WIN32
#ifndef _LARGEFILE64_SOURCE
#define _LARGEFILE64_SOURCE
#endif

#ifndef _FILE_OFFSET_BITS
#define _FILE_OFFSET_BITS 64
#endif
#endif



// Standard headers.
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>

#if defined(_MSC_VER)
    #pragma warning(push)
    #pragma warning(disable:4091)   // 'typedef ': ignored on left of 'tagGPFIDL_FLAGS' when no variable is declared
#endif

// Platform headers.
#ifdef _WIN32
#include <windows.h>
#endif
#ifdef __linux__
#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <fontconfig/fontconfig.h>
#endif

// Platform libraries, for simplifying MSVC builds.
#ifdef _WIN32
#if defined(_MSC_VER) || defined(__clang__)
#pragma comment(lib, "msimg32.lib")
#pragma comment(lib, "comctl32.lib")
#endif
#endif


// External libraries.
#define DR_IMPLEMENTATION
#include "../../../../dr_libs/dr.h"

#define DR_PATH_IMPLEMENTATION
#include "../../../../dr_libs/dr_path.h"

#ifndef DRED_NO_IMAGE_EDITOR
#define DR_PCX_IMPLEMENTATION
#include "../../../../dr_libs/dr_pcx.h"

#if defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wshift-negative-value"
    #if !defined(__clang__)
    #pragma GCC diagnostic ignored "-Wmisleading-indentation"
    #endif
#endif
#define STB_IMAGE_IMPLEMENTATION
#include "../external/stb_image.h"
#if defined(__GNUC__)
    #pragma GCC diagnostic pop
#endif
#endif

#define DR_GUI_IMPLEMENTATION
#define DR_GUI_INCLUDE_WIP
#define DRGUI_NO_TEXT_EDITING
#define DR_2D_IMPLEMENTATION
#include "../../../../dr_libs/dr_gui.h"

#define DR_TEXT_ENGINE_IMPLEMENTATION
#include "../external/dr_text_engine.h"

#define DR_CPP_IMPLEMENTATION
#include "../../../../dr_ci/source/lang/dr_cpp.h"

#define GB_STRING_IMPLEMENTATION
#include "../external/gb_string.h"


// dred header files.
#include "dred_autogenerated.h"
#include "dred_build_config.h"
#include "dred_types.h"
#include "dred_fs.h"
#include "dred_alias_map.h"
#include "dred_config.h"
#include "dred_accelerators.h"
#include "dred_shortcuts.h"
#include "dred_control.h"
#include "dred_textbox.h"
#include "dred_info_bar.h"
#include "dred_cmdbar.h"
#include "dred_tab.h"
#include "dred_tabgroup.h"
#include "dred_tabgroup_container.h"
#include "dred_editor.h"
#include "dred_settings_editor.h"
#include "dred_text_editor.h"
#include "dred_image_editor.h"
#include "dred_font.h"
#include "dred_font_library.h"
#include "dred_image.h"
#include "dred_image_library.h"
#include "dred_menus.h"
#include "dred_about_dialog.h"
#include "dred_settings_dialog.h"
#include "dred_context.h"
#include "dred_platform_layer.h"
#include "dred_commands.h"
#include "dred_misc.h"
#include "dred_stock_themes.h"
#include "dred_highlighters.h"
#include "common_controls/dred_button.h"
#include "common_controls/dred_color_button.h"
#include "common_controls/dred_checkbox.h"

// dred source files.
#include "dred_autogenerated.c"
#include "dred_fs.c"
#include "dred_alias_map.c"
#include "dred_config.c"
#include "dred_accelerators.c"
#include "dred_shortcuts.c"
#include "dred_control.c"
#include "dred_textbox.c"
#include "dred_info_bar.c"
#include "dred_cmdbar.c"
#include "dred_tab.c"
#include "dred_tabgroup.c"
#include "dred_tabgroup_container.c"
#include "dred_editor.c"
#include "dred_settings_editor.c"
#include "dred_text_editor.c"
#include "dred_image_editor.c"
#include "dred_font.c"
#include "dred_font_library.c"
#include "dred_image.c"
#include "dred_image_library.c"
#include "dred_menus.c"
#include "dred_about_dialog.c"
#include "dred_settings_dialog.c"
#include "dred_context.c"
#include "dred_platform_layer.c"
#include "dred_commands.c"
#include "dred_misc.c"
#include "dred_stock_themes.c"
#include "dred_highlighters.c"
#include "common_controls/dred_button.c"
#include "common_controls/dred_color_button.c"
#include "common_controls/dred_checkbox.c"

#if defined(_MSC_VER)
    #pragma warning(pop)
#endif

int dred_main(dr_cmdline cmdline)
{
    // The platform needs to be initialized first. In the case of Windows, this will register the window classes
    // and enable DPI awareness. Always make sure this is the first thing to be called.
    dred_platform_init();

    dred_context dred;
    if (!dred_init(&dred, cmdline)) {
        return -1;
    }

    int result = dred_run(&dred);

    dred_uninit(&dred);
    dred_platform_uninit();
    return result;
}

int main(int argc, char** argv)
{
    dr_cmdline cmdline;
    dr_init_cmdline(&cmdline, argc, argv);

    return dred_main(cmdline);
}

#ifdef _WIN32
#ifdef NDEBUG
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    (void)hInstance;
    (void)hPrevInstance;
    (void)lpCmdLine;
    (void)nCmdShow;

    dr_cmdline cmdline;
    dr_init_cmdline_win32(&cmdline, GetCommandLineA());

    return dred_main(cmdline);
}
#endif
#endif