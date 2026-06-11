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
        LOG("Engine initialization was failed.");
        return false;
    }

    auto timer = engine.getTimer();
    auto sController = engine.getStageController();

    auto AozoraInfo = std::make_unique<GameInfo>(
        "青空下的约定 Reimagined", 1, 0, true,
        std::make_unique<PreloadStage>(timer, sController), 1920, 1080, false);

    engine.GameRegistry(std::move(AozoraInfo));

    try
    {
        engine.MainLoop();
        engine.CleanUp();
    }
    catch (const std::exception &e)
    {
        LOG("The engine exited with error : {}", e.what());
        return false;
    }

    return true;
}