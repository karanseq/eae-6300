#pragma once

#include <string.h>

namespace engine
{

// An enum to describe the direction of movement
enum MoveDirection
{
	MoveDirectionLeft = 0,
	MoveDirectionRight,
	MoveDirectionUp,
	MoveDirectionDown
};

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

	static inline int GetMaxRows() { return max_rows_; }
	static inline void SetMaxRows(int max_rows) { max_rows_ = max_rows; }

	static inline int GetMaxCols() { return max_cols_; }
	static inline void SetMaxCols(int max_cols) { max_cols_ = max_cols; }

	// constants
	static const int MAX_NAME_LENGTH = 10;

protected:
	// data
	static int max_rows_, max_cols_;
	int row_, column_;
	char name_[MAX_NAME_LENGTH + 1];
};	// class Player

} // namespace engine


