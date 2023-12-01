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
	DrawModel(voxel, pos, size, color);
}
