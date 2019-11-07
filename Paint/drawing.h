/* 2018 ReVoL Primer Template */
/* drawing.h */
#pragma once
/* установить значение 2 для геометрического пера */
#define MAX_PEN_TYPE 2
/* текущий инструмент */
TOOLS painting_tool = TOOL_MOUSE;
/* текущий цвет */
COLORREF fore_color = RGB(0, 0, 0);
/* текущий фон */
COLORREF back_color = RGB(255, 255, 255);
/* перо для плана */
HPEN PenFore = 0;
/* перо для фона */
HPEN PenBack = 0;
/* тип пера */
int pen_type = PS_COSMETIC;
/* тип линии тонкого косметического пера */
int line_style = PS_SOLID;
/* размер пера */
int pen_size = 1;
/* кисть геометрического пера */
LOGBRUSH PenBrush;
/* тип концевой точки */
int pen_endcap = PS_ENDCAP_FLAT;
/* тип соединения */
int pen_jointy = PS_JOIN_ROUND;
/* тип кисти пера */
int g_style = BS_SOLID;
/* тип штриховки пера */
int g_hatch = HS_CROSS;
/* признак полилинии */
int poly_left = 0, poly_right = 0;

/* вычисляет направление движения мыши */
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

/* режим фона */
void SetBackStyle(COLORREF color) {
    if (backstyle_mode == 1) {
        /* режим фона прозрачный */
		SetBkMode(CDC, TRANSPARENT);
		// выбираем в контекст нулевую кисть стока
		SelectObject(CDC, GetStockObject(NULL_BRUSH));
    } else {
        /* режим фона непрозрачный */
		SetBkMode(CDC, OPAQUE);
		// цвет фона
		SetBkColor(CDC, color);
		// выбираем в контекст стандартную кисть стока
		SelectObject(CDC, GetStockObject(DC_BRUSH));
		// цвет кисти
		SetDCBrushColor(CDC, color);
    }
}

/* событие WM_LBUTTONDOWN */
int LeftButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	// нажата левая кнопка
	mouse_down = WM_LBUTTONDOWN;
	// захватываем события мыши
	SetCapture(hWnd);
	// координаты мыши
	GetMouse(MX1, MY1, wParam, lParam);
	// текущая точка рисования
	MoveToEx(CDC, MX1, MY1, 0);
	// выбираем перо плана
	SelectObject(CDC, PenFore);
	// сохраним изображение
	ImageSave();
	// если полилиния
	if (painting_tool == TOOL_POLY && poly_right == 0) poly_left = 1;
	poly_right = 0;
	// режим фона
	SetBackStyle(back_color);
	// событие обработано
    return 0;
}

/* событие WM_RBUTTONDOWN */
int RightButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	// нажата правая кнопка
	mouse_down = WM_RBUTTONDOWN;
	// захватываем события мыши
	SetCapture(hWnd);
	// координаты мыши
	GetMouse(MX1, MY1, wParam, lParam);
	// текущая точка рисования
	MoveToEx(CDC, MX1, MY1, 0);
	// выбираем перо плана
	SelectObject(CDC, PenBack);
	// сохраним изображение
	ImageSave();
	// событие обработано
    return 0;
}

/* событие WM_LBUTTONUP */
int LeftButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	// если полилиния, держим захват
	if (poly_left) return 0;
	// мышь отпущена
	mouse_down = 0;
	// освобождаем захват мыши
	ReleaseCapture();
	// событие обработано
    return 0;
}

/* событие WM_RBUTTONUP */
int RightButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	// мышь отпущена
	mouse_down = 0;
	// освобождаем захват мыши
	ReleaseCapture();
	// событие обработано
    return 0;
}

/* создает новое перо плана или фона */
void CreateNewPen(HPEN & Pen, COLORREF color) {
	// удаляем предыдущее перо
	DeleteObject(Pen);
    // создаем перо
    if (pen_type == PS_COSMETIC) {
        // косметическое перо
		Pen = CreatePen(line_style, pen_size, color);
    } else {
        // геометрическое перо:
		//		кисть для закраски линии(сплошная, штрихованная)
		PenBrush.lbColor = color;
		PenBrush.lbStyle = g_style;
		PenBrush.lbHatch = g_hatch;
		int style = PS_GEOMETRIC | line_style | pen_endcap | pen_jointy;
		Pen = ExtCreatePen(style, pen_size, &PenBrush, 0, 0);
    }
}

/* рисует линию */
void DrawMouse(short & X1, short & Y1, short X2, short Y2) {
	RECT rc;
	rc.left = min(X1, X2) - pen_size;
	rc.top = min(Y1, Y2) - pen_size;
	rc.right = max(X1, X2) + pen_size;
	rc.bottom = max(Y1, Y2) + pen_size;
	// обновление координат
	X1 = X2; Y1 = Y2;
	// рисует отрезок прямой
	LineTo(CDC, X2, Y2);
	// обновляем изображение
	InvalidateRect(hWndGDE, &rc, 0);
}

/* рисует прямую */
void DrawLine(short X1, short Y1, short X2, short Y2) {
	// восстанавливаем изображение
	ImageRestore();
	// устанавливаем текущую точку
	MoveToEx(CDC, X1, Y1, 0);
	// рисуем отрезок до нового поколения
	LineTo(CDC, X2, Y2);
	// обновляем окно графического вывода
	InvalidateRect(hWndGDE, 0, 0);
}

