#pragma once

namespace Minigin
{
	template <typename Type>
	class Singleton
	{
	public:
		static Type* Instance();	
		static void Destroy();	

	protected:
		virtual ~Singleton() = default;
		explicit Singleton() = default;

		Singleton(const Singleton& other) = delete;	
		Singleton(Singleton&& other)  noexcept = delete;	
		Singleton& operator=(const Singleton& other) = delete;	
		Singleton& operator=(Singleton&& other) noexcept = delete;	

	private:
		static Type* m_Instance;

	};

	template<typename Type>
	Type* Singleton<Type>::m_Instance{ nullptr };		

	template<typename Type>
	Type* Singleton<Type>::Instance()	
	{
		if (m_Instance == nullptr) m_Instance = new Type{};
		return m_Instance;
	}

	template<typename Type>
	void Singleton<Type>::Destroy()
	{
		delete m_Instance;
		m_Instance = nullptr;
	}
}