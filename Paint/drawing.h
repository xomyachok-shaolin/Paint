/* 2018 ReVoL Primer Template */
/* drawing.h */
#pragma once
/* ���������� �������� 2 ��� ��������������� ���� */
#define MAX_PEN_TYPE 2
/* ������� ���������� */
TOOLS painting_tool = TOOL_MOUSE;
/* ������� ���� */
COLORREF fore_color = RGB(0, 0, 0);
/* ������� ��� */
COLORREF back_color = RGB(255, 255, 255);
/* ���� ��� ����� */
HPEN PenFore = 0;
/* ���� ��� ���� */
HPEN PenBack = 0;
/* ��� ���� */
int pen_type = PS_COSMETIC;
/* ��� ����� ������� �������������� ���� */
int line_style = PS_SOLID;
/* ������ ���� */
int pen_size = 1;
/* ����� ��������������� ���� */
LOGBRUSH PenBrush;
/* ��� �������� ����� */
int pen_endcap = PS_ENDCAP_FLAT;
/* ��� ���������� */
int pen_jointy = PS_JOIN_ROUND;
/* ��� ����� ���� */
int g_style = BS_SOLID;
/* ��� ��������� ���� */
int g_hatch = HS_CROSS;
/* ������� ��������� */
int poly_left = 0, poly_right = 0;

/* ��������� ����������� �������� ���� */
void ShiftMouse(short X1, short Y1, short & X2, short & Y2) {
	int DX = X2 - X1;
	int DY = Y2 - Y2;
	int line = painting_tool == TOOL_LINE;
	int rect = painting_tool == TOOL_RECT || painting_tool == TOOL_OVAL;
	if (!shift_on) return;
	if (abs(DX) > abs(DY)) {
		if (line) {
			Y2 = Y1;
		} else if (rect) {
			Y2 = Y1; X2 = X1;
		}
	} else {
		if (line) {
			X2 = X1;
		} else if (rect) {
			Y2 = X1; X2 = Y1;
		}
	}
}

/* ����� ���� */
void SetBackStyle(COLORREF color) {
    if (backstyle_mode == 1) {
        /* ����� ���� ���������� */
		SetBkMode(CDC, TRANSPARENT);
		// �������� � �������� ������� ����� �����
		SelectObject(CDC, GetStockObject(NULL_BRUSH));
    } else {
        /* ����� ���� ������������ */
		SetBkMode(CDC, OPAQUE);
		// ���� ����
		SetBkColor(CDC, color);
		// �������� � �������� ����������� ����� �����
		SelectObject(CDC, GetStockObject(DC_BRUSH));
		// ���� �����
		SetDCBrushColor(CDC, color);
    }
}

/* ������� WM_LBUTTONDOWN */
int LeftButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	// ������ ����� ������
	mouse_down = WM_LBUTTONDOWN;
	// ����������� ������� ����
	SetCapture(hWnd);
	// ���������� ����
	GetMouse(MX1, MY1, wParam, lParam);
	// ������� ����� ���������
	MoveToEx(CDC, MX1, MY1, 0);
	// �������� ���� �����
	SelectObject(CDC, PenFore);
	// �������� �����������
	ImageSave();
	// ���� ���������
	if (painting_tool == TOOL_POLY && poly_right == 0) poly_left = 1;
	poly_right = 0;
	// ����� ����
	SetBackStyle(back_color);
	// ������� ����������
    return 0;
}

/* ������� WM_RBUTTONDOWN */
int RightButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	// ������ ������ ������
	mouse_down = WM_RBUTTONDOWN;
	// ����������� ������� ����
	SetCapture(hWnd);
	// ���������� ����
	GetMouse(MX1, MY1, wParam, lParam);
	// ������� ����� ���������
	MoveToEx(CDC, MX1, MY1, 0);
	// �������� ���� �����
	SelectObject(CDC, PenBack);
	// �������� �����������
	ImageSave();
	// ������� ����������
    return 0;
}

/* ������� WM_LBUTTONUP */
int LeftButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	// ���� ���������, ������ ������
	if (poly_left) return 0;
	// ���� ��������
	mouse_down = 0;
	// ����������� ������ ����
	ReleaseCapture();
	// ������� ����������
    return 0;
}

