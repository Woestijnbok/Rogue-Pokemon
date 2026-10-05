#define AUDIOMANAGER_EFFECT_CHANNEL_COUNT 5

#include <SDL.h>
#include <SDL_mixer.h>
#include <queue>
#include <array>
#include <utility>
#include <mutex>
#include <algorithm>
#include <string>
#include <atomic>
#include <utility>

#include "AudioManager.h"
#include "ResourceManager.h"

using namespace Minigin;

class AudioManager::Impl	
{
public:
	explicit Impl();
	~Impl();

	Impl(const Impl& other) = delete;
	Impl(Impl&& other) noexcept = delete;
	Impl& operator=(const Impl& other) = delete;
	Impl& operator=(Impl&& other) noexcept = delete;

	void Update();	
	void HandleMusic(const std::filesystem::path& path, Action action);
	void HandleEffect(const std::filesystem::path& path, Action action);
	void StopAllEffects();
	void StopAll();
	void StopRunning();
	void Mute(bool mute);


private:
	std::queue<Request> m_Requests;
	Mix_Music* m_Music;
	std::array<std::pair<Mix_Chunk*, std::filesystem::path>, AUDIOMANAGER_EFFECT_CHANNEL_COUNT> m_EffectChannels;
	std::mutex m_Mutex;
	std::atomic<bool> m_Running;

	void ProcessRequest(const Request& request);
	void PlayMusic(const std::filesystem::path& path);
	void PauseMusic();
	void ResumeMusic();
	void StopMusic();
	void PlayEffect(const std::filesystem::path& path);
	void PauseEffect(const std::filesystem::path& path);
	void ResumeEffect(const std::filesystem::path& path);
	void StopEffect(const std::filesystem::path& path);
	void StopAllSoundEffects();
	void StopAllEffectsAndMusic();
	/*
	* Tries to find an usable channel for playing a sound effect.
	* 
	* @param channel: The channel that will be used to play the sound effect.
	* @returns: True if the channel can be reused aka the same sound effect is already loaded and finished playing.
	*/
	bool TryFindUnusedChannel(int& channel, const std::filesystem::path& path);
};

AudioManager::Impl::Impl() :
	m_Requests{},
	m_Music{},	
	m_EffectChannels{},
	m_Mutex{},
	m_Running{ false }	
{
	if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_WAVPACK) == 0)	
	{
		throw std::runtime_error(std::string("Mix_Init Error: ") + Mix_GetError());	
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)	
	{
		throw std::runtime_error(std::string("Mix_OpenAudio Error: ") + Mix_GetError());	
	}

	if (Mix_AllocateChannels(AUDIOMANAGER_EFFECT_CHANNEL_COUNT) != AUDIOMANAGER_EFFECT_CHANNEL_COUNT)
	{
		throw std::runtime_error(std::string("Mix_AllocateChannels Error: ") + Mix_GetError());
	}
}

AudioManager::Impl::~Impl()
{
	Mix_FreeMusic(m_Music);

	for (const std::pair<Mix_Chunk*, std::filesystem::path>& pair : m_EffectChannels)
	{
		Mix_FreeChunk(pair.first);	
	}	

	Mix_CloseAudio();
	Mix_Quit();
}

void AudioManager::Impl::Update()
{
	m_Running = true;

	while (m_Running)
	{
		std::unique_lock lock{ m_Mutex };

		while (!m_Requests.empty())
		{
			ProcessRequest(m_Requests.front());

			m_Requests.pop();
		}
	}
}

void AudioManager::Impl::HandleMusic(const std::filesystem::path& path, Action action)	
{
	std::unique_lock lock{ m_Mutex };
	m_Requests.push(Request{ action, Type::Music, path });			
}

void AudioManager::Impl::HandleEffect(const std::filesystem::path& path, Action action)
{
	std::unique_lock lock{ m_Mutex };	
	m_Requests.push(Request{ action, Type::Effect, path });		
}

void AudioManager::Impl::StopAllEffects()
{
	std::unique_lock lock{ m_Mutex };
	m_Requests.push(Request{ Action::Stop, Type::AllEffects, "" });
}

void AudioManager::Impl::StopAll()
{
	std::unique_lock lock{ m_Mutex };	
	m_Requests.push(Request{ Action::Stop, Type::All, "" });	
}

void AudioManager::Impl::StopRunning()
{
	if(m_Running) m_Running = false;
}

