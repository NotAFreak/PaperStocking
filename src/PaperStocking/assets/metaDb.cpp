#pragma once
#include <cassert>
#include <PaperStocking/assets/metaDb.hpp>

using namespace PaperStocking::MetaDB;

MetaDatabase::MetaDatabase(const std::string &dbLocation) {
	assert(s_Instance == nullptr);
	s_Instance = this;
	this->m_DbLocation = dbLocation;
	int status = sqlite3_open_v2(this->m_DbLocation.c_str(), &m_Db, SQLITE_OPEN_READONLY, 0);
	if (status != SQLITE_OK) {
		m_Logger.Error("Failed to open database! Error code: " + std::to_string(status) + "\n, Message: " + sqlite3_errmsg(m_Db));
	}
	PrepareStatements();
	LoadToMemoryNoLock();
}

MetaDatabase::~MetaDatabase() {
	assert(s_Instance != nullptr);
	std::lock_guard lock(this->m_CommandMutex);
	assert(m_Db != nullptr);
	sqlite3_close_v2(m_Db);
	sqlite3_finalize(m_MetadataCmd);
	sqlite3_finalize(m_TextureCmd);
	UnloadFromMemoryNoLock();
	s_Instance = nullptr;
}

void MetaDatabase::PrepareStatements() {
	assert(s_Instance != nullptr);
	{
		int rc = sqlite3_prepare_v2(s_Instance->m_Db,
			"SELECT * FROM Metadata;",
			-1,&s_Instance->m_MetadataCmd, nullptr
		);

		if (rc != SQLITE_OK) {
			s_Instance->m_Logger.Error("Failed to prepare metadata query!");
		}
	}
	{
		int rc = sqlite3_prepare_v2(s_Instance->m_Db,
			"SELECT * FROM Texture;",
			-1,&s_Instance->m_TextureCmd, nullptr
		);

		if (rc != SQLITE_OK) {
			s_Instance->m_Logger.Error("Failed to prepare texture query!");
		}
	}
	{
		int rc = sqlite3_prepare_v2(s_Instance->m_Db,
			"SELECT * FROM Texture;",
			-1,&s_Instance->m_TextureCmd, nullptr
		);

		if (rc != SQLITE_OK) {
			s_Instance->m_Logger.Error("Failed to prepare texture query!");
		}
	}
}

void MetaDatabase::LoadToMemory() {
	assert(s_Instance != nullptr);
	assert(s_Instance->m_Db != nullptr);
	std::lock_guard lock(s_Instance->m_CommandMutex);
	LoadToMemoryNoLock();
}

void MetaDatabase::UnloadFromMemory() {
	assert(s_Instance->m_Db != nullptr);
	assert(s_Instance != nullptr);
	std::lock_guard lock(s_Instance->m_CommandMutex);
	UnloadFromMemoryNoLock();
}

void MetaDatabase::LoadToMemoryNoLock() {
	assert(s_Instance != nullptr);
	{
		std::map<std::string, T_MetaData> t_MetadataMap;
		int rc = 0;
		while ((rc = sqlite3_step(s_Instance->m_MetadataCmd)) == SQLITE_ROW) {
			T_MetaData row;
			row.fileId = sqlite3_column_int64(s_Instance->m_MetadataCmd, 0);
			row.filePath = reinterpret_cast<const char*>(sqlite3_column_text(s_Instance->m_MetadataCmd, 1));
			row.type = static_cast<FileType>(sqlite3_column_int(s_Instance->m_MetadataCmd, 2));

			t_MetadataMap.emplace(row.filePath, row);
		}
		if (rc != SQLITE_DONE) {
			s_Instance->m_Logger.Error("Error while reading metadata: " + std::string(sqlite3_errmsg(s_Instance->m_Db)));
		}
		s_Instance->t_MetaData.clear();
		s_Instance->t_MetaData = t_MetadataMap;
	} {
		std::map<uint64_t, T_Texture> t_TextureMap;
		int rc = 0;
		while ((rc = sqlite3_step(s_Instance->m_TextureCmd)) == SQLITE_ROW) {
			T_Texture row;
			row.fileId = sqlite3_column_int64(s_Instance->m_TextureCmd, 0);
			row.format = static_cast<TextureFormat>(sqlite3_column_int(s_Instance->m_TextureCmd, 1));
			row.width = sqlite3_column_int(s_Instance->m_TextureCmd, 2);
			row.height = sqlite3_column_int(s_Instance->m_TextureCmd, 3);
			row.minFilter = static_cast<TextureFilter>(sqlite3_column_int(s_Instance->m_TextureCmd, 4));
			row.magFilter = static_cast<TextureFilter>(sqlite3_column_int(s_Instance->m_TextureCmd, 5));
			row.wrap = static_cast<TextureWrap>(sqlite3_column_int(s_Instance->m_TextureCmd, 6));
			row.downsampleFilter = static_cast<TextureFilter>(sqlite3_column_int(s_Instance->m_TextureCmd, 7));
			row.genMipmaps = sqlite3_column_int(s_Instance->m_TextureCmd, 8);

			t_TextureMap.emplace(row.fileId, row);
		}
		if (rc != SQLITE_DONE) {
			s_Instance->m_Logger.Error("Error while reading textures: " + std::string(sqlite3_errmsg(s_Instance->m_Db)));
		}
		s_Instance->t_Texture.clear();
		s_Instance->t_Texture = t_TextureMap;
	}

	s_Instance->m_Logger.Info("Loaded asset metadata!");
}
void MetaDatabase::UnloadFromMemoryNoLock() {
	s_Instance->t_MetaData.clear();
	s_Instance->t_Texture.clear();
	s_Instance->m_Logger.Info("Unloaded asset metadata!");
}

bool MetaDatabase::HasTexture(std::string filePath) {
	if (!s_Instance->t_MetaData.contains(filePath)) {
		return false;
	}
	int fileId = s_Instance->t_MetaData[filePath].fileId;
	return s_Instance->t_Texture.contains(fileId);
}
T_Texture MetaDatabase::GetTexture(std::string filePath) {
	assert(HasTexture(filePath));
	return s_Instance->t_Texture[s_Instance->t_MetaData[filePath].fileId];
}