#pragma once

constexpr bool SlowModeEnabled = false;

#if _DEBUG
constexpr bool FullScreen = false;
constexpr bool DisableParticles = false;
#else
constexpr bool DisableParticles = false;
constexpr bool FullScreen = false;
#endif

#if _DEBUG
constexpr bool ShowDebugVisuals = false;
constexpr bool ShowDebugText = false;
constexpr bool ShowDebugQuadTree = false;
constexpr bool ShowDebugPlayerContainer = false;
#endif

constexpr const wchar_t* DebugAsset = L"1x1.dds";
constexpr const wchar_t* PlayerAsset = L"Player.dds";
constexpr const wchar_t* BulletAsset = L"Bullet.dds";
constexpr const wchar_t* SeekerEnemyAsset = L"Seeker.dds";
constexpr const wchar_t* SeekerAEnemyAsset = L"SeekerA.dds";
constexpr const wchar_t* WanderEnemyAsset = L"Wanderer.dds";
constexpr const wchar_t* BlackHoleAsset = L"BlackHole.dds";

constexpr const wchar_t* ParticleAsset = L"Particle.dds";
constexpr const wchar_t* ParticleGlowAsset = L"GlowParticle.dds";

constexpr const wchar_t* BackgroundLayer1 = L"BackgroundLayer1.dds";
constexpr const wchar_t* BackgroundLayer2 = L"BackgroundLayer2.dds";
constexpr const wchar_t* BackgroundLayer3 = L"BackgroundLayer3.dds";
constexpr const wchar_t* BackgroundLayer4 = L"BackgroundLayer4.dds";