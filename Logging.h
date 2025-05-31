#pragma once
#include <Windows.h>
#include <iostream>
namespace Log{
	void setConsoleAttribute(WORD attribute) {
		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(handle, attribute);
	}
	std::ostream& red(std::ostream& os) {
		setConsoleAttribute(FOREGROUND_RED | FOREGROUND_INTENSITY);
		return os;
	}
	std::ostream& white(std::ostream& os) {
		setConsoleAttribute(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		return os;
	}
	std::ostream& yellow(std::ostream& os) {
		setConsoleAttribute(FOREGROUND_RED | FOREGROUND_GREEN);
		return os;
	}
	std::ostream& green(std::ostream& os) {
		setConsoleAttribute(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		return os;
	}
}
#ifdef APPNAME
#define ASSERT(x) if(!(x)) __debugbreak(); 
#define LOG_INFO(x) std::cout << "[" << APPNAME << "]:" << x ;
#define LOG_WARN(x) std::cout << Log::yellow << "[" << APPNAME << "]:[WARN]:" << x << Log::white;
#define LOG_ERROR(x) std::cout << Log::red << "[" << APPNAME << "]:[ERROR]:" << __FILE__ << ":" << __LINE__ << ":" << x << "in function "<< __FUNCTION__ << "()" <<"\n" << Log::white;
#define LOG_SUCCESS(x) std::cout << Log::green << "[" << APPNAME << "]:" << x << Log::white;
#else
#error Appname must be defined
#endif