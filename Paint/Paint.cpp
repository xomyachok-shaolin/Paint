/* 2018 ReVoL Primer Template */
/* Paint.cpp */
#include "paint.h"
/* вычисляет координаты мыши */
void GetMouse(short & MX, short & MY, WPARAM wParam, LPARAM lParam) {
    MX = LOWORD(lParam);
    MY = HIWORD(lParam);
}
/* сохраняет рисунок из совместимого контекста в дополнительный */
void ImageSave() {
    /* копируем совместимый контекст в дополнительный */
    BitBlt(CD2, 0, 0, gde_w, gde_h, CDC, 0, 0, SRCCOPY);
}
/* восстанавливает рисунок из дополнительного контекста в совместимый */
void ImageRestore() {
    /* копируем дополнительный контекст в совместимый */
    BitBlt(CDC, 0, 0, gde_w, gde_h, CD2, 0, 0, SRCCOPY);
}
/* очищает картинку */
void ImageClear(COLORREF color) {
    HBRUSH brush = (HBRUSH)SelectObject(CDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(CDC, color);
    PatBlt(CDC, 0, 0, gde_w, gde_h, PATCOPY);
    InvalidateRect(hWndGDE, 0, 0);
    SelectObject(CDC, brush);
}
/* закрашивает окно */
void PaintWindow(HWND hWnd, COLORREF color) {
    RECT rc;
    HDC hdc = GetDC(hWnd);
    SelectObject(hdc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hdc, color);
    GetClientRect(hWnd, &rc);
    PatBlt(hdc, 0, 0, rc.right, rc.bottom, PATCOPY);
    ReleaseDC(hWnd, hdc);
}
#include "drawing.h"
/* оконная процедура окна графического вывода */
LRESULT CALLBACK GDProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    PAINTSTRUCT ps;
    HDC hdc;
    int L, T, W, H;
    if (!loaded) return DefWindowProc(hWnd, message, wParam, lParam);
    switch (message) {
    case WM_LBUTTONUP:
        return LeftButtonUp(hWnd, wParam, lParam);
    case WM_RBUTTONUP:
        return RightButtonUp(hWnd, wParam, lParam);
    case WM_LBUTTONDOWN:
        return LeftButtonDown(hWnd, wParam, lParam);
    case WM_RBUTTONDOWN:
        return RightButtonDown(hWnd, wParam, lParam);
    case WM_MOUSEMOVE:
        if (!mouse_down) return 0;
        GetMouse(MX2, MY2, wParam, lParam);
        ShiftMouse(MX1, MY1, MX2, MY2);
        switch (painting_tool) {
        case TOOL_MOUSE:
            DrawMouse(MX1, MY1, MX2, MY2);
            break;
        case TOOL_LINE:
            DrawLine(MX1, MY1, MX2, MY2);
            break;
        case TOOL_RECT:
            DrawRect(MX1, MY1, MX2, MY2);
            break;
        case TOOL_OVAL:
            DrawOval(MX1, MY1, MX2, MY2);
            break;
        case TOOL_BREZ:
            DrawBrez(MX1, MY1, MX2, MY2);
            break;
        case TOOL_BWOO:
            DrawBWoo(MX1, MY1, MX2, MY2);
            break;
        case TOOL_POLY:
            if (poly_left == 0 && poly_right == 0) break;
            DrawLine(MX1, MY1, MX2, MY2);
            break;
        }
        return 0;
    case WM_PAINT:
		/* перерисовка изображения */
        hdc = BeginPaint(hWnd, &ps);
		/* область обновления */
        L = ps.rcPaint.left;
        T = ps.rcPaint.top;
        W = ps.rcPaint.right - L;
        H = ps.rcPaint.bottom - T;
		/* копируем изображение из CDC в графическое устройство hdc */
        BitBlt(hdc, L, T, W, H, CDC, L, T, SRCCOPY);
        EndPaint(hWnd, &ps);
        return 0;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}
/* устанавливает цвет плана */
void SetForeColor(int index) {
    if (index < 0) index = 0;
    if (index > 15) index = 15;
    fore_color_index = index;
    fore_color = C16[index];
    SetBkColor(CDC, back_color);
    C16[PA_FC] = fore_color;
    PaintWindow(WPA[PA_FC], fore_color);
}
/* устанавливает цвет фона */
void SetBackColor(int index) {
    if (index < 0) index = 0;
    if (index > 15) index = 15;
    back_color_index = index;
    back_color = C16[index];
    SetBkColor(CDC, fore_color);
    C16[PA_BC] = back_color;
    PaintWindow(WPA[PA_BC], back_color);
}
/* устанавливает режим фона */
void SetBackStyleMode(int index) {
    if (index < 1) index = 1;
    if (index > 2) index = 2;
    backstyle_mode = index;
    CheckRadioButton(WBM[0], 1, 2, index);
    SetFocus(W0);
}
/* выбирает инструмент */
void SetTool(TOOLS index) {
    if (index < 1) index = (TOOLS)1;
    if (index > 7) index = (TOOLS)7;
    painting_tool = index;
    CheckRadioButton(WCT[0], 1, 7, index);
    SetFocus(W0);
}
/* выбирает стиль линии */
void SetLineStyle(int index) {
    if (index < 1) index = 1;
    if (index > 4) index = 4;
    line_style_index = index;
    line_style = linsty[index];
    CheckRadioButton(WLS[0], 1, 4, index);
    SetFocus(W0);
    CreateNewPen(PenFore, fore_color);
    CreateNewPen(PenBack, back_color);
}
/* выбирает тип пера */
void SetPenType(int index) {
    if (index < 1) index = 1;
    if (index > MAX_PEN_TYPE) index = MAX_PEN_TYPE;
    if (index > 2) index = 2;
    pen_type_index = index;
    pen_type = pentyp[index];
    CheckRadioButton(WPT[0], 1, 2, index);
    SetFocus(W0);
    CreateNewPen(PenFore, fore_color);
    CreateNewPen(PenBack, back_color);
}
/* выбирает размер пера */
void SetPenSize(int index) {
    if (index < 1) index = 1;
    if (index > 3) index = 3;
    pen_size_index = index;
    pen_size = pensiz[index];
    CheckRadioButton(WPS[0], 1, 3, index);
    SetFocus(W0);
    CreateNewPen(PenFore, fore_color);
    CreateNewPen(PenBack, back_color);
}
/* выбирает концевую точку */
void SetPenEndCap(int index) {
    if (index < 1) index = 1;
    if (index > 3) index = 3;
    pen_endcap_index = index;
    pen_endcap = endcap[index];
    CheckRadioButton(WEC[0], 1, 3, index);
    SetFocus(W0);
    CreateNewPen(PenFore, fore_color);
    CreateNewPen(PenBack, back_color);
}
/* выбирает тип соединения */
void SetPenJointy(int index) {
    if (index < 1) index = 1;
    if (index > 3) index = 3;
    pen_jointy_index = index;
    pen_jointy = jointy[index];
    CheckRadioButton(WJS[0], 1, 3, index);
    SetFocus(W0);
    CreateNewPen(PenFore, fore_color);
    CreateNewPen(PenBack, back_color);
}
void SetPenHatch(int index) {
    if (index < 1) index = 1;
    if (index > 7) index = 7;
    pen_hatch_index = index;
    g_hatch = brusty[index];
    if (g_hatch == HS_SOLID) {
        g_style = BS_SOLID;
        g_hatch = HS_CROSS;
    } else {
        g_style = BS_HATCHED;
    }
    CheckRadioButton(WHS[0], 1, 7, index);
    SetFocus(W0);
    CreateNewPen(PenFore, fore_color);
    CreateNewPen(PenBack, back_color);
}
void SetINIPath() {
    TCHAR path[260];
    int i, j;
    GetWindowModuleFileName(W0, path, 258);
    LPTSTR p = strrchr(path, 46);
    p[0] = 0;
    strcat(path, ".ini");
    for (i = 0, j = 0; i < (int)strlen(path); i++) {
        if (path[i] == 92) szIniPath[j++] = path[i];
        szIniPath[j++] = path[i];
    }
    szIniPath[j] = 0;
}
void GetSettings() {
    Settings s;
    s.PutPath(szIniPath);
    SetTool((TOOLS)s.GetSettingLong((char*)"painting_tool", TOOL_MOUSE));
    SetForeColor((int)s.GetSettingLong((char*)"fore_color_index", 0));
    SetBackColor((int)s.GetSettingLong((char*)"back_color_index", 15));
    SetBackStyleMode((int)s.GetSettingLong((char*)"backstyle_mode", 0));
    SetLineStyle((int)s.GetSettingLong((char*)"line_style", 1));
    SetPenType((int)s.GetSettingLong((char*)"pen_type", 1));
    SetPenSize((int)s.GetSettingLong((char*)"pen_size", 1));
    SetPenEndCap((int)s.GetSettingLong((char*)"pen_endcap", 1));
    SetPenJointy((int)s.GetSettingLong((char*)"pen_jointy", 1));
    SetPenHatch((int)s.GetSettingLong((char*)"pen_hatch", 1));
    ImageClear(RGB(255, 255, 255));
}
void PutSettings() {
    Settings s;
    s.PutPath(szIniPath);
    s.PutSettingLong((int)fore_color_index, (char*)"fore_color_index");
    s.PutSettingLong((int)back_color_index, (char*)"back_color_index");
    s.PutSettingLong((int)backstyle_mode, (char*)"backstyle_mode");
    s.PutSettingLong((int)painting_tool, (char*)"painting_tool");
    s.PutSettingLong((int)line_style_index, (char*)"line_style");
    s.PutSettingLong((int)pen_type_index, (char*)"pen_type");
    s.PutSettingLong((int)pen_size_index, (char*)"pen_size");
    s.PutSettingLong((int)pen_endcap_index, (char*)"pen_endcap");
    s.PutSettingLong((int)pen_jointy_index, (char*)"pen_jointy");
    s.PutSettingLong((int)pen_hatch_index, (char*)"pen_hatch");
}
LRESULT CALLBACK PAProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    int index = GetWindowLong(hWnd, GWL_ID);
    switch (message) {
    case WM_LBUTTONDOWN:
        if (index > 15) return 0;
        /* новый цвет плана */
        SetForeColor(index);
        /* создаем новое перо плана */
        CreateNewPen(PenFore, C16[index]);
        /* режим фона */
        SetBackStyle(back_color);
        return 0;
    case WM_RBUTTONDOWN:
        if (index > 15) return 0;
        /* новый цвет фона */
        SetBackColor(index);
        /* создаем новое перо фона */
        CreateNewPen(PenBack, C16[index]);
        /* режим фона */
        SetBackStyle(fore_color);
        return 0;
    case WM_ERASEBKGND:
        /* очистка окна */
        PaintWindow(hWnd, C16[index]);
        return 0;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}
LRESULT CALLBACK MWProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    int wmIdent = LOWORD(wParam), wmEvent = HIWORD(wParam);
    switch (message) {
    case WM_COMMAND:
        if (wmEvent == BN_CLICKED) {
            switch (wmIdent) {
            case IDI_CAN:
                /* отмена действия */
                ImageRestore();
                InvalidateRect(hWndGDE, 0, 0);
                SetFocus(W0);
                return 0;
            case IDI_CLR:
                /* очистка */
                ImageClear(back_color);
                SetFocus(W0);
                return 0;
            case IDI_CLW:
                /* очистка белым */
                ImageClear(RGB(255, 255, 255));
                SetFocus(W0);
                return 0;
            }
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}
LRESULT CALLBACK CTProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    int wmIdent = LOWORD(wParam), wmEvent = HIWORD(wParam);
    switch (message) {
    case WM_COMMAND:
        switch (wmIdent) {
        case TOOL_MOUSE: case TOOL_LINE: case TOOL_RECT: case TOOL_OVAL:
        case TOOL_BREZ: case TOOL_BWOO: case TOOL_POLY:
            if (HIWORD(wParam) == BN_CLICKED) {
                SetTool((TOOLS)wmIdent);
            }
            return 0;
        }
        break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}
LRESULT CALLBACK BMProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    int wmIdent = LOWORD(wParam), wmEvent = HIWORD(wParam);
    switch (message) {
    case WM_COMMAND:
        switch (wmIdent) {
        case 1: case 2:
            if (wmEvent == BN_CLICKED) {
                SetBackStyleMode(wmIdent);
            }
            return 0;
        }
        break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}
LRESULT CALLBACK PTProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    int wmIdent = LOWORD(wParam), wmEvent = HIWORD(wParam);
    switch (message) {
    case WM_COMMAND:
        switch (wmIdent) {
        case 1: case 2:
            if (wmEvent == BN_CLICKED) {
                SetPenType(wmIdent);
            }
            return 0;
        }
        break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}
LRESULT CALLBACK PSProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    int wmIdent = LOWORD(wParam), wmEvent = HIWORD(wParam);
    switch (message) {
    case WM_COMMAND:
        switch (wmIdent) {
        case 1: case 2: case 3:
            if (wmEvent == BN_CLICKED) {
                SetPenSize(wmIdent);
            }
            return 0;
        }
        break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}
LRESULT CALLBACK LSProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    int wmIdent = LOWORD(wParam), wmEvent = HIWORD(wParam);
    switch (message) {
    case WM_COMMAND:
        switch (wmIdent) {
        case 1: case 2: case 3: case 4:
            if (wmEvent == BN_CLICKED) {
                SetLineStyle(wmIdent);
            }
            return 0;
        }
        break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}
LRESULT CALLBACK ECProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    int wmIdent = LOWORD(wParam), wmEvent = HIWORD(wParam);
    switch (message) {
    case WM_COMMAND:
        switch (wmIdent) {
        case 1: case 2: case 3: case 4:
            if (wmEvent == BN_CLICKED) {
                SetPenEndCap(wmIdent);
            }
            return 0;
        }
        break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}
LRESULT CALLBACK JSProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    int wmIdent = LOWORD(wParam), wmEvent = HIWORD(wParam);
    switch (message) {
    case WM_COMMAND:
        switch (wmIdent) {
        case 1: case 2: case 3:
            if (wmEvent == BN_CLICKED) {
                SetPenJointy(wmIdent);
            }
            return 0;
        }
        break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}
LRESULT CALLBACK HSProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    int wmIdent = LOWORD(wParam), wmEvent = HIWORD(wParam);
    switch (message) {
    case WM_COMMAND:
        switch (wmIdent) {
        case 1: case 2: case 3: case 4: case 5: case 6: case 7:
            if (wmEvent == BN_CLICKED) {
                SetPenHatch(wmIdent);
            }
            return 0;
        }
        break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}
ATOM RegisterClasses(HINSTANCE hInstance) {
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = (WNDPROC)MWProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_PAINT);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    wcex.lpszMenuName = 0;
    wcex.lpszClassName = "PAINTAPP";
    wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);
    if (!RegisterClassEx(&wcex)) return 0;
    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wcex.lpfnWndProc = (WNDPROC)PAProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    wcex.lpszClassName = "PAINTPAL";
    return RegisterClassEx(&wcex);
}
BOOL CreateTools(int CW, int CH) {
    int i, ccx, ccy, ccw, cch, rid;
    ccx = PAD; ccy = TW_H; cch = 2 * BW_W - 1; ccw = BW_W;
    WBM[0] = CCreateStatic(hInst, W0, ccx, ccy, ccw, cch, BMProc);
    if (!WBM[0]) return FALSE;
    ccx = 0; ccy += cch + PAD + PAD; ccw = PW_W; cch = PAD + PAD + PAD + 9 * (PAL)-4;
    WPA[PA_TW] = CCreateStatic(hInst, W0, ccx, ccy, ccw, cch, 0);
    if (!WPA[PA_TW]) return FALSE;
    ccx = PAD; ccy = PAD; ccw = 7 * BW_W - 6; cch = BW_H;
    WCT[0] = CCreateStatic(hInst, W0, ccx, ccy, ccw, cch, CTProc);
    if (!WCT[0]) return FALSE;
    ccx += ccw + PAD; ccw = 2 * BW_W - 1;
    WPT[0] = CCreateStatic(hInst, W0, ccx, ccy, ccw, cch, PTProc);
    if (!WPT[0]) return FALSE;
    ccx += ccw + PAD; ccw = 3 * BW_W - 2;
    WPS[0] = CCreateStatic(hInst, W0, ccx, ccy, ccw, cch, PSProc);
    if (!WPS[0]) return FALSE;
    ccx += ccw + PAD + 1; ccw = 4 * BW_W - 3;
    WLS[0] = CCreateStatic(hInst, W0, ccx, ccy, ccw, cch, LSProc);
    if (!WLS[0]) return FALSE;
    ccx += ccw + PAD + 1; ccw = 3 * BW_W - 2;
    WEC[0] = CCreateStatic(hInst, W0, ccx, ccy, ccw, cch, ECProc);
    if (!WEC[0]) return FALSE;
    ccx = CW - PAD - BW_W; ccy = PAD + PAD + PAD + BW_W + BW_W + BW_W - 1; ccw = BW_W; cch = 3 * BW_W - 2;
    WJS[0] = CCreateStatic(hInst, W0, ccx, ccy, ccw, cch, JSProc);
    if (!WJS[0]) return FALSE;
    ccy += cch + PAD; cch = 7 * BW_W - 6;
    WHS[0] = CCreateStatic(hInst, W0, ccx, ccy, ccw, cch, HSProc);
    if (!WHS[0]) return FALSE;
    ccx = 0; rid = IDI_CT1; for (i = 1; i <= 7; i++, rid++, ccx += BW_W - 1) WCT[i] = CCreateButton3P(hInst, WCT[0], ccx, 0, BW_W, BW_H, i, rid, 0x50015049);
    ccx = 0; rid = IDI_PT1; for (i = 1; i <= 2; i++, rid++, ccx += BW_W - 1) WPT[i] = CCreateButton3P(hInst, WPT[0], ccx, 0, BW_W, BW_H, i, rid, 0x50015049);
    ccx = 0; rid = IDI_PS1; for (i = 1; i <= 3; i++, rid++, ccx += BW_W - 1) WPS[i] = CCreateButton3P(hInst, WPS[0], ccx, 0, BW_W, BW_H, i, rid, 0x50015049);
    ccx = 0; rid = IDI_LS1; for (i = 1; i <= 4; i++, rid++, ccx += BW_W - 1) WLS[i] = CCreateButton3P(hInst, WLS[0], ccx, 0, BW_W, BW_H, i, rid, 0x50015049);
    ccx = 0; rid = IDI_EC1; for (i = 1; i <= 3; i++, rid++, ccx += BW_W - 1) WEC[i] = CCreateButton3P(hInst, WEC[0], ccx, 0, BW_W, BW_H, i, rid, 0x50015049);
    ccx = 0; ccy = 0; rid = IDI_BM1; for (i = 1; i <= 2; i++, rid++, ccy += BW_W - 1) WBM[i] = CCreateButton3P(hInst, WBM[0], 0, ccy, BW_W, BW_H, i, rid, 0x50015049);
    ccy = 0; rid = IDI_JS1; for (i = 1; i <= 3; i++, rid++, ccy += BW_W - 1) WJS[i] = CCreateButton3P(hInst, WJS[0], 0, ccy, BW_W, BW_H, i, rid, 0x50015049);
    ccy = 0; rid = IDI_HS1; for (i = 1; i <= 7; i++, rid++, ccy += BW_W - 1) WHS[i] = CCreateButton3P(hInst, WHS[0], 0, ccy, BW_W, BW_H, i, rid, 0x50015049);
    ccx = PAD; ccy = 0; WPA[PA_FC] = CreateWindow("PAINTPAL", 0, 0x50810000, ccx, ccy, PAL, PAL, WPA[PA_TW], (HMENU)PA_FC, hInst, 0);
    ccx = PAD + PAL - 1; WPA[PA_BC] = CreateWindow("PAINTPAL", 0, 0x50810000, ccx, ccy, PAL, PAL, WPA[PA_TW], (HMENU)PA_BC, hInst, 0);
    ccx = PAD; ccy = PAL + PAD + PAD; for (i = 0; i < 8; i++, ccy += PAL - 1) WPA[i] = CreateWindow("PAINTPAL", 0, 0x50810000, ccx, ccy, PAL, PAL, WPA[PA_TW], (HMENU)i, hInst, 0);
    ccx = PAD + PAL - 1; ccy = PAL + PAD + PAD; for (i = 8; i < 16; i++, ccy += PAL - 1) WPA[i] = CreateWindow("PAINTPAL", 0, 0x50810000, ccx, ccy, PAL, PAL, WPA[PA_TW], (HMENU)i, hInst, 0);
    WCAN = CCreateButton3P(hInst, W0, CW - PAD - BW_W, PAD, BW_W, BW_H, IDI_CAN, IDI_CAN, 0x50014041);
    WCLR = CCreateButton3P(hInst, W0, CW - PAD - BW_W, PAD + PAD + BW_W, BW_W, BW_H, IDI_CLR, IDI_CLR, 0x50014041);
    WCLW = CCreateButton3P(hInst, W0, CW - PAD - BW_W, PAD + PAD + BW_W + BW_W - 1, BW_W, BW_H, IDI_CLW, IDI_CLW, 0x50014041);
    return TRUE;
}
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
    hInst = hInstance;
    RECT mr, gr;
    int CW, CH;
    int cx = (GetSystemMetrics(SM_CXSCREEN) - CW0) / 2;
    int cy = (GetSystemMetrics(SM_CYSCREEN) - CH0) / 2;
    W0 = CreateWindow("PAINTAPP", "Paint Primer App", 0x80c80000, cx, cy, CW0, CH0, 0, 0, hInst, 0);
    if (!W0) return FALSE;
    ShowWindow(W0, nCmdShow);
    UpdateWindow(W0);
    GetClientRect(W0, &mr);
    CW = mr.right;
    CH = mr.bottom - 8;
    hWndGDE = CCreatePaintBox(hInst, W0, PW_W, TW_H, CW - PW_W - TW_W, CH - TW_H - 8, GDProc);
    if (!hWndGDE) return FALSE;
    GetClientRect(hWndGDE, &gr);
    gde_w = gr.right;
    gde_h = gr.bottom;
    HDC hGDE = GetDC(hWndGDE);
    CDC = CreateCompatibleDC(hGDE);
    CBMP = CreateCompatibleBitmap(hGDE, gde_w, gde_h);
    SelectObject(CDC, (HGDIOBJ)CBMP);
    SelectObject(CDC, GetStockObject(WHITE_BRUSH));
    PatBlt(CDC, 0, 0, gde_w, gde_h, PATCOPY);
    InvalidateRect(hWndGDE, 0, 1);
    CD2 = CreateCompatibleDC(hGDE);
    CBM2 = CreateCompatibleBitmap(hGDE, gde_w, gde_h);
    SelectObject(CD2, (HGDIOBJ)CBM2);
    ImageSave();
    PenFore = CreatePen(0, 0, RGB(0, 0, 0));
    SelectObject(CDC, PenFore);
    PenBack = CreatePen(0, 0, RGB(255, 0, 0));
    SetBkMode(CDC, TRANSPARENT);
    SelectObject(CDC, GetStockObject(NULL_BRUSH));
    ReleaseDC(hWndGDE, hGDE);
    return CreateTools(CW, CH);
}
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    MSG msg;
    if (!RegisterClasses(hInstance)) return FALSE;
    if (!InitInstance(hInstance, nCmdShow)) return FALSE;
    SetINIPath();
    GetSettings();
    loaded = 1;
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (msg.message == WM_KEYDOWN) {
            if (msg.wParam == KEY_SHIFT) {
                shift_on = 1;
            } else if (msg.wParam == 27) {
                SendMessage(W0, WM_DESTROY, 0, 0);
            }
        } else if (msg.message == WM_KEYUP) {
            if (msg.wParam == KEY_SHIFT) {
                shift_on = 0;
            }
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    PutSettings();
    DeleteObject(CBMP);
    DeleteObject(CBM2);
    DeleteDC(CDC);
    DeleteDC(CD2);
    if (PenFore) DeleteObject(PenFore);
    if (PenBack) DeleteObject(PenBack);
    return (int)msg.wParam;
}
