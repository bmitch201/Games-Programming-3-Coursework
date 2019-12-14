#include "pch.h"
#include "MeshRenderer.h"
#include "Entity.h"
#include "Common.h"
#include "Application.h"
#include "Camera.h"

MeshRenderer::MeshRenderer(std::shared_ptr<Model> model, std::shared_ptr<ShaderProgram> program, std::shared_ptr<Texture> texture)
{
	m_model = model;
	m_program = program;
	m_texture = texture;
}

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
	m_program->Use();

	m_program->setMat4("model", m_entity->GetTransform()->GetTransformationMatrix());

	glUniform1i(glGetUniformLocation(m_program->Get(), "thisTexture"), 0);
	glUniform1i(glGetUniformLocation(m_program->Get(), "thisNormal"), 1);
	m_texture->Bind();
	if (m_normal != nullptr)
	{
		LOG_DEBUG(std::to_string(m_program->Get()), Log::Trace);
		glUniform1i(glGetUniformLocation(m_program->Get(), "thisNormal"), 1);
		m_normal->BindNormal();
	}

	for (Mesh* mesh : m_model->GetMeshes())
	{
		mesh->Bind();
		(glDrawElements(GL_TRIANGLES, mesh->GetIndiciesCount(), GL_UNSIGNED_INT, 0));
	}
}
