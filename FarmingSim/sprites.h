#pragma once

#include "raylib.h"
#include <vector>
#include <string>

typedef enum OBJECT_TYPES {WALL = 0,};

enum class PlayerState
{
    Idle,
    Run,
    JumpStart,
    JumpUp,
    FallDown,
};
struct Object
{
    Vector2 Position = { 0 };
    Vector2 Size = { 16, 16 };
    OBJECT_TYPES type;
};
struct Player
{
    Vector2 Position = { 0 };
    Vector2 Size = { 64, 64 };
    OBJECT_TYPES type = OBJECT_TYPES::WALL;
    PlayerState State = PlayerState::Idle;

    float RunSpeed = 200;
    float jumpAcceleration = -350;
    float jumpVelocityDampen = 1.125f;
    Vector2 Velocity = { 0,0 };
};

void DrawSprite(Object& obj);
void DrawPlayer(Player& obj,Color color);
