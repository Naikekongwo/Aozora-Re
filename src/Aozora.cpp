#include "Aozora/Aozora.hpp"
#include "Aozora/Stage/PreloadStage.hpp"
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

    GraphicsInfo gInfo = {1920, 1080, 144, false};

    auto AozoraInfo = std::make_unique<GameInfo>();

    AozoraInfo->gameName = "青空下的约定";
    AozoraInfo->beta = true;
    AozoraInfo->version_major = 0;
    AozoraInfo->version_minor = 1;
    AozoraInfo->entranceStage =
        std::make_unique<PreloadStage>(timer, sController);

    AozoraInfo->_graphicsInfo = gInfo;

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