#include "pch.h"
#include "MeshRenderer.h"
#include "Entity.h"
#include "Common.h"
#include "Application.h"
#include "Camera.h"

//Used for models without normal mapping
MeshRenderer::MeshRenderer(std::shared_ptr<Model> model, std::shared_ptr<ShaderProgram> program, std::shared_ptr<Texture> texture)
{
	m_model = model;
	m_program = program;
	m_texture = texture;
}

//Used for models with Normal Mapping
MeshRenderer::MeshRenderer(std::shared_ptr<Model> model, std::shared_ptr<ShaderProgram> program, std::shared_ptr<Texture> texture, std::shared_ptr<Texture> normal)
{
	m_model = model;
	m_program = program;
	m_texture = texture;
	m_normal = normal;
}

void MeshRenderer::OnUpdate(float deltaTime)
{

}

void MeshRenderer::OnRender()
{
	//Use the passed shader program
	m_program->Use();

	//Use the model transformation matrix
	m_program->setMat4("model", m_entity->GetTransform()->GetTransformationMatrix());

	//Apply the texture to the shader
	glUniform1i(glGetUniformLocation(m_program->Get(), "thisTexture"), 0);
	//Bind the texture to position 0
	m_texture->Bind();
	//If a normal is passed in
	if (m_normal != nullptr)
	{
		//Apply the normal to the shader
		glUniform1i(glGetUniformLocation(m_program->Get(), "thisNormal"), 1);
		//Bind the normal to position 1
		m_normal->BindNormal();
	}

	//Draw the model
	for (Mesh* mesh : m_model->GetMeshes())
	{
		mesh->Bind();
		(glDrawElements(GL_TRIANGLES, mesh->GetIndiciesCount(), GL_UNSIGNED_INT, 0));
	}
}
