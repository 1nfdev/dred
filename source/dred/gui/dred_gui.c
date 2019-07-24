// Copyright (C) 2019 David Reid. See included LICENSE file.

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <float.h>
#include <math.h>

#ifndef DRED_GUI_PRIVATE
#define DRED_GUI_PRIVATE static
#endif

/////////////////////////////////////////////////////////////////
//
// PRIVATE CORE API
//
/////////////////////////////////////////////////////////////////

#define DTK_CONTROL_TYPE_DRED   DTK_CONTROL_TYPE_CUSTOM + 1     // <-- Temporary until dred_control is removed entirely.

DTK_INLINE dtk_rect dtk_rect_init_dred(dred_rect rect)
{
    return dtk_rect_init((dtk_int32)rect.left, (dtk_int32)rect.top, (dtk_int32)rect.right, (dtk_int32)rect.bottom);
}


void dred_control__post_outbound_event_move(dred_control* pControl, int newRelativePosX, int newRelativePosY)
{
    if (pControl->onMove) {
        pControl->onMove(pControl, newRelativePosX, newRelativePosY);
    }
}

void dred_control__post_outbound_event_size(dred_control* pControl, int newWidth, int newHeight)
{
    if (pControl->onSize) {
        pControl->onSize(pControl, newWidth, newHeight);
    }
}

void dred_control__post_outbound_event_mouse_enter(dred_control* pControl)
{
    if (pControl->onMouseEnter) {
        pControl->onMouseEnter(pControl);
    }
}

void dred_control__post_outbound_event_mouse_leave(dred_control* pControl)
{
    if (pControl->onMouseLeave) {
        pControl->onMouseLeave(pControl);
    }
}

void dred_control__post_outbound_event_mouse_move(dred_control* pControl, int relativeMousePosX, int relativeMousePosY, int stateFlags)
{
    if (pControl->onMouseMove) {
        pControl->onMouseMove(pControl, relativeMousePosX, relativeMousePosY, stateFlags);
    }
}

void dred_control__post_outbound_event_mouse_button_down(dred_control* pControl, int mouseButton, int relativeMousePosX, int relativeMousePosY, int stateFlags)
{
    if (pControl->onMouseButtonDown) {
        pControl->onMouseButtonDown(pControl, mouseButton, relativeMousePosX, relativeMousePosY, stateFlags);
    }
}

void dred_control__post_outbound_event_mouse_button_up(dred_control* pControl, int mouseButton, int relativeMousePosX, int relativeMousePosY, int stateFlags)
{
    if (pControl->onMouseButtonUp) {
        pControl->onMouseButtonUp(pControl, mouseButton, relativeMousePosX, relativeMousePosY, stateFlags);
    }
}

void dred_control__post_outbound_event_mouse_button_dblclick(dred_control* pControl, int mouseButton, int relativeMousePosX, int relativeMousePosY, int stateFlags)
{
    if (pControl->onMouseButtonDblClick) {
        pControl->onMouseButtonDblClick(pControl, mouseButton, relativeMousePosX, relativeMousePosY, stateFlags);
    }
}

void dred_control__post_outbound_event_mouse_wheel(dred_control* pControl, int delta, int relativeMousePosX, int relativeMousePosY, int stateFlags)
{
    if (pControl->onMouseWheel) {
        pControl->onMouseWheel(pControl, delta, relativeMousePosX, relativeMousePosY, stateFlags);
    }
}

void dred_control__post_outbound_event_key_down(dred_control* pControl, dtk_key key, int stateFlags)
{
    if (pControl->onKeyDown) {
        pControl->onKeyDown(pControl, key, stateFlags);
    }
}

void dred_control__post_outbound_event_key_up(dred_control* pControl, dtk_key key, int stateFlags)
{
    if (pControl->onKeyUp) {
        pControl->onKeyUp(pControl, key, stateFlags);
    }
}

void dred_control__post_outbound_event_printable_key_down(dred_control* pControl, unsigned int character, int stateFlags)
{
    if (pControl->onPrintableKeyDown) {
        pControl->onPrintableKeyDown(pControl, character, stateFlags);
    }
}