void AudioManager::Impl::Mute(bool mute)
{
	if (mute)
	{
		Mix_Volume(-1, 0);
		Mix_VolumeMusic(0);
	}
	else
	{
		Mix_Volume(-1, MIX_MAX_VOLUME);	
		Mix_VolumeMusic(MIX_MAX_VOLUME);		
	}
}

void AudioManager::Impl::ProcessRequest(const Request& request)
{
	const std::filesystem::path audioPath{ request.GetPath() };
	const std::filesystem::path fullPath{ ResourceManager::Instance()->GetAudioRootPath() / audioPath };

	// If it's an action that requires a filepath, check if the file exists and is a regular file
	if (request.GetAction() != Action::Stop)	
	{
		if (std::filesystem::exists(fullPath))
		{
			if (!std::filesystem::is_regular_file(fullPath))
			{
				throw std::runtime_error("ResourceManager::ProcessRequest() - path given isn't a regular file");
			}
		}
		else throw std::runtime_error("ResourceManager::ProcessRequest() - path given doesn't exist");
	}

	switch (request.GetType())	
	{
	case Type::Music:	
		switch (request.GetAction())
		{
		case Action::Play:
			PlayMusic(fullPath);
			break;
		case Action::Pause:
			PauseMusic();
			break;
		case Action::Resume:
			ResumeMusic();
			break;
		case Action::Stop:
			StopMusic();
			break;
		}
		break;
	case Type::Effect:
		switch (request.GetAction())
		{
			case Action::Play:
				PlayEffect(audioPath);
				break;
			case Action::Pause:
				PauseEffect(audioPath);
				break;
			case Action::Resume:
				ResumeEffect(audioPath);
				break;
			case Action::Stop:
				StopEffect(audioPath);
				break;
		}	
		break;
	case Type::AllEffects:
		StopAllSoundEffects();
		break;
	case Type::All:
		StopAllEffectsAndMusic();	
		break;
	}
}

void AudioManager::Impl::PlayMusic(const std::filesystem::path& path)
{
	StopMusic();
	m_Music = Mix_LoadMUS(path.generic_string().c_str());

	if (!m_Music)
	{
		throw std::runtime_error(std::string{ "AudioManager::Impl::StartPlayingMusic() - " } + Mix_GetError());
	}

	if (Mix_PlayMusic(m_Music, -1) == -1)
	{
		throw std::runtime_error(std::string{ "AudioManager::Impl::StartPlayingMusic() - " } + Mix_GetError());
	}
}

void AudioManager::Impl::PauseMusic()
{
	Mix_PauseMusic();
}

void AudioManager::Impl::ResumeMusic()
{
	Mix_ResumeMusic();
}

void AudioManager::Impl::StopMusic()
{
	Mix_HaltMusic();
	Mix_FreeMusic(m_Music);
	m_Music = nullptr;
}

void AudioManager::Impl::PlayEffect(const std::filesystem::path& path)		
{
	int channel { -1 };
	bool canReuseEffect{ TryFindUnusedChannel(channel, path) };

	// Couldn't find an unused channel, so we can't play the effect
	if (channel == -1)	
	{
		return;
	}
	else
	{
		const std::filesystem::path fullPath{ ResourceManager::Instance()->GetAudioRootPath() / path };

		// Couldn't reuse the Mix_Chunk , so we need to load it a new one.
		if (!canReuseEffect)
		{
			m_EffectChannels.at(channel).first = Mix_LoadWAV(fullPath.generic_string().c_str());
			
			if (m_EffectChannels.at(channel).first != nullptr)
			{
				m_EffectChannels.at(channel).second = path;
			}
			else
			{
				throw std::runtime_error(std::string{ "AudioManager::Impl::StartPlayingEffect() - " } + Mix_GetError());
			}
		}

		// Try to play the Mix_Chunk
		if (Mix_PlayChannel(channel, m_EffectChannels.at(channel).first, 0) == -1)
		{
			Mix_FreeChunk(m_EffectChannels.at(channel).first);
			m_EffectChannels.at(channel).first = nullptr;
			m_EffectChannels.at(channel).second.clear();

			throw std::runtime_error(std::string{ "AudioManager::Impl::StartPlayingEffect() - " } + Mix_GetError());
		}
	}
}

