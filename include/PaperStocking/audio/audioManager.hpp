#pragma once
#include <al.h>
#include <alc.h>
#include <string>
#include <vector>
#include <cassert>

#include "audioListener.hpp"
#include <PaperStocking/logger.hpp>
using namespace PaperStocking;
namespace PaperStocking {
	#define alCall(function, ...) alCallImpl(__FILE__, __LINE__, function, __VA_ARGS__)
	#define alcCall(function, device, ...) alcCallImpl(__FILE__, __LINE__, function, device, __VA_ARGS__)

	struct AudioManager {
	public:
		AudioManager();
		~AudioManager();

		static std::vector<std::string> GetOutputDevices();
		static bool SetOutputDevice(std::string deviceName);
		static std::vector<std::string> GetInputDevices();
		static bool SetInputDevice(std::string deviceName);
	private:
		static inline AudioManager* s_Instance;
		AudioListener defaultListener;
		AudioListener* listener = &defaultListener;

		ALCcontext* m_Context;
		ALCdevice* m_CurrentDevice;

		Logger m_Logger = Logger("Audio Manager", LogColour::Blue);

		bool CheckALErrors(const std::string& filename, const std::uint_fast32_t line);

		//	Everything below is OpenAL shit
		// AL
		template<typename alFunction, typename... Params>
		static auto alCallImpl(const char* filename,
			const std::uint_fast32_t line,
			alFunction function,
			Params... params)
			->typename std::enable_if_t<!std::is_same_v<void, decltype(function(params...))>, decltype(function(params...))>
		{
			assert(s_Instance != nullptr);
			auto ret = function(std::forward<Params>(params)...);
			s_Instance->CheckALErrors(filename, line);
			return ret;
		}

		template<typename alFunction, typename... Params>
		static auto alCallImpl(const char* filename,
			const std::uint_fast32_t line,
			alFunction function,
			Params... params)
			->typename std::enable_if_t<std::is_same_v<void, decltype(function(params...))>, bool>
		{
			assert(s_Instance != nullptr);
			function(std::forward<Params>(params)...);
			return s_Instance->CheckALErrors(filename, line);
		}

		// ALC
		bool CheckALCErrors(const std::string& filename, const std::uint_fast32_t line, ALCdevice* device);
		template<typename alcFunction, typename... Params>
		static auto alcCallImpl(const char* filename,
						 const std::uint_fast32_t line,
						 alcFunction function,
						 ALCdevice* device,
						 Params... params)
		->typename std::enable_if_t<std::is_same_v<void,decltype(function(params...))>,bool>
		{
			assert(s_Instance != nullptr);
			function(std::forward<Params>(params)...);
			return s_Instance->CheckALCErrors(filename,line,device);
		}

		template<typename alcFunction, typename ReturnType, typename... Params>
		static auto alcCallImpl(const char* filename,
						 const std::uint_fast32_t line,
						 alcFunction function,
						 ReturnType& returnValue,
						 ALCdevice* device,
						 Params... params)
		->typename std::enable_if_t<!std::is_same_v<void,decltype(function(params...))>,bool>
		{
			assert(s_Instance != nullptr);
			returnValue = function(std::forward<Params>(params)...);
			return s_Instance->CheckALCErrors(filename,line,device);
		}
	};
}