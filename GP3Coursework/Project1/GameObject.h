//Base class for game objects

#ifndef GameObject.h
#define GameObject_h

#include <string>
#include <vector>

class Transform;
class Component;
class Mesh;
class SecondaryMaterial;
class PrimaryMaterial;
class Camera;
class Lighting;

class GameObject
{
public:
	GameObject();
	 ~GameObject();

	 void initialise();
	 void update();
	 void render();
	 void destroy();

	 void addComponent(Component * component);
	 void setMesh(Mesh * mesh);
	 void setPrimaryMaterial(PrimaryMaterial * primaryMaterial);
	 void setCamera(Camera * camera);
	 void setLighting(Lighting * light);

	 void addChild(GameObject * childObject);
	 void setParent(GameObject * parentObject);

	 GameObject * getParent();
	 Transform * getTransform();
	 Mesh * getMesh();
	 PrimaryMaterial * getMaterial();
	 Camera * getCamera();
	 Lighting * getLighting();

	 int getChildCount();
	 GameObject * getChild(int index);

	 Component * getComponentByType(const std::string& type);
	 Component * getComponentByType(const std::string& name);

private:
	std::vector<Component*> m_Components;
	std::vector<GameObject*> m_Children;
	std::string m_Name;

	Transform *m_Transform;
	Mesh *m_Mesh;
	PrimaryMaterial *m_Material;
	Camera *m_Camera;
	Lighting *m_Light;
	GameObject *m_Parent;


}
;

#endif