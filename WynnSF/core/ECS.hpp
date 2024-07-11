#pragma once

#include <iostream>
#include <memory>
#include "Manager/EntityManager.hpp"
#define MAX_COMPONENTS_SIZE 10


class Entity;

using ComponentID = std::size_t;
using EntityID = std::uint64_t;


class BaseComponent {

protected:
	ComponentID m_id = 0;

public:
	BaseComponent() {};

	virtual ~BaseComponent() {};
};


template <class T> class Component : public BaseComponent {

public:
	Component() : BaseComponent() { this->m_id = typeid(T).hash_code() % MAX_COMPONENTS_SIZE; };
	Entity* owner = nullptr;
	inline std::size_t GetID() const { return this->m_id; };
	~Component() {};
};


class Entity {

	friend class EntityManager;

private:
	EntityID m_eId;
	

	std::shared_ptr<BaseComponent> m_Components[MAX_COMPONENTS_SIZE];

	std::string m_eTag = "Default";

	EntityID genUEID();

	Entity();
	Entity(const std::string& tag);

	bool m_active = true;

public:



	template<class T, typename ...TArgs>
	std::shared_ptr<T> AddComponent(TArgs&&... args) {
		std::shared_ptr<T> newComponent = std::make_shared<T>(std::forward<TArgs>(args)...);
		newComponent->owner = this;
		m_Components[newComponent->GetID()] = newComponent;
		return newComponent;
	};

	template<class T>
	bool HasComponent() const {
		T component = {};
	
		if (m_Components[component.GetID()] != nullptr) {
			return true;
		}

		return false;

	};

	inline std::string GetTag() const {
		return m_eTag;
	}

	inline EntityID GetID() const { return this->m_eId; };

	template<class T>
	std::shared_ptr<T> GetComponent() {
		T component = {};

		if (m_Components[component.GetID()] != nullptr) {
			return std::static_pointer_cast<T>(m_Components[component.GetID()]);
		}

		std::cout << "Component : " << typeid(T).name() << " does not exist" << std::endl;
		return nullptr;
		

	}

	bool operator==(const Entity& other) const {
		return (this->GetID() == other.GetID() && this->GetTag() == other.GetTag());
	}

	inline void DestroyEntity() {
		
		
		this->m_active = false;
	
	}

	inline bool IsActive() const {
		return this->m_active;
	}

};
