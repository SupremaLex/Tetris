#include "TetrisModel.h"

#include <ctime>
#include <cstdlib>

#include "windows.h"

#define BELONG_TO_FIELD(x, y) (x >= 0 && x < WIDTH) && (y >= 0 && y < HEIGHT)

using namespace std;

TetrisFigure::TetrisFigure(const TetrisFigureType& type)
{
	figure_type = type;
	figure_status = FIGURE_INIT;
	figure_coords = { {0, 0} };
}

TetrisModel::TetrisModel()
{
	for (int i = 0; i < WIDTH; i++)
		for (int j = 0; j < HEIGHT; j++)
				game_field[i][j] = CELL_EMPTY;
	game_status = GAME_INIT;
	game_score = 0;
	game_speed = MIN_SPEED;
	current_figure = random_figure();
	// if we don't want two same figures
	Sleep(500);
	next_figure = random_figure();
}

int TetrisModel::get_game_score() const
{
	return game_score;
}

void TetrisModel::set_game_score(const int& score)
{
	if (score < 0)
		throw exception("Score must be positive");
	game_score = score;
	notify_update();
}

int TetrisModel::get_game_speed() const
{
	return game_speed;
}

void TetrisModel::set_game_speed(const int& speed)
{
	if (speed > MAX_SPEED)
	{
		game_speed = MAX_SPEED;
		return;
	}
	else if (speed < MIN_SPEED)
	{
		game_speed = MIN_SPEED;
		return;
	}
	game_speed = speed;
}

int TetrisModel::get_figure_counter() const
{
	return figure_counter;
}

void TetrisModel::increment_figure_counter()
{
	figure_counter++;
}

long long TetrisModel::get_game_iteration() const
{
	return game_iteration;
}

void TetrisModel::increment_game_iteration()
{
	game_iteration++;
}

GameStatus TetrisModel::get_game_status() const
{
	return game_status;
}

void TetrisModel::set_game_status(const GameStatus& status)
{
	game_status = status;
	notify_update();
}

TetrisFigure TetrisModel::get_current_figure() const
{
	return current_figure;
}

void TetrisModel::set_current_figure(const TetrisFigure& f)
{
	current_figure = f;
}

TetrisFigure TetrisModel::get_next_figure() const
{
	return next_figure;

}

void TetrisModel::set_next_figure(const TetrisFigure& f)
{
	next_figure = f;
	notify_update();
}

FigureStatus TetrisModel::get_current_figure_status() const
{
	return current_figure.figure_status;
}

void TetrisModel::set_current_figure_status(const FigureStatus& status)
{
	current_figure.figure_status = status;
}

TetrisFigureType TetrisModel::get_current_figure_type() const
{
	return current_figure.figure_type;
}

TetrisFigureType TetrisModel::get_next_figure_type() const
{
	return next_figure.figure_type;
}

void TetrisModel::set_current_figure_type(const TetrisFigureType& type)
{
	current_figure.figure_type = type;
}

vector<pair<int, int> > TetrisModel::get_current_figure_coords() const
{
	return current_figure.figure_coords;
}

void TetrisModel::set_current_figure_coords(const vector<pair<int, int> >& coords)
{
	for (const auto& coord : coords)
		if (!BELONG_TO_FIELD(coord.first, coord.second))
			throw exception("x must belong to [0, WIDTH), y must belong to [0, HEIGHT)");
	current_figure.figure_coords = coords;
	notify_update();
}

CellStatus TetrisModel::get_cell_status(const int& x, const int& y) const
{
	return get_cell_status(std::make_pair(x, y));
}

CellStatus TetrisModel::get_cell_status(const std::pair<int, int>& coords) const
{
	if (coords.first < 0 || coords.second < 0)
		throw exception("x must belong to [0, WIDTH], y must belong to [0, HEIGHT]");
	return game_field[coords.first][coords.second];
}

void TetrisModel::set_cell_status(const int& x, const int& y, const CellStatus& status)
{
	set_cell_status(std::make_pair(x, y), status);
}

void TetrisModel::set_cell_status(const std::pair<int, int>& coords, const CellStatus& status)
{
	if (!BELONG_TO_FIELD(coords.first, coords.second))
		throw exception("x must belong to [0, WIDTH), y must belong to [0, HEIGHT)");
	game_field[coords.first][coords.second] = status;
}

const GameField& TetrisModel::get_game_field() const
{
	return game_field;
}

vector<int> TetrisModel::get_filled_lines() const
{
	vector<int> filled_lines;
	for (int i = 0; i < HEIGHT; i++)
	{
		bool filled = true;
		for (int j = 0; j < WIDTH; j++)
			if (game_field[j][i] == CELL_EMPTY)
			{
				filled = false;
				break;
			}
		if (filled)
			filled_lines.push_back(i);
	}
	return filled_lines;
}

TetrisFigure random_figure()
{
	std::srand(std::time(0));
	return TetrisFigure((TetrisFigureType)(std::rand() % N_FIGURES));
}

std::vector<pair<int, int> > get_default_figure_coords(int x0, int y0, TetrisFigureType type)
{
	if (x0 < 0 || y0 < 0)
		throw exception("x must belong to [0, WIDTH], y must belong to [0, HEIGHT]");
	std::vector<pair<int, int> > coords;
	switch (type)
	{
	case I:
		coords = { {x0, y0}, {x0, y0 + 1}, {x0, y0 + 2}, {x0, y0 + 3} };
		break;
	case J:
		coords = { {x0, y0}, {x0, y0 + 1}, {x0 + 1, y0 + 1}, {x0 + 2, y0 + 1} };
		break;
	case L:
		coords = { {x0 + 2, y0}, {x0, y0 + 1}, {x0 + 1, y0 + 1}, {x0 + 2, y0 + 1} };
		break;
	case O:
		coords = { {x0, y0}, {x0 + 1, y0}, {x0 + 1, y0 + 1}, {x0, y0 + 1} };
		break;
	case S:
		coords = { {x0 + 1, y0}, {x0 + 2, y0}, {x0 + 1, y0 + 1} , {x0, y0 + 1} };
		break;
	case T:
		coords = { {x0 + 1, y0}, {x0, y0 + 1}, {x0 + 1, y0 + 1},  {x0 + 2, y0 + 1} };
		break;
	case Z:
		coords = { {x0, y0}, {x0 + 1, y0}, {x0 + 1, y0 + 1}, {x0 + 2, y0 + 1} };
		break;
	}
	return coords;
}