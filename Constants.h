#pragma once
 
#define MEANINGLESS_BOUNDARY 1e-5
#define KEYBOARD_BUFFER_SIZE 1024
#define MARIO_WALKING_SPEED		0.05f
#define MARIO_WALKING_SPEED_AT_JUMP 0.025f
#define MARIO_FLYING_SPEED_X    0.01
#define MARIO_JUMP_SPEED_Y		0.05f
#define MARIO_GRAVITY			0.001f
#define MARIO_SMALL_FRICTION    0.005f
#define MARIO_SMALL_FRICTION_AT_JUMP    0.005f
#define MARIO_BIG_FRICTION      0.01f
#define MARIO_Y_SPEED_BOUNDARY  0.015f
#define MARIO_JUMP_GAP_MAX      3
#define MARIO_JUMP_COUNT_MAX    10


#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200
#define MARIO_STATE_JUMP			300

#define MARIO_ANI_IDLE_RIGHT		0
#define MARIO_ANI_IDLE_LEFT			1
#define MARIO_ANI_WALKING_RIGHT		2
#define MARIO_ANI_WALKING_LEFT		3

#define MARIO_START_X 10.0f
#define MARIO_START_Y 130.0f
#define MARIO_START_VX 0.1f
/// Texture's constants
/// for defining textures' id 
#define TEX_ID_MARIO 0
#define TEX_ID_ENEMIES 1

#define TEX_MARIO_WIDTH 1044
#define TEX_MARIO_HEIGHT 1150
#define TEX_MARIO_RED 68
#define TEX_MARIO_GREEN 145
#define TEX_MARIO_BLUE 190

#define TEX_RIGHT_CENTER_MARIO 522
#define TEX_LEFT_CENTER_MARIO 521
#define TEX_RIGHT_CENTER_ENEMIES 688
#define TEX_LEFT_CENTER_ENEMIES 687
/// Sprites' constants
/// for defining sprites' id based on the formular F = DOOTSS
///     with    D   :   direction, which is 0 meaning left and 1 meaning right
///             OO  :   type of objects (mario, flower, turtle, etc)
///             T   :   state of the object
///             SS  :   order number of sprite
#define ANI_DUMP -1
#define SPR_OO_DUMP -1
#define SPR_OO_MARIO 0
#define SPR_OO_DOG 1
#define SPR_OO_ANGRY_DOG 2
#define SPR_OO_WING 3
#define SPR_OO_RED_FLOWER 4
#define SPR_OO_FIRE_BULLET 5
#define SPR_OO_GREEN_FLOWER 6
#define SPR_OO_BLACK_FLOWER 7
#define SPR_OO_RED_TURLE 8
#define SPR_OO_GREEN_GREEN 9

#define STR_SEPARATOR " "


#define X_GAP 30
#define UPPER_CENTER 7
#define LOWER_CENTER 8
#define Y_GAP 40
#define COLUMN_COUNT 7

#define STATE_MARIO_WALK 0
#define STATE_MARIO_RUN 1
#define STATE_MARIO_JUMP 2
#define STATE_MARIO_STOP 3
#define STATE_MARIO_KICK 4
#define STATE_MARIO_STAND 5
#define STATE_MARIO_HOLD 6
#define STATE_MARIO_HOLD_RUN 7
#define STATE_MARIO_DIE 8
#define STATE_MARIO_SLIDE 9
#define STATE_MARIO_QUICK_RUN 10
#define STATE_MARIO_QUICK_JUMP 11
#define STATE_MARIO_ATTACK 12
#define STATE_MARIO_FLOAT 13
#define STATE_MARIO_START_FLOATING 14
#define STATE_MARIO_FLY 15
#define STATE_MARIO_LANDING 16
#define STATE_MARIO_TRANSFORM_TO_BIG 17
#define STATE_MARIO_TRANSFORM_TO_SMALL 18
#define STATE_MARIO_TRANSFORM_TO_RACOON 19
#define STATE_MARIO_TRANSFORM_TO_FIRE 20

#define FORM_MARIO_SMALL 0
#define FORM_MARIO_BIG 1
#define FORM_MARIO_FIRE 2
#define FORM_MARIO_RACOON 3

#define MARIO_TO_RIGHT 1
#define MARIO_TO_LEFT 0



