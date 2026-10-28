#include <PaperStocking/audio/audioManager.hpp>

using namespace PaperStocking;

AudioManager::AudioManager() {
	assert(s_Instance == nullptr);
	s_Instance = this;

	std::vector<std::string> devices = GetOutputDevices();
	if (devices.empty()) {
		throw std::runtime_error("Couldn't find any output devices for OpenAL");
	}
	for (std::string& name : devices) {
		m_Logger.Info("Found output device: " + name);
	}
	m_CurrentDevice = alcOpenDevice(nullptr);
	if (m_CurrentDevice == nullptr) {
		throw std::runtime_error("Couldn't open default device!");
	}

	if(!alcCall(alcCreateContext, m_Context, m_CurrentDevice, m_CurrentDevice, nullptr) || !m_Context){
		m_Logger.Error("Could not create audio context");
	}

	ALCboolean contextMadeCurrent = false;
	if(!alcCall(alcMakeContextCurrent, contextMadeCurrent, m_CurrentDevice, m_Context)
	   || contextMadeCurrent != ALC_TRUE)
	{
		m_Logger.Error("Could not make audio context current");
	}
	m_Logger.Info("Initialized Audio Manager!");
}
AudioManager::~AudioManager() {
	assert(s_Instance != nullptr);
	ALCboolean contextMadeCurrent = false;

	alcCall(alcMakeContextCurrent, contextMadeCurrent, m_CurrentDevice, nullptr);
	alcCall(alcDestroyContext, m_CurrentDevice, m_Context);

	if (m_CurrentDevice != nullptr) {
		alcCloseDevice(m_CurrentDevice);
		m_CurrentDevice = nullptr;
	}
	s_Instance = nullptr;
	m_Logger.Info("Shutdown Audio Manager!");
}

std::vector<std::string> AudioManager::GetOutputDevices() {
	assert(s_Instance != nullptr);
	std::vector<std::string> deviceNames;
	const ALCchar* devices;

	if(!alcCall(alcGetString, devices, nullptr, nullptr, ALC_DEVICE_SPECIFIER))
		return {};

	const char* ptr = devices;

	do {
		deviceNames.push_back(std::string(ptr));
		ptr += deviceNames.back().size() + 1;
	}
	while(*(ptr + 1) != '\0');

	return deviceNames;
}
bool AudioManager::SetOutputDevice(std::string deviceName) {
	return false;
}

std::vector<std::string> AudioManager::GetInputDevices() {
	assert(s_Instance != nullptr);
	return { };
}
bool AudioManager::SetInputDevice(std::string deviceName) {
	return false;
}

bool AudioManager::CheckALErrors(const std::string& filename, const std::uint_fast32_t line) {
	assert(s_Instance != nullptr);
	ALenum error = alGetError();
	if(error != AL_NO_ERROR) {
		s_Instance->m_Logger.Error(std::string("***AL ERROR*** (") + filename + ": " + std::to_string(line) + ")");
		switch(error) {
			case AL_INVALID_NAME:
				s_Instance->m_Logger.Error("AL_INVALID_NAME: a bad name (ID) was passed to an OpenAL function");
				break;
			case AL_INVALID_ENUM:
				s_Instance->m_Logger.Error("AL_INVALID_ENUM: an invalid enum value was passed to an OpenAL function");
				break;
			case AL_INVALID_VALUE:
				s_Instance->m_Logger.Error("AL_INVALID_VALUE: an invalid value was passed to an OpenAL function");
				break;
			case AL_INVALID_OPERATION:
				s_Instance->m_Logger.Error("AL_INVALID_OPERATION: the requested operation is not valid");
				break;
			case AL_OUT_OF_MEMORY:
				s_Instance->m_Logger.Error("AL_OUT_OF_MEMORY: the requested operation resulted in OpenAL running out of memory");
				break;
			default:
				s_Instance->m_Logger.Error("UNKNOWN AL ERROR: " + std::to_string(error));
		}
		return false;
	}
	return true;
}
bool AudioManager::CheckALCErrors(const std::string& filename, const std::uint_fast32_t line, ALCdevice* device) {
	ALCenum error = alcGetError(device);
	if(error != ALC_NO_ERROR) {
		s_Instance->m_Logger.Error(std::string("***ALC ERROR*** (") + filename + ": " + std::to_string(line) + ")");
		switch(error) {
			case ALC_INVALID_VALUE:
				s_Instance->m_Logger.Error("ALC_INVALID_VALUE: an invalid value was passed to an OpenAL function");
				break;
			case ALC_INVALID_DEVICE:
				s_Instance->m_Logger.Error("ALC_INVALID_DEVICE: a bad device was passed to an OpenAL function");
				break;
			case ALC_INVALID_CONTEXT:
				s_Instance->m_Logger.Error("ALC_INVALID_CONTEXT: a bad context was passed to an OpenAL function");
				break;
			case ALC_INVALID_ENUM:
				s_Instance->m_Logger.Error("ALC_INVALID_ENUM: an unknown enum value was passed to an OpenAL function");
				break;
			case ALC_OUT_OF_MEMORY:
				s_Instance->m_Logger.Error("ALC_OUT_OF_MEMORY: an unknown enum value was passed to an OpenAL function");
				break;
			default:
				s_Instance->m_Logger.Error("UNKNOWN ALC ERROR: " + error);
		}
		return false;
	}
	return true;
}