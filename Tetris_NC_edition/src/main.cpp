#include "TetrisModel.h"
#include "TetrisApp.h"

int main(int argc, char * argv[]) {
	TetrisModel model;
	TetrisApp app(&model);
	app.Run();
}