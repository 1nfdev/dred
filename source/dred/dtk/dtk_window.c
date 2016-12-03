// Copyright (C) 2016 David Reid. See included LICENSE file.

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
// WIN32
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#ifdef DTK_WIN32
#define DTK_WIN32_WINDOW_CLASS              "dtk.window"
#define DTK_WIN32_WINDOW_CLASS_POPUP        "dtk.window.popup"
#define DTK_WIN32_WINDOW_CLASS_TIMER        "dtk.window.timer"

#define DTK_GET_X_LPARAM(lp)    ((int)(short)LOWORD(lp))
#define DTK_GET_Y_LPARAM(lp)    ((int)(short)HIWORD(lp))

static void dtk_track_mouse_leave_event__win32(HWND hWnd)
{
    TRACKMOUSEEVENT tme;
    ZeroMemory(&tme, sizeof(tme));
    tme.cbSize    = sizeof(tme);
    tme.dwFlags   = TME_LEAVE;
    tme.hwndTrack = hWnd;
    TrackMouseEvent(&tme);
}

dtk_bool32 dtk_is_win32_mouse_button_key_code(WPARAM wParam)
{
    return wParam == VK_LBUTTON || wParam == VK_RBUTTON || wParam == VK_MBUTTON || wParam == VK_XBUTTON1 || wParam == VK_XBUTTON2;
}

dtk_key dtk_win32_to_dtk_key(WPARAM wParam)
{
    switch (wParam)
    {
    case VK_BACK:   return DTK_KEY_BACKSPACE;
    case VK_SHIFT:  return DTK_KEY_SHIFT;
    case VK_ESCAPE: return DTK_KEY_ESCAPE;
    case VK_PRIOR:  return DTK_KEY_PAGE_UP;
    case VK_NEXT:   return DTK_KEY_PAGE_DOWN;
    case VK_END:    return DTK_KEY_END;
    case VK_HOME:   return DTK_KEY_HOME;
    case VK_LEFT:   return DTK_KEY_ARROW_LEFT;
    case VK_UP:     return DTK_KEY_ARROW_UP;
    case VK_RIGHT:  return DTK_KEY_ARROW_RIGHT;
    case VK_DOWN:   return DTK_KEY_ARROW_DOWN;
    case VK_DELETE: return DTK_KEY_DELETE;
    case VK_F1:     return DTK_KEY_F1;
    case VK_F2:     return DTK_KEY_F2;
    case VK_F3:     return DTK_KEY_F3;
    case VK_F4:     return DTK_KEY_F4;
    case VK_F5:     return DTK_KEY_F5;
    case VK_F6:     return DTK_KEY_F6;
    case VK_F7:     return DTK_KEY_F7;
    case VK_F8:     return DTK_KEY_F8;
    case VK_F9:     return DTK_KEY_F9;
    case VK_F10:    return DTK_KEY_F10;
    case VK_F11:    return DTK_KEY_F11;
    case VK_F12:    return DTK_KEY_F12;

    default: break;
    }

    return (dtk_key)wParam;
}

WORD dtk_key_to_win32(dtk_key key)
{
    switch (key)
    {
    case DTK_KEY_BACKSPACE:   return VK_BACK;
    case DTK_KEY_SHIFT:       return VK_SHIFT;
    case DTK_KEY_ESCAPE:      return VK_ESCAPE;
    case DTK_KEY_PAGE_UP:     return VK_PRIOR;
    case DTK_KEY_PAGE_DOWN:   return VK_NEXT;
    case DTK_KEY_END:         return VK_END;
    case DTK_KEY_HOME:        return VK_HOME;
    case DTK_KEY_ARROW_LEFT:  return VK_LEFT;
    case DTK_KEY_ARROW_UP:    return VK_UP;
    case DTK_KEY_ARROW_RIGHT: return VK_RIGHT;
    case DTK_KEY_ARROW_DOWN:  return VK_DOWN;
    case DTK_KEY_DELETE:      return VK_DELETE;
    case DTK_KEY_F1:          return VK_F1;
    case DTK_KEY_F2:          return VK_F2;
    case DTK_KEY_F3:          return VK_F3;
    case DTK_KEY_F4:          return VK_F4;
    case DTK_KEY_F5:          return VK_F5;
    case DTK_KEY_F6:          return VK_F6;
    case DTK_KEY_F7:          return VK_F7;
    case DTK_KEY_F8:          return VK_F8;
    case DTK_KEY_F9:          return VK_F9;
    case DTK_KEY_F10:         return VK_F10;
    case DTK_KEY_F11:         return VK_F11;
    case DTK_KEY_F12:         return VK_F12;

    default: break;
    }

    return (WORD)key;
}

static dtk_uint32 dtk_get_modifier_key_state_flags__win32()
{
    int stateFlags = 0;

    SHORT keyState = GetAsyncKeyState(VK_SHIFT);
    if (keyState & 0x8000) {
        stateFlags |= DTK_KEY_SHIFT_DOWN;
    }

    keyState = GetAsyncKeyState(VK_CONTROL);
    if (keyState & 0x8000) {
        stateFlags |= DTK_KEY_CTRL_DOWN;
    }

    keyState = GetAsyncKeyState(VK_MENU);
    if (keyState & 0x8000) {
        stateFlags |= DTK_KEY_ALT_DOWN;
    }

    return stateFlags;
}

static dtk_uint32 dtk_get_mouse_event_state_flags__win32(WPARAM wParam)
{
    int stateFlags = 0;

    if ((wParam & MK_LBUTTON) != 0) {
        stateFlags |= DTK_MOUSE_BUTTON_LEFT_DOWN;
    }
    if ((wParam & MK_RBUTTON) != 0) {
        stateFlags |= DTK_MOUSE_BUTTON_RIGHT_DOWN;
    }
    if ((wParam & MK_MBUTTON) != 0) {
        stateFlags |= DTK_MOUSE_BUTTON_MIDDLE_DOWN;
    }
    if ((wParam & MK_XBUTTON1) != 0) {
        stateFlags |= DTK_MOUSE_BUTTON_4_DOWN;
    }
    if ((wParam & MK_XBUTTON2) != 0) {
        stateFlags |= DTK_MOUSE_BUTTON_5_DOWN;
    }

    if ((wParam & MK_CONTROL) != 0) {
        stateFlags |= DTK_KEY_CTRL_DOWN;
    }
    if ((wParam & MK_SHIFT) != 0) {
        stateFlags |= DTK_KEY_SHIFT_DOWN;
    }

    SHORT keyState = GetAsyncKeyState(VK_MENU);
    if (keyState & 0x8000) {
        stateFlags |= DTK_KEY_ALT_DOWN;
    }

    return stateFlags;
}

static ACCEL dtk_win32_to_ACCEL(dtk_key key, uint32_t modifiers, WORD cmd)
{
    ACCEL a;
    a.key = dtk_key_to_win32(key);
    a.cmd = cmd;

    a.fVirt = FVIRTKEY;
    if (modifiers & DTK_KEY_SHIFT_DOWN) {
        a.fVirt |= FSHIFT;
    }
    if (modifiers & DTK_KEY_CTRL_DOWN) {
        a.fVirt |= FCONTROL;
    }
    if (modifiers & DTK_KEY_ALT_DOWN) {
        a.fVirt |= FALT;
    }

    return a;
}


static dtk_uint32 dtk_wm_event_to_mouse_button__win32(UINT msg)
{
    switch (msg)
    {
        case WM_NCRBUTTONDOWN:
        case WM_NCRBUTTONUP:
        case WM_NCRBUTTONDBLCLK:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_RBUTTONDBLCLK:
        {
            return DTK_MOUSE_BUTTON_RIGHT;
        }

        case WM_NCMBUTTONDOWN:
        case WM_NCMBUTTONUP:
        case WM_NCMBUTTONDBLCLK:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
        case WM_MBUTTONDBLCLK:
        {
            return DTK_MOUSE_BUTTON_MIDDLE;
        }

        case WM_NCLBUTTONDOWN:
        case WM_NCLBUTTONUP:
        case WM_NCLBUTTONDBLCLK:
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_LBUTTONDBLCLK:
        default:
        {
            return DTK_MOUSE_BUTTON_LEFT;
        }
    }
}

