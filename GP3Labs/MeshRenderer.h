#pragma once
#include "Component.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Model.h"
#include "ShaderProgram.h"

class MeshRenderer : public Component
{
private:
	//Mesh* m_mesh;
	std::shared_ptr<Model> m_model;
	std::shared_ptr<ShaderProgram> m_program;
	std::shared_ptr<Texture> m_texture;

public:
	// Inherited via Component
	MeshRenderer(std::shared_ptr<Model> model, std::shared_ptr<ShaderProgram> program, std::shared_ptr<Texture> texture);
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
};
