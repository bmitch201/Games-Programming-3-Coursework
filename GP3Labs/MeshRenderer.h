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
	Model* m_model;
	ShaderProgram* m_program;
	Texture* m_texture;

public:
	// Inherited via Component
	MeshRenderer(/*Mesh* mesh*/Model* model, ShaderProgram* program, Texture* texture);
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
};