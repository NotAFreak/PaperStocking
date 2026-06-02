#pragma once
#include <ini.h>
#include <memory>
#include <glm/glm.hpp>
#include <PaperStocking/input/rawInput.hpp>

namespace PaperStocking {
	struct InputBinding {
		public:
			union {
				SDL_Keycode key;
			};
			float ReadFloat();
			glm::vec2 ReadVector();
	};
	struct InputMap {
		public:
			std::string m_Name;
			std::map<std::string, InputBinding> m_Actions;
		private:
	};
	struct Input {
		public:
			Input();
			~Input();

			static std::shared_ptr<InputMap> LoadInputMap(std::string name);
			static void WriteInputMap(std::shared_ptr<InputMap>);
		private:
	};
}