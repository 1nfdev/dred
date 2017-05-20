// Copyright (C) 2017 David Reid. See included LICENSE file.

// Always include the headers first.
#include "dred.h"

// External libraries.
#ifndef DR_IMPLEMENTATION
    #define DR_IMPLEMENTATION
    #ifdef DRED_USE_EXTERNAL_REPOS
        #include "../../../../dr_libs/dr.h"
    #else
        #include "../external/dr.h"
    #endif
#endif

#define DR_IPC_IMPLEMENTATION
#ifdef DRED_USE_EXTERNAL_REPOS
#include "../../../dr_ipc/dr_ipc.h"
#else
#include "../external/dr_ipc.h"
#endif

#define DR_TEXT_ENGINE_IMPLEMENTATION
#include "../external/dr_text_engine.h"

#define GB_STRING_IMPLEMENTATION
#include "../external/gb_string.h"

#include "../external/json.c"

// dred source files.
#include "dtk/dtk.c"
#include "dred_autogenerated.c"
#include "dred_string_pool.c"
#include "dred_ipc.c"
#include "dred_dl.c"
#include "gui/dred_gui.c"
#include "gui/dred_scrollbar.c"
#include "gui/dred_tabbar.c"
#include "gui/dred_button.c"
#include "gui/dred_color_button.c"
#include "gui/dred_checkbox.c"
#include "gui/dred_tabgroup.c"
#include "gui/dred_tabgroup_container.c"
#include "gui/dred_textview.c"
#include "gui/dred_textbox.c"
#include "gui/dred_info_bar.c"
#include "gui/dred_cmdbar_popup.c"
#include "gui/dred_cmdbar.c"
#include "dred_cmdbox_cmdlist.c"
#include "dred_cmdbox.c"
#include "dred_fs.c"
#include "dred_alias_map.c"
#include "dred_config.c"
#include "dred_shortcuts.c"
#include "dred_editor.c"
#include "dred_settings_editor.c"
#include "dred_text_editor.c"
#include "dred_font.c"
#include "dred_font_library.c"
#include "dred_image.c"
#include "dred_image_library.c"
#include "dred_menus.c"
#include "dred_about_dialog.c"
#include "dred_settings_dialog.c"
#include "dred_printing.c"
#include "dred_context.c"
#include "dred_platform_layer.c"
#include "dred_commands.c"
#include "dred_misc.c"
#include "dred_stock_themes.c"
#include "dred_codegen.c"
#include "dred_package.c"
#include "dred_package_library.c"
#include "cmdline_funcs/dred_file2chex.c"
#include "cmdline_funcs/dred_file2cstring.c"
#include "cmdline_funcs/dred_main_f.c"