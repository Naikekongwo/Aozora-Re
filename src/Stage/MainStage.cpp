#include "Aozora/Stage/MainStage.hpp"
#include "OpenCore.hpp"
#include "Runtime/Graphics/UI/ImageBoard.hpp"
#include <memory>

namespace
{
// 上下遮罩色（信箱式黑边）
const Color kLetterBoxColor{0.0f, 0.03921568f, 0.3215686f, 0.5f};
// 调试用占位色（待替换为真实素材后移除）
const Color kDebugTint{0.0f, 0.0f, 1.0f, 1.0f};
} // namespace

void MainStage::onEnter() { initializeComponents(); }

void MainStage::onUpdate()
{
    Elements->onUpdate(timer->getTotalTime());
    pipeline.onUpdate(timer->getTotalTime());
}

void MainStage::onRender() { Elements->onRender(); }

bool MainStage::parseEvents(Event *event)
{
    Elements->parseEvents(event, timer->getTotalTime());
    return true;
}

void MainStage::initializeComponents()
{
    auto elem = Elements->find("past");
    if (!elem)
    {
        LOG("找不到 'past' 元素，元素转移可能失败");
    }
    else
    {
        elem->Animate().Fade(1.0f, 0.0f, 5.0f).Commit();
    }

    auto background =
        UI<ImageBoard>("background_main", 0, "menu_daytime", 1, 1);

    background->Configure()
        .Parent(nullptr)
        .Anchor(AnchorPoint::Center)
        .PositeR(0.5f, 0.5f)
        .ScaleR(1.0f, 1.0f)
        .Follow(20)
        .Sequence(true);

    Elements->PushElement(std::move(background));

    // 上下遮罩

    auto blackBarUp   = UI<ImageBoard>("barUp", 1, "", 1, 1);
    auto blackBarDown = UI<ImageBoard>("barDown", 1, "", 1, 1);

    blackBarUp->Configure()
        .Parent(nullptr)
        .Anchor(AnchorPoint::BottomCenter)
        .PositeR(0.5f, 0.0f)
        .ScaleR(1.0f, 0.208f)
        .Sequence(true);

    blackBarDown->Configure()
        .Parent(nullptr)
        .Anchor(AnchorPoint::TopCenter)
        .PositeR(0.5f, 1.0f)
        .ScaleR(1.0f, 0.208f)
        .Sequence(true);

    blackBarUp->setBackgroundColor(kLetterBoxColor);
    blackBarDown->setBackgroundColor(kLetterBoxColor);

    blackBarUp->Animate()
        .Timer(5.0f)
        .MoveR(0.5f, 0.0f, 0.5f, 0.2083f, 3.0f)
        .Commit();
    blackBarDown->Animate()
        .Timer(5.0f)
        .MoveR(0.5f, 1.0f, 0.5f, 0.7917f, 3.0f)
        .Commit();

    Elements->PushElement(std::move(blackBarUp));
    Elements->PushElement(std::move(blackBarDown));

    auto MainTitle =
        UI<ImageBoard>("main_title", 2, "HD_Logo_Transparent", 1, 1);

    MainTitle->Configure()
        .Anchor(AnchorPoint::TopLeft)
        .ScaleR(0.4f, 0.0f)
        .PositeR(0.0625f, 0.111f)
        .Alpha(0.0f)
        .Follow(10)
        .Sequence(true);

    MainTitle->Animate()
        .Timer(5.0f)
        .SubStart(true)
        ->MoveR(0.09375f, 0.1111f, 0.0625f, 0.1111f, 3.0f)
        .Fade(0.0f, 1.0f, 3.0f)
        .SubEnd()
        .Commit();

    Elements->PushElement(std::move(MainTitle));

    // 创建按钮

    auto button_new  = UI<Button>("newworld", 3, "button_newworld", 1, 3);
    auto button_con  = UI<Button>("continue", 3, "button_continue", 1, 3);
    auto button_set  = UI<Button>("settings", 3, "button_settings", 1, 3);
    auto button_exit = UI<Button>("exit", 3, "button_exit", 1, 3);

    button_new->Configure()
        .Anchor(AnchorPoint::TopRight)
        .PositeR(0.94f, 0.55f)
        .ScaleR(0.0f, 0.06f)
        .Alpha(0.0f)
        .Sequence(true);

    button_con->Configure()
        .Anchor(AnchorPoint::TopRight)
        .PositeR(0.94f, 0.62f)
        .ScaleR(0.0f, 0.06f)
        .Alpha(0.0f)
        .Sequence(true);

    button_set->Configure()
        .Anchor(AnchorPoint::TopRight)
        .PositeR(0.94f, 0.69f)
        .ScaleR(0.0f, 0.06f)
        .Alpha(0.0f)
        .Sequence(true);

    button_exit->Configure()
        .Anchor(AnchorPoint::TopRight)
        .PositeR(0.94f, 0.76f)
        .ScaleR(0.0f, 0.06f)
        .Alpha(0.0f)
        .Sequence(true);

    button_new->setBackgroundColor(kDebugTint);
    button_con->setBackgroundColor(kDebugTint);
    button_set->setBackgroundColor(kDebugTint);
    button_exit->setBackgroundColor(kDebugTint);

    button_new->Animate()
        .Timer(5.0f)
        .SubStart(true)
        ->MoveR(1.0f, 0.5556f, 0.9375f, 0.5556f, 3.0f)
        .Fade(0.0f, 1.0f, 3.0f)
        .SubEnd()
        .Commit();

    button_con->Animate()
        .Timer(5.0f)
        .SubStart(true)
        ->MoveR(1.0f, 0.625f, 0.9375f, 0.625f, 3.0f)
        .Fade(0.0f, 1.0f, 3.0f)
        .SubEnd()
        .Commit();
    button_set->Animate()
        .Timer(5.0f)
        .SubStart(true)
        ->MoveR(1.0f, 0.6944f, 0.9375f, 0.6944f, 3.0f)
        .Fade(0.0f, 1.0f, 3.0f)
        .SubEnd()
        .Commit();

    button_exit->Animate()
        .Timer(5.0f)
        .SubStart(true)
        ->MoveR(1.0f, 0.7639f, 0.9375f, 0.7639f, 3.0f)
        .Fade(0.0f, 1.0f, 3.0f)
        .SubEnd()
        .Commit();

    Elements->PushElement(std::move(button_new));
    Elements->PushElement(std::move(button_con));
    Elements->PushElement(std::move(button_set));
    Elements->PushElement(std::move(button_exit));

    auto copyright = UI<ImageBoard>("Copyright", 3, "icon_copyright", 1, 1);

    copyright->Configure()
        .Anchor(AnchorPoint::BottomRight)
        .PositeR(0.97f, 0.96f)
        .ScaleR(0.0f, 0.043f)
        .Alpha(0.0f)
        .Sequence(true);

    copyright->setBackgroundColor(kDebugTint);

    copyright->Animate()
        .Timer(8.0f)
        .SubStart(true)
        ->MoveR(1.0f, 0.9583f, 0.9714f, 0.9583f, 3.0f)
        .Fade(0.0f, 1.0f, 3.0f)
        .SubEnd()
        .Commit();
    Elements->PushElement(std::move(copyright));

    pipeline
        // 等待 startTitle 淡出动画完成，然后移除
        .next(
            [this]() -> bool
            {
                auto *title = Elements->find("startTitle");
                return !title || title->isAnimeFinished();
            })
        .next(
            [this]() -> bool
            {
                Elements->removeElement("startTitle");
                return true;
            });
}