#pragma once

#include "BaseApp.h"
#include "TetrisModel.h"

class TetrisView : public Observer, public virtual BaseApp
{
public:
	TetrisView(TetrisModel* _model);
	virtual void update();
private:
	void draw_borders();
	void draw_field();
	void draw_next_figure();
	void wipe_next_figure_field();
	void draw_score();
	void draw_speed();
	void draw_game_over();
	TetrisModel* model;
};