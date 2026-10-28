#include <fstream>
#include <PaperStocking/input/input.hpp>

using namespace PaperStocking;

Input::Input() {
	assert(s_Instance == nullptr);
	s_Instance = this;

	LoadAllInputs();
}

Input::~Input() {
	assert(s_Instance != nullptr);
	WriteAllInputs();
	s_Instance = nullptr;

}


void Input::RegisterDefaultController(std::shared_ptr<InputController> controller) {
	assert(s_Instance != nullptr);
	std::lock_guard lock(s_Instance->m_InputMaps_mutex);
	if (!s_Instance->m_InputControllers.contains(controller->m_Name)) {
		s_Instance->m_InputControllers.emplace(controller->m_Name, controller);
	}
}
void Input::LoadAllInputs() {
	assert(s_Instance != nullptr);
	std::shared_ptr<InputController> map = std::make_shared<InputController>();
	{
		std::string filePath = INPUT_CONFIG_FILEPATH;
		if (std::filesystem::exists(filePath)) {
			// Load
			std::string fileData;
			{	// Read input file
				std::ifstream inFile(filePath);
				std::stringstream stream;
				stream << inFile.rdbuf();
				fileData = stream.str();
			}
			nlohmann::json j = nlohmann::json::parse(fileData);
			std::vector<InputController> controllers;
			controllers = j.get<std::vector<InputController>>();

			std::lock_guard lock(s_Instance->m_InputMaps_mutex);
			for (InputController& controller : controllers) {
				if (s_Instance->m_InputControllers.contains(controller.m_Name)) {
					s_Instance->m_InputControllers[controller.m_Name] = std::make_shared<InputController>(controller);
				} else {
					s_Instance->m_InputControllers.emplace(controller.m_Name, std::make_shared<InputController>(controller));
				}
			}
		} else {
			// Save a new file
			WriteAllInputs();
		}
	}
}
void Input::WriteAllInputs() {
	assert(s_Instance != nullptr);
	std::lock_guard lock(s_Instance->m_InputMaps_mutex);
	{
		std::vector<InputController> controllers = std::vector<InputController>(s_Instance->m_InputControllers.size());
		for (auto& pair : s_Instance->m_InputControllers) {
			// create and add a copy
			controllers.push_back(InputController(*pair.second));
		}
		nlohmann::json complete;
		for (const InputController& controller : controllers) {
			complete.push_back(nlohmann::json(controller));
		}
		std::string filePath = INPUT_CONFIG_FILEPATH;
		std::ofstream outFile = std::ofstream(filePath);
		outFile << complete.dump(2);
	}
}

std::shared_ptr<InputController> Input::GetInputController(std::string name, std::thread::id thread) {
	assert(s_Instance != nullptr);
	std::lock_guard lock(s_Instance->m_InputMaps_mutex);
	std::shared_ptr<InputController>& newController = s_Instance->m_InputControllers[name];
	for (auto& action : newController->m_Actions) {
		action.second.m_ParentController = newController.get();
	}
	return std::shared_ptr<InputController>(newController);
}

// Controllers
void InputController::Update() {
	m_PrevState = m_CurrState;
	m_CurrState = StateData();
}
void PaperStocking::to_json(nlohmann::json& j, const InputController& data) {
	j = nlohmann::json {
		{"name", data.m_Name},
		{"actions", data.m_Actions},
	};
}
void PaperStocking::from_json(const nlohmann::json& j, InputController& data) {
	j.at("name").get_to(data.m_Name);
	j.at("actions").get_to(data.m_Actions);
}
// Actions
void to_json(nlohmann::json& j, const InputAction& data) {
	j = nlohmann::json {
		// {"name", data.m_ActionName},
		{"type", data.m_ActionType},
		{"bindings", data.m_Bindings},
	};
}
void from_json(const nlohmann::json& j, InputAction& data) {
	// j.at("name").get_to(data.m_ActionName);
	j.at("type").get_to(data.m_ActionType);
	// std::string bindings;
	j.at("bindings").get_to(data.m_Bindings);
}
// Bindings
InputBinding::InputBinding(InputBindingType type) {
	this->m_Type = type;
}
InputBinding::InputBinding(InputBindingType type, std::string mappingId) {
	this->m_Type = type;
	std::string id = mappingId;
	switch (this->m_Type) {
		case InputBindingType::BINDINGTYPE_Keycode: {
			std::vector<std::string> codes;
			std::string code = id;
			std::erase(id, ' ');
			std::stringstream ss(id);
			while (ss.good()) {
				std::string substr;
				getline(ss, substr, ',');
				codes.push_back(substr);
			}
			for (int i = 0; i < codes.size(); i++) {
				this->m_KeycodeMapping[i] = KeycodeInputMapping(codes[i]);
			}
		}
			break;
		case InputBindingType::BINDINGTYPE_Scancode: {
			std::vector<std::string> codes;
			std::string code = id;
			std::erase(id, ' ');
			std::stringstream ss(id);
			while (ss.good()) {
				std::string substr;
				getline(ss, substr, ',');
				codes.push_back(substr);
			}
			for (int i = 0; i < codes.size(); i++) {
				this->m_ScancodeMapping[i] = ScancodeInputMapping(codes[i]);
			}
		}
			break;
		case InputBindingType::BINDINGTYPE_Axis:
			// data.m_AxisMapping = AxisInputMapping(id);
			break;
		case InputBindingType::BINDINGTYPE_Button:
			// data.m_ButtonMapping = ButtonInputMapping(id);
			break;
		default:
			throw std::runtime_error("Unknown type: " + m_Type);
	}
}

