#include <cassert>
#include <SDL3_image/SDL_image.h>
#include <PaperStocking/assets/metaDb.hpp>
#include <PaperStocking/cursor/cursor.hpp>

using namespace PaperStocking;

CursorVisual::CursorVisual(std::filesystem::path filePath, int hotspotX, int hotspotY) {
	if (MetaDB::MetaDatabase::HasTexture(filePath)) {
		MetaDB::T_Cursor metadata = MetaDB::MetaDatabase::GetCursor(filePath);
		m_HotspotX = metadata.hotspotX;
		m_HotspotY = metadata.hotspotY;
	}
	m_Surface = IMG_Load(filePath.c_str());
	m_Cursor = SDL_CreateColorCursor(m_Surface, m_HotspotX, m_HotspotY);
}
CursorVisual::~CursorVisual() {

}

Cursor::Cursor() {
	assert(s_Instance == nullptr);
	s_Instance = this;
}
Cursor::~Cursor() {
	assert(s_Instance != nullptr);
	s_Instance = nullptr;
}

CursorState Cursor::GetState() {
	assert(s_Instance != nullptr);
	return s_Instance->m_State;
}
void Cursor::SetState(CursorState state) {
	assert(s_Instance != nullptr);
	s_Instance->m_State = state;
}

std::weak_ptr<CursorVisual> Cursor::GetVisual() {

}

void Cursor::SetVisual(std::shared_ptr<CursorVisual> visual) {

}
