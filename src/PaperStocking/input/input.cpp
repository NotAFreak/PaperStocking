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
	std::string bindings;
	j.at("bindings").get_to(bindings);
}
// Bindings
InputBinding::InputBinding(InputBindingType type) {
	this->m_Type = type;
}

InputBinding::InputBinding(InputBindingType type, std::string mappingId) {
	this->m_Type = type;
	switch (type) {
		case InputBindingType::BINDINGTYPE_Keycode:

			break;
		case InputBindingType::BINDINGTYPE_Scancode:
			// this->m_ScancodeMapping = ScancodeInputMapping(mappingId);
			break;
		case InputBindingType::BINDINGTYPE_Axis:
			// this->m_AxisMapping = AxisInputMapping(mappingId);
			break;
		case InputBindingType::BINDINGTYPE_Button:
			// this->m_ButtonMapping = ButtonInputMapping(mappingId);
			break;
		default:
			throw std::runtime_error("Unknown type: " + type);
	}
}

float InputBinding::ReadFloat() {

}

glm::vec2 InputBinding::ReadVector() {

}

void PaperStocking::to_json(nlohmann::json& j, const InputBinding& data) {
	std::string val;
	// god i hope the union doesnt fuck this up
	switch (data.m_Type) {
		case InputBindingType::BINDINGTYPE_Keycode:
			// val = data.m_KeycodeMapping.m_Identifier;
			break;
		case InputBindingType::BINDINGTYPE_Scancode:
			// val = data.m_ScancodeMapping.m_Identifier;
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
		{"value", data.m_ButtonMapping->m_Identifier},
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
	this->m_KeyCode = GetKeycode(id);
}
static const std::map<std::string, SDL_Keycode> keycodeMap = std::map<std::string, SDL_Keycode> {
	{"A", SDLK_A},
	{"B", SDLK_B},
	{"C", SDLK_C},
	{"D", SDLK_D},
	{"E", SDLK_E},
	{"F", SDLK_F},
	{"G", SDLK_G},
	{"H", SDLK_H},
	{"I", SDLK_I},
	{"J", SDLK_J},
	{"K", SDLK_K},
	{"L", SDLK_L},
	{"M", SDLK_M},
	{"N", SDLK_N},
	{"O", SDLK_O},
	{"P", SDLK_P},
	{"Q", SDLK_Q},
	{"R", SDLK_R},
	{"S", SDLK_S},
	{"T", SDLK_T},
	{"U", SDLK_U},
	{"V", SDLK_V},
	{"W", SDLK_W},
	{"X", SDLK_X},
	{"Y", SDLK_Y},
	{"Z", SDLK_Z}
};
SDL_Keycode KeycodeInputMapping::GetKeycode(std::string id) {
	if (keycodeMap.contains(id)) {
		return keycodeMap.at(id);
	}
	throw std::runtime_error("Unknown Keycode: " + id);
}
const static std::map<std::string, SDL_Scancode> scancodeMap = std::map<std::string, SDL_Scancode> {
	{"A", SDL_SCANCODE_A},
	{"B", SDL_SCANCODE_B},
	{"C", SDL_SCANCODE_C},
	{"D", SDL_SCANCODE_D},
	{"E", SDL_SCANCODE_E},
	{"F", SDL_SCANCODE_F},
	{"G", SDL_SCANCODE_G},
	{"H", SDL_SCANCODE_H},
	{"I", SDL_SCANCODE_I},
	{"J", SDL_SCANCODE_J},
	{"K", SDL_SCANCODE_K},
	{"L", SDL_SCANCODE_L},
	{"M", SDL_SCANCODE_M},
	{"N", SDL_SCANCODE_N},
	{"O", SDL_SCANCODE_O},
	{"P", SDL_SCANCODE_P},
	{"Q", SDL_SCANCODE_Q},
	{"R", SDL_SCANCODE_R},
	{"S", SDL_SCANCODE_S},
	{"T", SDL_SCANCODE_T},
	{"U", SDL_SCANCODE_U},
	{"V", SDL_SCANCODE_V},
	{"W", SDL_SCANCODE_W},
	{"X", SDL_SCANCODE_X},
	{"Y", SDL_SCANCODE_Y},
	{"Z", SDL_SCANCODE_Z}
};
ScancodeInputMapping::ScancodeInputMapping(std::string id) {
	this->m_Identifier = id;
	this->m_ScanCode = GetScancode(id);
}

SDL_Scancode ScancodeInputMapping::GetScancode(std::string id) {
	if (scancodeMap.contains(id)) {
		return scancodeMap.at(id);
	}
	throw std::runtime_error("Unknown scancode: " + id);
}

AxisInputMapping::AxisInputMapping(std::string id) {
	this->m_Identifier = id;
}

ButtonInputMapping::ButtonInputMapping(std::string id) {
	this->m_Identifier = id;
}
