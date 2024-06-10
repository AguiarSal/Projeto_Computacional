#ifndef WINDOW_H
#define WINDOW_H

#include <windows.h>
#include <windowsx.h>

#include <string>
using std::string;

enum WndPossibleModes{FULLSCREEN, WINDOWED};

class Window
{
private:
	//atributos utilizados nesse arquivo
	HINSTANCE hInstance;
	HWND hWND;
	int WndWidth;
	int WndHeight;
	HICON WndIcon;
	HCURSOR WndCursor;
	string WndTitle;
	float WndCenterX;
	float WndCenterY;
	static bool WndKeys[256];
	static int WndMouseXCoord;
	static int WndMouseYCoord;
	COLORREF WndColor;
	//atributos utilizados em outro(s) arquivo(s)
	DWORD WndStyle;
	int WndMode;
	int WndInitialPosX;
	int WndInitialPosY;

public:
	//construtor
	Window(); //dps

	// métodos getters

	HINSTANCE GetApplicationId();
	HWND GetWndId();
	int GetWndWidth();
	int GetWndHeigth();
	string GetWndTitle() const;
	int GetWndMode() const;
	float GetWndCenterX() const;
	float GetWndCenterY() const;
	int GetWndMouseXCoord();
	int GetWndMouseYCoord();
	COLORREF GetWndColor();

	//métodos setters
	void SetWndIcon(const unsigned int Icon);
	void SetWndCursor(const unsigned int Cursor);
	void SetWndTitle(const string Title);
	void SetWndMode(int Mode); //dps
	void SetWndSize(int Width, int Height); //dps
	void SetWndColor(int Red, int Green, int Blue);
	

	//outros métodos
	void ShowOrHideCursor(bool Show);
	void CloseWnd();
	bool KeyDown(int vkcode);
	bool KeyUp(int vkcode);
	bool CreateWnd(); //dps

	static LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};


inline HINSTANCE Window::GetApplicationId() {
	return hInstance;
}

inline HWND Window::GetWndId() {
	return hWND;
}

inline int Window::GetWndWidth() {
	return WndWidth;
}

inline int Window::GetWndHeigth() {
	return WndHeight;
}

inline string Window::GetWndTitle() const {
	return WndTitle;
}

inline int Window::GetWndMode() const {
	return WndMode;
}

inline float Window::GetWndCenterX() const {
	return WndCenterX;
}

inline float Window::GetWndCenterY() const {
	return WndCenterY;
}

inline int Window::GetWndMouseXCoord() {
	return WndMouseXCoord;
}

inline int Window::GetWndMouseYCoord() {
	return WndMouseYCoord;
}

inline COLORREF Window::GetWndColor() {
	return WndColor;
}





inline void Window::SetWndIcon(const unsigned int Icon) {
	WndIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(Icon));
}

inline void Window::SetWndCursor(const unsigned int Cursor) {
	WndCursor = LoadCursor(GetModuleHandle(NULL), MAKEINTRESOURCE(Cursor));
}

inline void Window::SetWndTitle(const string Title) {
	WndTitle = Title;
}

inline void Window::SetWndColor(int Red, int Green, int Blue) {
	WndColor = RGB(Red, Green, Blue);
}





inline void Window::ShowOrHideCursor(bool Show) {
	ShowCursor(Show);
}

inline void Window::CloseWnd() {
	PostMessage(hWND, WM_DESTROY, 0, 0);
}

inline bool Window::KeyDown(int vkcode) {
	return WndKeys[vkcode];
}

inline bool Window::KeyUp(int vkcode) {
	return !(WndKeys[vkcode]);
}

#endif
