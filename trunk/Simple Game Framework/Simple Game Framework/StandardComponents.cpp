#include "StandardComponents.h"

using namespace irr;
using namespace core;
using namespace scene;

const irr::core::matrix4& sgfTransformComponent::getTransform()
{
	onGetTransform.getPtr()->invoke(this->transform);
	return this->transform;
}
void sgfTransformComponent::setTransform(irr::core::matrix4& transform)
{
	onSetTransform(transform);
}

void sgfTransformComponent::setPosition(const irr::core::vector3df& position)
{
	transform.setTranslation(position);
	onSetTransform(transform);
}

const irr::core::vector3df sgfTransformComponent::getPosition()
{
	getTransform();
	return transform.getTranslation();
}

void sgfTransformComponent::setScale(const irr::core::vector3df& scale)
{
	transform.setScale(scale);
	onSetTransform(transform);
}

const irr::core::vector3df sgfTransformComponent::getScale()
{
	getTransform();
	return transform.getScale();
}

void sgfTransformComponent::setRotation(const irr::core::vector3df& rotation)
{
	transform.setRotationDegrees(rotation);
	onSetTransform(transform);
}

const irr::core::vector3df sgfTransformComponent::getRotation()
{
	getTransform();
	return transform.getRotationDegrees();
}

const char* sgfTransformComponent::getName() const
{
	return "TransformComponent";
}


sgfRenderComponent::sgfRenderComponent(ISceneNode* node)
	:node(node)
{
	setTransformDelegate.bind(this,&sgfRenderComponent::onSetTransform);
	setTransformDelegate.addRef();
	node->grab();
}

sgfRenderComponent::~sgfRenderComponent()
{
	node->drop();

}

irr::scene::ISceneNode* sgfRenderComponent::getSceneNode() const
{
	return node;
}

void sgfRenderComponent::onAdd()
{
	sgfTransformComponent* transform=(sgfTransformComponent*)entity->addComponent(new sgfTransformComponent);//it needs transformation to be rendered
	transform->onGetTransform=new sgfMethodDelegate<sgfRenderComponent,matrix4>(this,&sgfRenderComponent::onGetTransform);
	transform->onSetTransform.addDelegate(&setTransformDelegate);
	transform->getTransform();//to update transformation
}

void sgfRenderComponent::onRemove()
{
	node->remove();	
	sgfTransformComponent* transform=(sgfTransformComponent*)entity->getComponent("TransformComponent");
	transform->onGetTransform=NULL;
	transform->onSetTransform.removeDelegate(&setTransformDelegate);
}

bool sgfRenderComponent::isVisible() const
{
	return node->isVisible();
}

void sgfRenderComponent::setVisible(bool visible)
{
	node->setVisible(visible);
}

void sgfRenderComponent::onSetTransform(matrix4& transform)
{
	node->setPosition(transform.getTranslation());
	node->setRotation(transform.getRotationDegrees());
	node->setScale(transform.getScale());
}

void sgfRenderComponent::onGetTransform(matrix4& transform)
{
	transform=node->getAbsoluteTransformation();
}

const char* sgfRenderComponent::getName() const
{
	return "RenderComponent";
}