LRESULT CALLBACK CALLBACK dtk_GenericWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    dtk_window* pWindow = (dtk_window*)GetWindowLongPtrA(hWnd, 0);
    if (pWindow == NULL) {
        return DefWindowProcA(hWnd, msg, wParam, lParam);
    }

    dtk_event e;
    e.type = DTK_EVENT_NONE;
    e.pTK = DTK_CONTROL(pWindow)->pTK;
    e.pControl = DTK_CONTROL(pWindow);

    switch (msg)
    {
        case WM_CREATE:
        {
            //dtk_track_mouse_leave_event__win32(hWnd);   // <-- This allows us to track mouse enter and leave events for the window.
        } return 0;

        case WM_DESTROY:
        {
        } break;

        case WM_CLOSE:
        {
            e.type = DTK_EVENT_CLOSE;
            dtk__handle_event(&e);
        } return 0;

        case WM_PAINT:
        {
            RECT rect;
            if (GetUpdateRect(hWnd, &rect, FALSE)) {
                e.type = DTK_EVENT_PAINT;
                e.paint.rect.left = rect.left;
                e.paint.rect.top = rect.top;
                e.paint.rect.right = rect.right;
                e.paint.rect.bottom = rect.bottom;
                dtk__handle_event(&e);
            }
        } break;

        case WM_SIZE:
        {
            e.type = DTK_EVENT_SIZE;
            e.size.width  = LOWORD(lParam);
            e.size.height = HIWORD(lParam);
            dtk__handle_event(&e);
        } break;

        case WM_MOVE:
        {
            e.type = DTK_EVENT_MOVE;

        #if 1
            // This technique will use the position of the entire window, including the frame.
            RECT rect;
            GetWindowRect(hWnd, &rect);
            e.move.x = rect.left;
            e.move.y = rect.top;
        #else
            // This technique will use the position of the client area.
            e.move.x = DTK_GET_X_LPARAM(lParam);
            e.move.y = DTK_GET_Y_LPARAM(lParam);
        #endif

            dtk__handle_event(&e);
        } break;


        // show/hide
        case WM_WINDOWPOSCHANGING:
        {
            WINDOWPOS* pWindowPos = (WINDOWPOS*)lParam;
            assert(pWindowPos != NULL);

            if ((pWindowPos->flags & SWP_HIDEWINDOW) != 0) {
                e.type = DTK_EVENT_HIDE;
                dtk__handle_event(&e);
            }
            if ((pWindowPos->flags & SWP_SHOWWINDOW) != 0) {
                e.type = DTK_EVENT_SHOW;
                dtk__handle_event(&e);
            }
        } break;


        case WM_MOUSELEAVE:
        {
            pWindow->win32.isCursorOverClientArea = DTK_FALSE;

            e.type = DTK_EVENT_MOUSE_LEAVE;
            dtk__handle_event(&e);
        } break;

        case WM_MOUSEMOVE:
        {
            if (!pWindow->win32.isCursorOverClientArea) {
                pWindow->win32.isCursorOverClientArea = DTK_TRUE;
                e.type = DTK_EVENT_MOUSE_ENTER;
                dtk__handle_event(&e);

                dtk_track_mouse_leave_event__win32(hWnd);
            }

            e.type = DTK_EVENT_MOUSE_MOVE;
            e.mouseMove.x = DTK_GET_X_LPARAM(lParam);
            e.mouseMove.y = DTK_GET_Y_LPARAM(lParam);
            dtk__handle_event(&e);
        } break;

    // Uncomment this to enable mouse button events in the non-client area of the window. This is inconsistent with other backends, however.
    #if 0
        case WM_NCLBUTTONDOWN:
        case WM_NCRBUTTONDOWN:
        case WM_NCMBUTTONDOWN:
        {
            POINT p;
            p.x = DTK_GET_X_LPARAM(lParam);
            p.y = DTK_GET_Y_LPARAM(lParam);
            ScreenToClient(hWnd, &p);

            e.type = DTK_EVENT_MOUSE_BUTTON_DOWN;
            e.mouseButton.x = p.x;
            e.mouseButton.y = p.y;
            e.mouseButton.button = dtk_wm_event_to_mouse_button__win32(msg);
            e.mouseButton.state = dtk_get_mouse_event_state_flags__win32(wParam);
            dtk__handle_event(&e);
        } break;

        case WM_NCLBUTTONUP:
        case WM_NCRBUTTONUP:
        case WM_NCMBUTTONUP:
        {
            POINT p;
            p.x = DTK_GET_X_LPARAM(lParam);
            p.y = DTK_GET_Y_LPARAM(lParam);
            ScreenToClient(hWnd, &p);

            e.type = DTK_EVENT_MOUSE_BUTTON_UP;
            e.mouseButton.x = p.x;
            e.mouseButton.y = p.y;
            e.mouseButton.button = dtk_wm_event_to_mouse_button__win32(msg);
            e.mouseButton.state = dtk_get_mouse_event_state_flags__win32(wParam);
            dtk__handle_event(&e);
        } break;

        case WM_NCLBUTTONDBLCLK:
        case WM_NCRBUTTONDBLCLK:
        case WM_NCMBUTTONDBLCLK:
        {
            POINT p;
            p.x = DTK_GET_X_LPARAM(lParam);
            p.y = DTK_GET_Y_LPARAM(lParam);
            ScreenToClient(hWnd, &p);

            // Special case for double click because we want to post a normal mouse button down event first before the double-click event.
            e.type = DTK_EVENT_MOUSE_BUTTON_DOWN;
            e.mouseButton.x = p.x;
            e.mouseButton.y = p.y;
            e.mouseButton.button = dtk_wm_event_to_mouse_button__win32(msg);
            e.mouseButton.state = dtk_get_mouse_event_state_flags__win32(wParam);
            dtk__handle_event(&e);

            e.type = DTK_EVENT_MOUSE_BUTTON_DBLCLICK;
            dtk__handle_event(&e);
        } break;
    #endif

        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
        {
            POINT p;
            p.x = DTK_GET_X_LPARAM(lParam);
            p.y = DTK_GET_Y_LPARAM(lParam);

            e.type = DTK_EVENT_MOUSE_BUTTON_DOWN;
            e.mouseButton.x = p.x;
            e.mouseButton.y = p.y;
            e.mouseButton.button = dtk_wm_event_to_mouse_button__win32(msg);
            e.mouseButton.state = dtk_get_mouse_event_state_flags__win32(wParam);
            dtk__handle_event(&e);
        } break;

        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
        {
            POINT p;
            p.x = DTK_GET_X_LPARAM(lParam);
            p.y = DTK_GET_Y_LPARAM(lParam);

            e.type = DTK_EVENT_MOUSE_BUTTON_UP;
            e.mouseButton.x = p.x;
            e.mouseButton.y = p.y;
            e.mouseButton.button = dtk_wm_event_to_mouse_button__win32(msg);
            e.mouseButton.state = dtk_get_mouse_event_state_flags__win32(wParam);
            dtk__handle_event(&e);
        } break;

        case WM_LBUTTONDBLCLK:
        case WM_RBUTTONDBLCLK:
        case WM_MBUTTONDBLCLK:
        {
            POINT p;
            p.x = DTK_GET_X_LPARAM(lParam);
            p.y = DTK_GET_Y_LPARAM(lParam);

            // Special case for double click because we want to post a normal mouse button down event first before the double-click event.
            e.type = DTK_EVENT_MOUSE_BUTTON_DOWN;
            e.mouseButton.x = p.x;
            e.mouseButton.y = p.y;
            e.mouseButton.button = dtk_wm_event_to_mouse_button__win32(msg);
            e.mouseButton.state = dtk_get_mouse_event_state_flags__win32(wParam);
            dtk__handle_event(&e);

            e.type = DTK_EVENT_MOUSE_BUTTON_DBLCLICK;
            dtk__handle_event(&e);
        } break;

        case WM_MOUSEWHEEL:
        {
            dtk_int32 delta = GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;

            POINT p;
            p.x = DTK_GET_X_LPARAM(lParam);
            p.y = DTK_GET_Y_LPARAM(lParam);
            ScreenToClient(hWnd, &p);

            e.type = DTK_EVENT_MOUSE_WHEEL;
            e.mouseWheel.x = p.x;
            e.mouseWheel.y = p.y;
            e.mouseWheel.delta = delta;
            e.mouseWheel.state = dtk_get_mouse_event_state_flags__win32(wParam);
            dtk__handle_event(&e);
        } break;


        case WM_KEYDOWN:
        {
            if (!dtk_is_win32_mouse_button_key_code(wParam)) {
                int stateFlags = dtk_get_modifier_key_state_flags__win32();
                if ((lParam & (1 << 30)) != 0) {
                    stateFlags |= DTK_KEY_AUTO_REPEATED;
                }

                e.type = DTK_EVENT_KEY_DOWN;
                e.keyDown.key = dtk_win32_to_dtk_key(wParam);
                e.keyDown.state = stateFlags;
                dtk__handle_event(&e);
            }
        } break;

        case WM_KEYUP:
        {
            if (!dtk_is_win32_mouse_button_key_code(wParam)) {
                e.type = DTK_EVENT_KEY_UP;
                e.keyDown.key = dtk_win32_to_dtk_key(wParam);
                e.keyDown.state = dtk_get_modifier_key_state_flags__win32();
                dtk__handle_event(&e);
            }
        } break;

        // NOTE: WM_UNICHAR is not posted by Windows itself, but rather intended to be posted by applications. Thus, we need to use WM_CHAR. WM_CHAR
        //       posts events as UTF-16 code points. When the code point is a surrogate pair, we need to store it and wait for the next WM_CHAR event
        //       which will contain the other half of the pair.
        case WM_CHAR:
        {
            // Windows will post WM_CHAR events for keys we don't particularly want. We'll filter them out here (they will be processed by WM_KEYDOWN).
            if (wParam < 32 || wParam == 127) {      // 127 = ASCII DEL (will be triggered by CTRL+Backspace)
                if (wParam != VK_TAB  &&
                    wParam != VK_RETURN) {   // VK_RETURN = Enter Key.
                    break;
                }
            }

            if ((lParam & (1U << 31)) == 0) {    // Bit 31 will be 1 if the key was pressed, 0 if it was released.
                if (IS_HIGH_SURROGATE(wParam)) {
                    dtk_assert(pWindow->win32.utf16HighSurrogate == 0);
                    pWindow->win32.utf16HighSurrogate = (unsigned short)wParam;
                } else {
                    unsigned int character = (unsigned int)wParam;
                    if (IS_LOW_SURROGATE(wParam)) {
                        assert(IS_HIGH_SURROGATE(pWindow->win32.utf16HighSurrogate) != 0);
                        character = dtk_utf16pair_to_utf32_ch(pWindow->win32.utf16HighSurrogate, (unsigned short)wParam);
                    }

                    pWindow->win32.utf16HighSurrogate = 0;

                    int stateFlags = dtk_get_modifier_key_state_flags__win32();
                    if ((lParam & (1 << 30)) != 0) {
                        stateFlags |= DTK_KEY_AUTO_REPEATED;
                    }

                    e.type = DTK_EVENT_PRINTABLE_KEY_DOWN;
                    e.printableKeyDown.utf32 = character;
                    e.printableKeyDown.state = stateFlags;

                    int repeatCount = lParam & 0x0000FFFF;
                    for (int i = 0; i < repeatCount; ++i) {
                        dtk__handle_event(&e);
                    }
                }
            }
        } break;


        case WM_SETFOCUS:
        {
            e.type = DTK_EVENT_FOCUS;
            dtk__handle_event(&e);
        } break;

        case WM_KILLFOCUS:
        {
            e.type = DTK_EVENT_UNFOCUS;
            dtk__handle_event(&e);
        } break;


        case WM_COMMAND:
        {
            if (HIWORD(wParam) == 1) {
                WORD acceleratorIndex = LOWORD(wParam);
                e.type = DTK_EVENT_ACCELERATOR;
                e.accelerator.key       = e.pTK->win32.pAccelerators[acceleratorIndex].key;
                e.accelerator.modifiers = e.pTK->win32.pAccelerators[acceleratorIndex].modifiers;
                e.accelerator.id        = e.pTK->win32.pAccelerators[acceleratorIndex].id;
                dtk__handle_event(&e);
            }
        } break;

        case WM_MENUCOMMAND:
        {
            MENUINFO mi;
            dtk_zero_object(&mi);
            mi.cbSize = sizeof(mi);
            mi.fMask = MIM_MENUDATA;
            if (GetMenuInfo((HMENU)lParam, &mi)) {
                dtk_menu* pMenu = (dtk_menu*)mi.dwMenuData;
                dtk_assert(pMenu != NULL);

                e.type = DTK_EVENT_MENU;
                e.menu.pMenu = pMenu;
                e.menu.itemIndex = (dtk_uint32)wParam;
                dtk_menu_get_item_id(pMenu, e.menu.itemIndex, &e.menu.itemID);
                dtk__handle_event(&e);
            }
        } break;

        case WM_SETCURSOR:
        {
            if (LOWORD(lParam) == HTCLIENT) {
                SetCursor((HCURSOR)pWindow->win32.hCursor);
                return TRUE;
            }
        } break;
        
        default: break;
    }

    return DefWindowProcA(hWnd, msg, wParam, lParam);
}

