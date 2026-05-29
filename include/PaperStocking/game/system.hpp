#pragma once
#include <PaperStocking/engine.hpp>

namespace PaperStocking {
	class BaseSystem {
		public:
			BaseSystem() = default;
			virtual ~BaseSystem() = default;
			virtual void Load() { }
			virtual void Update() { }
	};
}