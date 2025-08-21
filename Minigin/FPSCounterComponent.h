#pragma once

#include <chrono>
#include <memory>

#include "Component.h"

namespace Minigin
{
	class Text;

	class FPSCounterComponent final : public Component
	{
	public:
		explicit FPSCounterComponent(GameObject* owner);
		~FPSCounterComponent();

		FPSCounterComponent(const FPSCounterComponent&) = delete;
		FPSCounterComponent(FPSCounterComponent&&) noexcept = delete;
		FPSCounterComponent& operator= (const FPSCounterComponent&) = delete;
		FPSCounterComponent& operator= (const FPSCounterComponent&&) noexcept = delete;

		virtual void Update() override;
		virtual void Render() const override;

	private:
		std::chrono::steady_clock::time_point m_LastTimePoint;
		unsigned int m_FrameCounter;
		std::unique_ptr<Text> m_Text;
		bool m_Running;

	};
}