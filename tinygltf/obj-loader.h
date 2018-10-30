#pragma once

#include <vector>
#include <string>
#include "mesh.h"
#include "material.h"

// Loads wavefront .obj mesh
bool LoadObj(const std::string &filename, float scale, std::vector<Mesh> *meshes, std::vector<Material> *materials, std::vector<Texture> *textures);