void AudioManager::Impl::PauseEffect(const std::filesystem::path& path)
{
	for (int channel { 0 }; channel < AUDIOMANAGER_EFFECT_CHANNEL_COUNT; channel++)
	{
		if (m_EffectChannels.at(channel).second == path)
		{
			Mix_Pause(channel);	
		}
	}
}

void AudioManager::Impl::ResumeEffect(const std::filesystem::path& path)
{
	for (int channel{ 0 }; channel < AUDIOMANAGER_EFFECT_CHANNEL_COUNT; channel++)
	{
		if (m_EffectChannels.at(channel).second == path)
		{
			Mix_Resume(channel);
		}
	}
}

void AudioManager::Impl::StopEffect(const std::filesystem::path& path)
{
	for (int channel{ 0 }; channel < AUDIOMANAGER_EFFECT_CHANNEL_COUNT; channel++)
	{
		if (m_EffectChannels.at(channel).second == path)
		{
			Mix_HaltChannel(channel);
		}
	}
}

void AudioManager::Impl::StopAllSoundEffects()
{
	for (int channel{ 0 }; channel < AUDIOMANAGER_EFFECT_CHANNEL_COUNT; channel++)
	{
		Mix_HaltChannel(channel);
	}
}

void AudioManager::Impl::StopAllEffectsAndMusic()
{
	StopAllSoundEffects();
	StopMusic();
}

bool AudioManager::Impl::TryFindUnusedChannel(int& channel, const std::filesystem::path& path)
{
	channel = -1;
	bool canReuseEffect{ false };

	int reusableChannel{ -1 };
	int freeChannel{ -1 };
	int finishedChannel{ -1 };

	for (int i{0}; i < AUDIOMANAGER_EFFECT_CHANNEL_COUNT; i++)
	{
		// The channel is in use
		if (m_EffectChannels.at(i).first != nullptr)
		{
			// The channel is not playing and is not paused
			if (Mix_Playing(i) == 0 and Mix_Paused(i) == 0)
			{
				// When we find a channel that can be reused, we can stop searching for a usable channel
				if (m_EffectChannels.at(i).second == path)
				{
					reusableChannel = i;
					break;
				}
				// keep looking for a channel we can free up only if there are no free channels found already
				else if(freeChannel == -1)
				{
					finishedChannel = i;
				}
			}
		}
		// Find a free channel
		if (freeChannel == -1 and m_EffectChannels.at(i).first == nullptr)
		{
			freeChannel = i;
		}
	}

	// Case 1: There is a channel with the same sound effect and it has finished playing.
	if (reusableChannel != -1)
	{
		channel = reusableChannel;
		canReuseEffect = true;
	}
	// Case 2: There is a free channel
	else if (freeChannel != -1)
	{
		channel = freeChannel;
	}
	// Case 3: We can free up the channel.
	else if (finishedChannel != -1)
	{
		Mix_FreeChunk(m_EffectChannels.at(finishedChannel).first);
		m_EffectChannels.at(finishedChannel).first = nullptr;
		m_EffectChannels.at(finishedChannel).second.clear();

		channel = finishedChannel;
	}
	// Case 4: No channels are free

	return canReuseEffect;
}

AudioManager::AudioManager() :	
	Singleton{},	
	m_Pimpl{ std::make_unique<AudioManager::Impl>() }	
{

}

AudioManager::~AudioManager() = default;

AudioManager::Request::Request(Action action, Type type, const std::filesystem::path& path) :
	m_Action{ action },
	m_Type{ type },
	m_Path{ path }
{

}

AudioManager::Action AudioManager::Request::GetAction() const
{
	return m_Action;
}

AudioManager::Type AudioManager::Request::GetType() const
{
	return m_Type;
}

const std::filesystem::path& AudioManager::Request::GetPath() const
{
	return m_Path;
}

void AudioManager::Update()
{
	m_Pimpl->Update();	
}

void AudioManager::HandleMusic(const std::filesystem::path& path, Action action)
{
	m_Pimpl->HandleMusic(path, action);
}

void AudioManager::HandleEffect(const std::filesystem::path& path, Action action)
{
	m_Pimpl->HandleEffect(path, action);
}

void AudioManager::StopAllEffects()
{
	m_Pimpl->StopAllEffects();	
}	

void AudioManager::StopAll()
{
	m_Pimpl->StopAll();	
}

void AudioManager::StopRunning()
{
	m_Pimpl->StopRunning();	
}

void Minigin::AudioManager::Mute(bool mute)
{
	m_Pimpl->Mute(mute);
}