#include "Aozora/Aozora.hpp"
#include "OpenCore.hpp"
#include <functional>
#include <memory>

MainStage::MainStage(Timer *timer, StageManager *sController)
{
    this->timer = timer;

    this->sController = sController;

    Elements = std::make_unique<ElementManager>();

    this->stageType = StageType::overlayStage;
}

void MainStage::setupBackground()
{
    auto elem = Elements->find("startTitle");
    if (!elem)
    {
        LOG("MainStage::setupBackground() warning: 'startTitle' not found, "
            "element transfer may have failed.");
    }
    else
    {
        elem->Animate().Fade(1.0f, 0.0f, 5.0f).Commit();
    }

    auto background =
        UI<ImageBoard>("background_main", 0, background_main, 1, 1);

    background->Configure()
        .Parent(nullptr)
        .Anchor(AnchorPoint::Center)
        .Posite(0.5f, 0.5f)
        .Scale(1.0f, 1.0f)
        .Sequence(true);

    Elements->PushElement(std::move(background));

    // 上下遮罩

    auto blackBarUp = UI<ImageBoard>("barUp", 1, blackbar, 1, 1);
    auto blackBarDown = UI<ImageBoard>("barDown", 1, blackbar, 1, 1);

    blackBarUp->Configure()
        .Parent(nullptr)
        .Anchor(AnchorPoint::BottomCenter)
        .Posite(0.5f, 0.0f)
        .Scale(1.0f, 0.208f)
        .Alpha(0.35f)
        .Sequence(true);

    blackBarDown->Configure()
        .Parent(nullptr)
        .Anchor(AnchorPoint::TopCenter)
        .Posite(0.5f, 1.0f)
        .Scale(1.0f, 0.208f)
        .Alpha(0.35f)
        .Sequence(true);

    blackBarUp->Animate().Timer(5.0f).Move(960, 0, 960, 225, 3.0f).Commit();
    blackBarDown->Animate()
        .Timer(5.0f)
        .Move(960, 1080, 960, 855, 3.0f)
        .Commit();

    Elements->PushElement(std::move(blackBarUp));
    Elements->PushElement(std::move(blackBarDown));
}

void MainStage::setupTitle()
{
    auto MainTitle = UI<ImageBoard>("main_title", 2, title_main, 1, 1);

    MainTitle->Configure()
        .Anchor(AnchorPoint::TopLeft)
        .Scale(0.4f, 0.0f)
        .Posite(0.0625f, 0.111f)
        .Alpha(0.0f)
        .Sequence(true);

    MainTitle->Animate()
        .Timer(5.0f)
        .SubStart(true)
        ->Move(240, 160, 160, 160, 3.0f)
        .Fade(0.0f, 1.0f, 3.0f)
        .SubEnd()
        .Commit();

    Elements->PushElement(std::move(MainTitle));
}

void MainStage::setupButtons()
{
    // 创建按钮

    auto button_new = UI<Button>("newworld", 3, img_bunewworld, 1, 3);
    auto button_con = UI<Button>("continue", 3, img_bucontinue, 1, 3);
    auto button_set = UI<Button>("settings", 3, img_busettings, 1, 3);
    auto button_exit = UI<Button>("exit", 3, img_buexit, 1, 3);

    button_new->Configure()
        .Anchor(AnchorPoint::TopRight)
        .Posite(0.94f, 0.55f)
        .Scale(0.0f, 0.06f)
        .Sequence(false);

    button_con->Configure()
        .Anchor(AnchorPoint::TopRight)
        .Posite(0.94f, 0.62f)
        .Scale(0.0f, 0.06f)
        .Sequence(false);

    button_set->Configure()
        .Anchor(AnchorPoint::TopRight)
        .Posite(0.94f, 0.69f)
        .Scale(0.0f, 0.06f)
        .Sequence(false);

    button_exit->Configure()
        .Anchor(AnchorPoint::TopRight)
        .Posite(0.94f, 0.76f)
        .Scale(0.0f, 0.06f)
        .Sequence(false);

    button_new->Animate()
        .Move(1920, 600, 1800, 600, 3.0f)
        .Fade(0.0f, 1.0f, 3.0f)
        .Commit();

    button_con->Animate()
        .Move(1920, 900, 1800, 900, 3.0f)
        .Fade(0.0f, 1.0f, 3.0f)
        .Commit();
    button_set->Animate()
        .Move(1920, 750, 1800, 750, 3.0f)
        .Fade(0.0f, 1.0f, 3.0f)
        .Commit();

    button_exit->Animate()
        .Move(1920, 825, 1800, 825, 3.0f)
        .Fade(0.0f, 1.0f, 3.0f)
        .Commit();

    Elements->PushElement(std::move(button_new));
    Elements->PushElement(std::move(button_con));
    Elements->PushElement(std::move(button_set));
    Elements->PushElement(std::move(button_exit));

    auto copyright = UI<ImageBoard>("Copyright", 3, icon_copyright, 1, 1);

    copyright->Configure()
        .Anchor(AnchorPoint::BottomRight)
        .Posite(0.97f, 0.96f)
        .Scale(0.0f, 0.043f);

    copyright->Animate()
        .Move(1920, 1035, 1865, 1035, 3.0f)
        .Fade(0.0f, 1.0f, 3.0f)
        .Commit();
    Elements->PushElement(std::move(copyright));
}

void MainStage::onEnter()
{
    // 播放进入动画、BGM
    OpenCoreManagers::SFXManager.stopBGM();
    phase = MainStagePhase::Idle;

    OpenCoreManagers::SFXManager.changeBGM(1003);
    OpenCoreManagers::SFXManager.playBGM();

    setupBackground();
    setupTitle();
}

void MainStage::onExit()
{
    // 停止动画、音效
}

void MainStage::onUpdate()
{
    Elements->onUpdate(timer->getTotalTime());
    if (phase == MainStagePhase::Idle)
    {
        auto connector = Elements->find("startTitle");
        if (connector and connector->isAnimeFinished())
        {
            Elements->removeElement("startTitle");
            OpenCoreManagers::SFXManager.playSE(titleumi);
            setupButtons();
        }
    }
}

void MainStage::onRender() { Elements->onRender(); }

bool MainStage::handlEvents(SDL_Event *event)
{
    Elements->handlEvents(*event, timer->getTotalTime());
    return true;
}

bool MainStage::parseEvents(Event *event)
{
    SDL_Event lEvent = *event;
    return handlEvents(&lEvent);
}