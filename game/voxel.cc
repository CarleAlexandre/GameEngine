#include "../include/engine.hh"

fl::Octree<Voxel> correctvoxe() {
	fl::Octree<Voxel> voxelMap = fl::Octree<Voxel>({0, 0, 0}, 30, 4, (Voxel){BLOCK_AIR});

	fl::OctreeNode<Voxel> *root;
	root = voxelMap.getRoot();
	for (unsigned short i = 0; i < 8; i++) {
		root->createChild();
		root->children[i]->setData((Voxel){(unsigned short)(i % 3)});
	}
	return (voxelMap);
}
