#pragma once

#include <memory>
#include <filesystem>

#include "Singleton.h"

namespace Minigin	
{
	class AudioManager final : public Singleton<AudioManager>
	{
	public:
		enum class Action	
		{
			Play,
			Stop,
		};

		enum class Type
		{
			Music,
			Effect,
			All
		};

		class Request final
		{
		public:
			explicit Request(Action action, Type type, const std::filesystem::path& path);
			virtual ~Request() = default;

			Request(const Request&) = default;
			Request(Request&&) noexcept = default;
			Request& operator= (const Request&) = delete;
			Request& operator= (const Request&&) noexcept = delete;

			Action GetAction() const;	
			Type GetType() const;	
			const std::filesystem::path& GetPath() const;

		private:
			const Action m_Action;	
			const Type m_Type;	
			const std::filesystem::path m_Path;

		};
			
		friend class Singleton<AudioManager>;	

		~AudioManager();

		AudioManager(const AudioManager&) = delete;
		AudioManager(AudioManager&&) noexcept = delete;
		AudioManager& operator= (const AudioManager&) = delete;
		AudioManager& operator= (const AudioManager&&) noexcept = delete;

		void Update();
		void PlayMusic(const std::filesystem::path& path);
		void PlayEffect(const std::filesystem::path& path);
		void StopMusic();
		void StopAll();
		void StopRunning();
		void Mute(bool mute);
		

	private:
		class Impl;
		std::unique_ptr<Impl> m_Pimpl;

		explicit AudioManager();
	};
}