/* рисует прямоугольник */
void DrawRect(short X1, short Y1, short X2, short Y2) {

	ImageRestore();
	Rectangle(CDC, X1, Y1, X2, Y2);
	InvalidateRect(hWndGDE, 0, 0);
}

/* рисует овал */
void DrawOval(short X1, short Y1, short X2, short Y2) {

	ImageRestore();
	Ellipse(CDC, X1, Y1, X2, Y2);
	
	// обновляем окно графического вывода
	InvalidateRect(hWndGDE, 0, 0);
}

/* рисует прямую по алгоритму Брезенхейма */
void DrawBrez(short X1, short Y1, short X2, short Y2) {
	ImageRestore();

	COLORREF color;
	if (mouse_down == WM_LBUTTONDOWN)
		color = fore_color;
	else
		color = back_color;

	int DX, DY, X, Y, D, D1, D2, SX, SY;
	// определяемся в какую сторону будем двигаться
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
	// следствие пропуска точек при большом угле наклона прямой
	// переворачиваем
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
	// обновляем окно графического вывода
	InvalidateRect(hWndGDE, 0, 0);
}

// Этот код "рисует" все 9 видов отрезков. Наклонные (из начала в конец и из конца в начало каждый), 
//				вертикальный и горизонтальный - тоже из начала в конец и из конца в начало, и точку.
int sign(int x) {
	return (x > 0) ? 1 : (x < 0) ? -1 : 0;
	//возвращает 0, если аргумент (x) равен нулю; -1, если x < 0 и 1, если x > 0.
}


//Целая часть числа
int IPart(float x)
{
	return (int)x;
}
//дробная часть числа
float FPart(float x)
{
	while (x >= 0)
		x--;
	x++;
	return x;
}

/* рисует прямую по алгоритму Ву */
void DrawBWoo(short X1, short Y1, short X2, short Y2) {
	ImageRestore();

	COLORREF color;
	if (mouse_down == WM_LBUTTONDOWN)
		color = fore_color;
	else
		color = back_color;

	//Вычисление изменения координат
	int dx = (X2 > X1) ? (X2 - X1) : (X1 - X2);
	int dy = (Y2 > Y1) ? (Y2 - Y1) : (Y1 - Y2);

	int rgb = RGB(255, 255, 255);

	//Если линия параллельна одной из осей, рисуем обычную линию - заполняем все пикселы в ряд
	if (dx == 0 || dy == 0)
	{
		//g.DrawLine(new Pen(clr), x0, y0, x1, y1);
		//return;
	}

	//Для Х-линии (коэффициент наклона < 1)
	if (dy < dx)
	{
		//Первая точка должна иметь меньшую координату Х
		if (X2 < X1)
		{
			X2 += X1; X1 = X2 - X1; X2 -= X1;
			Y2 += Y1; Y1 = Y2 - Y1; Y2 -= Y1;
		}
		//Относительное изменение координаты Y
		float grad = (float)dy / dx;
		if (Y2 < Y1)
			grad = -grad;
		//Промежуточная переменная для Y
		float intery = Y1 + grad;
		//Первая точка
		SetPixel(CDC, X1, Y1, color);

		for (int x = X1 + 1; x < X2; x++)
		{

			//Верхняя точка
			SetPixel(CDC, x, IPart(intery) + 1, COLORREF((int)(255 - (FPart(intery) * 255)) |
				(int)(255 - (FPart(intery) * 255)) << 8 |
				(int)(255 - (FPart(intery) * 255)) << 16));
			//Нижняя точка
			SetPixel(CDC, x, IPart(intery), COLORREF((int)(FPart(intery) * 255) |
				((int)(FPart(intery) * 255) << 8) |
				((int)(FPart(intery) * 255)) << 16));
			//Изменение координаты Y
			intery += grad;
		}
		//Последняя точка
		SetPixel(CDC, X2, Y2, color);
	//Для Y-линии (коэффициент наклона > 1)
	} else {

		//Первая точка должна иметь меньшую координату Y
		if (Y2 < Y1)
		{
			X2 += X1; X1 = X2 - X1; X2 -= X1;
			Y2 += Y1; Y1 = Y2 - Y1; Y2 -= Y1;
		}
		//Относительное изменение координаты X
		float grad = (float)dx / dy;
		if (X2 < X1)
			grad = -grad;
		//Промежуточная переменная для X
		float interx = X1 + grad;
		//Первая точка
		SetPixel(CDC, X1, Y1, color);
		
		for (int y = Y1 + 1; y < Y2; y++)
		{
			//Верхняя точка
			SetPixel(CDC, IPart(interx) + 1, y, COLORREF(255 - (int)(FPart(interx) * 255) |
				(255 - (int)(FPart(interx) * 255) << 8) |
				(255 - (int)(FPart(interx) * 255) << 16)));
			//Нижняя точка
			SetPixel(CDC, IPart(interx), y, COLORREF((int)(FPart(interx) * 255) |
				((int)(FPart(interx) * 255) << 8) |
				((int)(FPart(interx) * 255)) << 16));
			//Изменение координаты X
			interx += grad;
		}
		//Последняя точка
		SetPixel(CDC, X2, Y2, color);
	}
	
	// обновляем окно графического вывода
	InvalidateRect(hWndGDE, 0, 0);
}

