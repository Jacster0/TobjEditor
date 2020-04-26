#pragma once
#define _WIN32_WINNT_WINBLUE 0x0603 //Windows version 8.1 or later
#include <sdkddkver.h>

//Disable a bunch of unused windows stuff to decrease buildtime.
#define NOGDICAPMASKS
#define NOSYSMETRICS
#define NOMENUS
#define NOICONS
#define NOSYSCOMMANDS
#define NORASTEROPS
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
#define NODRAWTEXT
#define NOKERNEL
#define NONLS
#define NOMEMMGR
#define NOMETAFILE
#define NOMINMAX
#define NOMB
#define WINMESSAGES
#define NOVIRTUALKEYKODES
#define NOKEYSTATES
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX
#define NORPC
#define NOPROXYSTUB
#define NOIMAGE
#define NOTAPE
#define NOWIOFFSETS
#define STRICT

#include <Windows.h>
#include <optional>
#include "Resources.h"
#include "Tobj.h"

class Window {
private:
	class WindowClass {
	public:
		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator = (const WindowClass&) = delete;
		static constexpr const char* wndClassName = "Tobj Editor";
		static WindowClass wndClass;
		HINSTANCE hInst;
	};
public:
	Window(int width, int height, const char* name) noexcept;
	~Window();
	Window(const Window&) = delete;
	Window& operator = (const Window&) = delete;
	std::optional<int> ProcessMessages();
private:
	static LRESULT CALLBACK SetupProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK RedirectProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
private:
	void SetFont(HWND hWnd);
	bool OpenFile(HWND hWnd);
	bool SaveFile(HWND hWnd);
private:
	HWND hWnd;
	HWND hWndOpenTobjButton;
	HWND hWndSaveTobjButton;
	HWND hWndLoadEdit;
	HWND hWndSaveEdit;
	HWND hWndEditStatus;
	HFONT hFont;
	LOGFONT lf;
	Tobj t_object;
};
