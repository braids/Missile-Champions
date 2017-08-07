#include "../Objects.h"

void Cursor::Init(Assets::Image* cursor) {
	this->image = cursor;					// Cursor image
	this->column = 0;						// Cursor column
	this->row = 0;							// Cursor row
	this->SelectEvent = this->NoSelection;	// Cursor selection event
}

void Cursor::SetP1() {
	this->defaultX = CURSOR_P1_DEFAULT_X;	// Set default cursor X to P1 default location
	this->defaultY = CURSOR_P1_DEFAULT_Y;	// Set default cursor Y to P1 default location
	this->image->rect->x = this->defaultX;	// Set image X to default X
	this->image->rect->y = this->defaultY;	// Set image Y to default Y
	this->column = 0;						// Set column to first column
	this->row = 0;							// Set row to first row
	this->SelectEvent = this->NoSelection;	// Set default selection state
}

void Cursor::SetP2() {
	this->defaultX = CURSOR_P2_DEFAULT_X;	// Set default cursor X to P1 default location
	this->defaultY = CURSOR_P2_DEFAULT_Y;	// Set default cursor Y to P1 default location
	this->image->rect->x = this->defaultX;	// Set image X to default X
	this->image->rect->y = this->defaultY;	// Set image Y to default Y
	this->column = 0;						// Set column to first column
	this->row = 0;							// Set row to first row
	this->SelectEvent = this->NoSelection;	// Set default selection state
}

void Cursor::MoveCursor() {
	if (this->SelectEvent & this->SelectUp)		// If move event up, subtract one from row
		this->row--;
	if (this->SelectEvent & this->SelectDown)	// If move event down, add one to row
		this->row++;
	if (this->SelectEvent & this->SelectLeft)	// If move event left, subtract one from column
		this->column--;
	if (this->SelectEvent & this->SelectRight)	// If move event right, subtract one from column
		this->column++;

	if (this->row < CURSOR_ROW_MIN)		// Wraparound rows if too far up
		this->row = CURSOR_ROW_MAX;
	if (this->row > CURSOR_ROW_MAX)		// Wraparound rows if too far down
		this->row = CURSOR_ROW_MIN;
	if (this->column < CURSOR_COL_MIN)	// Wraparound columns if too far left
		this->column = CURSOR_COL_MAX;
	if (this->column > CURSOR_COL_MAX)	// Wraparound columns if too far right
		this->column = CURSOR_COL_MIN;

	// Set cursor image position based on distance from default position
	this->image->rect->x = this->defaultX + (CURSOR_MOVE_X * this->column);
	this->image->rect->y = this->defaultY + (CURSOR_MOVE_Y * this->row);

	// Reset selection event
	this->SelectEvent = this->NoSelection;
}

int Cursor::GetSelection() {
	// Return [0-7] + 1 to match team IDs (1-8)
	return 1 + this->row + (this->column * 4);
}