#include "../include/engine.hh"

void render_button(fl::vec<Button> &button, const Font font, fl::vec<Texture> &textAtlas, const Vector2 mouse_pos) {
	for (uint32_t k = 0; k < button.size(); k++) {
		if (IsMouseInBound(button[k].bound, button[k].pos, mouse_pos)) {		
			DrawTextureRec(textAtlas[button[k].textureId[1]], button[k].bound, button[k].pos, WHITE);
		} else {
			DrawTextureRec(textAtlas[button[k].textureId[0]], button[k].bound, button[k].pos, WHITE);
		}
		DrawTextEx(font, button[k].text, (Vector2){ button[k].pos.x + 10, static_cast<float>(button[k].pos.y + button[k].bound.height * 0.5 - 6)}, 22, 0, WHITE);
	}
}

void add_button(fl::vec<Button> *button, Rectangle bound, Vector2 pos, const char text[], void *(*fun)(engine_t *)) {
	button->push_back(Button{
		.bound = bound,
		.textureId = {0, 1},
		.pos = pos,
		.text = (char *)flMemDup(text, strlen(text) + 1, malloc),
		.fun = fun,
	});
}

void logic_button(fl::vec<Button> &button, Vector2 mouse_pos, engine_t *engine) {
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		for (size_t i = 0; i < button.size(); i++) {
			if (IsMouseInBound(button[i].bound, button[i].pos, mouse_pos)) {
				button[i].fun(engine);
			}
		}
	}
}
