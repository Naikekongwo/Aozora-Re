#include "Aozora/Aozora.hpp"
#include "Aozora/Stage/PreloadStage.hpp"
#include "Core/Info/ResourceInfo.hpp"
#include <exception>
#include <memory>
#include <stdexcept>
#include <system_error>

bool Aozora::StartUp()
{
    (void)OpenEngine::getInstance();

    OpenEngine &engine = OpenEngine::getInstance();

    GraphicsInfo gInfo;

    gInfo.keepRatio = true;
    gInfo.nearestScaling = false;
    gInfo.resizable = false;
    gInfo.targetFrameRate = 144;
    gInfo.resolutionWidth = 3840;
    gInfo.resolutionHeight = 2160;

    ResourceInfo rInfo;

    rInfo.keepStructureWhenPackaging = true;
    rInfo.packageOnly = false;
    rInfo.textureResampleFactor = 1.0f;

    auto AozoraInfo = std::make_unique<GameInfo>();

    AozoraInfo->gameName = "青空下的约定";
    AozoraInfo->beta = true;
    AozoraInfo->version_major = 0;
    AozoraInfo->version_minor = 1;
    AozoraInfo->entranceStage = std::make_unique<PreloadStage>();

    AozoraInfo->_graphicsInfo = gInfo;
    AozoraInfo->_resourceInfo = rInfo;

    engine.GameRegistry(std::move(AozoraInfo));

    if (!engine.Initialize())
    {
        LOG("Engine initialization was failed.");
        return false;
    }

    auto Package = engine.getPackageManager();

    Package->registerResources({
        {RscTexture, "Loading_CN", "assets/ui/Loading_CN.png"},
        {RscTexture, "icon_opencore", "assets/ui/icon_opencore.png"},
        {RscTexture, "icon_giga", "assets/ui/icon_giga.png"},
        {RscTexture, "menu_daytime", "assets/backgrounds/menu_daytime.png"},
        {RscTexture, "HD_Logo_Transparent",
         "assets/ui/HD_Logo_Transparent.png"},
        {RscTexture, "icon_copyright", "assets/ui/icon_copyright.png"},
        {RscTexture, "button_newworld", "assets/ui/button_newworld.png"},
        {RscTexture, "button_continue", "assets/ui/button_continue.png"},
        {RscTexture, "button_settings", "assets/ui/button_settings.png"},
        {RscTexture, "button_exit", "assets/ui/button_exit.png"},
        {RscTexture, "menu_nighttime", "assets/backgrounds/menu_nighttime.png"},
        {RscFont, "OpenCoreFont", "assets/ui/font/OpenCoreFont.ttf"},
    });

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