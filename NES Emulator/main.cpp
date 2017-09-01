#include <iostream>
#include <cstdio>
#include <cstdlib>

// #define USES_SDL

#include "CPU.h"
#include "FileHandler.h"

#ifdef USES_SDL
#include "SDL.h"
#include "GUI.h"
#endif


#ifdef USES_SDL
int main(int argc, char* args[])
#else
int main()
#endif
{
#ifdef USES_SDL

	GUI gui = GUI();

#else

	std::cout << ROM("donkeykong.nes").Status() << '\n';
	std::getchar();

#endif

	return 0;
}