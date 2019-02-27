#pragma once

#include "Entity.h"
#include "Component.h"
#include <unordered_map>
#include <memory>

class IComponentStore 
{
public:
	virtual bool Remove(Entity entity) = 0;
	virtual const size_t Count() const = 0;
};

template<typename T>
class ComponentStore : public IComponentStore 
{
	static_assert(std::is_base_of<Component, T>::value, "T must derive from component");
	static_assert(T::Type != InvalidComponentType, "T must define a valid type");

public:
	ComponentStore() { }
	
	~ComponentStore() { }

	inline bool Add(const Entity entity, T&& component) 
	{
		return m_store.insert(std::make_pair(entity, std::move(component))).second;
	}

	inline bool Remove(Entity entity) 
	{
		return (0 < m_store.erase(entity));
	}

	inline bool Has(Entity entity) const 
	{
		return (m_store.end() != m_store.find(entity));
	}

	inline T& Get(Entity entity) 
	{
		return m_store.at(entity);
	}

	inline T Extract(Entity entity) 
	{
		T component = std::move(m_store.at(entity));
		m_store.erase(entity);
		return component;
	}

	inline const std::unordered_map<Entity, T>& GetComponents() 
	{
		return m_store;
	}

	inline const size_t Count() const { return m_store.size(); }

private:
	std::unordered_map<Entity, T> m_store;             
	static const ComponentType s_type = T::Type;
};