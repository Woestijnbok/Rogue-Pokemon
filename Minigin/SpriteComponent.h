#pragma once

#include <memory>
#include <chrono>
#include <string>
#include <unordered_map>

#include "Component.h"

namespace Minigin
{
	class Sprite;

	class SpriteComponent final : public Component
	{
	public:
		explicit SpriteComponent(GameObject* owner);	
		~SpriteComponent();

		SpriteComponent(const SpriteComponent&) = delete;
		SpriteComponent(SpriteComponent&&) noexcept = delete;
		SpriteComponent& operator= (const SpriteComponent&) = delete;
		SpriteComponent& operator= (const SpriteComponent&&) noexcept = delete;

		virtual void Update() override;
		virtual void Render() const override;

		void AddSprite(const std::shared_ptr<Sprite>& sprite, const std::chrono::milliseconds frameTime, const std::string& name);
		void SetSprite(const std::string& name);	
		std::shared_ptr<Sprite> GetSprite(const std::string& name) const;	
		void Reset();

	private:
		std::unordered_map<std::string, std::pair<std::shared_ptr<Sprite>, std::chrono::milliseconds>> m_Sprites;
		std::string m_CurrentSpriteName;	
		std::chrono::steady_clock::time_point m_LastTimePoint;
		int m_Frame;
		bool m_UpdateTimePoint;
	};
}