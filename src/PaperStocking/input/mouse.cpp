#include <PaperStocking/input/mouse.hpp>

using namespace PaperStocking;

MouseState::MouseState() {
	m_Buttons = SDL_GetMouseState(&m_Position.x, &m_Position.y);
}
bool MouseState::IsButton(const SDL_MouseButtonFlags &button) const {
	return m_Buttons & button;
}

Mouse::Mouse() {
	m_CurrState = MouseState();
	m_PrevState = m_CurrState;
}
void Mouse::Update() {
	m_PrevState = m_CurrState;
	m_CurrState = MouseState();
}

bool Mouse::IsButtonDown(const SDL_MouseButtonFlags &button) const {
	return !m_PrevState.IsButton(button) && m_CurrState.IsButton(button);
}
bool Mouse::IsButtonUp(const SDL_MouseButtonFlags &button) const {
	return m_PrevState.IsButton(button) && !m_CurrState.IsButton(button);
}
bool Mouse::IsButton(const SDL_MouseButtonFlags &button) const {
	return m_CurrState.IsButton(button);
}
