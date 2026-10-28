#pragma once

namespace PaperStocking {
	class BaseRenderable {
		public:
			virtual void Render() { }
			virtual void Upload() { }
			virtual void Unload() { }
	};
}