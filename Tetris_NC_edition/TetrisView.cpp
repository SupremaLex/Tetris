#include "TetrisView.h"
#include "TetrisModel.h"
#include "Config.h"

TetrisView::TetrisView(TetrisModel* _model)
{
	model = _model;
	model->add_observer(this);
}

void TetrisView::update()
{
	if (model->get_game_status() == GAME_INIT)
	{
		draw_borders();
		draw_score();
		string word = "> SCORE: ";
		int size = word.size();
		for (int i = 1; i <= size; i++)
			SetChar(i, 1 + HEIGHT + HEIGHT / 8, word[i-1]);
		string word1 = "> SPEED: ";
		int size1 = word.size();
		for (int i = 1; i <= size1; i++)
			SetChar(i, 2 + HEIGHT + HEIGHT / 8, word1[i - 1]);
	}
	draw_field();
	draw_score();
	draw_speed();
	wipe_next_figure_field();
	draw_next_figure();
	if (model->get_game_status() == GAME_FINISHED)
		draw_game_over();
}

void TetrisView::draw_borders()
{
	for (auto x : { 0,  WIDTH + WIDTH / 2 })
		for (int i = 0; i < HEIGHT + HEIGHT / 4; i++)
			SetChar(x, i, BORDER_CHAR);
	for (int i = 0; i < HEIGHT + 1; i++)
		SetChar(WIDTH + 1, i, BORDER_CHAR);
	for (auto y : { 0, HEIGHT + 1, HEIGHT + HEIGHT / 4 })
		for (int i = 0; i <= WIDTH + WIDTH / 2; i++)
			SetChar(i, y, BORDER_CHAR);
}

void TetrisView::draw_field()
{
	const auto& field = model->get_game_field();
	for (int i = 1; i <= WIDTH; i++)
		for (int j = 1; j <= HEIGHT; j++)
			if (model->get_cell_status(i - 1, j - 1) == CELL_FILLED)
				SetChar(i, j, BLOCK_CHAR);
			else
				SetChar(i, j, ZERO_CHAR);
}

void TetrisView::draw_next_figure()
{

	TetrisFigureType next_figure_type = model->get_next_figure_type();
	int x = WIDTH + WIDTH / 4;
	int y = HEIGHT / 2;
	if (next_figure_type == I)
	{
		x++;
		y--;
	}
	auto next_figure = get_default_figure_coords(x, y, next_figure_type);
	for (const auto& coord : next_figure)
		SetChar(coord.first, coord.second, BLOCK_CHAR);

}
void TetrisView::wipe_next_figure_field()
{
	int x = WIDTH + 2;
	for (int i = x; i < WIDTH + WIDTH / 2; i++)
		for (int j = HEIGHT / 2 - 2; j < HEIGHT / 2  + 4; j++)
		{
			SetChar(i, j, L' ');
		}

}

void TetrisView::draw_score()
{
	string score = to_string(model->get_game_score());
	int size = score.size();
	for (int i = 0; i < size; i++)
		SetChar(9 + i, 1 + HEIGHT + HEIGHT / 8, score[i]);
}

void TetrisView::draw_speed()
{
	string speed = to_string(model->get_game_speed());
	int size = speed.size();
	SetChar(9, 2 + HEIGHT + HEIGHT / 8, L' ');
	SetChar(10, 2 + HEIGHT + HEIGHT / 8, L' ');
	for (int i = 0; i < size; i++)
		SetChar(9 + i, 2 + HEIGHT + HEIGHT / 8, speed[i]);
}

void TetrisView::draw_game_over()
{
	string message = "GAME OVER";
	int size = message.size();
	for (int i = 0; i < size; i++)
		SetChar(4 + i, HEIGHT / 2, message[i]);
}