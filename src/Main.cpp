#include "ApplicationInstance.h"



int real_main() {

    // Initialize instance
	ApplicationInstance appInstance;

    // Run application mainloop
	appInstance.mainLoop();
	
	return 0;

}

// Windows uses WinMain as entry point instead of main(). This will call
// real_main() from either platform
#ifdef _WIN32
#include <Windows.h>

int APIENTRY WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow)
{
    real_main();
    return 0;
}
#else
int main() {

    real_main();
    return 0;
}
#endif



