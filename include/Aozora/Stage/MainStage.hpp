// MainStage.hpp
// 定义主页面类的头文件
#ifndef _MAINSTAGE_H_
#define _MAINSTAGE_H_

#include "World/Stage/StageManager.hpp"

enum class MainStagePhase
{
    Entering,
    Idle,
    Exposure,
    Exiting
};

class StageManager;

class ElementManager;

class MainStage : public OverlayStage
{
  public:
    // 构造函数
    MainStage(Timer *timer, StageManager *sController);

    // 完成继承
    bool handlEvents(SDL_Event *event) override;

    void onEnter() override;
    void onExit() override;

    void onUpdate() override;
    void onRender() override;

    bool parseEvents(Event *event) override;

    void initializeComponents() override;

  private:
    MainStagePhase phase = MainStagePhase::Entering;
};

#endif //_MAINSTAGE_H_