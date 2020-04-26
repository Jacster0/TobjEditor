#include "Application.h"

//Entrypoint for the application
INT WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,INT) {
	return Application{}.Run();
}