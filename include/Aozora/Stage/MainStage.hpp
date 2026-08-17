// MainStage.hpp
// 定义主页面类的头文件
#ifndef _MAINSTAGE_H_
#define _MAINSTAGE_H_

#include "World/Stage/StageManager.hpp"

class MainStage : public OverlayStage
{
  public:
    void onEnter() override;

    void onUpdate() override;
    void onRender() override;

    bool parseEvents(Event *event) override;

    void initializeComponents() override;

  private:
};

#endif //_MAINSTAGE_H_