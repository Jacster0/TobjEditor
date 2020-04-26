#include "Window.h"

Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept
	:
	hInst(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc{ 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = SetupProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = nullptr;
	RegisterClassEx(&wc);
}
Window::WindowClass::~WindowClass() {
	UnregisterClass(wndClassName, GetInstance());
}

const char* Window::WindowClass::GetName() noexcept {
	return wndClassName;
}
HINSTANCE Window::WindowClass::GetInstance() noexcept {
	return wndClass.hInst;
}

Window::Window(int width, int height, const char* name) noexcept {
	//calculate window size based on desired client region size
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

	//create window 
	hWnd = CreateWindow(
		WindowClass::GetName(), 
		name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		WindowClass::GetInstance(),
		this
	);

	//Create button
	hWndOpenTobjButton = CreateWindow(
		"BUTTON",        // Predefined class
		"Open Tobj",     // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_FLAT,  // Button Styles 
		5,               // x position 
		10,				 // y position 
		75,				 // Button width
		23,				 // Button height
		hWnd,			 // Parent window
		(HMENU)OPEN_DIALOG_BUTTON, // Assign properties control ID
		WindowClass::GetInstance(),
		this
	);
	SetFont(hWndOpenTobjButton);

	hWndSaveTobjButton = CreateWindow(
		"BUTTON",        // Predefined class
		"Save Tobj",     // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_FLAT,  // Button Styles 
		5,               // x position 
		40,				 // y position 
		75,				 // Button width
		23,				 // Button height
		hWnd,			 // Parent window
		(HMENU)SAVE_DIALOG_BUTTON, // Assign properties control ID
		WindowClass::GetInstance(),
		this
	);
	SetFont(hWndSaveTobjButton);
	//Disable the button so that the user wont try to save an empty file
	EnableWindow(hWndSaveTobjButton, FALSE);

	//create Edit control 
	hWndLoadEdit = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		"EDIT",			  // Predefined class
		"The texturepath will be displayed here. Edit this.", // Edit text 
		 WS_VISIBLE| WS_CHILD | ES_LEFT | ES_AUTOHSCROLL | ES_WANTRETURN,
		95,				  // x position 
		12,				  // y position 
		321,			  // Edit width
		20,				  // Edit height
		hWnd,			  // Parent window
		(HMENU)OPEN_EDIT, // Assign properties control ID
		WindowClass::GetInstance(),
		this
	);
	//Set the font
	SetFont(hWndLoadEdit);

	hWndSaveEdit = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		"EDIT",		// Predefined class
		"The filepath for the new tobjfile will be displayed here",	// Edit text 
		WS_VISIBLE | WS_CHILD | ES_LEFT | ES_AUTOHSCROLL | ES_WANTRETURN,
		95,         // x position 
		42,			// y position 
		321,		// Edit width
		20,			// Edit height
		hWnd,		// Parent window
		(HMENU)SAVE_EDIT, // Assign properties control ID
		WindowClass::GetInstance(),
		this
	); 
	//Set the font
	SetFont(hWndSaveEdit);

	hWndEditStatus = CreateWindow(
		"EDIT",			// Predefined class
		"Status: OK",   // Edit text 
		WS_VISIBLE | WS_CHILD | ES_LEFT | ES_AUTOHSCROLL | ES_WANTRETURN, 
		5,				// x position 
		100,			// y position 
		321,			// Edit width
		20,				// Edit height
		hWnd,			// Parent window
		(HMENU)SAVE_EDIT, // Assign properties control ID
		WindowClass::GetInstance(),
		this
	);

	//Set the font
	SetFont(hWndEditStatus);

	//show and update window
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
}
Window::~Window() {
	DestroyWindow(hWnd);
}

LRESULT CALLBACK Window::SetupProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
	// use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
	if (msg == WM_NCCREATE)
	{
		// extract ptr to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		// set WinAPI-managed user data to store ptr to window instance
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		// set message proc to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::RedirectProc));
		// forward message to window instance handler
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	// if we get a message before the WM_NCCREATE message, handle with default handler
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT CALLBACK Window::RedirectProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
	// retrieve ptr to window instance
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// forward message to window instance handler
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}
LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
	if (msg == WM_CLOSE) {
		PostQuitMessage(0);
		return 0;
	}
	if (msg == WM_PAINT) {
		//Paints the window's client area white
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		EndPaint(hWnd, &ps);
		return 0;
	}
	if (msg == WM_COMMAND) {
		if (wParam == OPEN_DIALOG_BUTTON) {
				//Button is clicked
			if (OpenFile(hWnd)) {
				SetWindowText(hWndLoadEdit, t_object.GetTexturePath().c_str());
				//We can now enable the save button so the user can save the file.
				EnableWindow(hWndSaveTobjButton, TRUE); 
			}
			else {
				SetWindowText(hWndLoadEdit, "Unable to open/read file");
			}
		}
		if (wParam == SAVE_DIALOG_BUTTON)
			try {
			if (SaveFile(hWnd)){
				//Button is clicked
				SetWindowText(hWndSaveEdit, t_object.GetFullPath().c_str());
				SetWindowText(hWndEditStatus, "Status: File saved!");
			}
			else {
				SetWindowText(hWndEditStatus, "Status: Save canceled - file NOT saved!");
			}
		}
		catch (...) {
			SetWindowText(hWndEditStatus, "Status: Error occured");
		}
	}
	else
		return DefWindowProc(hWnd, msg, wParam, lParam);
	return 0;
}

void Window::SetFont(HWND hWnd)
{
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	hFont = CreateFont(
		lf.lfHeight,
		lf.lfWidth,
		lf.lfEscapement, 
		lf.lfOrientation,
		lf.lfWeight,
		lf.lfItalic, 
		lf.lfUnderline, 
		lf.lfStrikeOut, 
		lf.lfCharSet,
		lf.lfOutPrecision,
		lf.lfClipPrecision,
		lf.lfQuality,
		lf.lfPitchAndFamily,
		lf.lfFaceName
	);

	SendMessage(hWnd, WM_SETFONT, (WPARAM)hFont, TRUE);

}

bool Window::OpenFile(HWND hWnd) {
	OPENFILENAME ofn;
	
	char fileName[MAX_PATH];
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize		= sizeof(ofn);
	ofn.hwndOwner		= hWnd; //Parent window
	ofn.lpstrFile		= fileName; 
	ofn.lpstrFile[0]	= '\0'; // initilize fileName with a null terminated line
	ofn.nMaxFile		= MAX_PATH;
	ofn.lpstrFilter		= "TOBJ Files\0*.tobj\0";
	ofn.nFilterIndex	= 1; 

	if (GetOpenFileName(&ofn))
		return t_object.Load(std::string(fileName));
	return false;
}
bool Window::SaveFile(HWND hWnd) {
	char buffer[MAX_PATH];
	//Load Edit control's text content into the buffer
	GetWindowText(hWndLoadEdit, buffer, MAX_PATH);

	OPENFILENAME ofn;
	char fileName[MAX_PATH];
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd; //Parent window
	ofn.lpstrFile = fileName;
	ofn.lpstrFile[0] = '\0'; // initilize fileName with a null terminated line
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = "TOBJ Files\0*.tobj\0";
	ofn.nFilterIndex = 1; 
	ofn.lpstrDefExt = "tobj";

	if (GetSaveFileName(&ofn))
		return t_object.Save(std::string(fileName), std::string(buffer));
	return false;
}

std::optional<int> Window::ProcessMessages() {
	MSG msg;
	//while queue has messages, remove and dispatch them, but do not block on empty queue
	while ( PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT)
			return msg.wParam;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return {};
}