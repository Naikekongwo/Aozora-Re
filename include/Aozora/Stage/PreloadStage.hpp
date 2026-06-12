// PreloadStage.hpp
// 预加载页面

#ifndef _PRELOADSTAGE_H_
#define _PRELOADSTAGE_H_

#include "World/Stage/StageManager.hpp"

#include <future>

class PreloadStage : public Stage
{
  public:
    PreloadStage(Timer *timer, StageManager *sController);

    void onEnter() override;
    void onExit() override;
    void onDestroy() override;

    void onUpdate() override;
    void onRender() override;

    void initializeComponents() override;

    bool parseEvents(Event *event) override;

  protected:
    std::future<void> LoadingState;

    Timer *timer;
    StageManager *sController;
};

#endif //_PRELOADSTAGE_H_