void PaperStocking::to_json(nlohmann::json& j, const InputBinding& data) {
	std::string val;
	// god i hope the union doesnt fuck this up
	switch (data.m_Type) {
		case InputBindingType::BINDINGTYPE_Keycode:
			for (int i = 0; i < 4; i++) {
				val += data.m_KeycodeMapping[i].m_Identifier + ",";
			}
			break;
		case InputBindingType::BINDINGTYPE_Scancode:
			for (int i = 0; i < 4; i++) {
				val += data.m_ScancodeMapping[i].m_Identifier + ",";
			}
			break;
		case InputBindingType::BINDINGTYPE_Axis:
			// val = data.m_AxisMapping.m_Identifier;
			break;
		case InputBindingType::BINDINGTYPE_Button:
			// val = data.m_ButtonMappingm_Identifier;
			break;
	}
	j = nlohmann::json {
		{"type", data.m_Type},
		{"value", val},
	};
}
void PaperStocking::from_json(const nlohmann::json& j, InputBinding& data) {
	j.at("type").get_to(data.m_Type);
	std::string id = "";
	j.at("value").get_to(id);
	switch (data.m_Type) {
		case InputBindingType::BINDINGTYPE_Keycode: {
			std::vector<std::string> codes;
			std::string code = id;
			std::erase(id, ' ');
			std::stringstream ss(id);
			while (ss.good()) {
				std::string substr;
				getline(ss, substr, ',');
				codes.push_back(substr);
			}
			for (int i = 0; i < codes.size(); i++) {
				data.m_KeycodeMapping[i] = KeycodeInputMapping(codes[i]);
			}
			}
			break;
		case InputBindingType::BINDINGTYPE_Scancode: {
			std::vector<std::string> codes;
			std::string code = id;
			std::erase(id, ' ');
			std::stringstream ss(id);
			while (ss.good()) {
				std::string substr;
				getline(ss, substr, ',');
				codes.push_back(substr);
			}
			for (int i = 0; i < codes.size(); i++) {
				data.m_ScancodeMapping[i] = ScancodeInputMapping(codes[i]);
			}
			}
			break;
		case InputBindingType::BINDINGTYPE_Axis:
			// data.m_AxisMapping = AxisInputMapping(id);
			break;
		case InputBindingType::BINDINGTYPE_Button:
			// data.m_ButtonMapping = ButtonInputMapping(id);
			break;
		default:
			throw std::runtime_error("Unknown type: " + data.m_Type);
	}
}

glm::vec2 InputAction::ReadVector() {
	assert(m_ParentController != nullptr);
	glm::vec2 outputBinding = glm::vec2(0.0f);
	for (InputBinding& binding : m_Bindings) {
		glm::vec2 composite = glm::vec2(0.0f);
		switch (binding.m_Type) {
			case InputBindingType::BINDINGTYPE_Keycode: {
				KeyboardState& state = m_ParentController->m_CurrState.m_KeyboardState;
				if (state.IsKeycode(binding.m_KeycodeMapping[0].m_KeyCode)) {
					composite += glm::vec2{-1.0f, 0.0f};
				}
				if (state.IsKeycode(binding.m_KeycodeMapping[1].m_KeyCode)) {
					composite += glm::vec2{0.0f, 1.0f};
				}
				if (state.IsKeycode(binding.m_KeycodeMapping[2].m_KeyCode)) {
					composite += glm::vec2{1.0f, 0.0f};
				}
				if (state.IsKeycode(binding.m_KeycodeMapping[3].m_KeyCode)) {
					composite += glm::vec2{0.0f, -1.0f};
				}
			}
				break;
			case InputBindingType::BINDINGTYPE_Scancode: {
					KeyboardState& state = m_ParentController->m_CurrState.m_KeyboardState;
					if (state.IsScancode(binding.m_ScancodeMapping[0].m_ScanCode)) {
						composite += glm::vec2{-1.0f, 0.0f};
					}
					if (state.IsScancode(binding.m_ScancodeMapping[1].m_ScanCode)) {
						composite += glm::vec2{0.0f, 1.0f};
					}
					if (state.IsScancode(binding.m_ScancodeMapping[2].m_ScanCode)) {
						composite += glm::vec2{1.0f, 0.0f};
					}
					if (state.IsScancode(binding.m_ScancodeMapping[3].m_ScanCode)) {
						composite += glm::vec2{0.0f, -1.0f};
					}
				}
				break;
		}
		float compositeLength = glm::abs(glm::length(composite));
		float outputLength = glm::abs(glm::length(outputBinding));

		if (compositeLength > outputLength) {
			outputBinding = composite;
		}
	}
	return outputBinding;
}

void PaperStocking::to_json(nlohmann::json& j, const InputAction& data) {
	j = nlohmann::json {
		// {"name", data.m_ActionName},
		{"type", data.m_ActionType},
		{"bindings", data.m_Bindings}
	};
}
void PaperStocking::from_json(const nlohmann::json& j, InputAction& data) {
	// j.at("name").get_to(data.m_ActionName);
	j.at("type").get_to(data.m_ActionType);
	j.at("bindings").get_to(data.m_Bindings);
}

// oh god the mappings
KeycodeInputMapping::KeycodeInputMapping(std::string id) {
	this->m_Identifier = id;
	this->m_KeyCode = SDL_GetKeyFromName(id.c_str());
}
ScancodeInputMapping::ScancodeInputMapping(std::string id) {
	this->m_Identifier = id;
	this->m_ScanCode = SDL_GetScancodeFromName(id.c_str());
}

AxisInputMapping::AxisInputMapping(std::string id) {
	this->m_Identifier = id;
}

ButtonInputMapping::ButtonInputMapping(std::string id) {
	this->m_Identifier = id;
}
