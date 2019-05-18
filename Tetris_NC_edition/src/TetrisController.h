#pragma once

#include <vector>

#include "base/BaseApp.h"
#include "TetrisModel.h"

enum Direction {LEFT, RIGHT};

class TetrisController: public virtual BaseApp
{
public:
	TetrisController(TetrisModel* _model);
	void KeyPressed(int btn);
	void UpdateF(float delta_time);
	void start();
	void down();
	void accelerate();
	void shift(Direction d);
	void rotate();
	void spawn_figure();
	void delete_lines(const std::vector<int>& inds);
	void add_points(const int& p);
	void game_over_check();
private:
	TetrisModel* model;
};