#include "Aozora/Stage/PreloadStage.hpp"
#include "Aozora/Aozora.hpp"
#include "OpenCore.hpp"

PreloadStage::PreloadStage()
{
    this->timer = timer;
    this->sController = sController;

    this->stageType = StageType::overlayStage;

    Elements = std::make_unique<ElementManager>();
}

void PreloadStage::onEnter()
{
    LOG("PreloadStage: onEnter - starting resource load");
    initializeComponents();
}

void PreloadStage::onExit()
{
    Elements->onDestroy();
    LOG("PreloadStage: onExit - cleared elements");
}

void PreloadStage::onDestroy()
{
    LOG("PreloadStage: onDestroy - transitioning to MainStage");
}

bool PreloadStage::parseEvents(Event *event)
{
    Elements->parseEvents(event, timer->getTotalTime());
    return true;
}

void PreloadStage::onUpdate()
{
    Elements->onUpdate(timer->getTotalTime());
    pipeline.onUpdate(timer->getTotalTime());
}

void PreloadStage::onRender() { Elements->onRender(); }

void PreloadStage::initializeComponents()
{
    pipeline
        .next(
            [this]() -> bool
            {
                auto frameCounter =
                    UI<FrameCounter>("frameCounter", 100, "OpenCoreFont", 0, 0);
                frameCounter->Configure().Sequence(true);
                frameCounter->Animate().Timer(6.0f).Commit();
                frameCounter->setFontName("OpenCoreFont");
                frameCounter->setFontSize(36);
                Elements->PushElement(std::move(frameCounter));

                auto loadingTag =
                    UI<ImageBoard>("loadtag", 99, "Loading_CN", 1, 1);
                loadingTag->Configure()
                    .Parent(nullptr)
                    .Anchor(AnchorPoint::BottomRight)
                    .Posite(1.0f, 1.0f)
                    .Scale(0.0f, 0.3f);
                loadingTag->Animate().Timer(2.0f).Commit();
                Elements->PushElement(std::move(loadingTag));
                return true;
            })
        .next(
            [this]() -> bool
            {
                auto *entry = Elements->find("loadtag");
                return entry && entry->isAnimeFinished();
            })
        .next(
            [this]() -> bool
            {
                Elements->removeElement("loadtag");

                auto startTitle =
                    UI<ImageBoard>("startTitle", 9, "icon_opencore", 1, 1);
                startTitle->Configure()
                    .Scale(0.52f, 0.46f)
                    .Anchor(AnchorPoint::Center)
                    .Posite(0.5f, 0.5f)
                    .Sequence(true)
                    .Alpha(1.0f);
                startTitle->Animate().Timer(3.0f).Commit();
                Elements->PushElement(std::move(startTitle));
                return true;
            })
        .next(
            [this]() -> bool
            {
                auto *loadtag = Elements->find("loadtag");
                auto *title = Elements->find("startTitle");
                return (!loadtag || loadtag->isAnimeFinished()) && title &&
                       title->isAnimeFinished();
            })
        .next(
            [this]() -> bool
            {
                auto *title = Elements->find("startTitle");
                title->changeTexture(
                    OpenEngine::getInstance()
                        .getTextureMetaManager()
                        ->registerTexture({"icon_giga", 1, 1}));
                title->Configure().Scale(0.229f, 0.0f).Alpha(1.0f);
                title->setSequential(true);
                title->Animate().Timer(3.0f).Commit();
                Elements->removeElement("loadtag");
                return true;
            })
        .next(
            [this]() -> bool
            {
                auto *title = Elements->find("startTitle");
                return title && title->isAnimeFinished();
            })
        .next(
            [this]() -> bool
            {
                auto *title = Elements->find("startTitle");
                title->Animate().Timer(3.0f).Commit();
                return true;
            })
        .next(
            [this]() -> bool
            {
                auto *title = Elements->find("startTitle");
                return title && title->isAnimeFinished();
            })
        .next(
            [this]() -> bool
            {
                // 截取当前画面作为过渡背景，立刻创建并转移到 MainStage
                auto screenshot = OpenCoreManagers::GFXManager.captureScreen();
                auto past =
                    std::make_unique<ImageBoard>("past", 999, screenshot);
                past->Configure()
                    .Parent(nullptr)
                    .Anchor(AnchorPoint::Center)
                    .Posite(0.5f, 0.5f)
                    .Scale(1.0f, 1.0f)
                    .Sequence(true);

                Elements->PushElement(std::move(past));

                auto mainStage = std::make_unique<MainStage>();
                transferElementTo(mainStage.get(), "past");
                transferElementTo(mainStage.get(), "frameCounter");
                sController->changeStage(std::move(mainStage));
                return true;
            });
}