Sprite* player;
Sprite* thrust;
bool isPlayerHFlipped;
bool isPlayerTurning;
s32 player_vx;
s32 player_vy;
s32 player_ix;
s32 player_iy;
s16 player_x;
s16 player_y;

void PLAYER_animCallback(Sprite* sprite);

void PLAYER_init();

void PLAYER_update();