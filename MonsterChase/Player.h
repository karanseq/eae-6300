#pragma once

#include <string.h>

// forward declaration
enum MoveDirection;

class Player
{
public:
	Player();
	Player(int row, int column, const char* name);
	virtual ~Player();

	// player behavior
	void Move(MoveDirection move_direction);
	virtual void Print();

	// accessors and mutators
	inline int GetRow() { return row_; }
	inline void SetRow(int row) { row_ = row; }

	inline int GetColumn() { return column_; }
	inline void SetColumn(int column) { column_ = column; }

	inline const char* GetName() { return name_; }
	inline void SetName(const char* name) { strcpy_s(name_, name); }

	// constants
	static const int MAX_NAME_LENGTH = 10;

protected:
	// data
	int row_, column_;
	char name_[MAX_NAME_LENGTH + 1];
};