void dred_control__post_outbound_event_dirty(dred_control* pControl, dred_rect relativeRect)
{
    if (pControl->onDirty) {
        pControl->onDirty(pControl, relativeRect);
    }
}

void dred_control__post_outbound_event_capture_mouse(dred_control* pControl)
{
    if (pControl->onCaptureMouse) {
        pControl->onCaptureMouse(pControl);
    }
}

void dred_control__post_outbound_event_release_mouse(dred_control* pControl)
{
    if (pControl->onReleaseMouse) {
        pControl->onReleaseMouse(pControl);
    }
}

void dred_control__post_outbound_event_capture_keyboard(dred_control* pControl, dtk_control* pPrevCapturedControl)
{
    if (pControl->onCaptureKeyboard) {
        pControl->onCaptureKeyboard(pControl, pPrevCapturedControl);
    }
}

void dred_control__post_outbound_event_release_keyboard(dred_control* pControl, dtk_control* pNewCapturedControl)
{
    if (pControl->onReleaseKeyboard) {
        pControl->onReleaseKeyboard(pControl, pNewCapturedControl);
    }
}



/////////////////////////////////////////////////////////////////
// Controls

dtk_bool32 dred_control_event_handler(dtk_event* pEvent)
{
    dtk_control* pControl = pEvent->pControl;
    dred_control* pDredControl = DRED_CONTROL(pControl);

    switch (pEvent->type)
    {
        case DTK_EVENT_PAINT:
        {
            dred_rect rect = dred_make_rect(
                pEvent->paint.rect.left,
                pEvent->paint.rect.top,
                pEvent->paint.rect.right,
                pEvent->paint.rect.bottom
            );
            
            if (pDredControl->onPaint) {
                pDredControl->onPaint(pDredControl, rect, pEvent->paint.pSurface);
            }
        } break;

        case DTK_EVENT_SIZE:
        {
            dred_control__post_outbound_event_size(pDredControl, pEvent->size.width, pEvent->size.height);
        } break;

        case DTK_EVENT_MOVE:
        {
            dred_control__post_outbound_event_move(pDredControl, pEvent->move.x, pEvent->move.y);
        } break;

        case DTK_EVENT_MOUSE_LEAVE:
        {
            dred_control__post_outbound_event_mouse_leave(pDredControl);
        } break;

        case DTK_EVENT_MOUSE_ENTER:
        {
            dred_control__post_outbound_event_mouse_enter(pDredControl);
        } break;

        case DTK_EVENT_MOUSE_MOVE:
        {
            dred_control__post_outbound_event_mouse_move(pDredControl, pEvent->mouseMove.x, pEvent->mouseMove.y, pEvent->mouseMove.state);
        } break;

        case DTK_EVENT_MOUSE_BUTTON_DOWN:
        {
            dred_control__post_outbound_event_mouse_button_down(pDredControl, pEvent->mouseButton.button, pEvent->mouseButton.x, pEvent->mouseButton.y, pEvent->mouseButton.state);
        } break;

        case DTK_EVENT_MOUSE_BUTTON_UP:
        {
            dred_control__post_outbound_event_mouse_button_up(pDredControl, pEvent->mouseButton.button, pEvent->mouseButton.x, pEvent->mouseButton.y, pEvent->mouseButton.state);
        } break;

        case DTK_EVENT_MOUSE_BUTTON_DBLCLICK:
        {
            dred_control__post_outbound_event_mouse_button_dblclick(pDredControl, pEvent->mouseButton.button, pEvent->mouseButton.x, pEvent->mouseButton.y, pEvent->mouseButton.state);
        } break;

        case DTK_EVENT_MOUSE_WHEEL:
        {
            dred_control__post_outbound_event_mouse_wheel(pDredControl, pEvent->mouseWheel.delta, pEvent->mouseWheel.x, pEvent->mouseWheel.y, pEvent->mouseWheel.state);
        } break;

        case DTK_EVENT_KEY_DOWN:
        {
            dred_control__post_outbound_event_key_down(pDredControl, pEvent->keyDown.key, pEvent->keyDown.state);
        } break;

        case DTK_EVENT_KEY_UP:
        {
            dred_control__post_outbound_event_key_up(pDredControl, pEvent->keyUp.key, pEvent->keyUp.state);
        } break;

        case DTK_EVENT_PRINTABLE_KEY_DOWN:
        {
            dred_control__post_outbound_event_printable_key_down(pDredControl, pEvent->printableKeyDown.utf32, pEvent->printableKeyDown.state);
        } break;

        case DTK_EVENT_CAPTURE_KEYBOARD:
        {
            dred_control__post_outbound_event_capture_keyboard(pDredControl, pEvent->captureKeyboard.pOldCapturedControl);
        } break;

        case DTK_EVENT_RELEASE_KEYBOARD:
        {
            dred_control__post_outbound_event_release_keyboard(pDredControl, pEvent->releaseKeyboard.pNewCapturedControl);
        } break;

        case DTK_EVENT_CAPTURE_MOUSE:
        {
            dred_control__post_outbound_event_capture_mouse(pDredControl);
        } break;

        case DTK_EVENT_RELEASE_MOUSE:
        {
            dred_control__post_outbound_event_release_mouse(pDredControl);
        } break;

        default: break;
    }

    return dtk_control_default_event_handler(pEvent);
}

