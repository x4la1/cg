#include "TestWinApi.h"
#include "framework.h"

TCHAR const CLASS_NAME[] = _T("MainWndClass");
TCHAR const WINDOW_TITLE[] = _T("My first window");


const double M_PI = 3.14159265;

const double LETTER_SIZE = 250.0;

const double JUMP_AMPLITUDE = 80.0;
const double JUMP_PERIOD = 1.5;

const double FIRST_LETTER_PHASE_OFFSET = 0.0;
const double SECOND_LETTER_PHASE_OFFSET = 0.3;
const double THIRD_LETTER_PHASE_OFFSET = 0.6;

enum
{
	ANIMATION_TIMER_ID = 1
};

DWORD g_lastTick;
UINT_PTR g_timerId = 0;
double g_animationTime = 0;

double CalculateBounceOffset(double time, double phaseOffset)
{
	double adjustedTime = time + phaseOffset;
	double angle = (adjustedTime / JUMP_PERIOD) * 2.0 * M_PI;

	return JUMP_AMPLITUDE * sin(angle);
}

void DrawLetterL(HDC dc, int centerX, int centerY, int size)
{
	HPEN pen = CreatePen(PS_SOLID, 3, RGB(255, 255, 255));
	HPEN oldPen = SelectPen(dc, pen);
	HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));
	HBRUSH oldBrush = SelectBrush(dc, brush);

	double halfSize = size / 2;
	POINT letterPoints[] = {
		{ centerX, static_cast<LONG>(centerY - halfSize) },
		{ static_cast<LONG>(centerX + halfSize), static_cast<LONG>(centerY + halfSize) },
		{ static_cast<LONG>(centerX + (halfSize * 0.6)), static_cast<LONG>(centerY + halfSize) },
		{ centerX, centerY - (halfSize * 0.2) },
		{ static_cast<LONG>(centerX - (halfSize * 0.6)), static_cast<LONG>(centerY + halfSize) },
		{ static_cast<LONG>(centerX - halfSize), static_cast<LONG>(centerY + halfSize) },
	};

	Polygon(dc, letterPoints, 6);

	SelectPen(dc, oldPen);
	SelectBrush(dc, oldBrush);
	DeletePen(pen);
	DeleteBrush(brush);
}

void DrawLetterD(HDC dc, int centerX, int centerY, int size)
{
	HPEN pen = CreatePen(PS_SOLID, 3, RGB(0, 0, 255));
	HPEN oldPen = SelectPen(dc, pen);
	HBRUSH brush = CreateSolidBrush(RGB(0, 0, 255));
	HBRUSH oldBrush = SelectBrush(dc, brush);

	double halfSize = size / 2;

	Rectangle(
		dc,
		static_cast<int>(centerX - halfSize),
		static_cast<int>(centerY + (halfSize * 0.4)),
		static_cast<int>(centerX + halfSize),
		static_cast<int>(centerY + (halfSize * 0.8)));

	Rectangle(
		dc,
		static_cast<int>(centerX - halfSize),
		static_cast<int>(centerY + (halfSize * 0.8)),
		static_cast<int>(centerX - (halfSize * 0.6)),
		static_cast<int>(centerY + halfSize));

	Rectangle(
		dc,
		static_cast<int>(centerX + (halfSize * 0.6)),
		static_cast<int>(centerY + (halfSize * 0.8)),
		static_cast<int>(centerX + halfSize),
		static_cast<int>(centerY + halfSize));

	Rectangle(
		dc,
		static_cast<int>(centerX - (halfSize * 0.6)),
		static_cast<int>(centerY - halfSize),
		static_cast<int>(centerX + (halfSize * 0.6)),
		static_cast<int>(centerY - (halfSize * 0.6)));

	Rectangle(
		dc,
		static_cast<int>(centerX - (halfSize * 0.6)),
		static_cast<int>(centerY - (halfSize * 0.6)),
		static_cast<int>(centerX - (halfSize * 0.2)),
		static_cast<int>(centerY + (halfSize * 0.4)));

	Rectangle(
		dc,
		static_cast<int>(centerX + (halfSize * 0.2)),
		static_cast<int>(centerY - (halfSize * 0.6)),
		static_cast<int>(centerX + (halfSize * 0.6)),
		static_cast<int>(centerY + (halfSize * 0.4)));

	SelectPen(dc, oldPen);
	SelectBrush(dc, oldBrush);
	DeletePen(pen);
	DeleteBrush(brush);
}

