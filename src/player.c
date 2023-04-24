#include <genesis.h>
#include <resources.h>

Sprite* player;
Sprite* thrust;
bool isPlayerHFlipped;
bool isPlayerTurning;
bool isPlayerFinishingTurn;
s32 player_vx;
s32 player_vy;
s32 player_ix;
s32 player_iy;
s16 player_x;
s16 player_y;

void PLAYER_animCallback(Sprite* sprite)
{
    if (sprite->frameInd == 5)    
    {
        isPlayerFinishingTurn = TRUE;
    }
    else if (isPlayerFinishingTurn == TRUE)
    {
        isPlayerFinishingTurn = FALSE;
        isPlayerTurning = FALSE;
        isPlayerHFlipped = !isPlayerHFlipped;
        SPR_setHFlip(sprite, isPlayerHFlipped);
        sprite->timer = 0;
    }
    else if (sprite->frameInd == 0) //prevents sprite flipping at system start
    {
        sprite->timer = 0;
    }

    return;
}

void PLAYER_init()
{
    player = SPR_addSprite(&player_spr, 150, 100, TILE_ATTR(PAL1, TRUE, FALSE, FALSE));
    thrust = SPR_addSprite(&thrust_spr, 160, 96, TILE_ATTR(PAL1, TRUE, FALSE, FALSE));

    PAL_setPalette(PAL1, player_spr.palette->data, CPU);

    isPlayerHFlipped = FALSE;
    isPlayerTurning = FALSE;
    player_vx = 0;
    player_vy = 0;
    player_x = 150;
    player_y = 100;
    player_ix = player_x << 3;
    player_iy = player_y << 3;

    SPR_setFrameChangeCallback(player, &PLAYER_animCallback);
}

void PLAYER_update(u16 camera_offset)
{
    u16 j_value = JOY_readJoypad(JOY_1);

    if (player->timer == 0)
    {
        if (j_value & BUTTON_LEFT)
        {
            if (!isPlayerHFlipped) 
            {
                player->timer = 5; 
                isPlayerTurning = TRUE;
            }
            else { player_vx -= 2; }
        }
        else if (j_value & BUTTON_RIGHT)
        {
            if (isPlayerHFlipped)
            {
                player->timer = 5; 
                isPlayerTurning = TRUE;
            }
            else { player_vx += 2; }
        }
    }

    if (player_vx != 0) 
    {
        player_vx = player_vx > 0 ? player_vx - 1 : player_vx + 1;
    }

    if (abs(player_vx) > 80)
    {
        player_vx = player_vx > 0 ? 80 : -80;
    }

    if (j_value & BUTTON_UP)
    {
        player_vy = -14;
    }
    else if (j_value & BUTTON_DOWN)
    {
        player_vy = 14;
    }
    else
    {
        player_vy = 0;
    }

    player_ix += player_vx;
    player_iy += player_vy;

    if (player_ix > 2240 << 3)
    {
        player_ix -= 2240 << 3;
    }
    else if (player_ix < 0)
    {
        player_ix += 2240 << 3;
    }

    player_x = player_ix >> 3;
    player_y = player_iy >> 3;

    SPR_setPosition(player, 304 - camera_offset, player_y);

        //update engine thrust sprite

    if (player->frameInd != 0 || player_vx == 0)
    {
        SPR_setVisibility(thrust, HIDDEN);
    }
    else if (player->visibility != HIDDEN)
    {
        SPR_setVisibility(thrust, VISIBLE);
    }

    switch (isPlayerHFlipped)
    {
        case TRUE:
            SPR_setPosition(thrust, SPR_getPositionX(player) + 32, SPR_getPositionY(player) + 6);
            SPR_setHFlip(thrust, TRUE);
            break;
        case FALSE:
            SPR_setPosition(thrust, SPR_getPositionX(player) - 16, SPR_getPositionY(player) + 6);
            SPR_setHFlip(thrust, FALSE);
            break;
    }

    if (abs(player_vx) > 25)
    {
        SPR_setAnim(thrust, 1);
    }
    else
    {
        SPR_setAnim(thrust, 0);
    }
}