#define SDL_MIXER_AUDIO_NUMBER_OF_CHANNELS 5		

#include <SDL.h>
#include <SDL_mixer.h>
#include <queue>
#include <array>
#include <utility>
#include <mutex>
#include <algorithm>
#include <string>
#include <atomic>

#include "AudioManager.h"
#include "ResourceManager.h"

using namespace Minigin;

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
	void PlayMusic(const std::filesystem::path& path);
	void PlayEffect(const std::filesystem::path& path);	
	void StopMusic();
	void StopAll();
	void StopRunning();
	void Mute(bool mute);


private:
	std::queue<Request> m_Requests;
	Mix_Music* m_Music;
	std::array<Mix_Chunk*, SDL_MIXER_AUDIO_NUMBER_OF_CHANNELS> m_EffectChannels;
	std::mutex m_Mutex;
	std::atomic<bool> m_Running;

	void ProcessRequest(const Request& request);
	void StartPlayingMusic(const std::filesystem::path& path);
	void StartPlayingEffect(const std::filesystem::path& path);
	void StopPlayingMusic();
	void StopAllEffectsAndMusic();
	void CheckEffectChannels();
	/*
	* @brief Testing comment documentation (doxygen-style)
	* @return nothing a number
	* @param chunck is there for fun
	*/
	int GetChunckIndex(Mix_Chunk* chunck) const;

};

AudioManager::Impl::Impl() :
	m_Requests{},
	m_Music{},	
	m_EffectChannels{ nullptr, nullptr, nullptr, nullptr, nullptr },
	m_Mutex{},
	m_Running{ false }	
{
	if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_WAVPACK) == 0)	
	{
		throw std::runtime_error(std::string("Mix_Init Error: ") + Mix_GetError());	
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, SDL_MIXER_AUDIO_NUMBER_OF_CHANNELS, 2048) == -1)	
	{
		throw std::runtime_error(std::string("Mix_OpenAudio Error: ") + Mix_GetError());	
	}
}

AudioManager::Impl::~Impl()
{
	Mix_FreeMusic(m_Music);

	for (Mix_Chunk* chunck : m_EffectChannels)	
	{
		Mix_FreeChunk(chunck);	
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

		CheckEffectChannels();	
	}
}

void AudioManager::Impl::PlayMusic(const std::filesystem::path& path)	
{
	std::unique_lock lock{ m_Mutex };
	m_Requests.push(Request{ Action::Play, Type::Music, path });			
}

void AudioManager::Impl::PlayEffect(const std::filesystem::path& path)
{
	std::unique_lock lock{ m_Mutex };	
	m_Requests.push(Request{ Action::Play, Type::Effect, path });		
}

void AudioManager::Impl::StopMusic()
{
	std::unique_lock lock{ m_Mutex };	
	m_Requests.push(Request{ Action::Stop, Type::Music, "" });	
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
	const std::filesystem::path fullPath{ ResourceManager::Instance()->GetAudioRootPath() / request.GetPath()};

	if (request.GetAction() == Action::Play)	
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
		// either play or stop music
		if (request.GetAction() == Action::Play)
		{
			StartPlayingMusic(fullPath);
		}
		else if (request.GetAction() == Action::Stop)
		{
			StopPlayingMusic();
		}
		break;
	case Type::Effect:
		// only play effect
		StartPlayingEffect(fullPath);		
		break;
	case Type::All:
		// only stop all
		StopAllEffectsAndMusic();	
		break;
	}
}

void AudioManager::Impl::StartPlayingMusic(const std::filesystem::path& path)
{
	Mix_FreeMusic(m_Music);
	m_Music = nullptr;
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

void AudioManager::Impl::StartPlayingEffect(const std::filesystem::path& path)		
{
	const auto it{ std::ranges::find_if(m_EffectChannels, [](Mix_Chunk* chunck) -> bool { return chunck == nullptr; }) };

	if (it == m_EffectChannels.end())	
	{
		//throw std::runtime_error("AudioManager::Impl::StartPlayingEffect() - Failed find open sound channel.");
		return;
	}

	*it = Mix_LoadWAV(path.generic_string().c_str());

	if (*it == nullptr)			
	{
		throw std::runtime_error(std::string{ "AudioManager::Impl::StartPlayingEffect() - " } + Mix_GetError());	
	}

	if (Mix_PlayChannel(GetChunckIndex(*it), *it, 0) == -1)	
	{
		throw std::runtime_error(std::string{ "AudioManager::Impl::StartPlayingEffect() - " } + Mix_GetError());	
	}
}

void AudioManager::Impl::StopPlayingMusic()
{
	Mix_FreeMusic(m_Music);
	m_Music = nullptr;
}

void AudioManager::Impl::CheckEffectChannels()
{
	std::ranges::for_each	
	(
		m_EffectChannels, [this](Mix_Chunk*& chunck) -> void		
		{
			if (chunck != nullptr)	
			{
				if (Mix_Playing(GetChunckIndex(chunck)) == 0)
				{
					Mix_FreeChunk(chunck);
					chunck = nullptr;	
				}
			}
		}
	);
}

void AudioManager::Impl::StopAllEffectsAndMusic()
{
	StopPlayingMusic();

	std::ranges::for_each	
	(
		m_EffectChannels, [](Mix_Chunk*& chunck) -> void	
		{
			Mix_FreeChunk(chunck);
			chunck = nullptr;	
		}
	);
}

int AudioManager::Impl::GetChunckIndex(Mix_Chunk* chunck) const
{
	auto it{ std::ranges::find(m_EffectChannels, chunck) };

	if (it != m_EffectChannels.end())
	{
		return static_cast<int>(std::distance(m_EffectChannels.begin(), it));
	}
	else return -1;
}

AudioManager::AudioManager() :	
	Singleton{},	
	m_Pimpl{ std::make_unique<AudioManager::Impl>() }	
{

}

AudioManager::~AudioManager() = default;

void AudioManager::Update()
{
	m_Pimpl->Update();	
}

void AudioManager::PlayMusic(const std::filesystem::path& path)
{
	m_Pimpl->PlayMusic(path);
}

void AudioManager::PlayEffect(const std::filesystem::path& path)
{
	m_Pimpl->PlayEffect(path);
}

void AudioManager::StopMusic()
{
	m_Pimpl->StopMusic();
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