#include <PaperStocking/graphics/renderable/spriteRenderable.hpp>
#include <PaperStocking/graphics/material.hpp>
#include "PaperStocking/graphics/texture.hpp"

using namespace PaperStocking;

SpriteRenderable::SpriteRenderable(std::string materialName) {
	m_Material = MaterialManager::LoadMaterial(materialName);
}

SpriteRenderable::~SpriteRenderable() {

}

void SpriteRenderable::Render() {
	if (m_Material.get() == nullptr) return;
	glUseProgram(m_Material->shader->handle);
	glDisable(GL_DEPTH_TEST);
	glFrontFace(GL_CW);
	int modelUniform = glGetUniformLocation(m_Material->shader->handle, "uModel");
	if(modelUniform == -1) {
		throw std::runtime_error("Couldn't find model uniform!");
	}
	glm::mat4 mat = m_Transform.GetMatrix();
	glUniformMatrix4fv(modelUniform, 1, false, &mat[0][0]);

	glBindVertexArray(Renderer::m_QuadData.m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}