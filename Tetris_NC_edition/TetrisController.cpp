#include "TetrisController.h"

#include "conio.h"
#include <algorithm>

#include "Config.h"

using namespace std;

bool compare_coords(const pair<int, int>& p1, const pair<int, int>& p2)
{
	return p1.second < p2.second;
}

TetrisController::TetrisController(TetrisModel* _model): model(_model){}

void TetrisController::KeyPressed(int btnCode)
{
	// if figure is not spawned or fell or acceselarated
	if (model->get_current_figure_status() != FIGURE_FALL)
		return;

	if (btnCode == ROTATE_KEY) // space
	{
		rotate();
		return;
	}
	// functional keys
	char a = _getch();
	switch (a)
	{
	case LEFT_SHIFT_KEY: // left
		shift(LEFT);
		break;
	case RIGHT_SHIFT_KEY: // right
		shift(RIGHT);
		break;
	case ACCELERATE_KEY: // down
		accelerate();
		break;
	}
}

void TetrisController::UpdateF(float delta_time)
{
	long long it = model->get_game_iteration();
	// it % (MAX_SPEED - model->get_game_speed() + 1) == 0 this expr means that only one frame per 
	// (MAX_SPEED - model->get_game_speed() + 1) is "real"
	// P.S. Чего только не сделаешь ради FPS
	if (model->get_game_status() != GAME_FINISHED && it % (MAX_SPEED - model->get_game_speed() + MIN_SPEED) == 0)
	{
		switch (model->get_current_figure_status())
		{
		case FIGURE_INIT:
		{
			spawn_figure();
			if (model->get_game_status() == GAME_INIT)
				start();
			int c = model->get_figure_counter();
			if (c % N_FIGURES_SPEED_UP == 0 && c > 0)
				model->set_game_speed(model->get_game_speed() + 1);
			model->increment_figure_counter();
		}
			break;
		case FIGURE_FELL:
		{

			auto filled = model->get_filled_lines();
			delete_lines(filled);
			add_points(filled.size() * POINTS_PER_LINE);
			model->set_current_figure(model->get_next_figure());
			model->set_next_figure(random_figure());
			game_over_check();
		}
		break;
		default:
			down();
			break;
		}
	}
	model->increment_game_iteration();


}

void TetrisController::start()
{
	model->set_game_status(GAME_STARTED);
}

void TetrisController::down()
{
	vector<pair<int, int> > f_coords = model->get_current_figure_coords();
	for (const auto& coords : f_coords)
		if (find(f_coords.begin(), f_coords.end(), make_pair(coords.first, coords.second + 1)) != f_coords.end())
			continue;
		else if (coords.second == HEIGHT - 1 || model->get_cell_status(coords.first, coords.second + 1) == CELL_FILLED)
		{
			// figure fell so we can set previous speed
			if (model->get_current_figure_status() == FIGURE_ACCELERATED)
				model->set_game_speed(model->get_game_speed() - ACCELERATION);
			model->set_current_figure_status(FIGURE_FELL);
			return;
		}
	// wipe previous figures position
	for (const auto& coords : f_coords)
		model->set_cell_status(coords, CELL_EMPTY);
	vector<pair<int, int> > new_coords;
	// get new figure coordinates and fill them
	for (auto& coords : f_coords)
	{
		new_coords.push_back(make_pair(coords.first, coords.second + 1));
		model->set_cell_status(coords.first, coords.second + 1, CELL_FILLED);
	}
	model->set_current_figure_coords(new_coords);

}

void TetrisController::accelerate()
{
	model->set_current_figure_status(FIGURE_ACCELERATED);
	model->set_game_speed(model->get_game_speed() + ACCELERATION);
}

void TetrisController::shift(Direction d)
{
	int offset = 0;
	if (d == RIGHT)
		offset = 1;
	else
		offset = -1;
	vector<pair<int, int> > f_coords = model->get_current_figure_coords();
	for (const auto& coords : f_coords)
		// if next point filled by current figure all it's ok, so ignore it
		if (find(f_coords.begin(), f_coords.end(), make_pair(coords.first + offset, coords.second)) != f_coords.end())
			continue;
		else if ((coords.first == 0 && d == LEFT) || (coords.first == WIDTH - 1 && d == RIGHT)
			|| model->get_cell_status(coords.first + offset, coords.second) == CELL_FILLED)
			return;
	
	for (const auto& coords : f_coords)
		model->set_cell_status(coords, CELL_EMPTY);
	vector<pair<int, int> > new_coords;
	for (auto& coords : f_coords)
	{
		new_coords.push_back(make_pair(coords.first + offset, coords.second));
		model->set_cell_status(coords.first + offset, coords.second, CELL_FILLED);
	}
	model->set_current_figure_coords(new_coords);
}

void TetrisController::rotate()
{
	// don't rotate square
	if (model->get_current_figure_type() == O)
		return;
	vector<pair<int, int> > f_coords = model->get_current_figure_coords();
	vector<pair<int, int> > rotated;
	pair<int, int> center = f_coords[f_coords.size() / 2];
	for (const auto& coord : f_coords)
	{
		int rot_x, rot_y;
		rot_x = -coord.second + center.second + center.first;
		rot_y = coord.first + center.second - center.first;
		if ( (rot_x >= 0 && rot_x <= WIDTH - 1) && (rot_y >= 0 && rot_y <= HEIGHT - 1) && 
			(model->get_cell_status(rot_x, rot_y) == CELL_EMPTY 
			|| (model->get_cell_status(rot_x, rot_y) == CELL_FILLED
			&& find(f_coords.begin(), f_coords.end(), make_pair(rot_x, rot_y)) != f_coords.end())))
			rotated.push_back(make_pair(rot_x, rot_y));
		else
			return;
	}
	for (const auto& coords : f_coords)
		model->set_cell_status(coords, CELL_EMPTY);
	for (const auto& coords: rotated)
		model->set_cell_status(coords.first, coords.second, CELL_FILLED);
	model->set_current_figure_coords(rotated);
}

void TetrisController::spawn_figure()
{
	auto coords = get_default_figure_coords(WIDTH / 2, 0, model->get_current_figure_type());
	model->set_current_figure_coords(coords);
	model->set_current_figure_status(FIGURE_FALL);
	for (const auto& coord : coords)
		model->set_cell_status(coord, CELL_FILLED);
}

void TetrisController::delete_lines(const vector<int>& inds)
{
	if (!inds.size())
		return;
	int n = inds.size();
	for (int i = 0; i < n; i++)
		for (int j = 0; j < WIDTH; j++)
			model->set_cell_status(j, inds[i], CELL_EMPTY);

	for (int i = 0; i < n; i++)
		for (int j = 0; j < WIDTH; j++)
			for (int k = inds[i]; k > 0; k--)
				model->set_cell_status(j, k, model->get_cell_status(j, k - 1));
}

void TetrisController::add_points(const int& p)
{
	model->set_game_score(model->get_game_score() + p);
}

void TetrisController::game_over_check()
{
	for (auto& col : model->get_game_field())
		if (col[0] == CELL_FILLED)
		{
			model->set_game_status(GAME_FINISHED);
			break;
		}

}