dtk_result dtk_window_init__win32(dtk_context* pTK, dtk_control* pParent, dtk_window_type type, const char* title, dtk_uint32 width, dtk_uint32 height, dtk_window* pWindow)
{
    (void)pTK;

    DWORD dwStyleEx = 0;
    DWORD dwStyle = WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
    if (type == dtk_window_type_toplevel) {
        dwStyle   |= WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_OVERLAPPEDWINDOW;
    }
    if (type == dtk_window_type_dialog) {
        dwStyleEx |= WS_EX_DLGMODALFRAME;
        dwStyle   |= WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
    }
    if (type == dtk_window_type_popup) {
        dwStyle   |= WS_POPUP;
    }

    const char* pClassName = DTK_WIN32_WINDOW_CLASS;
    if (type == dtk_window_type_popup) {
        pClassName = DTK_WIN32_WINDOW_CLASS_POPUP;
    }

    HWND hWnd = CreateWindowExA(dwStyleEx, DTK_WIN32_WINDOW_CLASS, title, dwStyle, CW_USEDEFAULT, CW_USEDEFAULT, width, height, (pParent != NULL) ? (HWND)DTK_WINDOW(pParent)->win32.hWnd : NULL, NULL, NULL, NULL);
    if (hWnd == NULL) {
        return DTK_ERROR;
    }

    pWindow->win32.hWnd = (dtk_handle)hWnd;
    pWindow->win32.hCursor = pTK->win32.hCursorArrow;

    // The dtk window needs to be linked to the Win32 window handle so it can be accessed from the event handler.
    SetWindowLongPtrA(hWnd, 0, (LONG_PTR)pWindow);

    return DTK_SUCCESS;
}

dtk_result dtk_window_uninit__win32(dtk_window* pWindow)
{
    if (!DestroyWindow((HWND)pWindow->win32.hWnd)) {
        return DTK_ERROR;
    }

    return DTK_SUCCESS;
}

dtk_result dtk_window_set_title__win32(dtk_window* pWindow, const char* title)
{
    if (!SetWindowTextA((HWND)pWindow->win32.hWnd, title)) {
        return DTK_ERROR;
    }

    return DTK_SUCCESS;
}

dtk_result dtk_window_set_size__win32(dtk_window* pWindow, dtk_uint32 width, dtk_uint32 height)
{
    RECT windowRect;
    RECT clientRect;
    if (!GetWindowRect((HWND)pWindow->win32.hWnd, &windowRect)) return DTK_ERROR;
    if (!GetClientRect((HWND)pWindow->win32.hWnd, &clientRect)) return DTK_ERROR;

    int windowFrameX = (windowRect.right - windowRect.left) - (clientRect.right - clientRect.left);
    int windowFrameY = (windowRect.bottom - windowRect.top) - (clientRect.bottom - clientRect.top);

    assert(windowFrameX >= 0);
    assert(windowFrameY >= 0);

    int scaledWidth  = width  + windowFrameX;
    int scaledHeight = height + windowFrameY;
    if (!SetWindowPos((HWND)pWindow->win32.hWnd, NULL, 0, 0, scaledWidth, scaledHeight, SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE)) {
        return DTK_ERROR;
    }

    return DTK_SUCCESS;
}

dtk_result dtk_window_get_size__win32(dtk_window* pWindow, dtk_uint32* pWidth, dtk_uint32* pHeight)
{
    RECT rect;
    if (!GetClientRect((HWND)pWindow->win32.hWnd, &rect)) {
        return DTK_ERROR;
    }

    if (pWidth  != NULL) *pWidth  = rect.right - rect.left;
    if (pHeight != NULL) *pHeight = rect.bottom - rect.top;
    return DTK_SUCCESS;
}

dtk_result dtk_window_get_client_size__win32(dtk_window* pWindow, dtk_uint32* pWidth, dtk_uint32* pHeight)
{
    return dtk_window_get_size__win32(pWindow, pWidth, pHeight);
}

dtk_result dtk_window_set_absolute_position__win32(dtk_window* pWindow, dtk_int32 posX, dtk_int32 posY)
{
    // The absolute position of window's is relative to the screen.
    dtk_int32 screenPosX = posX;
    dtk_int32 screenPosY = posY;
    if (!SetWindowPos((HWND)pWindow->win32.hWnd, NULL, screenPosX, screenPosY, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE)) {
        return DTK_ERROR;
    }

    DTK_CONTROL(pWindow)->absolutePosX = screenPosX;
    DTK_CONTROL(pWindow)->absolutePosY = screenPosY;

    return DTK_SUCCESS;
}

dtk_result dtk_window_get_absolute_position__win32(dtk_window* pWindow, dtk_int32* pPosX, dtk_int32* pPosY)
{
    RECT rect;
    if (!GetWindowRect((HWND)pWindow->win32.hWnd, &rect)) {
        return DTK_ERROR;
    }

    if (pPosX) *pPosX = rect.left;
    if (pPosY) *pPosY = rect.top;
    return DTK_SUCCESS;
}

dtk_result dtk_window_move_to_center_of_screen__win32(dtk_window* pWindow)
{
    MONITORINFO mi;
    ZeroMemory(&mi, sizeof(mi));
    mi.cbSize = sizeof(MONITORINFO);
    if (!GetMonitorInfoA(MonitorFromWindow((HWND)pWindow->win32.hWnd, 0), &mi)) {
        return DTK_ERROR;
    }

    LONG screenSizeX = mi.rcMonitor.right - mi.rcMonitor.left;
    LONG screenSizeY = mi.rcMonitor.bottom - mi.rcMonitor.top;

    dtk_uint32 windowSizeX;
    dtk_uint32 windowSizeY;
    dtk_window_get_size(pWindow, &windowSizeX, &windowSizeY);

    return dtk_window_set_absolute_position(pWindow, (screenSizeX - windowSizeX)/2, (screenSizeY - windowSizeY)/2);
}


dtk_result dtk_window_show__win32(dtk_window* pWindow, int mode)
{
    int nCmdShow[] = {
        SW_HIDE,            // DTK_HIDE
        SW_SHOWNORMAL,      // DTK_SHOW_NORMAL
        SW_SHOWMAXIMIZED,   // DTK_SHOW_MAXIMIZED
    };

    if (!ShowWindow((HWND)pWindow->win32.hWnd, nCmdShow[mode])) {
        return DTK_ERROR;
    }

    return DTK_SUCCESS;
}

dtk_result dtk_window_bring_to_top__win32(dtk_window* pWindow)
{
    if (IsZoomed(pWindow->win32.hWnd)) {
        ShowWindow(pWindow->win32.hWnd, SW_SHOWMAXIMIZED);
    } else if (IsIconic(pWindow->win32.hWnd)) {
        ShowWindow(pWindow->win32.hWnd, SW_RESTORE);
    }

    SetForegroundWindow(pWindow->win32.hWnd);
    return DTK_SUCCESS;
}

dtk_bool32 dtk_window_is_maximized__win32(dtk_window* pWindow)
{
    return IsZoomed(pWindow->win32.hWnd);
}


dtk_result dtk_window_set_cursor__win32(dtk_window* pWindow, dtk_system_cursor_type cursor)
{
    dtk_context* pTK = DTK_CONTROL(pWindow)->pTK;

    HCURSOR hCursor = NULL;
    switch (cursor)
    {
        case dtk_system_cursor_type_text:
        {
            hCursor = pTK->win32.hCursorIBeam;
        } break;

        case dtk_system_cursor_type_cross:
        {
            hCursor = pTK->win32.hCursorCross;
        } break;

        case dtk_system_cursor_type_double_arrow_h:
        {
            hCursor = pTK->win32.hCursorSizeWE;
        } break;

        case dtk_system_cursor_type_double_arrow_v:
        {
            hCursor = pTK->win32.hCursorSizeNS;
        } break;


        case dtk_system_cursor_type_none:
        {
            hCursor = NULL;
        } break;

        //case dtk_systemcursor_type_arrow:
        case dtk_system_cursor_type_default:
        default:
        {
            hCursor = pTK->win32.hCursorArrow;
        } break;
    }

    pWindow->win32.hCursor = (HCURSOR)hCursor;

    // If the cursor is currently inside the window it needs to be changed right now.
    if (dtk_window_is_cursor_over(pWindow)) {
        SetCursor(hCursor);
    }

    return DTK_SUCCESS;
}

dtk_bool32 dtk_window_is_cursor_over__win32(dtk_window* pWindow)
{
    return pWindow->win32.isCursorOverClientArea;
}

