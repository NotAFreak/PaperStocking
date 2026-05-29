#pragma once
#include <vector>
#include <memory>
#include "system.hpp"
namespace PaperStocking {
	struct GameManager {
		public:
			
		private:
			std::vector<std::shared_ptr<BaseSystem>> gameSystems;
	};
}