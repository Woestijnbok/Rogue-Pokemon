#pragma once

#include <string>

#include "Singleton.h"
#include "Scene.h"
#include "ObjectController.h"	

namespace Minigin
{
	class SceneManager final : public Singleton<SceneManager>, public ObjectController<Scene>
	{
	public:
		friend class Singleton<SceneManager>;

		~SceneManager() = default;

		SceneManager(const SceneManager&) = delete;
		SceneManager(SceneManager&&) noexcept = delete;
		SceneManager& operator= (const SceneManager&) = delete;
		SceneManager& operator= (const SceneManager&&) noexcept = delete;

		Scene* CreateScene(const std::string& name, bool enabled = true);
		Scene* GetScene(const std::string& name) const;
		void RemoveScene(const std::string name);
		void IsolateScene(const std::string& name);

	private:
		explicit SceneManager() = default;

	};
}