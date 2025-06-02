#pragma once
#include <Windows.h>
#include <iostream>
#include "Defs.h"
namespace Log{
	void setConsoleAttribute(WORD attribute);
	std::ostream& red(std::ostream& os);
	std::ostream& white(std::ostream& os);
	std::ostream& yellow(std::ostream& os);
	std::ostream& green(std::ostream& os);
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