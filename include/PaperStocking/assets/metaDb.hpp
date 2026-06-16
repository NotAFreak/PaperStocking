#pragma once
#include <map>
#include <thread>
#include <cstdint>
#include <sqlite3.h>
#include <PaperStocking/logger.hpp>
#include <PaperStocking/graphics/texture.hpp>
// The database for storing asset metadata
namespace PaperStocking::MetaDB {
	enum FileType : int64_t {
		TEXTURE = 0,
		CURSOR = 1
	};
	struct T_MetaData {
		int64_t fileId;
		std::string filePath;
		FileType type;
	};
	struct T_Texture {
		int64_t fileId;
		TextureFormat format;
		uint16_t width;
		uint16_t height;
		TextureFilter minFilter;
		TextureFilter magFilter;
		TextureWrap wrap;
		TextureFilter downsampleFilter;
		bool genMipmaps;
	};
	struct T_Cursor {
		int64_t fileId;
		uint16_t hotspotX;
		uint16_t hotspotY;
	};

	struct MetaDatabase {
		public:
			MetaDatabase(const std::string &dbLocation);
			~MetaDatabase();

			static bool HasTexture(std::string filePath);
			static T_Texture GetTexture(std::string filePath);

			static bool HasCursor(std::string filePath);
			static T_Cursor GetCursor(std::string filePath);

			static void LoadToMemory();
			static void UnloadFromMemory();
		private:
			static inline MetaDatabase* s_Instance;
			sqlite3* m_Db;
			sqlite3_stmt* m_MetadataCmd;
			sqlite3_stmt* m_TextureCmd;
			sqlite3_stmt* m_CursorCmd;

			std::mutex m_CommandMutex;
			std::string m_DbLocation;

			std::map<std::string, T_MetaData> t_MetaData;
			std::map<uint64_t, T_Texture> t_Texture;
			std::map<uint64_t, T_Cursor> t_Cursor;

			Logger m_Logger = Logger("MetaData Database", LogColour::Yellow);

			static void LoadToMemoryNoLock();
			static void UnloadFromMemoryNoLock();

			static void PrepareStatements();
	};
}