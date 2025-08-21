#pragma once

namespace Minigin
{
	class ControllableObject
	{
	public:
		enum class Status
		{
			Enabled,
			Disabled,
			Destroyed
		};

		void SetStatus(ControllableObject::Status status);
		ControllableObject::Status GetStatus() const;

	protected:
		explicit ControllableObject() = default;
		~ControllableObject() = default;

		ControllableObject(const ControllableObject&) = delete;
		ControllableObject(ControllableObject&&) noexcept = delete;
		ControllableObject& operator= (const ControllableObject&) = delete;
		ControllableObject& operator= (const ControllableObject&&) noexcept = delete;

	private:
		ControllableObject::Status m_Status;

	};
}