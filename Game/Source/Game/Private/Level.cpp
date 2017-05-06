#include "Game\Level.h"

// engine includes
#include "Assert\Assert.h"
#include "Data\PooledString.h"

namespace game {

Level::Level(const engine::data::PooledString& i_file_name)
{
    ASSERT(i_file_name.GetLength() > 0);
}

Level::~Level()
{}

} // namespace game