/* ������� WM_RBUTTONUP */
int RightButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	// ���� ��������
	mouse_down = 0;
	// ����������� ������ ����
	ReleaseCapture();
	// ������� ����������
    return 0;
}

/* ������� ����� ���� ����� ��� ���� */
void CreateNewPen(HPEN & Pen, COLORREF color) {
	// ������� ���������� ����
	DeleteObject(Pen);
    // ������� ����
    if (pen_type == PS_COSMETIC) {
        // ������������� ����
		Pen = CreatePen(line_style, pen_size, color);
    } else {
        // �������������� ����:
		//		����� ��� �������� �����(��������, ������������)
		PenBrush.lbColor = color;
		PenBrush.lbStyle = g_style;
		PenBrush.lbHatch = g_hatch;
		int style = PS_GEOMETRIC | line_style | pen_endcap | pen_jointy;
		Pen = ExtCreatePen(style, pen_size, &PenBrush, 0, 0);
    }
}

/* ������ ����� */
void DrawMouse(short & X1, short & Y1, short X2, short Y2) {
	RECT rc;
	rc.left = min(X1, X2) - pen_size;
	rc.top = min(Y1, Y2) - pen_size;
	rc.right = max(X1, X2) + pen_size;
	rc.bottom = max(Y1, Y2) + pen_size;
	// ���������� ���������
	X1 = X2; Y1 = Y2;
	// ������ ������� ������
	LineTo(CDC, X2, Y2);
	// ��������� �����������
	InvalidateRect(hWndGDE, &rc, 0);
}

/* ������ ������ */
void DrawLine(short X1, short Y1, short X2, short Y2) {
	// ��������������� �����������
	ImageRestore();
	// ������������� ������� �����
	MoveToEx(CDC, X1, Y1, 0);
	// ������ ������� �� ������ ���������
	LineTo(CDC, X2, Y2);
	// ��������� ���� ������������ ������
	InvalidateRect(hWndGDE, 0, 0);
}

/* ������ ������������� */
void DrawRect(short X1, short Y1, short X2, short Y2) {

	ImageRestore();
	Rectangle(CDC, X1, Y1, X2, Y2);
	InvalidateRect(hWndGDE, 0, 0);
}

/* ������ ���� */
void DrawOval(short X1, short Y1, short X2, short Y2) {

	ImageRestore();
	Ellipse(CDC, X1, Y1, X2, Y2);
	
	// ��������� ���� ������������ ������
	InvalidateRect(hWndGDE, 0, 0);
}

/* ������ ������ �� ��������� ����������� */
void DrawBrez(short X1, short Y1, short X2, short Y2) {
	ImageRestore();

	COLORREF color;
	if (mouse_down == WM_LBUTTONDOWN)
		color = fore_color;
	else
		color = back_color;

	int DX, DY, X, Y, D, D1, D2, SX, SY;
	// ������������ � ����� ������� ����� ���������
	DX = abs(X2 - X1);	DY = abs(Y2 - Y1);
	if (X1 < X2) SX = 1; else SX = -1;
	if (Y1 < Y2) SY = 1; else SY = -1;

	X = X1;	Y = Y1;
	SetPixel(CDC, X, Y, color);
	if (DY > DX) {
		D1 = DX + DX;
		D = D1 - DY;
		D2 = D - DY;
		for (int Y(Y1 + SY); Y != Y2; Y += SY) {
			if (D > 0) {
				D += D2;
				X += SX;
			} else
				D += D1;
			SetPixel(CDC, X, Y, color);
		}
	// ��������� �������� ����� ��� ������� ���� ������� ������
	// ��������������
	} else {
		D1 = DY + DY;
		D = D1 - DX;
		D2 = D - DX;
		for (int X(X1 + SX); X != X2; X += SX) {
			if (D > 0) {
				D += D2;
				Y += SY;
			} else
				D += D1;
			SetPixel(CDC, X, Y, color);
		}
	}
	// ��������� ���� ������������ ������
	InvalidateRect(hWndGDE, 0, 0);
}

