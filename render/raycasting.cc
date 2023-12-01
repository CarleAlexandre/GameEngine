#include "../include/engine.hh"

Ray
getPixelRay(Vector2 pos, Camera camera, display_t display) {
	Ray ray;

	ray = GetMouseRay(pos, camera);

	Vector4 clipCoords;
	clipCoords = {(2.0f * pos.x) / display.width - 1.0f, 1.0f - (2.0f * pos.y) / display.height, -1.0f, 1.0f};
	
	return (ray);
}

float
raycasting(Camera camera, const display_t display) {
	Ray ray[display.height][display.width];

	for (float i = 0; i < display.height; i += 1) {
		for (float j = 0; j < display.width; j += 1) {
			ray[(int)i][(int)j] = getPixelRay({i, j}, camera, display);
		}
	}
	return (0);
}