dtk_result dtk_window_set_menu__win32(dtk_window* pWindow, dtk_menu* pMenu)
{
    if (!SetMenu((HWND)pWindow->win32.hWnd, (pMenu == NULL) ? NULL : (HMENU)pMenu->win32.hMenu)) {
        return DTK_ERROR;
    }

    return DTK_SUCCESS;
}

dtk_result dtk_window_show_popup_menu__win32(dtk_window* pWindow, dtk_menu* pMenu, int posX, int posY)
{
    POINT screenCoords;
    screenCoords.x = posX;
    screenCoords.y = posY;
    ClientToScreen(pWindow->win32.hWnd, &screenCoords);

    UINT flags = TPM_RIGHTBUTTON | TPM_HORIZONTAL | TPM_VERTICAL;
    int alignment = GetSystemMetrics(SM_MENUDROPALIGNMENT);
    if (alignment == 0) {
        flags |= TPM_RIGHTALIGN;
    }

    if (!TrackPopupMenuEx(pMenu->win32.hMenu, flags, screenCoords.x, screenCoords.y, pWindow->win32.hWnd, NULL)) {
        return DTK_FALSE;
    }

    return DTK_SUCCESS;
}
#endif




///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
// GTK
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#ifdef DTK_GTK
static dtk_key gtk_to_dtk_key(guint keyval)
{
    switch (keyval)
    {
    case GDK_KEY_BackSpace: return DTK_KEY_BACKSPACE;
    case GDK_KEY_Shift_L:   return DTK_KEY_SHIFT;
    case GDK_KEY_Shift_R:   return DTK_KEY_SHIFT;
    case GDK_KEY_Escape:    return DTK_KEY_ESCAPE;
    case GDK_KEY_Page_Up:   return DTK_KEY_PAGE_UP;
    case GDK_KEY_Page_Down: return DTK_KEY_PAGE_DOWN;
    case GDK_KEY_End:       return DTK_KEY_END;
    case GDK_KEY_Home:      return DTK_KEY_HOME;
    case GDK_KEY_Left:      return DTK_KEY_ARROW_LEFT;
    case GDK_KEY_Up:        return DTK_KEY_ARROW_UP;
    case GDK_KEY_Right:     return DTK_KEY_ARROW_RIGHT;
    case GDK_KEY_Down:      return DTK_KEY_ARROW_DOWN;
    case GDK_KEY_Delete:    return DTK_KEY_DELETE;
    case GDK_KEY_F1:        return DTK_KEY_F1;
    case GDK_KEY_F2:        return DTK_KEY_F2;
    case GDK_KEY_F3:        return DTK_KEY_F3;
    case GDK_KEY_F4:        return DTK_KEY_F4;
    case GDK_KEY_F5:        return DTK_KEY_F5;
    case GDK_KEY_F6:        return DTK_KEY_F6;
    case GDK_KEY_F7:        return DTK_KEY_F7;
    case GDK_KEY_F8:        return DTK_KEY_F8;
    case GDK_KEY_F9:        return DTK_KEY_F9;
    case GDK_KEY_F10:       return DTK_KEY_F10;
    case GDK_KEY_F11:       return DTK_KEY_F11;
    case GDK_KEY_F12:       return DTK_KEY_F12;

    default: break;
    }

    if (keyval == GDK_KEY_Tab) {
        return '\t';
    }

    return (dtk_key)keyval;
}

guint dtk_to_gtk_key(dtk_key key)
{
    switch (key)
    {
    case DTK_KEY_BACKSPACE:   return GDK_KEY_BackSpace;
    case DTK_KEY_SHIFT:       return GDK_KEY_Shift_L;
    //case DTK_KEY_SHIFT:       return GDK_KEY_Shift_R;
    case DTK_KEY_ESCAPE:      return GDK_KEY_Escape;
    case DTK_KEY_PAGE_UP:     return GDK_KEY_Page_Up;
    case DTK_KEY_PAGE_DOWN:   return GDK_KEY_Page_Down;
    case DTK_KEY_END:         return GDK_KEY_End;
    case DTK_KEY_HOME:        return GDK_KEY_Begin;
    case DTK_KEY_ARROW_LEFT:  return GDK_KEY_Left;
    case DTK_KEY_ARROW_UP:    return GDK_KEY_Up;
    case DTK_KEY_ARROW_RIGHT: return GDK_KEY_Right;
    case DTK_KEY_ARROW_DOWN:  return GDK_KEY_Down;
    case DTK_KEY_DELETE:      return GDK_KEY_Delete;
    case DTK_KEY_F1:          return GDK_KEY_F1;
    case DTK_KEY_F2:          return GDK_KEY_F2;
    case DTK_KEY_F3:          return GDK_KEY_F3;
    case DTK_KEY_F4:          return GDK_KEY_F4;
    case DTK_KEY_F5:          return GDK_KEY_F5;
    case DTK_KEY_F6:          return GDK_KEY_F6;
    case DTK_KEY_F7:          return GDK_KEY_F7;
    case DTK_KEY_F8:          return GDK_KEY_F8;
    case DTK_KEY_F9:          return GDK_KEY_F9;
    case DTK_KEY_F10:         return GDK_KEY_F10;
    case DTK_KEY_F11:         return GDK_KEY_F11;
    case DTK_KEY_F12:         return GDK_KEY_F12;

    default: break;
    }

    if (key == '\t') {
        return GDK_KEY_Tab;
    }

    return (guint)key;
}

static int dtk_get_modifier_state_flags__gtk(guint stateFromGTK)
{
    int result = 0;

    if ((stateFromGTK & GDK_SHIFT_MASK) != 0) {
        result |= DTK_KEY_SHIFT_DOWN;
    }
    if ((stateFromGTK & GDK_CONTROL_MASK) != 0) {
        result |= DTK_KEY_CTRL_DOWN;
    }
    if ((stateFromGTK & GDK_MOD1_MASK) != 0) {
        result |= DTK_KEY_ALT_DOWN;
    }

    if ((stateFromGTK & GDK_BUTTON1_MASK) != 0) {
        result |= DTK_MOUSE_BUTTON_LEFT_DOWN;
    }
    if ((stateFromGTK & GDK_BUTTON2_MASK) != 0) {
        result |= DTK_MOUSE_BUTTON_MIDDLE_DOWN;
    }
    if ((stateFromGTK & GDK_BUTTON3_MASK) != 0) {
        result |= DTK_MOUSE_BUTTON_RIGHT_DOWN;
    }
    if ((stateFromGTK & GDK_BUTTON4_MASK) != 0) {
        result |= DTK_MOUSE_BUTTON_4_DOWN;
    }
    if ((stateFromGTK & GDK_BUTTON5_MASK) != 0) {
        result |= DTK_MOUSE_BUTTON_5_DOWN;
    }

    return result;
}

GdkModifierType dtk_accelerator_modifiers_to_gtk(dtk_uint32 modifiers)
{
    guint result = 0;
    if (modifiers & DTK_KEY_SHIFT_DOWN) {
        result |= GDK_SHIFT_MASK;
    }
    if (modifiers & DTK_KEY_CTRL_DOWN) {
        result |= GDK_CONTROL_MASK;
    }
    if (modifiers & DTK_KEY_ALT_DOWN) {
        result |= GDK_MOD1_MASK;
    }

    return result;
}

dtk_uint32 dtk_accelerator_modifiers_from_gtk(GdkModifierType modifiers)
{
    dtk_uint32 result = 0;
    if (modifiers & GDK_SHIFT_MASK) {
        result |= DTK_KEY_SHIFT_DOWN;
    }
    if (modifiers & GDK_CONTROL_MASK) {
        result |= DTK_KEY_CTRL_DOWN;
    }
    if (modifiers & GDK_MOD1_MASK) {
        result |= DTK_KEY_ALT_DOWN;
    }

    return result;
}

static int dtk_from_gtk_mouse_button(guint buttonGTK)
{
    switch (buttonGTK) {
        case 1: return DTK_MOUSE_BUTTON_LEFT;
        case 2: return DTK_MOUSE_BUTTON_MIDDLE;
        case 3: return DTK_MOUSE_BUTTON_RIGHT;
        default: return (int)buttonGTK;
    }
}


static gboolean dtk_window__on_close__gtk(GtkWidget* pWidget, GdkEvent* pEvent, gpointer pUserData)
{
    (void)pWidget;
    (void)pEvent;

    dtk_window* pWindow = (dtk_window*)pUserData;
    if (pWindow == NULL) return DTK_TRUE;
    
    dtk_event e = dtk_event_init(DTK_EVENT_CLOSE, DTK_CONTROL(pWindow));
    dtk__handle_event(&e);
    
    return DTK_TRUE;
}

static gboolean dtk_window__on_configure__gtk(GtkWidget* pWidget, GdkEventConfigure* pEvent, gpointer pUserData)
{
    dtk_window* pWindow = (dtk_window*)pUserData;
    if (pWindow == NULL) {
        return DTK_FALSE;
    }

    if (pEvent->x != DTK_CONTROL(pWindow)->absolutePosX || pEvent->y != DTK_CONTROL(pWindow)->absolutePosY) {    
        // Position has changed.
        dtk_event e = dtk_event_init(DTK_EVENT_MOVE, DTK_CONTROL(pWindow));
        e.move.x = pEvent->x;
        e.move.y = pEvent->y;
        dtk__handle_event(&e);
    }

    return DTK_FALSE;
}

static void dtk_window__on_hide__gtk(GtkWidget* pWidget, gpointer pUserData)
{
    (void)pWidget;

    dtk_window* pWindow = (dtk_window*)pUserData;
    if (pWindow == NULL) {
        return;
    }

    dtk_event e = dtk_event_init(DTK_EVENT_HIDE, DTK_CONTROL(pWindow));
    dtk__handle_event(&e);
}

static void dtk_window__on_show__gtk(GtkWidget* pWidget, gpointer pUserData)
{
    (void)pWidget;

    dtk_window* pWindow = (dtk_window*)pUserData;
    if (pWindow == NULL) {
        return;
    }

    dtk_event e = dtk_event_init(DTK_EVENT_SHOW, DTK_CONTROL(pWindow));
    dtk__handle_event(&e);

    gtk_widget_grab_focus(GTK_WIDGET(pWidget)); // <-- Is this needed?
}

