#pragma once

#include <string>

#include "GameObject.h"
#include "ObjectController.h"

namespace Minigin
{
	class Scene final : public ControllableObject, public ObjectController<GameObject>
	{
	public:
		explicit Scene(const std::string& name);
		~Scene() = default;

		Scene(const Scene& other) = delete;
		Scene(Scene&& other) noexcept = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) noexcept = delete;

		GameObject* CreateGameObject(const std::string& name, bool enabled = true);
		GameObject* GetGameObject(const std::string& name) const;
		const std::string& GetName() const;

	private:
		const std::string m_Name;

	};
}