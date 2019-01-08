#pragma once

#include "SimpleMath.h"

class Camera
{
public:
	Camera(DirectX::SimpleMath::Rectangle screenBounds);

	void Update(float dt);
	
	DirectX::SimpleMath::Matrix GetViewMatrix(DirectX::SimpleMath::Vector2 parallax = DirectX::SimpleMath::Vector2::One);
	DirectX::SimpleMath::Matrix GetScrollMatrix(DirectX::SimpleMath::Vector2 textureSize = DirectX::SimpleMath::Vector2::One);
	
	void LookAt(DirectX::SimpleMath::Vector2 targetPosition);
	void Move(DirectX::SimpleMath::Vector2 displacement, bool respectRotation = false);
	void SetZoom(float zoom);
	void Zoom(float delta);

	void SetLimits(DirectX::SimpleMath::Rectangle limits);

	void Trauma(float traumaAmount);
private:
	DirectX::SimpleMath::Rectangle m_screenBounds;

	DirectX::SimpleMath::Vector2 m_targetPosition;
	DirectX::SimpleMath::Vector2 m_position;
	DirectX::SimpleMath::Rectangle m_limits;
	DirectX::SimpleMath::Vector2 m_origin;
	float m_zoom;
	float m_rotation;
	float m_moveSpeed;
	
	float m_trauma;
	DirectX::SimpleMath::Vector2 m_offsetPosition;
	float m_offsetRotation;

	bool m_limitSet;
		
	void SetPosition(DirectX::SimpleMath::Vector2 position);
	void ValidatePosition();
	void ValidateZoom();
	static const float MinZoom;
};
