#pragma once

#define KOOPA_STATE_MOVE			"MOVE"
#define KOOPA_STATE_WITH_DRAW		"WITH-DRAW"

#define KOOPA_SHELL_STATE_IDLE		"IDLE"
#define KOOPA_SHELL_STATE_RUN		"RUN"

#define KOOPA_SPEED					0.039f 
#define KOOPA_SHELL_SPEED			0.1f 
#define KOOPA_SHELL_DEFLECT			0.4f 

const D3DXVECTOR2 KOOPA_BBOX		(16 * 3, 27 * 3);
const D3DXVECTOR2 KOOPA_SHELL_BBOX	(16 * 3, 16 * 3);

#define KOOPA_GRAVITY				0.00093f