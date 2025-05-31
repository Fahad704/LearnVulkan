#include "Window.h"

Window::Window(const char* name, unsigned int width, unsigned int height)
{
	this->name = name;
	renderState.height = height;
	renderState.width = width;
	input = {};
	init();
}

void Window::init() {

	unsigned int width = renderState.width, height = renderState.height;

	WNDCLASSA mWindowClass = {};
	mWindowClass.lpfnWndProc = windowProcedure;
	mWindowClass.lpszClassName = "ClassName";
	mWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	mWindowClass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClassA(&mWindowClass);

	mWindowHandle = CreateWindowA(mWindowClass.lpszClassName, name.c_str(), WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, NULL, this);

	mDeviceContextHandle = GetDC(mWindowHandle);

	int bufferSize = height * width * sizeof(unsigned int);

	if (renderState.screenBuffer)VirtualFree(renderState.screenBuffer, 0, MEM_RELEASE);
	renderState.screenBuffer = VirtualAlloc(0, bufferSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	bufferSize = height * width * sizeof(float);
	if (renderState.depthBuffer)VirtualFree(renderState.depthBuffer, 0, MEM_RELEASE);
	renderState.depthBuffer = (float*)VirtualAlloc(0, bufferSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	renderState.bitmapInfo.bmiHeader.biSize = sizeof(renderState.bitmapInfo.bmiHeader);
	renderState.bitmapInfo.bmiHeader.biWidth = renderState.width;
	renderState.bitmapInfo.bmiHeader.biHeight = renderState.height;
	renderState.bitmapInfo.bmiHeader.biBitCount = 32;
	renderState.bitmapInfo.bmiHeader.biPlanes = 1;
	renderState.bitmapInfo.bmiHeader.biCompression = BI_RGB;
}

Window::~Window() {
	if (renderState.screenBuffer)VirtualFree(renderState.screenBuffer, 0, MEM_RELEASE);
	ReleaseDC(mWindowHandle, mDeviceContextHandle);
	DestroyWindow(mWindowHandle);
}

inline HDC Window::getDC() const {
	return mDeviceContextHandle;
}

Window::Window(const Window& copy) {
	*this = copy;
}


LRESULT windowProcedure(HWND window, UINT msg, WPARAM wParam, LPARAM lParam) {
	LRESULT result = 0;
	Window* pWindow = nullptr;
	if ((msg != WM_NCCREATE)) {
		pWindow = reinterpret_cast<Window*>(GetWindowLongPtr(window, GWLP_USERDATA));
	}
	switch (msg) {
	case WM_NCCREATE: {
		CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
		pWindow = reinterpret_cast<Window*>(pCreate->lpCreateParams);
		SetWindowLongPtr(window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
	}break;
	case WM_CLOSE:
	case WM_DESTROY: {
		DestroyWindow(window);
		return result;
	}break;
	case WM_SIZE: {
		RECT rect;
		GetClientRect(window, &rect);
		RenderState* renderStateU = pWindow->getRenderState();
		renderStateU->width = rect.right - rect.left;
		renderStateU->height = rect.bottom - rect.top;

		int bufferSize = renderStateU->width * renderStateU->height * sizeof(unsigned int);

		if (renderStateU->screenBuffer)VirtualFree(renderStateU->screenBuffer, 0, MEM_RELEASE);
		renderStateU->screenBuffer = VirtualAlloc(0, bufferSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		renderStateU->bitmapInfo.bmiHeader.biSize = sizeof(renderStateU->bitmapInfo.bmiHeader);
		renderStateU->bitmapInfo.bmiHeader.biWidth = renderStateU->width;
		renderStateU->bitmapInfo.bmiHeader.biHeight = renderStateU->height;
		renderStateU->bitmapInfo.bmiHeader.biBitCount = 32;
		renderStateU->bitmapInfo.bmiHeader.biPlanes = 1;
		renderStateU->bitmapInfo.bmiHeader.biCompression = BI_RGB;

		return 0;
	}break;
	default: {
		return DefWindowProcA(window, msg, wParam, lParam);
	}
	}
	return DefWindowProcA(window, msg, wParam, lParam);
}

void Window::putPixelD(unsigned int x, unsigned int y, unsigned int colour) {

}

void Window::processMessages(Input& input)
{
	MSG message;
	while (PeekMessage(&message, mWindowHandle, 0, 0, PM_REMOVE)) {
		switch (message.message) {
		case WM_KEYUP:
		case WM_KEYDOWN: {
			u32 vk_code = (u32)message.wParam;
			bool isDown = ((message.lParam & (1 << 31)) == 0);
#define process_messages(b,vk)\
case vk:{\
input.buttons[b].changed = (isDown != input.buttons[b].isDown);\
input.buttons[b].isDown = isDown;\
}break;
			switch (vk_code) {
				process_messages(BUTTON_UP, VK_UP)
					process_messages(BUTTON_DOWN, VK_DOWN)
					process_messages(BUTTON_LEFT, VK_LEFT)
					process_messages(BUTTON_RIGHT, VK_RIGHT)
					process_messages(BUTTON_SPACE, VK_SPACE)
					process_messages(BUTTON_ESC, VK_ESCAPE)
					process_messages(BUTTON_SHIFT, VK_SHIFT)
					process_messages(BUTTON_A, 'A')
					process_messages(BUTTON_B, 'B')
					process_messages(BUTTON_C, 'C')
					process_messages(BUTTON_D, 'D')
					process_messages(BUTTON_F, 'F')
					process_messages(BUTTON_G, 'G')
					process_messages(BUTTON_L, 'L')
					process_messages(BUTTON_M, 'M')
					process_messages(BUTTON_N, 'N')
					process_messages(BUTTON_P, 'P')
					process_messages(BUTTON_Q, 'Q')
					process_messages(BUTTON_R, 'R')
					process_messages(BUTTON_S, 'S')
					process_messages(BUTTON_T, 'T')
					process_messages(BUTTON_V, 'V')
					process_messages(BUTTON_W, 'W')
					process_messages(BUTTON_X, 'X')
					process_messages(BUTTON_Z, 'Z')
			}
		}break;
		case WM_LBUTTONDOWN: {
			input.buttons[MOUSE_BUTTON_LEFT].changed = !(input.buttons[MOUSE_BUTTON_LEFT].isDown);
			input.buttons[MOUSE_BUTTON_LEFT].isDown = true;

		}break;
		case WM_LBUTTONUP: {
			input.buttons[MOUSE_BUTTON_LEFT].changed = input.buttons[MOUSE_BUTTON_LEFT].isDown;
			input.buttons[MOUSE_BUTTON_LEFT].isDown = false;

		}break;
		case WM_RBUTTONDOWN: {
			input.buttons[MOUSE_BUTTON_RIGHT].changed = !(input.buttons[MOUSE_BUTTON_RIGHT].isDown);
			input.buttons[MOUSE_BUTTON_RIGHT].isDown = true;

		}break;
		case WM_RBUTTONUP: {
			input.buttons[MOUSE_BUTTON_RIGHT].changed = input.buttons[MOUSE_BUTTON_RIGHT].isDown;
			input.buttons[MOUSE_BUTTON_RIGHT].isDown = false;

		}break;
		default: {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		}
	}
}

void Window::swapBuffers()
{
	StretchDIBits(mDeviceContextHandle, 0, renderState.height - 1, renderState.width, -renderState.height, 0, 0, renderState.width, renderState.height, renderState.screenBuffer, &renderState.bitmapInfo, DIB_RGB_COLORS, SRCCOPY);

}

void Window::addConsole() const
{
	AllocConsole();
	std::freopen("CONOUT$", "w", stdout);
}

void Window::removeConsole() const {
	FreeConsole();
	std::fclose(stdout);
}
void Window::clear(u32 color)
{
	//reset input before clearing buffer
	for (int i = 0; i < Buttons::BUTTON_COUNT; i++) {
		input.buttons->isDown = false;
	}

	u32* buffer = (u32*)renderState.screenBuffer;
	for (int y = 0; y < renderState.height; y++) {
		for (int x = 0; x < renderState.width; x++) {
			*buffer++ = color;
		}
	}
}