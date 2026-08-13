#pragma once
#include <DX3D/Prerequisites.h>
#include <DX3D/GameObject/Component.h>
#include <DX3D/Math/Vector4D.h>

class LightComponent : public Component{
public:
	LightComponent();
	virtual ~LightComponent();

	void setLightType(LightType type);
	void setLightType(std::string type);
	void setColor(Vector4D color);
	void setRadius(f32 radius);
	void setIntensity(f32 intensity);

	Vector4D getColor() const { return m_color; }
	LightType getLightType() const { return m_light_type; }
	std::string getLightTypeString() const;
	f32 getRadius() const { return m_radius; }
	f32 getIntensity() const { return m_intensity; }
protected:
	virtual void onCreateInternal();
	void onActivate() override;
	void onDeactivate() override;
private:
	Vector4D m_color = Vector4D(1,1,1,1);
	LightType m_light_type = LightType::DirectionLight;
	f32 m_radius = 1.0f;
	f32 m_intensity = 1.0f;
};