static gboolean dtk_window__on_key_down__gtk(GtkWidget* pWidget, GdkEventKey* pEvent, gpointer pUserData)
{
    (void)pWidget;

    dtk_window* pWindow = (dtk_window*)pUserData;
    if (pWindow == NULL) {
        return DTK_TRUE;
    }

    dtk_context* pTK = DTK_CONTROL(pWindow)->pTK;

    int stateFlags = dtk_get_modifier_state_flags__gtk(pEvent->state);
    // TODO: Check here if key is auto-repeated.

    // If the key is a tab key and there are modifiers we will need to simulate an accelerator because GTK doesn't let
    // us bind the tab key to an accelerator... sigh...
    if ((pEvent->keyval == GDK_KEY_Tab || pEvent->keyval == GDK_KEY_ISO_Left_Tab) && stateFlags != 0) {
        for (size_t i = 0; i < pTK->gtk.acceleratorCount; ++i) {
            dtk_accelerator_gtk* pAccel = &pTK->gtk.pAccelerators[i];
            if (pAccel->accelerator.key == '\t' && (int)pAccel->accelerator.modifiers == stateFlags) {
                dtk_event e;
                e.type = DTK_EVENT_ACCELERATOR;
                e.pTK = pTK;
                e.pControl = DTK_CONTROL(pWindow);
                e.accelerator.key = pTK->gtk.pAccelerators[i].accelerator.key;
                e.accelerator.modifiers = pTK->gtk.pAccelerators[i].accelerator.modifiers;
                e.accelerator.id = pTK->gtk.pAccelerators[i].accelerator.id;
                dtk__handle_event(&e);
                return DTK_FALSE;
            }
        }
    }

    dtk_event e = dtk_event_init(DTK_EVENT_KEY_DOWN, DTK_CONTROL(pWindow));
    e.keyDown.key = gtk_to_dtk_key(pEvent->keyval);
    e.keyDown.state = stateFlags;
    dtk__handle_event(&e);


    // Printable keys. These are posted as UTF-32 code points.
    guint32 utf32 = gdk_keyval_to_unicode(pEvent->keyval);
    if (utf32 == 0) {
        if (pEvent->keyval == GDK_KEY_KP_Enter) {
            utf32 = '\r';
        }
    }

    if (utf32 != 0 && (stateFlags & DTK_KEY_CTRL_DOWN) == 0 && (stateFlags & DTK_KEY_ALT_DOWN) == 0) {
        if (!(utf32 < 32 || utf32 == 127) || utf32 == '\t' || utf32 == '\r') {
            e.type = DTK_EVENT_PRINTABLE_KEY_DOWN;
            e.printableKeyDown.utf32 = utf32;
            e.printableKeyDown.state = stateFlags;
            dtk__handle_event(&e);
        }
    }

    return DTK_FALSE;
}

static gboolean dtk_window__on_key_up__gtk(GtkWidget* pWidget, GdkEventKey* pEvent, gpointer pUserData)
{
    (void)pWidget;

    dtk_window* pWindow = (dtk_window*)pUserData;
    if (pWindow == NULL) {
        return DTK_TRUE;
    }

    dtk_event e = dtk_event_init(DTK_EVENT_KEY_UP, DTK_CONTROL(pWindow));
    e.keyDown.key = gtk_to_dtk_key(pEvent->keyval);
    e.keyDown.state = dtk_get_modifier_state_flags__gtk(pEvent->state);
    dtk__handle_event(&e);

    return DTK_FALSE;
}

static gboolean dtk_window__on_receive_focus__gtk(GtkWidget* pWidget, GdkEventFocus* pEvent, gpointer pUserData)
{
    (void)pWidget;
    (void)pEvent;

    dtk_window* pWindow = (dtk_window*)pUserData;
    if (pWindow == NULL) {
        return DTK_TRUE;
    }

    dtk_event e = dtk_event_init(DTK_EVENT_FOCUS, DTK_CONTROL(pWindow));
    dtk__handle_event(&e);

    return DTK_FALSE;
}

static gboolean dtk_window__on_lose_focus__gtk(GtkWidget* pWidget, GdkEventFocus* pEvent, gpointer pUserData)
{
    (void)pWidget;
    (void)pEvent;

    dtk_window* pWindow = (dtk_window*)pUserData;
    if (pWindow == NULL) {
        return DTK_TRUE;
    }

    dtk_event e = dtk_event_init(DTK_EVENT_UNFOCUS, DTK_CONTROL(pWindow));
    dtk__handle_event(&e);

    return DTK_FALSE;
}



static gboolean dtk_window_clientarea__on_draw__gtk(GtkWidget* pClientArea, cairo_t* cr, gpointer pUserData)
{
    dtk_window* pWindow = (dtk_window*)pUserData;
    if (pWindow == NULL || DTK_CONTROL(pWindow)->pSurface == NULL) return DTK_FALSE;

    double clipLeft;
    double clipTop;
    double clipRight;
    double clipBottom;
    cairo_clip_extents(DTK_CONTROL(pWindow)->pSurface->cairo.pContext, &clipLeft, &clipTop, &clipRight, &clipBottom);

    dtk_event e = dtk_event_init(DTK_EVENT_PAINT, DTK_CONTROL(pWindow));
    e.paint.rect.left = clipLeft;
    e.paint.rect.top = clipTop;
    e.paint.rect.right = clipRight;
    e.paint.rect.bottom = clipBottom;
    dtk__handle_event(&e);
    
    cairo_set_source_surface(cr, DTK_CONTROL(pWindow)->pSurface->cairo.pSurface, 0, 0);
    cairo_paint(cr);

    return DTK_FALSE;
}

static gboolean dtk_window_clientarea__on_configure__gtk(GtkWidget* pClientArea, GdkEventConfigure* pEvent, gpointer pUserData)
{
    dtk_window* pWindow = (dtk_window*)pUserData;
    if (pWindow == NULL) {
        return DTK_FALSE;
    }

    // If the window's size has changed, it's panel and surface need to be resized, and then redrawn.
    if (pEvent->width != DTK_CONTROL(pWindow)->width || pEvent->height != DTK_CONTROL(pWindow)->height) {
        // Size has changed.
        dtk_event e = dtk_event_init(DTK_EVENT_SIZE, DTK_CONTROL(pWindow));
        e.size.width = pEvent->width;
        e.size.height = pEvent->height;
        dtk__handle_event(&e);

        // Invalidate the window to force a redraw.
        gtk_widget_queue_draw(pClientArea);
    }

    return DTK_FALSE;
}

static gboolean dtk_window_clientarea__on_mouse_enter__gtk(GtkWidget* pClientArea, GdkEventCrossing* pEvent, gpointer pUserData)
{
    (void)pClientArea;
    (void)pEvent;

    dtk_window* pWindow = (dtk_window*)pUserData;
    if (pWindow == NULL) {
        return DTK_TRUE;
    }

    pWindow->gtk.isCursorOverClientArea = DTK_TRUE;
    gdk_window_set_cursor(gtk_widget_get_window(GTK_WIDGET(pWindow->gtk.pWidget)), pWindow->gtk.pCursor);

    dtk_event e = dtk_event_init(DTK_EVENT_MOUSE_ENTER, DTK_CONTROL(pWindow));
    dtk__handle_event(&e);

    return DTK_FALSE;
}

static gboolean dtk_window_clientarea__on_mouse_leave__gtk(GtkWidget* pClientArea, GdkEventCrossing* pEvent, gpointer pUserData)
{
    (void)pClientArea;
    (void)pEvent;

    dtk_window* pWindow = (dtk_window*)pUserData;
    if (pWindow == NULL) {
        return DTK_TRUE;
    }

    pWindow->gtk.isCursorOverClientArea = DTK_FALSE;

    dtk_event e = dtk_event_init(DTK_EVENT_MOUSE_LEAVE, DTK_CONTROL(pWindow));
    dtk__handle_event(&e);

    return DTK_TRUE;
}

static gboolean dtk_window_clientarea__on_mouse_move__gtk(GtkWidget* pClientArea, GdkEventMotion* pEvent, gpointer pUserData)
{
    (void)pClientArea;

    dtk_window* pWindow = (dtk_window*)pUserData;
    if (pWindow == NULL) {
        return DTK_TRUE;
    }

    dtk_event e = dtk_event_init(DTK_EVENT_MOUSE_MOVE, DTK_CONTROL(pWindow));
    e.mouseMove.x = pEvent->x;
    e.mouseMove.y = pEvent->y;
    e.mouseMove.state = dtk_get_modifier_state_flags__gtk(pEvent->state);
    dtk__handle_event(&e);

    return DTK_FALSE;
}

static gboolean dtk_window_clientarea__on_mouse_button_down__gtk(GtkWidget* pClientArea, GdkEventButton* pEvent, gpointer pUserData)
{
    (void)pClientArea;

    dtk_window* pWindow = (dtk_window*)pUserData;
    if (pWindow == NULL) {
        return DTK_TRUE;
    }

    dtk_event e = dtk_event_init(DTK_EVENT_NONE, DTK_CONTROL(pWindow));
    if (pEvent->type == GDK_BUTTON_PRESS) {
        e.type = DTK_EVENT_MOUSE_BUTTON_DOWN;
    } else if (pEvent->type == GDK_2BUTTON_PRESS) {
        e.type = DTK_EVENT_MOUSE_BUTTON_DBLCLICK;
    }

    e.mouseButton.x = pEvent->x;
    e.mouseButton.y = pEvent->y;
    e.mouseButton.button = dtk_from_gtk_mouse_button(pEvent->button);
    e.mouseButton.state = dtk_get_modifier_state_flags__gtk(pEvent->state);
    dtk__handle_event(&e);

    return DTK_TRUE;
}