dtk_bool32 dred_control_init(dred_control* pControl, dred_context* pDred, dred_control* pParent, dtk_control* pDTKParent, const char* type, dtk_event_proc onEvent)
{
    if (pControl == NULL || pDred == NULL) return DTK_FALSE;
    memset(pControl, 0, sizeof(*pControl));


    if (dtk_control_init(&pDred->tk, DTK_CONTROL_TYPE_DRED, (onEvent != NULL) ? onEvent : dred_control_event_handler, (pParent != NULL) ? &pParent->baseControl : pDTKParent, &pControl->baseControl) != DTK_SUCCESS) {
        return DTK_FALSE;
    }
    
    pControl->pDred = pDred;

    dred_control_set_type(pControl, type);
    return DTK_TRUE;
}

void dred_control_uninit(dred_control* pControl)
{
    if (pControl == NULL) {
        return;
    }

    dtk_control_uninit(&pControl->baseControl);
}


dred_context* dred_control_get_context(dred_control* pControl)
{
    if (pControl == NULL) {
        return NULL;
    }

    return pControl->pDred;
}


dtk_bool32 dred_control_set_type(dred_control* pControl, const char* type)
{
    if (pControl == NULL) {
        return DTK_FALSE;
    }

    return strcpy_s(pControl->type, sizeof(pControl->type), (type == NULL) ? "" : type) == 0;
}

const char* dred_control_get_type(dred_control* pControl)
{
    if (pControl == NULL) {
        return NULL;
    }

    return pControl->type;
}

dtk_bool32 dred_control_is_of_type(dred_control* pControl, const char* type)
{
    if (pControl == NULL || type == NULL) {
        return DTK_FALSE;
    }

    return dred_is_control_type_of_type(pControl->type, type);
}

dtk_bool32 dred_is_control_type_of_type(const char* type, const char* base)
{
    if (type == NULL || base == NULL) {
        return DTK_FALSE;
    }

    return strncmp(type, base, strlen(base)) == 0;
}



//// Events ////

void dred_control_set_on_move(dred_control * pControl, dred_gui_on_move_proc callback)
{
    if (pControl != NULL) {
        pControl->onMove = callback;
    }
}

void dred_control_set_on_size(dred_control * pControl, dred_gui_on_size_proc callback)
{
    if (pControl != NULL) {
        pControl->onSize = callback;
    }
}

void dred_control_set_on_mouse_enter(dred_control* pControl, dred_gui_on_mouse_enter_proc callback)
{
    if (pControl != NULL) {
        pControl->onMouseEnter = callback;
    }
}

