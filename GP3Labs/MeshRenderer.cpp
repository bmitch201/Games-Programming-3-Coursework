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
	glm::mat4 view = Application::GetInstance()->GetCamera()->GetView();
	glm::mat4 projection = Application::GetInstance()->GetCamera()->GetProj();

	glm::vec3 viewPos = Application::GetInstance()->GetCamera()->GetParentTransform()->GetPosition();
	
	GLuint loc = glGetUniformLocation(m_program->Get(), "model");
	glUniformMatrix4fv(loc, 1, false, (const GLfloat*)glm::value_ptr(model));
	loc = glGetUniformLocation(m_program->Get(), "view");
	glUniformMatrix4fv(loc, 1, false, (const GLfloat*)glm::value_ptr(view));
	loc = glGetUniformLocation(m_program->Get(), "projection");
	glUniformMatrix4fv(loc, 1, false, (const GLfloat*)glm::value_ptr(projection));

	loc = glGetUniformLocation(m_program->Get(), "viewPos");
	glUniform3f(loc, viewPos.x, viewPos.y, viewPos.z);

	glm::vec4 lightColor = glm::vec4(1.f, 1.f, 1.f, 1.f);
	
	glm::vec3 lightDir = glm::vec3(0.f, 5.f, 1.f);
	
	loc = glGetUniformLocation(m_program->Get(), "lightColor");
	glUniform4f(loc, lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	loc = glGetUniformLocation(m_program->Get(), "lightDir");
	glUniform3f(loc, lightDir.x, lightDir.y, lightDir.z);

	m_texture->Bind();
	for (Mesh* mesh : m_model->GetMeshes())
	{
		mesh->Bind();
		if (m_texture) m_texture->Bind();
		GL_ATTEMPT(glDrawElements(GL_TRIANGLES, mesh->GetIndiciesCount(), GL_UNSIGNED_INT, 0));
	}


}