static gboolean dtk_window_clientarea__on_mouse_button_up__gtk(GtkWidget* pClientArea, GdkEventButton* pEvent, gpointer pUserData)
{
    (void)pClientArea;

    dtk_window* pWindow = (dtk_window*)pUserData;
    if (pWindow == NULL) {
        return DTK_TRUE;
    }

    dtk_event e = dtk_event_init(DTK_EVENT_MOUSE_BUTTON_UP, DTK_CONTROL(pWindow));
    e.mouseButton.x = pEvent->x;
    e.mouseButton.y = pEvent->y;
    e.mouseButton.button = dtk_from_gtk_mouse_button(pEvent->button);
    e.mouseButton.state = dtk_get_modifier_state_flags__gtk(pEvent->state);
    dtk__handle_event(&e);

    return DTK_TRUE;
}

static gboolean dtk_window_clientarea__on_mouse_wheel__gtk(GtkWidget* pClientArea, GdkEventScroll* pEvent, gpointer pUserData)
{
    (void)pClientArea;

    dtk_window* pWindow = (dtk_window*)pUserData;
    if (pWindow == NULL) {
        return DTK_TRUE;
    }

    gdouble delta_y = 0;
    if (pEvent->direction == GDK_SCROLL_UP) {
        delta_y = 1;
    } else if (pEvent->direction == GDK_SCROLL_DOWN) {
        delta_y = -1;
    }

    dtk_event e = dtk_event_init(DTK_EVENT_MOUSE_WHEEL, DTK_CONTROL(pWindow));
    e.mouseWheel.x = pEvent->x;
    e.mouseWheel.y = pEvent->y;
    e.mouseWheel.delta = (dtk_int32)delta_y;
    e.mouseWheel.state = dtk_get_modifier_state_flags__gtk(pEvent->state);
    dtk__handle_event(&e);

    return DTK_TRUE;
}


dtk_result dtk_window_init__gtk(dtk_context* pTK, dtk_control* pParent, dtk_window_type type, const char* title, dtk_uint32 width, dtk_uint32 height, dtk_window* pWindow)
{
    // Client area. This is where everything is drawn.
    GtkWidget* pClientArea = gtk_drawing_area_new();
    if (pClientArea == NULL) {
        return DTK_ERROR;
    }
    
    gtk_widget_add_events(pClientArea,
        GDK_ENTER_NOTIFY_MASK   |
        GDK_LEAVE_NOTIFY_MASK   |
        GDK_POINTER_MOTION_MASK |
        GDK_BUTTON_PRESS_MASK   |
        GDK_BUTTON_RELEASE_MASK |
        GDK_SCROLL_MASK);
        
    g_signal_connect(pClientArea, "draw",                 G_CALLBACK(dtk_window_clientarea__on_draw__gtk),              pWindow);
    g_signal_connect(pClientArea, "configure-event",      G_CALLBACK(dtk_window_clientarea__on_configure__gtk),         pWindow);
    g_signal_connect(pClientArea, "enter-notify-event",   G_CALLBACK(dtk_window_clientarea__on_mouse_enter__gtk),       pWindow);     // Mouse enter.
    g_signal_connect(pClientArea, "leave-notify-event",   G_CALLBACK(dtk_window_clientarea__on_mouse_leave__gtk),       pWindow);     // Mouse leave.
    g_signal_connect(pClientArea, "motion-notify-event",  G_CALLBACK(dtk_window_clientarea__on_mouse_move__gtk),        pWindow);     // Mouse move.
    g_signal_connect(pClientArea, "button-press-event",   G_CALLBACK(dtk_window_clientarea__on_mouse_button_down__gtk), pWindow);     // Mouse button down.
    g_signal_connect(pClientArea, "button-release-event", G_CALLBACK(dtk_window_clientarea__on_mouse_button_up__gtk),   pWindow);     // Mouse button up.
    g_signal_connect(pClientArea, "scroll-event",         G_CALLBACK(dtk_window_clientarea__on_mouse_wheel__gtk),       pWindow);     // Mouse wheel.
    
    // Box container. This is used to laying out the menu bar and client area.
    GtkWidget* pBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    if (pBox == NULL) {
        gtk_widget_destroy(pClientArea);
        return DTK_ERROR;
    }
    
    gtk_box_pack_start(GTK_BOX(pBox), pClientArea, TRUE, TRUE, 0);
    
    
    // Main window.
    GtkWidget* pWidget = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    if (pWidget == NULL) {
        gtk_widget_destroy(pBox);
        gtk_widget_destroy(pClientArea);
        return DTK_ERROR;
    }
    
    gtk_widget_add_events(pWidget,
        GDK_KEY_PRESS_MASK      |
        GDK_KEY_RELEASE_MASK    |
        GDK_FOCUS_CHANGE_MASK);


    
    gtk_window_set_title(GTK_WINDOW(pWidget), title);
    if (type == dtk_window_type_toplevel) {
        gtk_window_set_resizable(GTK_WINDOW(pWidget), TRUE);
        gtk_window_resize(GTK_WINDOW(pWidget), (gint)width, (gint)height);
    } else if (type == dtk_window_type_dialog) {
        gtk_window_set_resizable(GTK_WINDOW(pWidget), FALSE);
        gtk_widget_set_size_request(GTK_WIDGET(pWidget), (gint)width, (gint)height);
        gtk_window_set_type_hint(GTK_WINDOW(pWidget), GDK_WINDOW_TYPE_HINT_DIALOG);
        gtk_window_set_skip_taskbar_hint(GTK_WINDOW(pWidget), TRUE);
    }
    
    g_signal_connect(pWidget, "delete-event",      G_CALLBACK(dtk_window__on_close__gtk),         pWindow);     // Close
    g_signal_connect(pWidget, "configure-event",   G_CALLBACK(dtk_window__on_configure__gtk),     pWindow);     // Size/Move
    g_signal_connect(pWidget, "hide",              G_CALLBACK(dtk_window__on_hide__gtk),          pWindow);     // Hide.
    g_signal_connect(pWidget, "show",              G_CALLBACK(dtk_window__on_show__gtk),          pWindow);     // Show.
    g_signal_connect(pWidget, "key-press-event",   G_CALLBACK(dtk_window__on_key_down__gtk),      pWindow);     // Key down.
    g_signal_connect(pWidget, "key-release-event", G_CALLBACK(dtk_window__on_key_up__gtk),        pWindow);     // Key up.
    g_signal_connect(pWidget, "focus-in-event",    G_CALLBACK(dtk_window__on_receive_focus__gtk), pWindow);     // Receive focus.
    g_signal_connect(pWidget, "focus-out-event",   G_CALLBACK(dtk_window__on_lose_focus__gtk),    pWindow);     // Lose focus.
    
    gtk_container_add(GTK_CONTAINER(pWidget), pBox);

    pWindow->gtk.pWidget     = pWidget;
    pWindow->gtk.pBox        = pBox;
    pWindow->gtk.pClientArea = pClientArea;
    
    gtk_widget_show_all(GTK_WIDGET(pBox));
    gtk_widget_realize(pWidget);

    // Bind the accelerator group to every window for now.
    gtk_window_add_accel_group(GTK_WINDOW(pWidget), pTK->gtk.pAccelGroup);

    return DTK_SUCCESS;
}

dtk_result dtk_window_uninit__gtk(dtk_window* pWindow)
{
    gtk_widget_destroy(GTK_WIDGET(pWindow->gtk.pClientArea));
    gtk_widget_destroy(GTK_WIDGET(pWindow->gtk.pBox));
    gtk_widget_destroy(GTK_WIDGET(pWindow->gtk.pWidget));
    return DTK_SUCCESS;
}

dtk_result dtk_window_set_title__gtk(dtk_window* pWindow, const char* title)
{
    gtk_window_set_title(GTK_WINDOW(pWindow->gtk.pWidget), title);
    return DTK_SUCCESS;
}

dtk_result dtk_window_set_size__gtk(dtk_window* pWindow, dtk_uint32 width, dtk_uint32 height)
{
    if (pWindow->gtk.pMenu != NULL) {
        GtkAllocation alloc;
        gtk_widget_get_allocation(pWindow->gtk.pMenu->gtk.pWidget, &alloc);

        height += alloc.height;
    }

    gtk_window_resize(GTK_WINDOW(pWindow->gtk.pWidget), (int)width, (int)height);
    return DTK_SUCCESS;
}

dtk_result dtk_window_get_size__gtk(dtk_window* pWindow, dtk_uint32* pWidth, dtk_uint32* pHeight)
{
    gint width;
    gint height;
    gtk_window_get_size(GTK_WINDOW(pWindow->gtk.pWidget), &width, &height);

    if (pWidth) *pWidth = width;
    if (pHeight) *pHeight = height;
    return DTK_SUCCESS;
}

dtk_result dtk_window_get_client_size__gtk(dtk_window* pWindow, dtk_uint32* pWidth, dtk_uint32* pHeight)
{
    GtkAllocation alloc;
    gtk_widget_get_allocation(pWindow->gtk.pClientArea, &alloc);

    if (pWidth) *pWidth = alloc.width;
    if (pHeight) *pHeight = alloc.height;
    return DTK_SUCCESS;
}

dtk_result dtk_window_set_absolute_position__gtk(dtk_window* pWindow, dtk_int32 posX, dtk_int32 posY)
{
    gtk_window_move(GTK_WINDOW(pWindow->gtk.pWidget), (gint)posX, (gint)posY);
    return DTK_SUCCESS;
}

dtk_result dtk_window_get_absolute_position__gtk(dtk_window* pWindow, dtk_int32* pPosX, dtk_int32* pPosY)
{
    gint posX = 0;
    gint posY = 0;
    gtk_window_get_position(GTK_WINDOW(pWindow->gtk.pWidget), &posX, &posY);

    if (pPosX) *pPosX = (dtk_int32)posX;
    if (pPosY) *pPosY = (dtk_int32)posY;
    return DTK_SUCCESS;
}

dtk_result dtk_window_move_to_center_of_screen__gtk(dtk_window* pWindow)
{
    gtk_window_set_position(GTK_WINDOW(pWindow->gtk.pWidget), GTK_WIN_POS_CENTER);
    return DTK_SUCCESS;
}

