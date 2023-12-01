#include "../include/ide.hh"

int main(void) {
	display_t	display;
	engine_t	engine;

	display.width = 720;
	display.height = 480;
	display.fullscreen = false;
	display.configflag += FLAG_MSAA_4X_HINT;
	display.title = _strdup("engine");
	SetConfigFlags(display.configflag);
	InitWindow(display.width, display.height, display.title);
//	display.handle = GetWindowHandle();

	EnableEventWaiting();
	while (!WindowShouldClose()) {
		if (IsKeyPressed(KEY_ESCAPE)) {
			CloseWindow();
		}
	}

	CloseWindow();
//	display.handle = NULL;
	free(display.title);
	return (0);
}