#include <genesis.h>
#include <resources.h>
#include <player.h>

u16 tileset_index[8];
char debug_display[40];
Map* mtnbg;

u32 camera_x;
u16 camera_offset;
u16 camera_lookahead;

int main()
{
    SYS_disableInts();

        VDP_setScreenWidth320();
        VDP_setScreenHeight224();
        VDP_setPlaneSize(64, 32, TRUE);

        PAL_setPalette(PAL2, mtnbg_pal.data, CPU);

        tileset_index[0] = TILE_USER_INDEX;
        VDP_loadTileSet(&mtnbg_tileset, TILE_USER_INDEX, DMA);
        tileset_index[1] = tileset_index[0] += mtnbg_tileset.numTile;

        mtnbg = MAP_create(&mtnbg_map, BG_B, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, TILE_USER_INDEX));

        SPR_init();
        PLAYER_init();

        camera_x = 0;
        camera_lookahead = 64;
        camera_offset = 148;

    SYS_enableInts();

    while(TRUE)
    {
        PLAYER_update(camera_offset);

        SPR_update();

        if ((!isPlayerHFlipped && isPlayerTurning) || (isPlayerHFlipped && !isPlayerTurning))
        {
            camera_offset = camera_offset <= 148 - camera_lookahead ? 148 - camera_lookahead : camera_offset - 3;
        }
        else if ((isPlayerHFlipped && isPlayerTurning) || (!isPlayerHFlipped && !isPlayerTurning))
        {
            camera_offset = camera_offset >= 148 + camera_lookahead ? 148 + camera_lookahead : camera_offset + 3;
        }

        camera_x = player_x + camera_offset;
        
        if (camera_x > 2240)
        {
            camera_x -= 2240;
        }

        MAP_scrollTo(mtnbg, camera_x, 0);

        //sprintf(debug_display,"X: %i Y: %i VX: %i VY: %i  ", player_x, player_y, player_vx, player_vy);
        //VDP_drawText(debug_display,1,0);

        SYS_doVBlankProcess();
    }
    return (0);
}
