#pragma once

#include <array>
#include <vector>
#include <utility>

#include "Observer.h"
#include "Config.h"

enum TetrisFigureType {I, J, L, O, S, T, Z};
enum GameStatus {GAME_INIT, GAME_STARTED, GAME_FINISHED};
enum FigureStatus { FIGURE_INIT, FIGURE_FALL, FIGURE_ACCELERATED, FIGURE_FELL };
enum CellStatus {CELL_EMPTY, CELL_FILLED};

typedef std::array<std::array<CellStatus, HEIGHT>, WIDTH> GameField;
typedef std::vector<std::pair<int, int> > FigureCoords;

class TetrisFigure
{
	friend class TetrisModel;
public:
	TetrisFigure(const TetrisFigureType& type = I);
private:
	FigureStatus figure_status;
	TetrisFigureType figure_type;
	FigureCoords figure_current_coords;
	FigureCoords figure_prev_coords;
};


class TetrisModel : public Observable
{
public:
	TetrisModel();
	
	// general game data
	int get_game_score() const;
	void set_game_score(const int& score);
	int get_game_speed() const;
	void set_game_speed(const int& speed);
	int get_figure_counter() const;
	void increment_figure_counter();
	long long get_game_iteration() const;
	void increment_game_iteration();
	GameStatus get_game_status() const;
	void set_game_status(const GameStatus& status);

	TetrisFigure get_current_figure() const;
	void set_current_figure(const TetrisFigure& f);
	TetrisFigure get_next_figure() const;
	void set_next_figure(const TetrisFigure& f);
	
	// figures data
	FigureStatus get_current_figure_status() const;
	void set_current_figure_status(const FigureStatus& status);
	TetrisFigureType get_current_figure_type() const;
	TetrisFigureType get_next_figure_type() const;
	void set_current_figure_type(const TetrisFigureType& type);
	FigureCoords get_current_figure_current_coords() const;
	void set_current_figure_current_coords(const FigureCoords& coords);
	FigureCoords get_current_figure_prev_coords() const;
	
	// game field data
	CellStatus get_cell_status(const int& x, const int& y) const;
	CellStatus get_cell_status(const std::pair<int, int>& coords) const;
	void set_cell_status(const int& x, const int& y, const CellStatus& status);
	void set_cell_status(const std::pair<int, int>& coords, const CellStatus& status);
	const GameField& get_game_field() const;
	
	//computed getter
	std::vector<int> get_filled_lines() const;

private:
	int game_score;
	int game_speed;
	int figure_counter;
	long long game_iteration;
	GameStatus game_status;
	TetrisFigure current_figure;
	TetrisFigure next_figure;
	GameField game_field;
};

TetrisFigure random_figure();

std::vector<std::pair<int, int> > get_default_figure_coords(int x0, int y0, TetrisFigureType type);