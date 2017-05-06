#ifndef LEVEL_H_
#define LEVEL_H_

namespace engine {
namespace data {
    class PooledString;
}
}

namespace game {

class Level
{
public:
    Level(const engine::data::PooledString& i_file_name);
    ~Level();

private:
    // disable copy constructor & copy assignment operator
    Level(const Level&) = delete;
    Level& operator=(const Level&) = delete;

}; // class Level

} // namespace game

#endif // LEVEL_H_
