#include "Aozora/Aozora.hpp"
#include "OpenCore/OpenCore.hpp"

PreloadStage::PreloadStage(Timer *timer, StageManager *sController)
{
    this->timer = timer;
    this->sController = sController;

    this->stageType = StageType::overlayStage;
    phase = PreloadPhase::WaitingForResource;

    Elements = std::make_unique<ElementManager>();
    pendingNextStage = false;
}

// 生命周期安全：进入舞台时初始化资源
void PreloadStage::onEnter()
{
    SDL_Log("PreloadStage: onEnter - starting resource load");

    LoadingState = OpenCoreManagers::ResManager.LoadResourcesFromJson(10001);
}

// 生命周期安全：退出舞台前清理
void PreloadStage::onExit()
{
    Elements->onDestroy();
    SDL_Log("PreloadStage: onExit - cleared elements");
}

// 生命周期安全：彻底销毁
void PreloadStage::onDestroy() { SDL_Log("PreloadStage: onDestroy"); }

// 事件传递：顶层拦截
bool PreloadStage::handlEvents(SDL_Event *event)
{
    if (phase >= PreloadPhase::BuildLoadingUI)
    {
        Elements->handlEvents(*event, timer->getTotalTime());
    }
    return true; // 拦截所有事件
}

// 分阶段更新函数
void PreloadStage::onUpdate()
{
    float totalTime = timer->getTotalTime();

    // 1️⃣ 先检查异步加载是否完成
    if (phase == PreloadPhase::WaitingForResource)
    {
        if (LoadingState.wait_for(std::chrono::milliseconds(0)) ==
            std::future_status::ready)
        {
            phase = PreloadPhase::BuildLoadingUI;
        }
    }

    // 2️⃣ 构建 UI 和动画
    if (phase == PreloadPhase::BuildLoadingUI)
    {
        buildLoadingUI();
        phase = PreloadPhase::WaitAnimation;
    }

    // 3️⃣ 等待动画完成
    if (phase == PreloadPhase::WaitAnimation)
    {
        Elements->onUpdate(totalTime);
        auto entry = Elements->find("loadtag");
        if (entry && entry->isAnimeFinished())
        {
            Elements->removeElement("animation");
            phase = PreloadPhase::InitAudio;
        }
    }

    // 4️⃣ 初始化音乐
    if (phase == PreloadPhase::InitAudio)
    {
        auto &SFX = OpenCoreManagers::SFXManager;
        SFX.loadBGM(1002);
        SFX.playBGM();
        SFX.setVolume(30);
        buildTitleAndWater();
        phase = PreloadPhase::ShowCoreIcon;
    }

    // 5️⃣ 处理标题动画阶段
    if (phase >= PreloadPhase::ShowCoreIcon && phase < PreloadPhase::Finished)
    {
        Elements->onUpdate(totalTime);
        handleTitleSequence();
    }

    // 6️⃣ 切换下一场景
    if (phase == PreloadPhase::Finished && !pendingNextStage)
    {
        pendingNextStage = true;
    }

    if (pendingNextStage)
    {
        unique_ptr<MainStage> mainStage =
            std::make_unique<MainStage>(timer, sController);

        transferElementTo(mainStage.get(), "frameCounter");
        transferElementTo(mainStage.get(), "startTitle");
        sController->changeStage(std::move(mainStage));
        return;
    }
}

// 渲染
void PreloadStage::onRender()
{
    if (phase >= PreloadPhase::BuildLoadingUI)
    {
        Elements->onRender();
    }
}

////////////////////////////////////////////////////////////////////////////////
// 内部辅助函数
////////////////////////////////////////////////////////////////////////////////

void PreloadStage::buildLoadingUI()
{
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

    loadingTag->Animate().Timer(13.0f).Commit();

    Elements->PushElement(std::move(loadingTag));
}

void PreloadStage::buildTitleAndWater()
{
    auto startTitle = UI<ImageBoard>("startTitle", 9, icon_opencore, 1, 1);
    startTitle->Configure()
        .Scale(0.52f, 0.46f)
        .Anchor(AnchorPoint::Center)
        .Posite(0.5f, 0.5f)
        .Sequence(true)
        .Alpha(1.0f);

    Elements->PushElement(std::move(startTitle));
}

void PreloadStage::handleTitleSequence()
{
    auto Title = Elements->find("startTitle");
    if (!Title || !Title->isAnimeFinished())
        return;

    switch (phase)
    {
    case PreloadPhase::ShowCoreIcon:
    {
        auto element = Elements->find("loadtag");
        if (element && element->isAnimeFinished())
        {
            Title->Configure().Scale(0.5f, 0.0f).Alpha(1.0f);
            Title->setSequential(true);
            Title->Animate().Timer(3.0f).Commit();

            Elements->removeElement("loadtag");

            phase = PreloadPhase::ShowGigaIcon;
        }
        break;
    }
    case PreloadPhase::ShowGigaIcon:
        Title->changeTexture(MakeTexture(1, 1, icon_giga));
        Title->Configure().Scale(1.0f, 0.0f);
        Title->setSequential(true);
        Title->Animate().Timer(3.0f).Commit();
        phase = PreloadPhase::WaitGiga;
        break;
    case PreloadPhase::WaitGiga:
        phase = PreloadPhase::Finished;
        break;
    default:
        break;
    }
}