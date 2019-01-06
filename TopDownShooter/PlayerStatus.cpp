#include "stdafx.h"

#include "PlayerStatus.h"

PlayerStatus::PlayerStatus() :
	m_alive(false),
	m_lives(3),
	m_score(0),
	m_multiplier(1),
	m_maxMultiplier(20),
	m_multiplierTimeLeft(0),
	m_multiplerExpiryTime(1.5f),
	m_scoreForExtraLife(2000)
{
}

void PlayerStatus::Reset(Entity entityId)
{
	m_currentEntityId = entityId;
	m_score = 0;
	m_multiplier = 1;
	m_lives = 3;
	m_scoreForExtraLife = 2000;
	m_multiplierTimeLeft = 0;
	m_alive = true;
}

void PlayerStatus::Update(float dt)
{
	if (m_multiplier > 1)
	{
		if ((m_multiplierTimeLeft -= dt) <= 0)
		{
			m_multiplierTimeLeft = m_multiplerExpiryTime;
			ResetMultiplier();
		}
	}
}

void PlayerStatus::AddPoints(int basePoints)
{
	if (IsGameOver())
	{
		return;
	}

	m_score += basePoints * m_multiplier;

	while (m_score >= m_scoreForExtraLife)
	{
		m_scoreForExtraLife += 2000;
		m_lives++;
	}
}

void PlayerStatus::IncreaseMultiplier()
{
	if (IsGameOver())
	{
		return;
	}

	m_multiplierTimeLeft = m_multiplerExpiryTime;

	if (m_multiplier < m_maxMultiplier)
	{
		m_multiplier++;
	}
}

void PlayerStatus::ResetMultiplier()
{
	m_multiplier = 1;
}

void PlayerStatus::RemoveLife()
{
	m_lives--;
}

bool PlayerStatus::IsGameOver()
{
	return m_lives == 0;
}

bool PlayerStatus::IsAlive()
{
	return m_alive;
}

void PlayerStatus::SetAlive(bool alive)
{
	m_alive = alive;
}

int PlayerStatus::GetScore()
{
	return m_score;
}

int PlayerStatus::GetMultiplier()
{
	return m_multiplier;
}

int PlayerStatus::GetLives()
{
	return m_lives;
}

Entity PlayerStatus::GetCurrentEntityId()
{
	return m_currentEntityId;
}

void PlayerStatus::SetCurrentEntityId(Entity id)
{
	m_currentEntityId = id;
	m_alive = true;
}