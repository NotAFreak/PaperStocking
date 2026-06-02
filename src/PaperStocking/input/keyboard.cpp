#include <PaperStocking/input/keyboard.hpp>

using namespace PaperStocking;

KeyboardState::KeyboardState() {
	const bool* sdlKeys = SDL_GetKeyboardState(&m_KeyCount);
	// Make a copied snapshot
	m_Keys.reserve(m_KeyCount);
	m_Keys.insert(m_Keys.begin(), &sdlKeys[0], &sdlKeys[m_KeyCount]);
}
KeyboardState::~KeyboardState() {
	m_Keys.clear();
}

bool KeyboardState::IsScancode(const SDL_Scancode& scancode) {
	return m_Keys[scancode];
	return false;
}
bool KeyboardState::IsKeycode(const SDL_Keycode& keycode) {
	return m_Keys[keycode];
	return false;
}

Keyboard::Keyboard() {
	m_CurrState = KeyboardState();
	m_PrevState = m_CurrState;
}

void Keyboard::Update() {
	m_PrevState = m_CurrState;
	m_CurrState = KeyboardState();
}
// started press this frame
bool Keyboard::IsScancodeDown(const SDL_Scancode& scancode) {
	return m_CurrState.IsScancode(scancode) && !m_PrevState.IsScancode(scancode);
}
// stopped this frame
bool Keyboard::IsScancodeUp(const SDL_Scancode& scancode) {
	return !m_CurrState.IsScancode(scancode) && m_PrevState.IsScancode(scancode);
}
// just if it's down this frame
bool Keyboard::IsScancode(const SDL_Scancode& scancode) {
	return m_CurrState.IsScancode(scancode);
}

// started press this frame
bool Keyboard::IsKeycodeDown(const SDL_Keycode& keycode) {
	return m_CurrState.IsKeycode(keycode) && !m_PrevState.IsKeycode(keycode);
}
// stopped this frame
bool Keyboard::IsKeycodeUp(const SDL_Keycode& keycode) {
	return m_CurrState.IsKeycode(keycode) && m_PrevState.IsKeycode(keycode);
}
// just if it's down this frame
bool Keyboard::IsKeycode(const SDL_Keycode& keycode) {
	return m_CurrState.IsKeycode(keycode);
}