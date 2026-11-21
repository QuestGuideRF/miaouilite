
#pragma once
#ifndef __MUI__CONFIG__
#define __MUI__CONFIG__
#define MUI_CFG_ENABLE_V1DMRES 1
#if (MUI_CFG_ENABLE_V1DMRES)
#define MUI_CFG_ENABLE_CRYPTOPP 0
#endif
#if (!MUI_CFG_ENABLE_V1DMRES && !MUI_CFG_ENABLE_V2DMRES)
#error "没有可用的文件系统支持!"
#endif
#define MUI_MXML_ENABLE_DEFSTYLE 1
#define MUI_MXML_THROW_UNKNOWCTRL 0
#define MUI_CFG_ENABLE_DBGSOURCE 0
#define MUI_CFG_ENABLE_DBGFUNNAME 1
#endif