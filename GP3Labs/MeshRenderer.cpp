#include "pch.h"
#include "MeshRenderer.h"
#include "Entity.h"
#include "Common.h"
#include "Application.h"
#include "Camera.h"

MeshRenderer::MeshRenderer(/*Mesh* mesh*/std::shared_ptr<Model> model, std::shared_ptr<ShaderProgram> program, std::shared_ptr<Texture> texture)
{
	//m_mesh = mesh;
	m_model = model;
	m_program = program;
	m_texture = texture;
}

void MeshRenderer::OnUpdate(float deltaTime)
{
	//LOG_DEBUG("I'm Updating!");
}

void MeshRenderer::OnRender()
{
	m_program->Use();
	//set uniforms here!
	glm::mat4 model = m_entity->GetTransform()->GetTransformationMatrix();
	glm::mat4 view = glm::lookAt(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 1.f) - glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
	glm::mat4 mvp = Application::GetInstance()->GetCamera()->Get() * model;
	
	GLuint loc = glGetUniformLocation(m_program->Get(), "model");
	glUniformMatrix4fv(loc, 1, false, (const GLfloat*)glm::value_ptr(model));
	loc = glGetUniformLocation(m_program->Get(), "view");
	glUniformMatrix4fv(loc, 1, false, (const GLfloat*)glm::value_ptr(view));
	loc = glGetUniformLocation(m_program->Get(), "MVP");
	glUniformMatrix4fv(loc, 1, false, (const GLfloat*)glm::value_ptr(mvp));
	
	glm::vec3 oColor = glm::vec3(.5f, .5f, .5f);
	glm::vec3 lightDir = glm::normalize(Application::GetInstance()->GetCamera()->GetParentTransform()->GetPosition() - (Application::GetInstance()->GetCamera()->GetParentTransform()->GetPosition() + glm::vec3(5.f, 5.f, 0.f)));

	loc = glGetUniformLocation(m_program->Get(), "objectColor");
	glUniform3f(loc, oColor.x, oColor.y, oColor.z);
	loc = glGetUniformLocation(m_program->Get(), "lightDir");
	glUniform3f(loc, lightDir.x, lightDir.y, lightDir.z);

	m_texture->Bind();
	//m_mesh->Bind();	
	//GL_ATTEMPT(glDrawElements(GL_TRIANGLES, m_mesh->GetIndiciesCount(), GL_UNSIGNED_INT, 0));
	for (Mesh* mesh : m_model->GetMeshes())
	{
		mesh->Bind();
		if (m_texture) m_texture->Bind();
		GL_ATTEMPT(glDrawElements(GL_TRIANGLES, mesh->GetIndiciesCount(), GL_UNSIGNED_INT, 0));
	}


}
