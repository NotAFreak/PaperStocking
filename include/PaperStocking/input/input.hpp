#pragma once
#include <memory>
#include <thread>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
#include <PaperStocking/input/rawInput.hpp>

#ifndef INPUT_CONFIG_FILEPATH
#define INPUT_CONFIG_FILEPATH "input.json"
#endif

namespace PaperStocking {
	enum InputActionType : uint8_t {
		ACTIONTYPE_None = 0,
		ACTIONTYPE_Button = 1,
		ACTIONTYPE_Value = 2,
		ACTIONTYPE_Vector = 3 // automatically determine if 2-value composite or 4-button composite.
		// 4-button composite goes Left-Up-Right-Down order
	};
	enum InputBindingType : uint8_t {
		BINDINGTYPE_Keycode = 0,
		BINDINGTYPE_Scancode = 1,
		BINDINGTYPE_Button = 2,
		BINDINGTYPE_Axis = 3
	};
	class BaseInputMapping {
		public:
			std::string m_Identifier;
			virtual std::string ToString() { }
			virtual BaseInputMapping FromString() { }
	};
	class KeycodeInputMapping final : public BaseInputMapping {
		public:
			SDL_Keycode m_KeyCode;
			static KeycodeInputMapping GetMapping(std::string id);
			static KeycodeInputMapping GetMapping(SDL_Keycode keycode);
	};
	class ScancodeInputMapping final : public BaseInputMapping {
		public:
			SDL_Scancode m_ScanCode;
			static ScancodeInputMapping GetMapping(std::string id);
	};
	class ButtonInputMapping final : public BaseInputMapping {
		public:
			union {
				SDL_GamepadButton m_Button_GP;
				SDL_MouseButtonFlags m_Button_M;
			};
			static ButtonInputMapping GetMapping(std::string id);
	};
	class AxisInputMapping final : public BaseInputMapping {
		public:
			union {

			};
			static AxisInputMapping GetMapping(std::string id);
	};
	struct InputBinding {
		public:
			// Serialized
			std::string m_Name;
			// Serialized
			BaseInputMapping m_Value;

			InputBinding() = default;
			~InputBinding() = default;

			float ReadFloat();
			glm::vec2 ReadVector();
	};
	void to_json(nlohmann::json& j, const InputBinding& data);
	void from_json(const nlohmann::json& j, InputBinding& data);
	struct InputAction {
		// Serialized
		std::string m_ActionName;
		// Serialized
		InputActionType m_ActionType;
		// Serialized
		std::vector<InputBinding> m_Bindings;

		void(*OnStarted);
		void(*OnCanceled);
		void(*OnPerformed);

		// Button type
		bool OnDown();
		bool OnUp();
		bool IsDown();
		bool IsUp();

		// Value type
		float ReadValue();

		// Vector type
		glm::vec2 ReadVector();
	};
	void to_json(nlohmann::json& j, const InputAction& data);
	void from_json(const nlohmann::json& j, InputAction& data);
	// Serialized
	struct InputController {
		public:
			// Serialized
			std::string m_Name;
			std::thread::id m_ThreadId = std::this_thread::get_id();
			// Serialized
			std::map<std::string, InputAction> m_Actions;

	};
	void to_json(nlohmann::json& j, const InputController& data);
	void from_json(const nlohmann::json& j, InputController& data);
	struct Input {
		public:
			Input();
			~Input();

			static void UnloadInputController(std::shared_ptr<InputController> map);
			static std::shared_ptr<InputController> LoadInputController(std::string name, std::thread::id thread = std::this_thread::get_id());
			static void WriteInputController(std::shared_ptr<InputController>);
		private:
			static inline Input* s_Instance;
			std::map<std::thread::id, std::map<std::string,std::shared_ptr<InputController>>> m_InputControllers;
			std::mutex m_InputMaps_mutex;
	};
}