void dred_control_set_on_mouse_leave(dred_control* pControl, dred_gui_on_mouse_leave_proc callback)
{
    if (pControl != NULL) {
        pControl->onMouseLeave = callback;
    }
}

void dred_control_set_on_mouse_move(dred_control* pControl, dred_gui_on_mouse_move_proc callback)
{
    if (pControl != NULL) {
        pControl->onMouseMove = callback;
    }
}

void dred_control_set_on_mouse_button_down(dred_control* pControl, dred_gui_on_mouse_button_down_proc callback)
{
    if (pControl != NULL) {
        pControl->onMouseButtonDown = callback;
    }
}

void dred_control_set_on_mouse_button_up(dred_control* pControl, dred_gui_on_mouse_button_up_proc callback)
{
    if (pControl != NULL) {
        pControl->onMouseButtonUp = callback;
    }
}

void dred_control_set_on_mouse_button_dblclick(dred_control* pControl, dred_gui_on_mouse_button_dblclick_proc callback)
{
    if (pControl != NULL) {
        pControl->onMouseButtonDblClick = callback;
    }
}

void dred_control_set_on_mouse_wheel(dred_control* pControl, dred_gui_on_mouse_wheel_proc callback)
{
    if (pControl != NULL) {
        pControl->onMouseWheel = callback;
    }
}

void dred_control_set_on_key_down(dred_control* pControl, dred_gui_on_key_down_proc callback)
{
    if (pControl != NULL) {
        pControl->onKeyDown = callback;
    }
}

void dred_control_set_on_key_up(dred_control* pControl, dred_gui_on_key_up_proc callback)
{
    if (pControl != NULL) {
        pControl->onKeyUp = callback;
    }
}

void dred_control_set_on_printable_key_down(dred_control* pControl, dred_gui_on_printable_key_down_proc callback)
{
    if (pControl != NULL) {
        pControl->onPrintableKeyDown = callback;
    }
}

void dred_control_set_on_paint(dred_control* pControl, dred_gui_on_paint_proc callback)
{
    if (pControl != NULL) {
        pControl->onPaint = callback;
    }
}

void dred_control_set_on_dirty(dred_control * pControl, dred_gui_on_dirty_proc callback)
{
    if (pControl != NULL) {
        pControl->onDirty = callback;
    }
}

void dred_control_set_on_capture_mouse(dred_control* pControl, dred_gui_on_capture_mouse_proc callback)
{
    if (pControl != NULL) {
        pControl->onCaptureMouse = callback;
    }
}

void dred_control_set_on_release_mouse(dred_control* pControl, dred_gui_on_release_mouse_proc callback)
{
    if (pControl != NULL) {
        pControl->onReleaseMouse = callback;
    }
}

void dred_control_set_on_capture_keyboard(dred_control* pControl, dred_gui_on_capture_keyboard_proc callback)
{
    if (pControl != NULL) {
        pControl->onCaptureKeyboard = callback;
    }
}

void dred_control_set_on_release_keyboard(dred_control* pControl, dred_gui_on_release_keyboard_proc callback)
{
    if (pControl != NULL) {
        pControl->onReleaseKeyboard = callback;
    }
}






//// Layout ////

void dred_control_set_absolute_position(dred_control* pControl, dtk_int32 positionX, dtk_int32 positionY)
{
    if (pControl == NULL) return;
    dtk_control_set_absolute_position(DTK_CONTROL(pControl), (dtk_int32)positionX, (dtk_int32)positionY);
}

void dred_control_get_absolute_position(const dred_control* pControl, dtk_int32 * positionXOut, dtk_int32 * positionYOut)
{
    dtk_control_get_absolute_position(DTK_CONTROL(pControl), positionXOut, positionYOut);
}

dtk_int32 dred_control_get_absolute_position_x(const dred_control* pControl)
{
    if (pControl == NULL) return 0;
    return pControl->baseControl.absolutePosX;
}

dtk_int32 dred_control_get_absolute_position_y(const dred_control* pControl)
{
    if (pControl == NULL) return 0;
    return pControl->baseControl.absolutePosY;
}


