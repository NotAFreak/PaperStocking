#pragma once

namespace PaperStocking::Components {
	struct cmp_Camera {
		int priority = 0;
		float size = 10.0f;
		float near = 0.01f;
		float far = 50.0f;
	};
}