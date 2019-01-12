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
	m_moveSpeed(3.5f),
	m_limitSet(false)
{
}

void Camera::Reset()
{
	m_trauma = 0.0f;
}

void Camera::Update(float dt)
{
	Vector2 targetPosition = m_targetPosition - m_origin;

	Vector2 delta = m_position + ((targetPosition - m_position) * m_moveSpeed * dt);

	if (delta.LengthSquared() > 0.1f)
	{
		SetPosition(delta);
	}

	m_offsetPosition = Vector2::Zero;
	m_offsetRotation = 0.0f;

	if (m_trauma > 0.0f)
	{
		m_trauma -= (dt * 4.0f);
		m_offsetPosition = Vector2(10.0f, 10.0f) * std::pow(m_trauma, 2) * MathHelper::Random(-1.0f, 1.0f);
		m_offsetRotation = 0.02f * std::pow(m_trauma, 2) * MathHelper::Random(-1.0f, 1.0f);
	}
	else
	{
		m_trauma = 0.0f;
	}
}

Matrix Camera::GetViewMatrix(Vector2 parallax)
{
	Vector2 position = m_position + m_offsetPosition;
	float rotation = m_rotation + m_offsetRotation;

	return Matrix::CreateTranslation(Vector3(-position.x * parallax.x, -position.y * parallax.y, 0.0f)) *
		Matrix::CreateTranslation(Vector3(-m_origin.x, -m_origin.y, 0.0f)) *
		Matrix::CreateRotationZ(rotation) *
		Matrix::CreateScale(m_zoom, m_zoom, 1.0f) *
		Matrix::CreateTranslation(Vector3(m_origin.x, m_origin.y, 0.0f));
}

Matrix Camera::GetScrollMatrix(Vector2 textureSize)
{
	return Matrix::CreateTranslation(Vector3(-m_origin.x / textureSize.x, -m_origin.y / textureSize.y, 0.0f)) *
		Matrix::CreateScale(1.0f / m_zoom) *
		Matrix::CreateRotationZ(m_rotation) *
		Matrix::CreateTranslation(Vector3(m_origin.x / textureSize.x, m_origin.y / textureSize.y, 0.0f)) *
		Matrix::CreateTranslation(Vector3(m_position.x / textureSize.x, m_position.y / textureSize.y, 0.0f));
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
	m_targetPosition = position;
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
}

void Camera::Zoom(float delta)
{
	m_zoom = std::max(m_zoom + delta, MinZoom);

	ValidateZoom();
	ValidatePosition();	
}

void Camera::SetPosition(Vector2 position)
{
	m_position = position;

	ValidatePosition();
}

void Camera::Trauma(float traumaAmount)
{
	m_trauma += traumaAmount;
	m_trauma = std::min(m_trauma, 5.0f);
}

void Camera::ValidatePosition()
{
	if (!m_limitSet)
	{
		return;
	}

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

	auto minZoomX = static_cast<float>(m_screenBounds.width / m_limits.width);
	auto minZoomY = static_cast<float>(m_screenBounds.height / m_limits.height);

	m_zoom = std::max(m_zoom, std::max(minZoomX, minZoomY));
}