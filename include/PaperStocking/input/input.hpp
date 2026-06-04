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
	};
	class KeycodeInputMapping final : public BaseInputMapping {
		public:
			SDL_Keycode m_KeyCode;
			KeycodeInputMapping() = default;
			KeycodeInputMapping(std::string id);

			static SDL_Keycode GetKeycode(std::string id);
	};
	class ScancodeInputMapping final : public BaseInputMapping {
		public:
			SDL_Scancode m_ScanCode;
			ScancodeInputMapping() = default;
			ScancodeInputMapping(std::string id);

			static SDL_Scancode GetScancode(std::string id);
	};
	class ButtonInputMapping final : public BaseInputMapping {
		public:
			SDL_GamepadButton m_Button_GP;
			SDL_MouseButtonFlags m_Button_M;

			ButtonInputMapping() = default;
			ButtonInputMapping(std::string id);
	};
	class AxisInputMapping final : public BaseInputMapping {
		public:
			AxisInputMapping() = default;
			AxisInputMapping(std::string id);
	};

	// Serialized
	struct InputBinding {
		public:
			// Serialized
			InputBindingType m_Type;
			// Serialized
			// union {
				KeycodeInputMapping m_KeycodeMapping[4];
				ScancodeInputMapping m_ScancodeMapping[4];
				AxisInputMapping m_AxisMapping[2];
				ButtonInputMapping m_ButtonMapping[4];
			// };

			InputBinding() = default;
			InputBinding(InputBindingType type);
			InputBinding(InputBindingType type, std::string mappingId);
			~InputBinding() = default;

			float ReadFloat();
			glm::vec2 ReadVector();
	};
	void to_json(nlohmann::json& j, const InputBinding& data);
	void from_json(const nlohmann::json& j, InputBinding& data);

	// Serialized
	struct InputController;
	struct InputAction {
		// Serialized
		InputActionType m_ActionType;
		// Serialized
		std::vector<InputBinding> m_Bindings;

		InputController* m_ParentController = nullptr;

		void(*OnStarted) = nullptr;
		void(*OnCanceled) = nullptr;
		void(*OnPerformed) = nullptr;

		InputAction() = default;
		~InputAction() = default;

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
			struct StateData {
				public:
					KeyboardState m_KeyboardState;
					MouseState m_MouseState;
					GamepadState m_GamepadState;

					StateData() {
						m_KeyboardState = KeyboardState();
						m_MouseState = MouseState();
						m_GamepadState = GamepadState();
					}
			};
			// Serialized
			std::string m_Name;
			// Serialized
			std::map<std::string, InputAction> m_Actions;

			StateData m_PrevState = StateData();
			StateData m_CurrState = StateData();

			InputController() = default;
			~InputController() = default;
			void Update();
	};
	void to_json(nlohmann::json& j, const InputController& data);
	void from_json(const nlohmann::json& j, InputController& data);

	struct Input {
		public:
			Input();
			~Input();

			static void LoadAllInputs();
			static void WriteAllInputs();

			static void RegisterDefaultController(std::shared_ptr<InputController> controller);
			static std::shared_ptr<InputController> GetInputController(std::string name, std::thread::id thread = std::this_thread::get_id());
		private:
			static inline Input* s_Instance;
			std::map<std::string, std::shared_ptr<InputController>> m_InputControllers;
			std::mutex m_InputMaps_mutex;
	};
}