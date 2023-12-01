#include "../include/engine.hh"

bool IsMouseInBound(Rectangle rec, Vector2 pos, Vector2 mouse_pos) {
	return (mouse_pos.x >= pos.x && mouse_pos.x <= pos.x + rec.width
		&& mouse_pos.y >= pos.y && mouse_pos.y <= pos.y + rec.height);
};