#pragma once

// Aozora.hpp
// 项目实际的头库

#define music_seawave 1001
#define music_seawave1 1002
#define se_seagull 1003
#define titleumi 1004

#define img_loadtag 2001
#define icon_opencore 2002
#define icon_giga 2003
#define background_main 2004
#define blackbar 2005
#define title_main 2006
#define icon_copyright 2007
#define img_bunewworld 2008
#define img_bucontinue 2009
#define img_busettings 2010
#define img_buexit 2011

#define font_normal 9001

#include "OpenCore.hpp"

#include "Stage/MainStage.hpp"
#include "Stage/PreloadStage.hpp"

class Aozora
{
  public:
    bool StartUp();

  private:
};