#include "Aozora/Stage/PreloadStage.hpp"
#include "Aozora/Aozora.hpp"
#include "OpenCore.hpp"

PreloadStage::PreloadStage(Timer *timer, StageManager *sController)
{
    this->timer = timer;
    this->sController = sController;

    this->stageType = StageType::overlayStage;

    Elements = std::make_unique<ElementManager>();
}

void PreloadStage::onEnter()
{
    LOG("PreloadStage: onEnter - starting resource load");
    LoadingState = OpenCoreManagers::ResManager.LoadResourcesFromJson(10001);
    initializeComponents();
}

void PreloadStage::onExit()
{
    Elements->onDestroy();
    LOG("PreloadStage: onExit - cleared elements");
}

void PreloadStage::onDestroy() { LOG("PreloadStage: onDestroy"); }

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

void PreloadStage::onRender()
{
    Elements->onRender();
}

void PreloadStage::initializeComponents()
{
    pipeline
        .next([this]() -> bool {
            return LoadingState.wait_for(std::chrono::milliseconds(0)) ==
                   std::future_status::ready;
        })
        .next([this]() -> bool {
            auto frameCounter = UI<FrameCounter>("frameCounter", 100, 0, 0, 0);
            frameCounter->Configure().Sequence(true);
            frameCounter->Animate().Timer(6.0f).Commit();
            Elements->PushElement(std::move(frameCounter));

            auto loadingTag = UI<ImageBoard>("loadtag", 99, img_loadtag, 1, 1);
            loadingTag->Configure()
                .Parent(nullptr)
                .Anchor(AnchorPoint::BottomRight)
                .Posite(1.0f, 1.0f)
                .Scale(0.0f, 0.3f);
            loadingTag->Animate().Timer(2.0f).Commit();
            Elements->PushElement(std::move(loadingTag));
            return true;
        })
        .next([this]() -> bool {
            auto *entry = Elements->find("loadtag");
            return entry && entry->isAnimeFinished();
        })
        .next([this]() -> bool {
            Elements->removeElement("animation");

            auto startTitle = UI<ImageBoard>("startTitle", 9, icon_opencore, 1, 1);
            startTitle->Configure()
                .Scale(0.52f, 0.46f)
                .Anchor(AnchorPoint::Center)
                .Posite(0.5f, 0.5f)
                .Sequence(true)
                .Alpha(1.0f);
            Elements->PushElement(std::move(startTitle));
            return true;
        })
        .next([this]() -> bool {
            auto *loadtag = Elements->find("loadtag");
            auto *title = Elements->find("startTitle");
            return (!loadtag || loadtag->isAnimeFinished()) &&
                   title && title->isAnimeFinished();
        })
        .next([this]() -> bool {
            auto *title = Elements->find("startTitle");
            title->Configure().Scale(0.5f, 0.0f).Alpha(1.0f);
            title->setSequential(true);
            title->Animate().Timer(3.0f).Commit();
            Elements->removeElement("loadtag");
            return true;
        })
        .next([this]() -> bool {
            auto *title = Elements->find("startTitle");
            return title && title->isAnimeFinished();
        })
        .next([this]() -> bool {
            auto *title = Elements->find("startTitle");
            title->changeTexture(MakeTexture(1, 1, icon_giga));
            title->Configure().Scale(1.0f, 0.0f);
            title->setSequential(true);
            title->Animate().Timer(3.0f).Commit();
            return true;
        })
        .next([this]() -> bool {
            auto *title = Elements->find("startTitle");
            return title && title->isAnimeFinished();
        })
        .next([this]() -> bool {
            auto mainStage = std::make_unique<MainStage>(timer, sController);
            transferElementTo(mainStage.get(), "frameCounter");
            transferElementTo(mainStage.get(), "startTitle");
            sController->changeStage(std::move(mainStage));
            return true;
        });
}