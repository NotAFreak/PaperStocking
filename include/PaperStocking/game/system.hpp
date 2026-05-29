#pragma once
#include "../PaperStocking/engine.hpp"

namespace PaperStocking {
	class BaseSystem {
		virtual void Load();
		virtual void Update();
	};
}