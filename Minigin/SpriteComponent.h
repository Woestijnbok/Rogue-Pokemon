#pragma once

#include <memory>
#include <chrono>
#include <string>
#include <unordered_map>
#include <optional>
#include <vec2.hpp>

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
		void SetSprite(const std::string& name, bool loop);	
		std::shared_ptr<Sprite> GetSprite(const std::string& name) const;
		void SetPaused(bool paused);
		void SetRenderOffset(const glm::ivec2& offset);
		void SetRenderScale(const glm::vec2& scale);
		glm::ivec2 GetRenderOffset() const;
		glm::vec2 GetRenderScale() const;
		void ClearRenderScale();
		void ClearRenderOffset();
		void Reset();

	private:
		std::unordered_map<std::string, std::pair<std::shared_ptr<Sprite>, std::chrono::milliseconds>> m_Sprites;
		std::string m_CurrentSpriteName;
		bool m_Paused;
		bool m_Loop;
		std::chrono::milliseconds m_AccumulatedFrameTime;
		int m_CurrentFrame;
		std::optional<glm::ivec2> m_RenderOffset;
		std::optional<glm::vec2> m_RenderScale;
	};
}