#include "../include/engine.hh"

static inline void
loadAllButton(fl::vec<Button> *button_menu, fl::vec<Button> *button_setting, fl::vec<Button> *button_pause) {
	(void)button_setting;
	(void)button_pause;
	add_button(button_menu, (Rectangle){0, 0, 64, 32}, (Vector2){64, 50}, "Solo\0", solo_engine);
	add_button(button_menu, (Rectangle){0, 0, 64, 32}, (Vector2){64, 100}, "Online\0", dummy_function);
	add_button(button_menu, (Rectangle){0, 0, 64, 32}, (Vector2){64, 150}, "Setting\0", dummy_function);
	add_button(button_menu, (Rectangle){0, 0, 64, 32}, (Vector2){64, 200}, "Exit\0", close_engine);
}

static inline void
unloadAllButton(fl::vec<Button> *button_menu, fl::vec<Button> *button_setting, fl::vec<Button> *button_pause) {
	for (size_t i = 0; i < button_menu->size() && i < button_setting->size() && i < button_pause->size(); i++) {
		if (i < button_menu->size()) {
			free(button_menu->at(i).text);
		}
		if (i < button_setting->size()) {
			free(button_setting->at(i).text);
		}
		if (i < button_pause->size()) {
			free(button_pause->at(i).text);
		}
	}
}

int
main(void) {
	display_t display;
	engine_t engine;
	fl::vec<Button> button_menu;
	bool window_close = false;

	engine.status = st_menu;
	display.width = 720;
	display.height = 480;
	display.fullscreen = false;
	display.configflag = FLAG_MSAA_4X_HINT;
	display.title = static_cast<char *>(flMemDup("engine\0", 8, malloc));
	SetConfigFlags(display.configflag);
	InitWindow(display.width, display.height, display.title);
	engine.font = LoadFont("assets/font/menu_font.ttf");
	display.icon = LoadImage("assets/icon.png");
	SetWindowIcon(display.icon);

	Camera camera = { 0 };
    camera.position = (Vector3){ 2.0f, 4.0f, 6.0f };    // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type
	SetCameraMode(camera, CAMERA_FREE);


	engine.shader = LoadShader("assets/shader/shader.vs", "assets/shader/shader.fs");
	engine.shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(engine.shader, "viewPos");
	float shaderposlocaltmp[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
    SetShaderValue(engine.shader, GetShaderLocation(engine.shader, "ambient"), shaderposlocaltmp, SHADER_UNIFORM_VEC4);

	engine.fbo_shader = LoadShader(0, "assets/shader/sobel_filter.fs");
	engine.depth_shader = LoadShader(0, "assets/shader/depth_filter.fs");

	Light light = CreateLight(LIGHT_POINT, {10, 10, 15}, Vector3Zero(), WHITE, engine.shader);

	Voxel *voxel_dirt = loadVoxel(engine.shader);
	engine.fbo = LoadRenderTextureDepthTex(display.width, display.height);
    SetTextureFilter(engine.fbo.texture, TEXTURE_FILTER_BILINEAR);

# ifdef DEBUG_CONSOLE_USE
	initConsole();
# endif
	LoadTextureAtlas(&engine.textures);
	loadAllButton(&button_menu, NULL, NULL);

	SetTargetFPS(60);

	while ((engine.status & st_close) == 0) {
		double delta_time = GetFrameTime();
		Vector2 mouse_pos = GetMousePosition();
		if (IsKeyPressed(KEY_ESCAPE)) {
			window_close = true;
		}
		if (engine.status & st_menu) {
			logic_button(button_menu, mouse_pos, &engine);
		}
		if (window_close == true) {
			if (IsKeyPressed(KEY_Y)) {
				engine.status ^= st_close;
			}
			else if (IsKeyPressed(KEY_N)) {
				window_close ^= window_close;
			}
		}
# ifdef DEBUG_CONSOLE_USE
		if (engine.status & st_debug) {
			console_logic(delta_time, &engine);
		} else {
			if (IsKeyPressed(KEY_F1)) {
				engine.status ^= st_debug;
			}
		}
# endif
		if (engine.status & st_game) {
			//things to do, add a zbuffer g_buffer
			// draw outline of object;
			UpdateCamera(&camera);
			UpdateLightValues(engine.shader, light);
			SetShaderValue(engine.shader, engine.shader.locs[SHADER_LOC_VECTOR_VIEW], &camera.position, SHADER_UNIFORM_VEC3);
			SetShaderValue(engine.fbo_shader, engine.fbo_shader.locs[SHADER_LOC_VECTOR_VIEW], &camera.position, SHADER_UNIFORM_VEC3);
			SetShaderValue(engine.depth_shader, engine.depth_shader.locs[SHADER_LOC_VECTOR_VIEW], &camera.position, SHADER_UNIFORM_VEC3);

			BeginTextureMode(engine.fbo);
				ClearBackground(WHITE);
				rlEnableDepthTest();
				BeginMode3D(camera);

				//BeginShaderMode(engine.depth_shader);
					for (float i = 0; i < 10; i+=1) {
						for (float j = 0; j < 10; j+=1) {
							DrawVoxel(voxel_dirt->model, {i, 0, j}, 1, RED);
						}
					}
					for (float i = -1; i > -11; i-=1) {
						for (float j = 0; j > -10; j-=1) {
							DrawVoxel(voxel_dirt->model, {i, 0, j}, 1, BLUE);
						}
					}
				//EndShaderMode();

				EndMode3D();
			EndTextureMode();
		}
			ClearBackground(BLACK);
			if (engine.status & st_game) {
				DrawTextureRec(engine.fbo.texture, {0, 0, static_cast<float>(display.width), -static_cast<float>(display.height)}, {0, 0}, WHITE);
			}
# ifdef DEBUG_CONSOLE_USE
			if (engine.status & st_debug) {
				console_render(delta_time, &display);
			}
# endif
			if (engine.status & st_menu) {
				render_button(button_menu, engine.font, engine.textures, mouse_pos);
			}
			if (window_close == true) {
				DrawRectangle(10, display.height * 0.5 - 100, display.width, 200, BLACK);
				DrawTextEx(engine.font, "Are you sure you want to exit program? [Y/N]", (Vector2){50, static_cast<float>(display.height * 0.5 - 20)}, 24, 0, GREEN);
			}
		EndDrawing();
	}
	CloseWindow();
	UnloadRenderTextureDepthTex(engine.fbo);
	UnloadShader(engine.shader);
	unloadVoxel(voxel_dirt);
	free(display.title);
	UnloadImage(display.icon);
	UnloadTextureAtlas(engine.textures);
	UnloadShader(engine.fbo_shader);
	UnloadFont(engine.font);
	return (0);
}