#include "../include/engine.hh"

void getvertexidfromoctreenode(int node_depth, int face_to_render) {
	
}

void DrawVoxel(Model voxel, Vector3 pos, float size, Color color) {
	//i should send a ray, get the distance, use it to draw the z_buffer,
	//get the color of the block hit, but it in the color buffer,
	//get the block normal. put it in the normal buffer,
	//blend all of them together;
	//apply shadow and light on the scene from the light data and the buffer data;
	//render the buffer
	DrawModel(voxel, pos, size, color);
}
