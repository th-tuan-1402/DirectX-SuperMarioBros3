#pragma once
#define QB_STATE_SEALED		"SEALED"
#define QB_STATE_EMPTY		"EMPTY"

const int BOUNCE_TIME = 100;
const int BOUNCE_DELTA = 16; // pixels
const float BOUNCE_VEL = (float)BOUNCE_DELTA / (float)BOUNCE_TIME * 6.0f;