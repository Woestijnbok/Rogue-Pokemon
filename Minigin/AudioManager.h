#pragma once

#include <memory>
#include <filesystem>

#include "Singleton.h"
#include "Subject.h"

namespace Minigin	
{
	class AudioManager final : public Singleton<AudioManager>
	{
	public:
		enum class Action	
		{
			Play,
			Pause,
			Resume,
			Stop,
			Invalid
		};

		enum class Type
		{
			Music,
			Effect,
			AllEffects,
			All,
			Invalid
		};

		class Request final
		{
		public:
			explicit Request(Action action, Type type, const std::filesystem::path& path);
			explicit Request();
			virtual ~Request() = default;

			Request(const Request&) = default;
			Request(Request&&) noexcept = default;
			Request& operator= (const Request&) = default;
			Request& operator= (const Request&&) noexcept = delete;

			Action GetAction() const;	
			Type GetType() const;	
			const std::filesystem::path& GetPath() const;

		private:
			Action m_Action;	
			Type m_Type;	
			std::filesystem::path m_Path;

		};
			
		friend class Singleton<AudioManager>;	

		~AudioManager();

		AudioManager(const AudioManager&) = delete;
		AudioManager(AudioManager&&) noexcept = delete;
		AudioManager& operator= (const AudioManager&) = delete;
		AudioManager& operator= (const AudioManager&&) noexcept = delete;

		void Update();
		void HandleMusic(const std::filesystem::path& path, Action action);
		void HandleEffect(const std::filesystem::path& path, Action action);
		void StopAllEffects();
		void StopAll();
		void StopRunning();
		void Mute(bool mute);
		/*
		* Getter for the OnEffectFinished event.
		* 
		* @return: The OnEffectFinished event which only gets called when a sound effect is finished playing not by calling stop on it.
		*/
		Minigin::Subject<const std::filesystem::path&>& OnEffectFinished();
		

	private:
		class Impl;
		std::unique_ptr<Impl> m_Pimpl;

		explicit AudioManager();
	};
}