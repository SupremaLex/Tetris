#pragma once

#include "TetrisModel.h"
#include "TetrisController.h"
#include "TetrisView.h"
#include "Config.h"

class TetrisApp : public TetrisController, public TetrisView
{
public:
	TetrisApp(TetrisModel* _model) : TetrisController(_model), TetrisView(_model), BaseApp(CON_X_SIZE, CON_Y_SIZE) {}

	void KeyPressed(int btnCode)
	{
		TetrisController::KeyPressed(btnCode);
	}
	void UpdateF(float delta_time)
	{
		TetrisController::UpdateF(delta_time);
	}
	TetrisModel* model;
};