#pragma once

#include "sprites.h"

#include <math.h>

void DrawSprite(Object& obj)
{
    DrawRectangle(obj.Position.x, obj.Position.y, obj.Size.x, obj.Size.y , RED);

    //Rectangle frame = obj.Sheet->Frames[sprite.CurrentFrame];
    //DrawTexturePro(obj.Sheet->SheetTexture, frame, Rectangle{ obj.Position.x,sprite.Position.y,fabsf(frame.width),fabsf(frame.height) }, sprite.Offset, 0, WHITE);
}

void DrawPlayer(Player& obj,Color color)
{
    DrawRectangle(obj.Position.x, obj.Position.y, obj.Size.x, obj.Size.y, color);

    //Rectangle frame = obj.Sheet->Frames[sprite.CurrentFrame];
    //DrawTexturePro(obj.Sheet->SheetTexture, frame, Rectangle{ obj.Position.x,sprite.Position.y,fabsf(frame.width),fabsf(frame.height) }, sprite.Offset, 0, WHITE);
}