// ���� ��� "������" ��� 9 ����� ��������. ��������� (�� ������ � ����� � �� ����� � ������ ������), 
//				������������ � �������������� - ���� �� ������ � ����� � �� ����� � ������, � �����.
int sign(int x) {
	return (x > 0) ? 1 : (x < 0) ? -1 : 0;
	//���������� 0, ���� �������� (x) ����� ����; -1, ���� x < 0 � 1, ���� x > 0.
}


//����� ����� �����
int IPart(float x)
{
	return (int)x;
}
//������� ����� �����
float FPart(float x)
{
	while (x >= 0)
		x--;
	x++;
	return x;
}

/* ������ ������ �� ��������� �� */
void DrawBWoo(short X1, short Y1, short X2, short Y2) {
	ImageRestore();

	COLORREF color;
	if (mouse_down == WM_LBUTTONDOWN)
		color = fore_color;
	else
		color = back_color;

	//���������� ��������� ���������
	int dx = (X2 > X1) ? (X2 - X1) : (X1 - X2);
	int dy = (Y2 > Y1) ? (Y2 - Y1) : (Y1 - Y2);

	int rgb = RGB(255, 255, 255);

	//���� ����� ����������� ����� �� ����, ������ ������� ����� - ��������� ��� ������� � ���
	if (dx == 0 || dy == 0)
	{
		//g.DrawLine(new Pen(clr), x0, y0, x1, y1);
		//return;
	}

	//��� �-����� (����������� ������� < 1)
	if (dy < dx)
	{
		//������ ����� ������ ����� ������� ���������� �
		if (X2 < X1)
		{
			X2 += X1; X1 = X2 - X1; X2 -= X1;
			Y2 += Y1; Y1 = Y2 - Y1; Y2 -= Y1;
		}
		//������������� ��������� ���������� Y
		float grad = (float)dy / dx;
		if (Y2 < Y1)
			grad = -grad;
		//������������� ���������� ��� Y
		float intery = Y1 + grad;
		//������ �����
		SetPixel(CDC, X1, Y1, color);

		for (int x = X1 + 1; x < X2; x++)
		{

			//������� �����
			SetPixel(CDC, x, IPart(intery) + 1, COLORREF((int)(255 - (FPart(intery) * 255)) |
				(int)(255 - (FPart(intery) * 255)) << 8 |
				(int)(255 - (FPart(intery) * 255)) << 16));
			//������ �����
			SetPixel(CDC, x, IPart(intery), COLORREF((int)(FPart(intery) * 255) |
				((int)(FPart(intery) * 255) << 8) |
				((int)(FPart(intery) * 255)) << 16));
			//��������� ���������� Y
			intery += grad;
		}
		//��������� �����
		SetPixel(CDC, X2, Y2, color);
	//��� Y-����� (����������� ������� > 1)
	} else {

		//������ ����� ������ ����� ������� ���������� Y
		if (Y2 < Y1)
		{
			X2 += X1; X1 = X2 - X1; X2 -= X1;
			Y2 += Y1; Y1 = Y2 - Y1; Y2 -= Y1;
		}
		//������������� ��������� ���������� X
		float grad = (float)dx / dy;
		if (X2 < X1)
			grad = -grad;
		//������������� ���������� ��� X
		float interx = X1 + grad;
		//������ �����
		SetPixel(CDC, X1, Y1, color);
		
		for (int y = Y1 + 1; y < Y2; y++)
		{
			//������� �����
			SetPixel(CDC, IPart(interx) + 1, y, COLORREF(255 - (int)(FPart(interx) * 255) |
				(255 - (int)(FPart(interx) * 255) << 8) |
				(255 - (int)(FPart(interx) * 255) << 16)));
			//������ �����
			SetPixel(CDC, IPart(interx), y, COLORREF((int)(FPart(interx) * 255) |
				((int)(FPart(interx) * 255) << 8) |
				((int)(FPart(interx) * 255)) << 16));
			//��������� ���������� X
			interx += grad;
		}
		//��������� �����
		SetPixel(CDC, X2, Y2, color);
	}
	
	// ��������� ���� ������������ ������
	InvalidateRect(hWndGDE, 0, 0);
}

