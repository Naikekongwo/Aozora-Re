#include <cstdlib>

// 引入内核
#include "Aozora/Aozora.hpp"

// 游戏入口
int main(int argc, char *argv[])
{
    Aozora game;

    if (!game.StartUp())
    {
        LOG("游戏启动失败，请检查日志以获取详细信息。");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
