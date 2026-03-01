#include "Aozora/Aozora.hpp"
#include <exception>
#include <memory>
#include <stdexcept>
#include <system_error>

bool Aozora::StartUp()
{
    (void)OpenEngine::getInstance();

    OpenEngine &engine = OpenEngine::getInstance();

    if (!engine.Initialize())
    {
        SDL_Log("Engine initialization was failed.");
        return false;
    }

    auto timer = engine.getTimer();
    auto sController = engine.getStageController();

    auto AozoraInfo = std::make_unique<GameInfo>(
        "青空下的约定 Reimagined", 1, 0, true,
        std::make_unique<PreloadStage>(timer, sController));

    engine.GameRegistry(std::move(AozoraInfo));

    try
    {
        engine.MainLoop();
        engine.CleanUp();
    }
    catch (std::exception e)
    {
        SDL_Log("The engine exited with error : %s", e.what());
        return false;
    }

    return true;
}