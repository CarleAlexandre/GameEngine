#include "../include/engine.hh"

namespace fl {

void updateCamera(Camera *camera, vec3 player_pos, vec3 player_dir) {
	camera->position = (Vector3){player_pos.x, player_pos.y, player_pos.z};
	camera->target = (Vector3){player_pos.x + player_dir.x, player_pos.x + player_dir.y, player_pos.x + player_dir.z};
}

};