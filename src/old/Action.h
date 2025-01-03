#pragma once
class Action
{
public:
	enum class ActionType {
		AT_NONE,
		
		// Scene Control
		AT_PAN,
		AT_ZOOM,

		// Grid
		AT_MOVE_GRID_CORNER,

		// Paint Tool
		AT_PAINT_ADD_POINT,
		AT_PAINT_ERASE_POINT,
		AT_PAINT_CLEAR_POINTS,

		// Bezier Tool
		AT_BEZIER_MOVE_HANDLE,
		AT_BEZIER_MOVE_POINT,

		AT_BEZIER_ADD_POINT,
		AT_BEZIER_REMOVE_POINT,
		AT_BEZIER_NEW_SPLINE,

	};

	enum ActionCategory {
		None = 0,
		SCENE_CONTROL_ACTIONS = (1 << 0),
		GRID_ACTIONS = (1 << 0),
		PAINT_ACTIONS = (1 << 1),
		BEZIER_ACTIONS = (1 << 2),
	};


};

