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

sv_player_t
initPlayer() {
	sv_player_t sv_player;
	sv_player = {};
	sv_player.mass = 10;
	sv_player.pos = { 0.0f, 1.0f, 2.0f };
	sv_player.vel = {0.0f, 0.0f, 0.0f};
	sv_player.airborn = false;
	sv_player.jumpforce = 6.0f;
	sv_player.speed = 100;
	return (sv_player);
}

# define TIME_TO_JUMP 0.13f

sv_physic_t
initPhysic() {
	sv_physic_t sv_physic;

	sv_physic.friction = 0.3;
	sv_physic.airborn_friction = 0.1;
	sv_physic.restitution = 10;
	sv_physic.gravity = {0, -2.0f, 0};
	sv_physic.sensibility = 0.3f;
	return (sv_physic);
}

void
playerUpdateFps(display_t display, sv_player_t *sv_player, Camera *camera, sv_physic_t sv_physic, double delta_time) {
	Vector2 mouse_delta = GetMouseDelta();
	static double sv_jump_timing = 0;
	SetMousePosition(display.width * 0.5, display.height * 0.5);
	sv_player->forward = fl::tovec3(GetCameraForward(camera));
	sv_player->forward.y = 0;
	sv_player->forward = fl::vec3norm(sv_player->forward);
	sv_player->right = fl::tovec3(GetCameraRight(camera));
	sv_player->right.y = 0;
	sv_player->right = fl::vec3norm(sv_player->right);
	if (IsKeyDown(KEY_W)) {
		if (sv_player->sliding) {
			addImpulse(sv_player->accel, sv_player->forward, sv_player->mass, sv_player->speed * 0.3);
		} else {
			addImpulse(sv_player->accel, sv_player->forward, sv_player->mass, sv_player->speed);
		}
	}
	if (IsKeyDown(KEY_S)) {
		if (sv_player->sliding) {
			addImpulse(sv_player->accel, fl::vec3neg(sv_player->forward), sv_player->mass, sv_player->speed * 0.3);
		} else {
			addImpulse(sv_player->accel, fl::vec3neg(sv_player->forward), sv_player->mass, sv_player->speed);
		}
	}
	if (IsKeyDown(KEY_D)) {
		if (sv_player->sliding) {
			addImpulse(sv_player->accel, sv_player->right, sv_player->mass, sv_player->speed * 0.3);
		} else {
			addImpulse(sv_player->accel, sv_player->right, sv_player->mass, sv_player->speed);
		}
	}
	if (IsKeyDown(KEY_A)) {
		if (sv_player->sliding) {
			addImpulse(sv_player->accel, fl::vec3neg(sv_player->right), sv_player->mass, sv_player->speed * 0.3);
		} else {
			addImpulse(sv_player->accel, fl::vec3neg(sv_player->right), sv_player->mass, sv_player->speed);
		}
	}
	if (IsKeyPressed(KEY_SPACE) && sv_player->pos.y <= 2) {
		sv_player->jumping = true;
	}
	if (IsKeyPressed(KEY_C)) {
		addImpulse(sv_player->accel, {sv_player->forward.x * 5, 0, sv_player->forward.z * 5}, sv_player->mass, sv_player->speed * 4);
		sv_player->sliding = true;
		camera->target.y -= 0.5f;
	}
	if (IsKeyReleased(KEY_C)) {
		sv_player->sliding = false;
		camera->target.y += 0.5f;
	}
	if (sv_player->pos.y > 0.01) {
		sv_player->airborn = true;
	} else if (sv_player->airborn){
		sv_player->airborn = false;
		addImpulse(sv_player->accel, {sv_player->forward.x * 5, 0, sv_player->forward.z * 5}, sv_player->mass, sv_player->speed * 4);
	}
	if (sv_player->jumping == true && sv_jump_timing < TIME_TO_JUMP) {
		addImpulse(sv_player->accel, {0.0f, (-sv_physic.gravity.y + sv_player->jumpforce), 0.0f}, sv_player->mass, sv_player->speed);
		sv_jump_timing += delta_time;
	}
	if (sv_jump_timing >= TIME_TO_JUMP) {
		sv_jump_timing = 0;
		sv_player->jumping = false;
	}
	applyGravity(sv_player->accel, sv_physic.gravity, sv_player->mass);
	sv_player->vel = resolveAccel(sv_player->vel, sv_player->accel, sv_player->mass, delta_time);
	if (sv_player->airborn == true || sv_player->sliding == true) {
		applyFriction(sv_player->vel, sv_physic.airborn_friction);
	} else {
		applyFriction(sv_player->vel, sv_physic.friction);
	}
	fl::vec3 step = {sv_player->vel.x * (float)delta_time, sv_player->vel.y * (float)delta_time, sv_player->vel.z * (float)delta_time};
	if (sv_player->pos.y <= 0 && step.y < 0) {
		step.y = 0;
	}
	sv_player->pos = fl::vec3add(sv_player->pos, step);
	camera->target = Vector3Add(camera->target, fl::vec3to(step));
	if (sv_player->sliding) {
		camera->position = {sv_player->pos.x, sv_player->pos.y + 0.5f, sv_player->pos.z};
	} else {
		camera->position = {sv_player->pos.x, sv_player->pos.y + 1, sv_player->pos.z};
	}
	camera->target = fl::vec3to(fl::rotateYaw(fl::tovec3(camera->target), sv_player->pos, mouse_delta.x * sv_physic.sensibility * DEG2RAD));
	CameraPitch(camera, -mouse_delta.y * sv_physic.sensibility * DEG2RAD, true, false, false);
}

int
main(void) {
	display_t display;
	engine_t engine;
	sv_player_t sv_player;
	//mv_player mplayer[16];
	fl::vec<Button> button_menu;
	bool window_close = false;
	sv_physic_t sv_physic;

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
	sv_physic = initPhysic();

    Camera camera = { 0 };
    camera.position = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.target = (Vector3){ 1.0f, 1.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

	initRender(engine, display);

	Light light = CreateLight(LIGHT_POINT, {10, 10, 15}, Vector3Zero(), WHITE, engine.shader);
	Voxel *voxel_dirt = loadVoxel(engine.shader);

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
			playerUpdateFps(display, &sv_player, &camera, sv_physic, delta_time);
			UpdateLightValues(engine.shader, light);
			renderUpdate(engine, display, &camera, delta_time);
		}
		BeginDrawing();
			ClearBackground(BLACK);
		if (engine.status & st_game) {
			renderRender(engine, display, &camera, delta_time, voxel_dirt, sv_player);
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