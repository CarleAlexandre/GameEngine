#include "../include/engine.hh"

void unloadVoxel(Voxel *voxel) {
	UnloadModel(voxel->model);
	UnloadMesh(voxel->mesh);
	delete voxel;
	voxel = NULL;
}

void getvertexidfromoctreenode(int node_depth, int face_to_render) {
	
}

Voxel *loadVoxel(Shader voxel_shader) {
	Voxel *voxel = new Voxel;
	voxel->mesh = GenMeshCube(1.0f, 1.0f, 1.0f);
	voxel->model = LoadModelFromMesh(voxel->mesh);
	voxel->shader = voxel_shader;
	voxel->model.materials[0].shader = voxel_shader;
	return (voxel);
}

void DrawVoxel(Model voxel, Vector3 pos, float size, Color color) {
	//i should send a ray, get the distance, use it to draw the z_buffer,
	//get the color of the block hit, but it in the color buffer,
	//get the block normal. put it in the normal buffer,
	//blend all of them together;
	//apply shadow and light on the scene from the light data and the buffer data;
	//render the buffer
}
