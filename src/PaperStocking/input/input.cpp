#include <PaperStocking/input/input.hpp>

using namespace PaperStocking;

Input::Input() {
	assert(s_Instance == nullptr);
	s_Instance = this;
}

Input::~Input() {
	assert(s_Instance != nullptr);
	s_Instance = nullptr;
}

std::shared_ptr<InputController> Input::LoadInputController(std::string name, std::thread::id thread) {
	assert(s_Instance != nullptr);
	std::shared_ptr<InputController> map = std::make_shared<InputController>();
	{
		std::lock_guard lock(s_Instance->m_InputMaps_mutex);
		
	}
	return std::shared_ptr<InputController>(map);
}
void Input::WriteInputController(std::shared_ptr<InputController> map) {

}
void Input::UnloadInputController(std::shared_ptr<InputController> map) {
	assert(s_Instance != nullptr);
	std::map<std::thread::id, std::map<std::string, std::shared_ptr<InputController>>>::iterator it;
	std::lock_guard lock(s_Instance->m_InputMaps_mutex);
	bool found = false;
	std::thread::id thread;
	std::map<std::string, std::shared_ptr<InputController>>::iterator mapIt;
	for (it = s_Instance->m_InputControllers.begin(); it != s_Instance->m_InputControllers.end(); ++it) {
		for (mapIt = it->second.begin(); mapIt != it->second.end(); ++mapIt) {
			if (mapIt->second.get() == map.get()) {
				found = true;
				thread = it->first;
				goto Found;
			}
		}
	}
	Found:
	assert(found);
	s_Instance->m_InputControllers[thread].erase(mapIt);
}
// Controllers
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
		{"name", data.m_ActionName},
		{"type", data.m_ActionType},
		{"bindings", data.m_Bindings},
	};
}
void from_json(const nlohmann::json& j, InputAction& data) {
	j.at("name").get_to(data.m_ActionName);
	j.at("type").get_to(data.m_ActionType);
	std::string bindings;
	j.at("bindings").get_to(bindings);
}
// Bindings
void PaperStocking::to_json(nlohmann::json& j, const InputBinding& data) {
	j = nlohmann::json {
		{"name", data.m_Name}// ,
		// {"value", data.m_Value.ToString()},
	};
}
void PaperStocking::from_json(const nlohmann::json& j, InputBinding& data) {
	j.at("name").get_to(data.m_Name);
}

void PaperStocking::to_json(nlohmann::json& j, const InputAction& data) {
	j = nlohmann::json {
		{"name", data.m_ActionName},
		{"type", data.m_ActionType},
		{"bindings", data.m_Bindings}
	};
}
void PaperStocking::from_json(const nlohmann::json& j, InputAction& data) {
	j.at("name").get_to(data.m_ActionName);
	j.at("type").get_to(data.m_ActionType);
	j.at("bindings").get_to(data.m_Bindings);
}