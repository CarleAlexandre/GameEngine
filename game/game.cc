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

sv_player_t initPlayer() {
	sv_player_t sv_player;

	sv_player = {};
	sv_player.mass = 10;
	sv_player.pos = { 0.0f, 1.0f, 2.0f };
	sv_player.vel = {0.0f, 0.0f, 0.0f};
	sv_player.airborn = false;
	sv_player.jumpforce = 30;
	sv_player.speed = 100;
	return (sv_player);
}

Quaternion rotateYawPitch(Vector3 axis, float angleYaw, float anglePitch) {
    Quaternion qYaw = QuaternionFromAxisAngle((Vector3){0, 1, 0}, angleYaw);
    Quaternion qPitch = QuaternionFromAxisAngle(axis, anglePitch);
    return QuaternionMultiply(qPitch, qYaw);
}

int
main(void) {
	display_t display;
	engine_t engine;
	sv_player_t sv_player;
	//mv_player mplayer[16];
	fl::vec<Button> button_menu;
	bool window_close = false;

	float sv_friction = 0.6;
	float sv_airborn_friction = 0.02;
	float sv_restitution = 10;
	fl::vec3 sv_gravity = {0, -0.2f, 0};
	float sv_sensibility = 0.3f;

	engine.status = st_menu;
	display.width = 720;
	display.height = 480;
	display.fullscreen = false;
	display.configflag = FLAG_MSAA_4X_HINT | FLAG_BORDERLESS_WINDOWED_MODE;
	display.title = static_cast<char *>(flMemDup("engine\0", 8, malloc));
	SetConfigFlags(display.configflag);
	InitWindow(display.width, display.height, display.title);
	engine.font = LoadFont("assets/font/menu_font.ttf");
	display.icon = LoadImage("assets/icon.png");
	SetWindowIcon(display.icon);
	sv_player = initPlayer();

    Camera camera = { 0 };
    camera.position = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.target = (Vector3){ 1.0f, 1.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

	engine.shader = LoadShader("assets/shader/shader.vs", "assets/shader/shader.fs");
	engine.shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(engine.shader, "viewPos");
	float shaderposlocaltmp[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
    SetShaderValue(engine.shader, GetShaderLocation(engine.shader, "ambient"), shaderposlocaltmp, SHADER_UNIFORM_VEC4);

	engine.fbo_shader = LoadShader(0, "assets/shader/sobel_filter.fs");

	Light light = CreateLight(LIGHT_POINT, {10, 10, 15}, Vector3Zero(), WHITE, engine.shader);

	Voxel *voxel_dirt = loadVoxel(engine.shader);
	engine.fbo = LoadRenderTextureDepthTex(display.width, display.height);
    SetTextureFilter(engine.fbo.texture, TEXTURE_FILTER_BILINEAR);

# ifdef DEBUG_CONSOLE_USE
	initConsole();
# endif
	LoadTextureAtlas(&engine.textures);
	loadAllButton(&button_menu, NULL, NULL);

	SetTargetFPS(120);

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
			Vector2 mouse_delta = GetMouseDelta();
			SetMousePosition(display.width * 0.5, display.height * 0.5);

			sv_player.forward = fl::tovec3(GetCameraForward(&camera));
			sv_player.right = fl::tovec3(GetCameraRight(&camera));

			if (IsKeyDown(KEY_W)) {
				addImpulse(sv_player.accel, sv_player.forward, sv_player.mass, sv_player.speed);
			}
			if (IsKeyDown(KEY_S)) {
				addImpulse(sv_player.accel, fl::vec3neg(sv_player.forward), sv_player.mass, sv_player.speed);
			}
			if (IsKeyDown(KEY_D)) {
				addImpulse(sv_player.accel, sv_player.right, sv_player.mass, sv_player.speed);
			}
			if (IsKeyDown(KEY_A)) {
				addImpulse(sv_player.accel, fl::vec3neg(sv_player.right), sv_player.mass, sv_player.speed);
			}
			if (IsKeyPressed(KEY_SPACE) && sv_player.pos.y <= 1) {
				addImpulse(sv_player.accel, {0.0f, sv_player.jumpforce, 0.0f}, sv_player.mass, sv_player.speed);
				sv_player.airborn = true;
			}
			if (IsKeyDown(KEY_C)) {
				if (sv_player.pos.y <= 0.1 && sv_player.airborn) {
					addImpulse(sv_player.accel, {sv_player.forward.x * 3, 0, sv_player.forward.z * 3}, sv_player.mass, sv_player.speed);
				}
				sv_player.sliding = true;
			} else if (sv_player.sliding == true) {
				sv_player.sliding = false;
			}

			applyGravity(sv_player.accel, sv_gravity, sv_player.mass);
			sv_player.vel = resolveAccel(sv_player.vel, sv_player.accel, sv_player.mass, delta_time);
			if (sv_player.airborn == true || sv_player.sliding == true) {
				applyFriction(sv_player.vel, sv_airborn_friction);
			} else {
				applyFriction(sv_player.vel, sv_friction);
			}

			fl::vec3 step = {sv_player.vel.x * (float)delta_time, sv_player.vel.y * (float)delta_time, sv_player.vel.z * (float)delta_time};

			if (sv_player.pos.y <= 0 && step.y <= 0) {
				step.y = 0;
				sv_player.airborn = false;
			}

			sv_player.pos = fl::vec3add(sv_player.pos, step);
			camera.target = Vector3Add(camera.target, fl::vec3to(step));
			camera.position = fl::vec3to(sv_player.pos);

			camera.target = fl::vec3to(fl::rotateYaw(fl::tovec3(camera.target), sv_player.pos, mouse_delta.x * sv_sensibility * DEG2RAD));
			CameraPitch(&camera, -mouse_delta.y * sv_sensibility * DEG2RAD, true, false, false);

			UpdateLightValues(engine.shader, light);
			SetShaderValue(engine.shader, engine.shader.locs[SHADER_LOC_VECTOR_VIEW], &camera.position, SHADER_UNIFORM_VEC3);
			SetShaderValue(engine.fbo_shader, engine.fbo_shader.locs[SHADER_LOC_VECTOR_VIEW], &camera.position, SHADER_UNIFORM_VEC3);

			BeginTextureMode(engine.fbo);
				ClearBackground(WHITE);
				rlEnableDepthTest();
				BeginMode3D(camera);
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
					DrawLine3D({-100, 0,}, {100, 0, 0}, RED);
					DrawLine3D({0,-100,0}, {0, 100, 0}, GREEN);
					DrawLine3D({0,0,-100}, {0, 0, 100}, BLUE);
					DrawLine3D(camera.position, camera.target, PINK);
				EndMode3D();
			EndTextureMode();
		}
			ClearBackground(BLACK);
			if (engine.status & st_game) {
				//BeginShaderMode(engine.fbo_shader);
					DrawTextureRec(engine.fbo.texture, {0, 0, static_cast<float>(display.width), -static_cast<float>(display.height)}, {0, 0}, WHITE);
				//EndShaderMode();
				//BeginBlendMode(BLEND_ADDITIVE);
				//	DrawTextureRec(engine.fbo.texture, {0, 0, static_cast<float>(display.width), -static_cast<float>(display.height)}, {0, 0}, WHITE);
				//EndBlendMode();
				DrawRectangle(0, 0, 100, 100, BLACK);
				DrawLine(50, 0, 50, 100, BLUE);
				DrawLine(0, 50, 100, 50, RED);
				DrawLine(50, 50, sv_player.vel.x + 50, sv_player.vel.z + 50, WHITE);
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
	ShowCursor();
	return (0);
}