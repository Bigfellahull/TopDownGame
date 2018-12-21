#pragma once

#include "Entity.h"

class PlayerStatus
{
public:
	PlayerStatus();

	void Reset(Entity entityId);
	void Update(float dt);
	void AddPoints(int points);
	void IncreaseMultiplier();
	void ResetMultiplier();
	void RemoveLife();
	bool IsGameOver();
	bool IsAlive();
	void SetAlive(bool alive);

	int GetScore();
	int GetMultiplier();
	int GetLives();
	Entity GetCurrentEntityId();
	void SetCurrentEntityId(Entity id);
private:
	Entity m_currentEntityId;
	bool m_alive;
	int m_lives;
	int m_score;
	int m_multiplier;
	float m_maxMultiplier;
	float m_multiplerExpiryTime;
	float m_multiplierTimeLeft;
	int m_scoreForExtraLife;
};