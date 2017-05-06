#ifndef GAME_DATA_H_
#define GAME_DATA_H_

// library includes
#include <functional>

// engine includes
#include "Util\FileUtils.h"

namespace game {

class GameData
{
public:
    GameData();
    ~GameData();

    void LoadAssetsListedInConfig(const std::function<void(void)>& i_on_loading_complete, const std::function<void(void)>& i_on_loading_failed);

    // constants
    static const char* GAME_CONFIG_FILE;

private:
    // disable copy constructor & copy assignment operator
    GameData(const GameData& i_copy) = delete;
    GameData& operator=(const GameData& i_copy) = delete;

    void OnFileLoaded(const engine::util::FileUtils::FileData& i_file_data);

private:
    std::function<void(void)>                               on_loading_complete_;
    std::function<void(void)>                               on_loading_failed_;
    size_t                                                  files_left_to_load_;
    size_t                                                  jobs_left_to_finish_;

}; // class GameData

} // namespace monsterchase

#endif // GAME_DATA_H_
