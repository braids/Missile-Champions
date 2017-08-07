#include "../Objects.h"

void Cursor::Init(Assets::Image* cursor) {
	this->image = cursor;
	this->column = 0;
	this->row = 0;
	this->SelectEvent = this->NoSelection;
}

void Cursor::SetP1() {
	this->defaultX = CURSOR_P1_DEFAULT_X;
	this->defaultY = CURSOR_P1_DEFAULT_Y;
	this->image->rect->x = this->defaultX;
	this->image->rect->y = this->defaultY;
	this->column = 0;
	this->row = 0;
	this->SelectEvent = this->NoSelection;
}

void Cursor::SetP2() {
	this->defaultX = CURSOR_P2_DEFAULT_X;
	this->defaultY = CURSOR_P2_DEFAULT_Y;
	this->image->rect->x = this->defaultX;
	this->image->rect->y = this->defaultY;
	this->column = 0;
	this->row = 0;
	this->SelectEvent = this->NoSelection;
}

void Cursor::MoveCursor() {
	if (this->SelectEvent & this->SelectUp)
		this->row--;
	if (this->SelectEvent & this->SelectDown)
		this->row++;
	if (this->SelectEvent & this->SelectLeft)
		this->column--;
	if (this->SelectEvent & this->SelectRight)
		this->column++;

	if (this->row < CURSOR_ROW_MIN)
		this->row = CURSOR_ROW_MAX;
	if (this->row > CURSOR_ROW_MAX)
		this->row = CURSOR_ROW_MIN;
	if (this->column < CURSOR_COL_MIN)
		this->column = CURSOR_COL_MAX;
	if (this->column > CURSOR_COL_MAX)
		this->column = CURSOR_COL_MIN;

	this->image->rect->x = this->defaultX + (CURSOR_MOVE_X * this->column);
	this->image->rect->y = this->defaultY + (CURSOR_MOVE_Y * this->row);

	this->SelectEvent = this->NoSelection;
}

int Cursor::GetSelection() {
	// Return [0-7] + 1 to match team IDs (1-8)
	return 1 + this->row + (this->column * 4);
}