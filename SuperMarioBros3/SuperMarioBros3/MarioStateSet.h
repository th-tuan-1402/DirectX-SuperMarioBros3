#pragma once

enum class MoveOnGroundStates
{
	Idle, Walk, Run, HighSpeed, Skid, Crouch, Attack, JumpAttack, Hold, Kick
};

enum class JumpOnAirStates
{
	Stand, Jump, HighJump, Fall, LowJump, Fly, Float
};

struct MarioStateSet
{
	MoveOnGroundStates move;
	JumpOnAirStates jump;
};

enum class MarioStates
{
	SmallMario, SuperMario, FireMario, RacoonMario
};