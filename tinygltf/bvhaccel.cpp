#include "stdafx.h"

#include "nanort.h"

namespace nanort {

void BVHAccel::Debug() {
	for (size_t i = 0; i < indices_.size(); i++) {
		printf("index[%d] = %d\n", int(i), int(indices_[i]));
	}

	for (size_t i = 0; i < nodes_.size(); i++) {
		printf("node[%d] : bmin %f, %f, %f, bmax %f, %f, %f\n", int(i),
					 nodes_[i].bmin[0], nodes_[i].bmin[1], nodes_[i].bmin[1],
					 nodes_[i].bmax[0], nodes_[i].bmax[1], nodes_[i].bmax[1]);
	}
}

bool BVHAccel::Dump(const char *filename) {
	FILE *fp = fopen(filename, "wb");
	if (!fp) {
		// fprintf(stderr, "[BVHAccel] Cannot write a file: %s\n", filename);
		return false;
	}

	size_t numNodes = nodes_.size();
	assert(nodes_.size() > 0);

	size_t numIndices = indices_.size();

	size_t r = 0;
	r = fwrite(&numNodes, sizeof(size_t), 1, fp);
	assert(r == 1);

	r = fwrite(&nodes_.at(0), sizeof(BVHNode), numNodes, fp);
	assert(r == numNodes);

	r = fwrite(&numIndices, sizeof(size_t), 1, fp);
	assert(r == 1);

	r = fwrite(&indices_.at(0), sizeof(unsigned int), numIndices, fp);
	assert(r == numIndices);

	fclose(fp);

	return true;
}

bool BVHAccel::Load(const char *filename) {
	FILE *fp = fopen(filename, "rb");
	if (!fp) {
		// fprintf(stderr, "Cannot open file: %s\n", filename);
		return false;
	}

	size_t numNodes;
	size_t numIndices;

	size_t r = 0;
	r = fread(&numNodes, sizeof(size_t), 1, fp);
	assert(r == 1);
	assert(numNodes > 0);

	nodes_.resize(numNodes);
	r = fread(&nodes_.at(0), sizeof(BVHNode), numNodes, fp);
	assert(r == numNodes);

	r = fread(&numIndices, sizeof(size_t), 1, fp);
	assert(r == 1);

	indices_.resize(numIndices);

	r = fread(&indices_.at(0), sizeof(unsigned int), numIndices, fp);
	assert(r == numIndices);

	fclose(fp);

	return true;
}

}