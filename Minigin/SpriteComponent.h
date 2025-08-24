#pragma once

#include <memory>
#include <chrono>
#include <string>
#include <unordered_map>
#include <optional>
#include <vec2.hpp>

#include "Component.h"
#include "Transform.h"

namespace Minigin
{
	class Sprite;

	struct SpriteInformation final
	{
	public:
		explicit SpriteInformation(const std::shared_ptr<Sprite>& sprite, const std::chrono::milliseconds& frameTime, const Transform& renderOffset);
		explicit SpriteInformation(const std::shared_ptr<Sprite>& sprite, const std::chrono::milliseconds& frameTime);
		~SpriteInformation() = default;

		SpriteInformation(const SpriteInformation&) = default;
		SpriteInformation(SpriteInformation&&) noexcept = default;
		SpriteInformation& operator= (const SpriteInformation&) = default;
		SpriteInformation& operator= (SpriteInformation&&) noexcept = default;

		std::shared_ptr<Sprite> Sprite;
		std::chrono::milliseconds FrameTime;
		std::optional<Transform> RenderOffset;
	};

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

		void AddSprite(const std::string& name, const SpriteInformation& spriteInformation);
		void SetSprite(const std::string& name);	
		SpriteInformation* GetSprite(const std::string& name);
		SpriteInformation const* GetSprite(const std::string& name) const;
		SpriteInformation* GetCurrentSprite();
		SpriteInformation const* GetCurrentSprite() const;
		void SetPaused(bool paused);
		void SetLoop(bool loop);
		void Reset();

	private:
		std::unordered_map<std::string, SpriteInformation> m_Sprites;
		SpriteInformation* m_CurrentSprite;
		bool m_Paused;
		bool m_Loop;
		std::chrono::milliseconds m_AccumulatedFrameTime;
		int m_CurrentFrame;
	};
}