void DrawLetterA(HDC dc, int centerX, int centerY, int size)
{
	HPEN pen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
	HPEN oldPen = SelectPen(dc, pen);
	HBRUSH brush = CreateSolidBrush(RGB(255, 0, 0));
	HBRUSH oldBrush = SelectBrush(dc, brush);

	double halfSize = size / 2;
	POINT letterPoints[] = {
		{ centerX, static_cast<LONG>(centerY - halfSize) },
		{ static_cast<LONG>(centerX + halfSize), static_cast<LONG>(centerY + halfSize) },
		{ static_cast<LONG>(centerX + (halfSize * 0.6)), static_cast<LONG>(centerY + halfSize) },
		{ centerX, centerY - (halfSize * 0.2) },
		{ static_cast<LONG>(centerX - (halfSize * 0.6)), static_cast<LONG>(centerY + halfSize) },
		{ static_cast<LONG>(centerX - halfSize), static_cast<LONG>(centerY + halfSize) },
	};

	Polygon(dc, letterPoints, 6);
	Rectangle(
		dc,
		static_cast<int>(centerX - (halfSize * 0.4)),
		static_cast<int>(centerY + (halfSize * 0.4)),
		static_cast<int>(centerX + (halfSize * 0.4)),
		static_cast<int>(centerY + (halfSize * 0.6)));

	SelectPen(dc, oldPen);
	SelectBrush(dc, oldBrush);
	DeletePen(pen);
	DeleteBrush(brush);
}

void Animate(HWND hwnd)
{
	DWORD currentTick = GetTickCount64();
	int delta = currentTick - g_lastTick;
	g_lastTick = currentTick;

	double deltaSeconds = delta * 0.001;
	g_animationTime += deltaSeconds;

	InvalidateRect(hwnd, NULL, TRUE);
	UpdateWindow(hwnd);
}

void OnDestroy(HWND hwnd)
{
	KillTimer(hwnd, ANIMATION_TIMER_ID);
	PostQuitMessage(0);
}

void OnPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC dc = BeginPaint(hwnd, &ps);

	RECT rcClient;
	GetClientRect(hwnd, &rcClient);
	int centerX = rcClient.right / 2;
	int centerY = rcClient.bottom / 2;

	double firstOffset = CalculateBounceOffset(g_animationTime, FIRST_LETTER_PHASE_OFFSET);
	double secondOffset = CalculateBounceOffset(g_animationTime, SECOND_LETTER_PHASE_OFFSET);
	double thirdOffset = CalculateBounceOffset(g_animationTime, THIRD_LETTER_PHASE_OFFSET);

	DrawLetterL(dc, centerX - LETTER_SIZE * 1.5, centerY + firstOffset, LETTER_SIZE);
	DrawLetterD(dc, centerX, centerY + secondOffset, LETTER_SIZE);
	DrawLetterA(dc, centerX + LETTER_SIZE * 1.5, centerY + thirdOffset, LETTER_SIZE);

	EndPaint(hwnd, &ps);
}

void OnTimer(HWND hwnd, UINT id)
{
	switch (id)
	{
	case ANIMATION_TIMER_ID:
		Animate(hwnd);
		break;
	}
}

BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
	g_lastTick = GetTickCount64();
	g_timerId = SetTimer(hwnd, ANIMATION_TIMER_ID, 20, NULL);

	return (g_timerId != 0);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		HANDLE_MSG(hwnd, WM_DESTROY, OnDestroy);
		HANDLE_MSG(hwnd, WM_PAINT, OnPaint);
		HANDLE_MSG(hwnd, WM_TIMER, OnTimer);
		HANDLE_MSG(hwnd, WM_CREATE, OnCreate);
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

bool RegisterWndClass(HINSTANCE hInstance)
{
	WNDCLASSEX wndClass = {
		sizeof(wndClass), // UINT cbSize;
		CS_HREDRAW | CS_VREDRAW, // UINT style;
		&WindowProc, // WNDPROC lpfnWndProc;
		0, // int cbClsExtra;
		0, // int cbWndExtra;
		hInstance, // HINSTANCE hInstance;
		NULL, // HICON hIcon;
		LoadCursor(NULL, IDC_ARROW), // HCURSOR hCursor;
		(HBRUSH)GetStockObject(GRAY_BRUSH), // HBRUSH hbrBackground;
		NULL, // LPCTSTR lpszMenuName;
		CLASS_NAME, // LPCTSTR lpszClassName;
		NULL, // HICON hIconSm;
	};

	return RegisterClassEx(&wndClass) != FALSE;
}

HWND CreateMainWindow(HINSTANCE hInstance)
{
	HWND hMainWindow = CreateWindowEx(
		0,
		CLASS_NAME,
		WINDOW_TITLE,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL);

	return hMainWindow;
}

int MainLoop()
{
	MSG msg;
	BOOL res;
	while ((res = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (res == -1)
		{
			return 1;
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow)
{
	if (!RegisterWndClass(hInstance))
	{
		return 1;
	}

	HWND hMainWindow = CreateMainWindow(hInstance);
	if (hMainWindow == NULL)
	{
		return 1;
	}

	ShowWindow(hMainWindow, nCmdShow);
	UpdateWindow(hMainWindow);

	return (int)MainLoop();
}