#ifndef MESH_RENDERER_H
#define MESH_RENDERER_H

#include "ObjectRenderer.h"
#include "Mesh.h"
#include "Material.h"

class MeshRenderer : public ObjectRenderer {
public:
	void init() override;
	void render() override;
	void onDestroy() override;

	MeshRenderer& setMesh(Mesh* mesh) { _mesh = mesh; return *this; }
	MeshRenderer& setMaterial(Material* material) { _material = material; return *this; }
private: 
	Mesh* _mesh;
	Material* _material;

	GLuint _vao, _vbo, _ebo;
};

#endif /* defined(MESH_RENDERER_H) */