#pragma once

#include "Entity.h"
#include "Component.h"
#include "ComponentStore.h"
#include "System.h"
#include <map>

class EntityManager 
{
public:
	EntityManager();
	
	virtual ~EntityManager();

	template<typename T>
	inline bool CreateComponentStore() 
	{
		static_assert(std::is_base_of<Component, T>::value, "T must derived from Component");
		static_assert(T::Type != InvalidComponentType, "T must define a valid type");

		return m_componentStores.insert(std::make_pair(T::Type, IComponentStore::Ptr(new ComponentStore<T>()))).second;
	}

	template<typename T>
	inline ComponentStore<T>& GetComponentStore() 
	{
		static_assert(std::is_base_of<Component, T>::value, "T must derived from Component");
		static_assert(T::Type != InvalidComponentType, "T must define a valid type");

		auto componentStore = m_componentStores.find(T::Type);

		if (m_componentStores.end() == componentStore) 
		{
			throw std::runtime_error("The ComponentStore does not exist");
		}

		return reinterpret_cast<ComponentStore<T>&>(*(componentStore->second));
	}

	void AddSystem(const System::Ptr& systemPtr);

	inline Entity CreateEntity() 
	{
		assert(m_lastEntity < std::numeric_limits<Entity>::max());
		m_entities.insert(std::make_pair((m_lastEntity + 1), ComponentTypeSet()));
		return (++m_lastEntity);
	}

	template<typename T>
	inline bool AddComponent(const Entity entity, T&& component) 
	{
		static_assert(std::is_base_of<Component, T>::value, "T must derived from Component");
		static_assert(T::Type != InvalidComponentType, "T must define a valid type");
		
		auto foundEntity = m_entities.find(entity);
		if (m_entities.end() == foundEntity) 
		{
			throw std::runtime_error("The Entity does not exist");
		}
		
		(*foundEntity).second.insert(T::Type);
		
		return GetComponentStore<T>().Add(entity, std::move(component));
	}

	size_t RegisterEntity(const Entity entity);
	size_t UnregisterEntity(const Entity entity);
	size_t UpdateEntities(DX::StepTimer const& timer);
	size_t RenderEntities();

private:
	Entity m_lastEntity;
	std::unordered_map<Entity, ComponentTypeSet> m_entities;
	std::map<ComponentType, IComponentStore::Ptr> m_componentStores;
	std::vector<System::Ptr> m_systems;
};