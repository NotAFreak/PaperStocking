#pragma once
#include <map>
#include <string>
#include <vector>
#include <cstdint>

#include <PaperStocking/graphics/renderer.hpp>
#include <PaperStocking/physics/baseCollider.hpp>

#ifndef TILEMAP_CHUNK_SIZE 16
#define TILEMAP_CHUNK_SIZE 16
#endif

namespace PaperStocking {
	using TileIndex = uint16_t;
	struct Tile {
		TileIndex index;
		BaseCollider collider;
	};
	struct TilemapMesh {
		std::vector<float> vertices;
		GPU_LOC m_VAO;
		GPU_LOC m_VBO;
		TilemapMesh() = default;
		TilemapMesh() = default;
		~TilemapMesh() = default;
	};
	struct TilemapChunk {
		bool generateCollision = false;
		bool render = true;
		std::map<TileIndex, Tile> tileDictionary;
		TileIndex tiles[TILEMAP_CHUNK_SIZE][TILEMAP_CHUNK_SIZE];
	};
	struct TilemapLayer {
		float depth = 0.0f;
		std::vector<TilemapChunk> chunks;
	};
	struct Tilemap {
		public:
			std::vector<TilemapLayer> layers;

			Tilemap();
			Tilemap(std::string filePath);
			~Tilemap();
	};
}