void dred_control_set_relative_position(dred_control* pControl, dtk_int32 relativePosX, dtk_int32 relativePosY)
{
    if (pControl == NULL) return;
    dtk_control_set_relative_position(DTK_CONTROL(pControl), relativePosX, relativePosY);
}

void dred_control_get_relative_position(const dred_control* pControl, dtk_int32* positionXOut, dtk_int32* positionYOut)
{
    dtk_control_get_relative_position(DTK_CONTROL(pControl), positionXOut, positionYOut);
}

dtk_int32 dred_control_get_relative_position_x(const dred_control* pControl)
{
    dtk_int32 x;
    dred_control_get_relative_position(pControl, &x, NULL);
    return x;
}

dtk_int32 dred_control_get_relative_position_y(const dred_control* pControl)
{
    dtk_int32 y;
    dred_control_get_relative_position(pControl, NULL, &y);
    return y;
}


void dred_control_set_size(dred_control* pControl, dtk_int32 width, dtk_int32 height)
{
    if (pControl == NULL) return;
    dtk_control_set_size(DTK_CONTROL(pControl), width, height);
}

void dred_control_get_size(const dred_control* pControl, dtk_int32* widthOut, dtk_int32* heightOut)
{
    dtk_control_get_size(DTK_CONTROL(pControl), widthOut, heightOut);
}

dtk_int32 dred_control_get_width(const dred_control * pControl)
{
    return pControl->baseControl.width;
}

dtk_int32 dred_control_get_height(const dred_control * pControl)
{
    return pControl->baseControl.height;
}


dred_rect dred_control_get_absolute_rect(const dred_control* pControl)
{
    dtk_rect rect = dtk_control_get_absolute_rect(DTK_CONTROL(pControl));
    return dred_make_rect(
        rect.left,
        rect.top,
        rect.right,
        rect.bottom
    );
}

dred_rect dred_control_get_relative_rect(const dred_control* pControl)
{
    dtk_rect rect = dtk_control_get_relative_rect(DTK_CONTROL(pControl));
    return dred_make_rect(
        rect.left,
        rect.top,
        rect.right,
        rect.bottom
    );
}

dred_rect dred_control_get_local_rect(const dred_control* pControl)
{
    dtk_rect rect = dtk_control_get_local_rect(DTK_CONTROL(pControl));
    return dred_make_rect(
        rect.left,
        rect.top,
        rect.right,
        rect.bottom
    );
}




/////////////////////////////////////////////////////////////////
//
// UTILITY API
//
/////////////////////////////////////////////////////////////////

dred_rect dred_clamp_rect(dred_rect rect, dred_rect other)
{
    dred_rect result;
    result.left   = (rect.left   >= other.left)   ? rect.left   : other.left;
    result.top    = (rect.top    >= other.top)    ? rect.top    : other.top;
    result.right  = (rect.right  <= other.right)  ? rect.right  : other.right;
    result.bottom = (rect.bottom <= other.bottom) ? rect.bottom : other.bottom;

    return result;
}

dtk_bool32 dred_clamp_rect_to_element(const dred_control* pControl, dred_rect* pRelativeRect)
{
    if (pControl == NULL || pRelativeRect == NULL) {
        return DTK_FALSE;
    }

    dtk_rect rect = dtk_rect_init_dred(*pRelativeRect);
    dtk_bool32 result = dtk_control_clamp_rect(DTK_CONTROL(pControl), &rect);
    
    pRelativeRect->left   = rect.left;
    pRelativeRect->top    = rect.top;
    pRelativeRect->right  = rect.right;
    pRelativeRect->bottom = rect.bottom;

    return result;
}

dred_rect dred_make_rect_relative(const dred_control* pControl, dred_rect* pRect)
{
    if (pControl == NULL || pRect == NULL) {
        return dred_make_rect(0, 0, 0, 0);
    }

    pRect->left   -= pControl->baseControl.absolutePosX;
    pRect->top    -= pControl->baseControl.absolutePosY;
    pRect->right  -= pControl->baseControl.absolutePosX;
    pRect->bottom -= pControl->baseControl.absolutePosY;

    return *pRect;
}

