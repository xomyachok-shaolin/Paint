// 2018 ReVoL Primer Template
// Paint.h
#pragma once
#include "targetver.h"
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "ccreate.h"
#include "settings.h"
#include "resource.h"
// инструменты
enum TOOLS {
    TOOL_NULL = 0,
    TOOL_MOUSE,
    TOOL_LINE,
    TOOL_POLY,
    TOOL_RECT,
    TOOL_OVAL,
    TOOL_BREZ,
    TOOL_BWOO
};
// цвета 16-цветной палитры
COLORREF C16[18] = {
    RGB(0,   0,   0),
    RGB(0,   0, 128),
    RGB(0, 128,   0),
    RGB(128,   0,   0),
    RGB(0, 128, 128),
    RGB(128,   0, 128),
    RGB(128, 128,   0),
    RGB(128, 128, 128),
    RGB(192, 192, 192),
    RGB(0,   0, 255),
    RGB(0, 255,   0),
    RGB(255,   0,   0),
    RGB(0, 255, 255),
    RGB(255,   0, 255),
    RGB(255, 255,   0),
    RGB(255, 255, 255),
    RGB(0,   0,   0),
    RGB(255, 255, 255)
};
HWND WCT[10];
int pentyp[3] = { 0, PS_COSMETIC, PS_GEOMETRIC };
HWND WPT[3];
int pensiz[4] = {0, 1, 11, 25};
HWND WPS[4];
int linsty[5] = { 0, PS_SOLID, PS_DASH, PS_DOT, PS_DASHDOT };
HWND WLS[5];
int endcap[4] = { 0, PS_ENDCAP_ROUND, PS_ENDCAP_SQUARE, PS_ENDCAP_FLAT };
HWND WEC[4];
int jointy[4] = { 0, PS_JOIN_ROUND, PS_JOIN_BEVEL, PS_JOIN_MITER };
HWND WJS[4];
#define HS_SOLID 6
int brusty[8] = { 0, HS_SOLID, HS_HORIZONTAL, HS_VERTICAL, HS_FDIAGONAL, HS_BDIAGONAL, HS_CROSS, HS_DIAGCROSS };
HWND WHS[8];
HWND WCAN, WCLR, WCLW;
HWND W0;
HWND WBM[3];
HWND WPA[19];
#define CW0 860
#define CH0 598
#define TW_H 56
#define TW_W 56
#define BW_W 40
#define BW_H 40
#define PW_W 79
#define PAL 31
#define PA_FC 16
#define PA_BC 17
#define PA_TW 18
#define PAD  8
#define KEY_SHIFT 16
HINSTANCE hInst;
TCHAR szIniPath[260];
HWND hWndGDE;
HDC CDC;
HDC CD2;
HBITMAP CBMP;
HBITMAP CBM2;
HFONT tahoma;
int mouse_down = 0;
short MX0, MY0;
short MX1, MY1;
short MX2, MY2;
int direct = 0;
int shift_on = 0;
int loaded = 0;
// переменные для записи в ini-файл
int fore_color_index = 0;
int back_color_index = 15;
int backstyle_mode = 0;
int pen_type_index = 1;
int pen_size_index = 1;
int line_style_index = 1;
int pen_endcap_index = 1;
int pen_jointy_index = 1;
int pen_hatch_index = 1;
// размер устройства графического вывода
int gde_w;
int gde_h;