dtk_result dtk_window_show__gtk(dtk_window* pWindow, int mode)
{
    if (mode == DTK_HIDE) {
        gtk_widget_hide(GTK_WIDGET(pWindow->gtk.pWidget));
    } else if (mode == DTK_SHOW_NORMAL) {
        gtk_window_present(GTK_WINDOW(pWindow->gtk.pWidget));
    } else if (mode == DTK_SHOW_MAXIMIZED) {
        gtk_window_present(GTK_WINDOW(pWindow->gtk.pWidget));
        gtk_window_maximize(GTK_WINDOW(pWindow->gtk.pWidget));
    } else {
        return DTK_INVALID_ARGS;
    }
    
    return DTK_SUCCESS;
}

dtk_result dtk_window_bring_to_top__gtk(dtk_window* pWindow)
{
    gtk_window_present(GTK_WINDOW(pWindow->gtk.pWidget));
    gdk_flush();    // <-- Is this needed?

    return DTK_SUCCESS;
}

dtk_bool32 dtk_window_is_maximized__gtk(dtk_window* pWindow)
{
    return gtk_window_is_maximized(GTK_WINDOW(pWindow->gtk.pWidget));
}

dtk_result dtk_window_set_cursor__gtk(dtk_window* pWindow, dtk_system_cursor_type cursor)
{
    dtk_context* pTK = DTK_CONTROL(pWindow)->pTK;

    GdkCursor* pGTKCursor = NULL;
    switch (cursor)
    {
        case dtk_system_cursor_type_text:
        {
            pGTKCursor = pTK->gtk.pCursorIBeam;
        } break;

        case dtk_system_cursor_type_cross:
        {
            pGTKCursor = pTK->gtk.pCursorCross;
        } break;

        case dtk_system_cursor_type_double_arrow_h:
        {
            pGTKCursor = pTK->gtk.pCursorDoubleArrowH;
        } break;

        case dtk_system_cursor_type_double_arrow_v:
        {
            pGTKCursor = pTK->gtk.pCursorDoubleArrowV;
        } break;


        case dtk_system_cursor_type_none:
        {
            pGTKCursor = NULL;
        } break;

        //case dtk_systemcursor_type_arrow:
        case dtk_system_cursor_type_default:
        default:
        {
            pGTKCursor = pTK->gtk.pCursorDefault;
        } break;
    }

    pWindow->gtk.pCursor = pGTKCursor;

    // If the cursor is currently inside the window it needs to be changed right now.
    if (dtk_window_is_cursor_over(pWindow)) {
        gdk_window_set_cursor(gtk_widget_get_window(GTK_WIDGET(pWindow->gtk.pWidget)), pWindow->gtk.pCursor);
    }

    return DTK_SUCCESS;
}

dtk_bool32 dtk_window_is_cursor_over__gtk(dtk_window* pWindow)
{
    return pWindow->gtk.isCursorOverClientArea;
}

dtk_result dtk_window_set_menu__gtk(dtk_window* pWindow, dtk_menu* pMenu)
{
    // The old menu bar needs to be removed.
    if (pWindow->gtk.pMenu != NULL) {
        gtk_container_remove(GTK_CONTAINER(pWindow->gtk.pBox), pWindow->gtk.pMenu->gtk.pWidget);
    }

    // Add the new menu to the top.
    gtk_box_pack_start(GTK_BOX(pWindow->gtk.pBox), pMenu->gtk.pWidget, FALSE, FALSE, 0);
    gtk_box_reorder_child(GTK_BOX(pWindow->gtk.pBox), pMenu->gtk.pWidget, 0);
    gtk_widget_show(pMenu->gtk.pWidget);

    pWindow->gtk.pMenu = pMenu;
    return DTK_SUCCESS;
}


dtk_result dtk_window_show_popup_menu__gtk(dtk_window* pWindow, dtk_menu* pMenu, dtk_int32 posX, dtk_int32 posY)
{
    GtkRequisition size;
    gtk_widget_get_preferred_size(GTK_WIDGET(pMenu->gtk.pWidget), NULL, &size);

    GtkRequisition menubarSize = {0, 0};
    if (pWindow->gtk.pMenu) {
        gtk_widget_get_preferred_size(GTK_WIDGET(pWindow->gtk.pMenu->gtk.pWidget), NULL, &menubarSize);
    }

    GdkRectangle rect;
    rect.x = posX;
    rect.y = posY + menubarSize.height;
    rect.width = size.width;
    rect.height = size.height;
    gtk_menu_popup_at_rect(GTK_MENU(pMenu->gtk.pWidget), gtk_widget_get_window(GTK_WIDGET(pWindow->gtk.pWidget)), &rect, GDK_GRAVITY_NORTH_WEST, GDK_GRAVITY_NORTH_WEST, NULL);

    return DTK_SUCCESS;
}
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

dtk_result dtk_window_init(dtk_context* pTK, dtk_control* pParent, dtk_window_type type, const char* title, dtk_uint32 width, dtk_uint32 height, dtk_event_proc onEvent, dtk_window* pWindow)
{
    if (pWindow == NULL) return DTK_INVALID_ARGS;
    dtk_zero_object(pWindow);

    if (title  == NULL) title = "";
    if (width  == 0) width  = 1;
    if (height == 0) height = 1;
    
    dtk_result result = dtk_control_init(pTK, pParent, DTK_CONTROL_TYPE_WINDOW, onEvent, DTK_CONTROL(pWindow));
    if (result != DTK_SUCCESS) {
        return result;
    }
    
    // Do an upwards traversal until we find the overarching window control.
    dtk_window* pParentWindow = dtk_control_get_window(pParent);
    
#ifdef DTK_WIN32
    if (pTK->platform == dtk_platform_win32) {
        result = dtk_window_init__win32(pTK, DTK_CONTROL(pParentWindow), type, title, width, height, pWindow);
    }
#endif
#ifdef DTK_GTK
    if (pTK->platform == dtk_platform_gtk) {
        result = dtk_window_init__gtk(pTK, DTK_CONTROL(pParentWindow), type, title, width, height, pWindow);
    }
#endif
    if (result != DTK_SUCCESS) {
        return result;
    }

    result = dtk_surface_init_window(pTK, pWindow, &pWindow->surface);
    if (result != DTK_SUCCESS) {
        dtk_window_uninit(pWindow);
        return result;
    }
    DTK_CONTROL(pWindow)->pSurface = &pWindow->surface;

    // Make sure the position attributes of the structure are updated.
    dtk_window_get_absolute_position(pWindow, &DTK_CONTROL(pWindow)->absolutePosX, &DTK_CONTROL(pWindow)->absolutePosY);

    if (type == dtk_window_type_dialog) {
        pWindow->flags |= DTK_WINDOW_FLAG_DIALOG;
    }
    if (type == dtk_window_type_popup) {
        pWindow->flags |= DTK_WINDOW_FLAG_POPUP;
    }

    return DTK_SUCCESS;
}

dtk_result dtk_window_uninit(dtk_window* pWindow)
{
    if (pWindow == NULL) return DTK_INVALID_ARGS;

    // Window controls always own their surfaces.
    if (DTK_CONTROL(pWindow)->pSurface) {
        dtk_surface_uninit(DTK_CONTROL(pWindow)->pSurface);
    }

    dtk_result result = DTK_NO_BACKEND;
#ifdef DTK_WIN32
    if (DTK_CONTROL(pWindow)->pTK->platform == dtk_platform_win32) {
        result = dtk_window_uninit__win32(pWindow);
    }
#endif
#ifdef DTK_GTK
    if (DTK_CONTROL(pWindow)->pTK->platform == dtk_platform_gtk) {
        result = dtk_window_uninit__gtk(pWindow);
    }
#endif
    if (result != DTK_SUCCESS) {
        return result;
    }

    return dtk_control_uninit(DTK_CONTROL(pWindow));
}

dtk_result dtk_window_set_title(dtk_window* pWindow, const char* title)
{
    if (pWindow == NULL) return DTK_INVALID_ARGS;

    if (title == NULL) {
        title = "";
    }

    dtk_result result = DTK_NO_BACKEND;
#ifdef DTK_WIN32
    if (DTK_CONTROL(pWindow)->pTK->platform == dtk_platform_win32) {
        result = dtk_window_set_title__win32(pWindow, title);
    }
#endif
#ifdef DTK_GTK
    if (DTK_CONTROL(pWindow)->pTK->platform == dtk_platform_gtk) {
        result = dtk_window_set_title__gtk(pWindow, title);
    }
#endif

    return result;
}


dtk_result dtk_window_set_size(dtk_window* pWindow, dtk_uint32 width, dtk_uint32 height)
{
    if (pWindow == NULL) return DTK_INVALID_ARGS;
    if (width  == 0) width  = 1;
    if (height == 0) height = 1;

    dtk_result result = DTK_NO_BACKEND;
#ifdef DTK_WIN32
    if (DTK_CONTROL(pWindow)->pTK->platform == dtk_platform_win32) {
        result = dtk_window_set_size__win32(pWindow, width, height);
    }
#endif
#ifdef DTK_GTK
    if (DTK_CONTROL(pWindow)->pTK->platform == dtk_platform_gtk) {
        result = dtk_window_set_size__gtk(pWindow, width, height);
    }
#endif
    if (result != DTK_SUCCESS) {
        return result;
    }

    dtk_window_get_size(pWindow, &DTK_CONTROL(pWindow)->width, &DTK_CONTROL(pWindow)->height);
    return result;
}

dtk_result dtk_window_get_size(dtk_window* pWindow, dtk_uint32* pWidth, dtk_uint32* pHeight)
{
    if (pWidth) *pWidth = 0;
    if (pHeight) *pHeight = 0;
    if (pWindow == NULL) return DTK_INVALID_ARGS;

    dtk_result result = DTK_NO_BACKEND;
#ifdef DTK_WIN32
    if (DTK_CONTROL(pWindow)->pTK->platform == dtk_platform_win32) {
        result = dtk_window_get_size__win32(pWindow, pWidth, pHeight);
    }
#endif
#ifdef DTK_GTK
    if (DTK_CONTROL(pWindow)->pTK->platform == dtk_platform_gtk) {
        result = dtk_window_get_size__gtk(pWindow, pWidth, pHeight);
    }
#endif

    return result;
}

