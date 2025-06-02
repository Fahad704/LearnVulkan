#include "Logging.h"

void Log::setConsoleAttribute(WORD attribute) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, attribute);
}
std::ostream& Log::red(std::ostream& os) {
	setConsoleAttribute(FOREGROUND_RED | FOREGROUND_INTENSITY);
	return os;
}
std::ostream& Log::white(std::ostream& os) {
	setConsoleAttribute(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	return os;
}
std::ostream& Log::yellow(std::ostream& os) {
	setConsoleAttribute(FOREGROUND_RED | FOREGROUND_GREEN);
	return os;
}
std::ostream& Log::green(std::ostream& os) {
	setConsoleAttribute(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	return os;
}