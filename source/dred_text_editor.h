
#define DRED_CONTROL_TYPE_TEXT_EDITOR  "dred.editor.text"

typedef dred_editor dred_text_editor;


// dred_text_editor_create()
dred_text_editor* dred_text_editor_create(dred_context* pDred, dred_control* pParent, const char* filePathAbsolute);

// dred_text_editor_delete()
void dred_text_editor_delete(dred_text_editor* pTextEditor);


// Shows the line numbers.
void dred_editor_text_subeditor_show_line_numbers(dred_text_editor* pTextEditor);

// Hides the line numbers.
void dred_editor_text_subeditor_hide_line_numbers(dred_text_editor* pTextEditor);


// Retrieves the index of the line the caret is currently sitting on.
size_t dred_text_editor_get_cursor_line(dred_text_editor* pTextEditor);

// Retrieves the index of the column the caret is currently sitting on.
size_t dred_text_editor_get_cursor_column(dred_text_editor* pTextEditor);


// dred_text_editor_goto_ratio()
void dred_text_editor_goto_ratio(dred_text_editor* pTextEditor, size_t ratio);

// dred_text_editor_goto_line()
void dred_text_editor_goto_line(dred_text_editor* pTextEditor, size_t lineNumber);


// Finds and selects the next occurance of the given string, starting from the cursor and looping back to the start.
bool dred_text_editor_find_and_select_next(dred_text_editor* pTextEditor, const char* text);

// Finds the next occurance of the given string and replaces it with another.
bool dred_text_editor_find_and_replace_next(dred_text_editor* pTextEditor, const char* text, const char* replacement);

// Finds every occurance of the given string and replaces it with another.
bool dred_text_editor_find_and_replace_all(dred_text_editor* pTextEditor, const char* text, const char* replacement);