dred_rect dred_make_rect_absolute(const dred_control * pControl, dred_rect * pRect)
{
    if (pControl == NULL || pRect == NULL) {
        return dred_make_rect(0, 0, 0, 0);
    }

    pRect->left   += pControl->baseControl.absolutePosX;
    pRect->top    += pControl->baseControl.absolutePosY;
    pRect->right  += pControl->baseControl.absolutePosX;
    pRect->bottom += pControl->baseControl.absolutePosY;

    return *pRect;
}

void dred_make_point_relative(const dred_control* pControl, dtk_int32* positionX, dtk_int32* positionY)
{
    if (pControl != NULL)
    {
        if (positionX != NULL) {
            *positionX -= pControl->baseControl.absolutePosX;
        }

        if (positionY != NULL) {
            *positionY -= pControl->baseControl.absolutePosY;
        }
    }
}

void dred_make_point_absolute(const dred_control* pControl, dtk_int32* positionX, dtk_int32* positionY)
{
    if (pControl != NULL)
    {
        if (positionX != NULL) {
            *positionX += pControl->baseControl.absolutePosX;
        }

        if (positionY != NULL) {
            *positionY += pControl->baseControl.absolutePosY;
        }
    }
}

dred_rect dred_make_rect(dtk_int32 left, dtk_int32 top, dtk_int32 right, dtk_int32 bottom)
{
    dred_rect rect;
    rect.left   = left;
    rect.top    = top;
    rect.right  = right;
    rect.bottom = bottom;

    return rect;
}

dred_rect dred_make_inside_out_rect()
{
    dred_rect rect;
    rect.left   = INT_MAX;
    rect.top    = INT_MAX;
    rect.right  = INT_MIN;
    rect.bottom = INT_MIN;

    return rect;
}

dred_rect dred_grow_rect(dred_rect rect, dtk_int32 amount)
{
    dred_rect result = rect;
    result.left   -= amount;
    result.top    -= amount;
    result.right  += amount;
    result.bottom += amount;

    return result;
}

dred_rect dred_scale_rect(dred_rect rect, dtk_int32 scaleX, dtk_int32 scaleY)
{
    dred_rect result = rect;
    result.left   *= scaleX;
    result.top    *= scaleY;
    result.right  *= scaleX;
    result.bottom *= scaleY;

    return result;
}

dred_rect dred_offset_rect(dred_rect rect, dtk_int32 offsetX, dtk_int32 offsetY)
{
    return dred_make_rect(rect.left + offsetX, rect.top + offsetY, rect.right + offsetX, rect.bottom + offsetY);
}

dred_rect dred_rect_union(dred_rect rect0, dred_rect rect1)
{
    dred_rect result;
    result.left   = (rect0.left   < rect1.left)   ? rect0.left   : rect1.left;
    result.top    = (rect0.top    < rect1.top)    ? rect0.top    : rect1.top;
    result.right  = (rect0.right  > rect1.right)  ? rect0.right  : rect1.right;
    result.bottom = (rect0.bottom > rect1.bottom) ? rect0.bottom : rect1.bottom;

    return result;
}

dtk_bool32 dred_rect_contains_point(dred_rect rect, dtk_int32 posX, dtk_int32 posY)
{
    if (posX < rect.left || posY < rect.top) {
        return DTK_FALSE;
    }

    if (posX >= rect.right || posY >= rect.bottom) {
        return DTK_FALSE;
    }

    return DTK_TRUE;
}

dtk_bool32 dred_rect_equal(dred_rect rect0, dred_rect rect1)
{
    return
        rect0.left   == rect1.left  &&
        rect0.top    == rect1.top   &&
        rect0.right  == rect1.right &&
        rect0.bottom == rect1.bottom;
}

dtk_bool32 dred_rect_has_volume(dred_rect rect)
{
    return rect.right > rect.left && rect.bottom > rect.top;
}
