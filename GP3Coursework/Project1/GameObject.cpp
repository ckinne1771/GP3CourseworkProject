#include "GameObject.h"
#include "Component.h"
//Create the other necessary Classes

GameObject::GameObject()
{
	m_Transform = NULL;
	m_Mesh = NULL;
	m_Material = NULL;
	m_Camera = NULL;
	m_Light = NULL;
	m_Name = "GameObject";
	m_Parent = NULL;
}

GameObject::~GameObject()
{

}

void GameObject::initialise()
{
	for (auto iter = m_Components.begin(); iter != m_Components.end(); iter++)
	{
		(*iter)->initialise();
	}
	for (auto iter = m_Children.begin(); iter != m_Children.end(); iter++)
	{
		(*iter)->initialise();
	}
}