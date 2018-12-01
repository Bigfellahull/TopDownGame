#include "stdafx.h"

#include "Camera.h"
#include <algorithm>

using namespace DirectX;
using namespace DirectX::SimpleMath;

const float Camera::MinZoom = 0.1f;

Camera::Camera(Rectangle screenBounds) :
	m_screenBounds(screenBounds),
	m_position(Vector2(0, 0)),
	m_limits(),
	m_origin(Vector2(screenBounds.width / 2.0f, screenBounds.height / 2.0f)),
	m_zoom(1.0f),
	m_rotation(0.0f),
	m_limitSet(false),
	m_viewMatrixDirty(true),
	m_cachedViewMatrix()
{
}

Matrix Camera::GetViewMatrix(Vector2 parallax)
{
	if (m_viewMatrixDirty)
	{
		m_cachedViewMatrix = Matrix::CreateTranslation(Vector3(-m_position.x * parallax.x, -m_position.y * parallax.y, 0.0f)) *
			Matrix::CreateTranslation(Vector3(-m_origin.x, -m_origin.y, 0.0f)) *
			Matrix::CreateRotationZ(m_rotation) *
			Matrix::CreateScale(m_zoom, m_zoom, 1.0f) *
			Matrix::CreateTranslation(Vector3(m_origin.x, m_origin.y, 0.0f));

		m_viewMatrixDirty = false;
	}

	return m_cachedViewMatrix;
}

void Camera::SetLimits(Rectangle limits)
{
	m_limits = Rectangle(
		limits.x, 
		limits.y, 
		std::max(m_screenBounds.width, limits.width), 
		std::max(m_screenBounds.height, limits.height));

	m_limitSet = true;

	ValidateZoom();
	ValidatePosition();
}

void Camera::LookAt(Vector2 position)
{
	SetPosition(position - m_origin);
}

void Camera::Move(Vector2 displacement, bool respectRotation)
{
	if (respectRotation)
	{
		displacement = Vector2::Transform(displacement, Matrix::CreateRotationZ(-m_rotation));
	}

	SetPosition(m_position += displacement);
}

void Camera::SetZoom(float zoom)
{
	m_zoom = std::max(zoom, MinZoom);

	ValidateZoom();
	ValidatePosition();

	m_viewMatrixDirty = true;
}

void Camera::Zoom(float delta)
{
	m_zoom = std::max(m_zoom + delta, MinZoom);

	ValidateZoom();
	ValidatePosition();	

	m_viewMatrixDirty = true;
}

void Camera::SetPosition(Vector2 position)
{
	m_position = position;

	ValidatePosition();

	m_viewMatrixDirty = true;
}

void Camera::ValidatePosition()
{
	if (!m_limitSet)
	{
		return;
	}

	m_viewMatrixDirty = true;

	auto cameraWorldMin = Vector2::Transform(Vector2::Zero, GetViewMatrix().Invert());
	auto cameraSize = Vector2(static_cast<float>(m_screenBounds.width), static_cast<float>(m_screenBounds.height / m_zoom));
	auto limitWorldMin = Vector2(static_cast<float>(m_limits.x), static_cast<float>(m_limits.y));
	auto limitWorldMax = Vector2(static_cast<float>(m_limits.x + m_limits.width), static_cast<float>(m_limits.y + m_limits.height));
	auto positionOffset = m_position - cameraWorldMin;

	cameraWorldMin.Clamp(limitWorldMin, limitWorldMax - cameraSize);

	m_position = cameraWorldMin + positionOffset;
}

void Camera::ValidateZoom()
{
	if (!m_limitSet)
	{
		return;
	}

	m_viewMatrixDirty = true;

	auto minZoomX = static_cast<float>(m_screenBounds.width / m_limits.width);
	auto minZoomY = static_cast<float>(m_screenBounds.height / m_limits.height);

	m_zoom = std::max(m_zoom, std::max(minZoomX, minZoomY));
}