#include "LightComponent.h"
#include "EventDefinitions.h"
#include "EventBus.h"

LightComponent::LightComponent()
{
	componentName = "LightComponent";
}

void LightComponent::Awake()
{	

}

void LightComponent::Serialize(nlohmann::json& j)
{
	j = nlohmann::json{
	{"ComponentName","LightComponent"}
	};
}

std::unique_ptr<Component> LightComponent::Clone() const
{
	std::unique_ptr<LightComponent> Temp = std::make_unique<LightComponent>(*this);
	return std::move(Temp);
}