dtk_result dtk_window_get_client_size(dtk_window* pWindow, dtk_uint32* pWidth, dtk_uint32* pHeight)
{
    if (pWidth) *pWidth = 0;
    if (pHeight) *pHeight = 0;
    if (pWindow == NULL) return DTK_INVALID_ARGS;

    dtk_result result = DTK_NO_BACKEND;
#ifdef DTK_WIN32
    if (DTK_CONTROL(pWindow)->pTK->platform == dtk_platform_win32) {
        result = dtk_window_get_client_size__win32(pWindow, pWidth, pHeight);
    }
#endif
#ifdef DTK_GTK
    if (DTK_CONTROL(pWindow)->pTK->platform == dtk_platform_gtk) {
        result = dtk_window_get_client_size__gtk(pWindow, pWidth, pHeight);
    }
#endif

    return result;
}

dtk_result dtk_window_set_absolute_position(dtk_window* pWindow, dtk_int32 posX, dtk_int32 posY)
{
    if (pWindow == NULL) return DTK_INVALID_ARGS;

    dtk_result result = DTK_NO_BACKEND;
#ifdef DTK_WIN32
    if (DTK_CONTROL(pWindow)->pTK->platform == dtk_platform_win32) {
        result = dtk_window_set_absolute_position__win32(pWindow, posX, posY);
    }
#endif
#ifdef DTK_GTK
    if (DTK_CONTROL(pWindow)->pTK->platform == dtk_platform_gtk) {
        result = dtk_window_set_absolute_position__gtk(pWindow, posX, posY);
    }
#endif

    return result;
}

dtk_result dtk_window_get_absolute_position(dtk_window* pWindow, dtk_int32* pPosX, dtk_int32* pPosY)
{
    if (pPosX) *pPosX = 0;
    if (pPosY) *pPosY = 0;
    if (pWindow == NULL) return DTK_INVALID_ARGS;

    dtk_result result = DTK_NO_BACKEND;
#ifdef DTK_WIN32
    if (DTK_CONTROL(pWindow)->pTK->platform == dtk_platform_win32) {
        result = dtk_window_get_absolute_position__win32(pWindow, pPosX, pPosY);
    }
#endif
#ifdef DTK_GTK
    if (DTK_CONTROL(pWindow)->pTK->platform == dtk_platform_gtk) {
        result = dtk_window_get_absolute_position__gtk(pWindow, pPosX, pPosY);
    }
#endif

    return result;
}

dtk_result dtk_window_set_relative_position(dtk_window* pWindow, dtk_int32 posX, dtk_int32 posY)
{
    return dtk_control_set_relative_position(DTK_CONTROL(pWindow), posX, posY);
}

dtk_result dtk_window_get_relative_position(dtk_window* pWindow, dtk_int32* pPosX, dtk_int32* pPosY)
{
    return dtk_control_get_relative_position(DTK_CONTROL(pWindow), pPosX, pPosY);
}

dtk_result dtk_window_move_to_center(dtk_window* pWindow)
{
    if (pWindow == NULL) return DTK_INVALID_ARGS;

    if (DTK_CONTROL(pWindow)->pParent) {
        dtk_uint32 parentSizeX;
        dtk_uint32 parentSizeY;
        if (dtk_control_get_size(DTK_CONTROL(pWindow)->pParent, &parentSizeX, &parentSizeY) != DTK_SUCCESS) {
            return DTK_ERROR;
        }

        dtk_uint32 sizeX;
        dtk_uint32 sizeY;
        if (dtk_window_get_size(pWindow, &sizeX, &sizeY) != DTK_SUCCESS) {
            return DTK_ERROR;
        }

        dtk_int32 newRelativePosX = (parentSizeX - sizeX)/2;
        dtk_int32 newRelativePosY = (parentSizeY - sizeY)/2;
        return dtk_window_set_relative_position(pWindow, newRelativePosX, newRelativePosY);
    } else {
        dtk_result result = DTK_NO_BACKEND;
    #ifdef DTK_WIN32
        if (DTK_CONTROL(pWindow)->pTK->platform == dtk_platform_win32) {
            result = dtk_window_move_to_center_of_screen__win32(pWindow);
        }
    #endif
    #ifdef DTK_GTK
        if (DTK_CONTROL(pWindow)->pTK->platform == dtk_platform_gtk) {
            result = dtk_window_move_to_center_of_screen__gtk(pWindow);
        }
    #endif

        return result;
    }
}


dtk_result dtk_window_show(dtk_window* pWindow, int mode)
{
    if (pWindow == NULL) return DTK_INVALID_ARGS;
    if (mode != DTK_HIDE &&
        mode != DTK_SHOW_NORMAL &&
        mode != DTK_SHOW_MAXIMIZED) {
        return DTK_INVALID_ARGS;
    }

    dtk_result result = DTK_NO_BACKEND;
#ifdef DTK_WIN32
    if (DTK_CONTROL(pWindow)->pTK->platform == dtk_platform_win32) {
        result = dtk_window_show__win32(pWindow, mode);
    }
#endif
#ifdef DTK_GTK
    if (DTK_CONTROL(pWindow)->pTK->platform == dtk_platform_gtk) {
        result = dtk_window_show__gtk(pWindow, mode);
    }
#endif

    if (result == DTK_SUCCESS) {
        if (mode == DTK_HIDE) {
            DTK_CONTROL(pWindow)->flags |=  DTK_CONTROL_FLAG_HIDDEN;
        } else {
            DTK_CONTROL(pWindow)->flags &= ~DTK_CONTROL_FLAG_HIDDEN;
        }
    }

    return result;
}

dtk_result dtk_window_bring_to_top(dtk_window* pWindow)
{
    if (pWindow == NULL) return DTK_INVALID_ARGS;

    dtk_result result = DTK_NO_BACKEND;
#ifdef DTK_WIN32
    if (DTK_CONTROL(pWindow)->pTK->platform == dtk_platform_win32) {
        result = dtk_window_bring_to_top__win32(pWindow);
    }
#endif
#ifdef DTK_GTK
    if (DTK_CONTROL(pWindow)->pTK->platform == dtk_platform_gtk) {
        result = dtk_window_bring_to_top__gtk(pWindow);
    }
#endif

    return result;
}

dtk_bool32 dtk_window_is_maximized(dtk_window* pWindow)
{
    if (pWindow == NULL) return DTK_INVALID_ARGS;

    dtk_bool32 result = DTK_FALSE;
#ifdef DTK_WIN32
    if (DTK_CONTROL(pWindow)->pTK->platform == dtk_platform_win32) {
        result = dtk_window_is_maximized__win32(pWindow);
    }
#endif
#ifdef DTK_GTK
    if (DTK_CONTROL(pWindow)->pTK->platform == dtk_platform_gtk) {
        result = dtk_window_is_maximized__gtk(pWindow);
    }
#endif

    return result;
}


dtk_result dtk_window_set_cursor(dtk_window* pWindow, dtk_system_cursor_type cursor)
{
    if (pWindow == NULL) return DTK_INVALID_ARGS;

    dtk_result result = DTK_NO_BACKEND;
#ifdef DTK_WIN32
    if (DTK_CONTROL(pWindow)->pTK->platform == dtk_platform_win32) {
        result = dtk_window_set_cursor__win32(pWindow, cursor);
    }
#endif
#ifdef DTK_GTK
    if (DTK_CONTROL(pWindow)->pTK->platform == dtk_platform_gtk) {
        result = dtk_window_set_cursor__gtk(pWindow, cursor);
    }
#endif

    return result;
}

dtk_bool32 dtk_window_is_cursor_over(dtk_window* pWindow)
{
    if (pWindow == NULL) return DTK_INVALID_ARGS;

    dtk_bool32 result = DTK_FALSE;
#ifdef DTK_WIN32
    if (DTK_CONTROL(pWindow)->pTK->platform == dtk_platform_win32) {
        result = dtk_window_is_cursor_over__win32(pWindow);
    }
#endif
#ifdef DTK_GTK
    if (DTK_CONTROL(pWindow)->pTK->platform == dtk_platform_gtk) {
        result = dtk_window_is_cursor_over__gtk(pWindow);
    }
#endif

    return result;
}

dtk_result dtk_window_set_menu(dtk_window* pWindow, dtk_menu* pMenu)
{
    if (pWindow == NULL) return DTK_INVALID_ARGS;

    // Only menu bars are allowed to be set on a window.
    if (pMenu != NULL && pMenu->type != dtk_menu_type_menubar) {
        return DTK_INVALID_ARGS;
    }

    dtk_result result = DTK_NO_BACKEND;
#ifdef DTK_WIN32
    if (DTK_CONTROL(pWindow)->pTK->platform == dtk_platform_win32) {
        result = dtk_window_set_menu__win32(pWindow, pMenu);
    }
#endif
#ifdef DTK_GTK
    if (DTK_CONTROL(pWindow)->pTK->platform == dtk_platform_gtk) {
        result = dtk_window_set_menu__gtk(pWindow, pMenu);
    }
#endif

    return result;
}

dtk_result dtk_window_show_popup_menu(dtk_window* pWindow, dtk_menu* pMenu, int posX, int posY)
{
    if (pWindow == NULL || pMenu == NULL) return DTK_INVALID_ARGS;

    if (pMenu->type != dtk_menu_type_popup) {
        return DTK_INVALID_ARGS;
    }

    dtk_result result = DTK_NO_BACKEND;
#ifdef DTK_WIN32
    if (DTK_CONTROL(pWindow)->pTK->platform == dtk_platform_win32) {
        result = dtk_window_show_popup_menu__win32(pWindow, pMenu, posX, posY);
    }
#endif
#ifdef DTK_GTK
    if (DTK_CONTROL(pWindow)->pTK->platform == dtk_platform_gtk) {
        result = dtk_window_show_popup_menu__gtk(pWindow, pMenu, posX, posY);
    }
#endif

    return result;
}
