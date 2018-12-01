#pragma once

#include "SimpleMath.h"

class Camera
{
public:
	Camera(DirectX::SimpleMath::Rectangle screenBounds);
	
	DirectX::SimpleMath::Matrix GetViewMatrix(DirectX::SimpleMath::Vector2 parallax = DirectX::SimpleMath::Vector2(1, 1));
	
	void LookAt(DirectX::SimpleMath::Vector2 position);
	void Move(DirectX::SimpleMath::Vector2 displacement, bool respectRotation = false);
	void SetZoom(float zoom);
	void Zoom(float delta);

	void SetLimits(DirectX::SimpleMath::Rectangle limits);
private:
	DirectX::SimpleMath::Rectangle m_screenBounds;

	DirectX::SimpleMath::Vector2 m_position;
	DirectX::SimpleMath::Rectangle m_limits;
	DirectX::SimpleMath::Vector2 m_origin;
	float m_zoom;
	float m_rotation;

	bool m_limitSet;
	bool m_viewMatrixDirty;
	DirectX::SimpleMath::Matrix m_cachedViewMatrix;
	
	void SetPosition(DirectX::SimpleMath::Vector2 position);
	void ValidatePosition();
	void ValidateZoom();
	static const float MinZoom;
};
