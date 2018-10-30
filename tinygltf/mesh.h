#pragma once

#include <vector>
#include <algorithm>
#include <cmath>
#include <limits>

inline void lerp(float dst[3], const float v0[3], const float v1[3], const float v2[3], float u, float v) {
	dst[0] = (1.0f - u - v) * v0[0] + u * v1[0] + v * v2[0];
	dst[1] = (1.0f - u - v) * v0[1] + u * v1[1] + v * v2[1];
	dst[2] = (1.0f - u - v) * v0[2] + u * v1[2] + v * v2[2];
}

inline float vlength(const float v[3]) {
	const float d = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
	if (std::fabs(d) > std::numeric_limits<float>::epsilon()) {
		return std::sqrt(d);
	} else {
		return 0.0f;
	}
}

inline void vnormalize(float dst[3], const float v[3]) {
	dst[0] = v[0];
	dst[1] = v[1];
	dst[2] = v[2];
	const float len = vlength(v);
	if (std::fabs(len) > std::numeric_limits<float>::epsilon()) {
		const float inv_len = 1.0f / len;
		dst[0] *= inv_len;
		dst[1] *= inv_len;
		dst[2] *= inv_len;
	}
}

inline void vcross(float dst[3], const float a[3], const float b[3]) {
	dst[0] = a[1] * b[2] - a[2] * b[1];
	dst[1] = a[2] * b[0] - a[0] * b[2];
	dst[2] = a[0] * b[1] - a[1] * b[0];
}

inline void vsub(float dst[3], const float a[3], const float b[3]) {
	dst[0] = a[0] - b[0];
	dst[1] = a[1] - b[1];
	dst[2] = a[2] - b[2];
}

inline void calculate_normal(float Nn[3], const float v0[3], const float v1[3], const float v2[3]) {
	float v10[3];
	float v20[3];
	vsub(v10, v1, v0);
	vsub(v20, v2, v0);
	float N[3];
	vcross(N, v20, v10);
	vnormalize(Nn, N);
}

class Mesh { public:
	explicit Mesh(const size_t vertex_stride) :
		stride(vertex_stride) {
	}

	std::string name;

	std::vector<float> vertices;                  // stride * num_vertices
	std::vector<float> facevarying_normals;       // [xyz] * 3(triangle) * num_faces
	std::vector<float> facevarying_tangents;      // [xyz] * 3(triangle) * num_faces
	std::vector<float> facevarying_binormals;     // [xyz] * 3(triangle) * num_faces
	std::vector<float> facevarying_uvs;           // [xy ] * 3(triangle) * num_faces
	std::vector<float> facevarying_vertex_colors; // [xyz] * 3(triangle) * num_faces
	std::vector<unsigned int> faces;				 /// triangle x num_faces
	std::vector<unsigned int> material_ids;	/// index x num_faces

	float pivot_xform[4][4];
	size_t stride;													 /// stride for vertex data.

	// --- Required methods in Scene::Traversal. ---

	///
	/// Get the geometric normal and the shading normal at `face_idx' th face.
	///
	void GetNormal(float Ng[3], float Ns[3], const unsigned int face_idx, const float u, const float v) const {
		// Compute geometric normal.
		unsigned int f0, f1, f2;
		float v0[3], v1[3], v2[3];

		f0 = faces[3 * face_idx + 0];
		f1 = faces[3 * face_idx + 1];
		f2 = faces[3 * face_idx + 2];

		v0[0] = vertices[3 * f0 + 0];
		v0[1] = vertices[3 * f0 + 1];
		v0[2] = vertices[3 * f0 + 2];

		v1[0] = vertices[3 * f1 + 0];
		v1[1] = vertices[3 * f1 + 1];
		v1[2] = vertices[3 * f1 + 2];

		v2[0] = vertices[3 * f2 + 0];
		v2[1] = vertices[3 * f2 + 1];
		v2[2] = vertices[3 * f2 + 2];
		
		calculate_normal(Ng, v0, v1, v2);

		if (facevarying_normals.size() > 0) {

			float n0[3], n1[3], n2[3];

			n0[0] = facevarying_normals[9 * face_idx + 0];
			n0[1] = facevarying_normals[9 * face_idx + 1];
			n0[2] = facevarying_normals[9 * face_idx + 2];

			n1[0] = facevarying_normals[9 * face_idx + 3];
			n1[1] = facevarying_normals[9 * face_idx + 4];
			n1[2] = facevarying_normals[9 * face_idx + 5];

			n2[0] = facevarying_normals[9 * face_idx + 6];
			n2[1] = facevarying_normals[9 * face_idx + 7];
			n2[2] = facevarying_normals[9 * face_idx + 8];

			lerp(Ns, n0, n1, n2, u, v);

		} else {

			// Use geometric normal.
			Ns[0] = Ng[0];
			Ns[1] = Ng[1];
			Ns[2] = Ng[2];

		}
	
	}

	// --- end of required methods in Scene::Traversal. ---

	///
	/// Get texture coordinate at `face_idx' th face.
	///
	void GetTexCoord(float tcoord[3], const unsigned int face_idx, const float u, const float v) {

		if (facevarying_uvs.size() > 0) {

			float t0[3], t1[3], t2[3];

			t0[0] = facevarying_uvs[6 * face_idx + 0];
			t0[1] = facevarying_uvs[6 * face_idx + 1];
			t0[2] = 0.0f;

			t1[0] = facevarying_uvs[6 * face_idx + 2];
			t1[1] = facevarying_uvs[6 * face_idx + 3];
			t1[2] = 0.0f;

			t2[0] = facevarying_uvs[6 * face_idx + 4];
			t2[1] = facevarying_uvs[6 * face_idx + 5];
			t2[2] = 0.0f;

			lerp(tcoord, t0, t1, t2, u, v);

		} else {
			tcoord[0] = 0.0f;
			tcoord[1] = 0.0f;
			tcoord[2] = 0.0f;
		}
	}
};
