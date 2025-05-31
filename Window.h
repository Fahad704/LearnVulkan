#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX
#include <Windows.h>
#include <iostream>
#include <string>
#include "Input.h"
typedef unsigned int u32;
LRESULT CALLBACK windowProcedure(HWND window, UINT msg, WPARAM wParam, LPARAM lParam);
struct RenderState {
	int width;
	int height;
	void* screenBuffer;
	float* depthBuffer;
	BITMAPINFO bitmapInfo;
};
class Window
{
private:
	HWND mWindowHandle;
	HDC mDeviceContextHandle;
public:
	std::string name;
	RenderState renderState;
	Input input;
	Window(const char* name = "NULL", unsigned int width = 720, unsigned int height = 720);
	Window(const Window& copy);
	~Window();
	void init();
	void processMessages(Input& input);
	void swapBuffers();
	void addConsole() const;
	void removeConsole() const;
	inline void set(HWND window) { mWindowHandle = window; }
	inline HWND get() const { return mWindowHandle; }
	void putPixelD(unsigned int x, unsigned int y, unsigned int colour);
	void clear(u32 color = 0x000000);
	inline HDC getDC() const;
	inline RenderState* getRenderState() { return  &renderState; }
	inline bool isOpen() const { return IsWindow(mWindowHandle); }
};