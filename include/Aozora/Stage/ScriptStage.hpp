#pragma once

#include "Gameplay/AVG/Stage/VisualNovelStage.hpp"

class ScriptStage : public VisualNovelStage
{
  public:
    // 重写父类初始化方法 : 构造自己的舞台场景
    void initializeComponents() override;
};