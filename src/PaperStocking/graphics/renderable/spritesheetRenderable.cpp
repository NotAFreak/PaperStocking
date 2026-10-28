#include <PaperStocking/graphics/renderable/spritesheetRenderable.hpp>
#include <PaperStocking/graphics/material.hpp>
#include "PaperStocking/graphics/texture.hpp"

using namespace PaperStocking;

SpritesheetRenderable::SpritesheetRenderable(std::string materialName) {
	m_Material = MaterialManager::LoadMaterial(materialName);
}

SpritesheetRenderable::~SpritesheetRenderable() {

}

void SpritesheetRenderable::Render() {
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

	uint textureIndex = 0;
	for (Uniform& uniform : m_Material->uniforms) {
		switch (uniform.type) {
			case Type_Texture:
				glActiveTexture(GL_TEXTURE0 + textureIndex);
				Renderer::SetUniformI(m_Material->shader->handle, uniform.name, textureIndex);
				glBindTexture(GL_TEXTURE_2D, uniform.textureValue->GetHandle());
				textureIndex++;
				break;
			default:
				throw std::runtime_error("Unknown uniform type: " + std::to_string(uniform.type));
				break;
		}
	}

	glBindVertexArray(Renderer::m_QuadData.m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

Vector2 SpritesheetRenderable::GenerateUVs(SpriteIndex index, Vector2 originalUV) {
	switch (m_Direction) {
		case DIR_RIGHTDOWN:
			return {0.0f, 0.0f};
			break;
	}
	return originalUV;
}
