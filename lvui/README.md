# LVUI - 基于LVGL的轻量级操作系统UI框架

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![LVGL](https://img.shields.io/badge/Built%20with-LVGL-green.svg)](https://lvgl.io/)

LVUI是一个基于LVGL的轻量级操作系统UI框架，专为嵌入式系统和资源受限设备设计，提供现代、流畅的用户界面体验。

<img width="400" height="603" alt="example.gif" src="./example.gif" />

> **⚠️ 重要提示**
>
> 本项目当前处于**开发阶段**，API可能会有变更，请在使用时注意版本兼容性。

## ✨ 特性

- **现代UI设计**：模拟现代移动操作系统的界面风格，提供流畅的用户体验
- **模块化架构**：清晰的模块划分，易于扩展和维护
- **面向对象设计**：采用面向对象的设计思想，使用结构体和函数指针模拟类的概念，提高代码的可维护性和可扩展性
- **完整UI组件**：包含桌面、锁屏、状态栏、控制中心等完整UI组件
- **动画支持**：内置动画系统，提供流畅的UI交互效果
- **主题系统**：支持运行时切换明暗模式和自定义颜色
- **手势支持**：内置手势识别系统，支持多种手势操作
- **系统级API**：提供类系统API，可自定义时间、时区、状态图标等系统元素
- **壁纸支持**：锁屏和启动器支持自定义壁纸背景
- **配置系统**：通过 `lvui_conf.h` 提供编译时配置，支持日志和内存策略切换
- **跨平台**：基于LVGL，支持多种显示设备和输入设备
- **轻量级**：优化的资源占用，适合资源受限的嵌入式设备

## 📁 项目结构

```
lvui/
├── app/                  # 应用主题预设
│   └── lu_app_theme.c/h  #   应用容器主题
├── example/              # 示例代码
│   └── lu_example.c/h    #   完整初始化例程
├── frame/                # 框架核心模块
│   ├── anim/             #   动画系统
│   ├── disp/             #   显示管理 & 字体系统
│   ├── input/            #   触摸输入处理
│   ├── theme/            #   主题 & 颜色系统
│   └── widget/           #   基础组件 & 布局
├── misc/                 # 杂项工具
│   ├── lu_log.c/h        #   日志系统
│   ├── lu_pdc.c/h        #   PDC功能
│   └── lu_sll.c/h        #   单链表实现
├── plat/                 # 平台抽象层
│   ├── lu_lvgl.h         #   LVGL接口绑定
│   ├── lu_mem.c/h        #   内存管理
│   └── lu_type.h         #   类型定义
├── sys/                  # 系统模块
│   ├── lu_ctrcent.c/h    #   控制中心
│   ├── lu_launcher.c/h   #   启动器（应用网格）
│   ├── lu_sysinfo.c/h    #   系统信息（时间/时区管理）
│   └── lu_sysui.c/h      #   系统UI总入口
├── view/                 # 视图模块
│   ├── lu_ctrbar.c/h     #   控制栏
│   ├── lu_desktop.c/h    #   桌面容器
│   ├── lu_gesture.c/h    #   手势识别
│   ├── lu_locker.c/h     #   锁屏界面
│   └── lu_statbar.c/h    #   状态栏
├── lu_init.c/h           # 框架初始化
├── lvui.c/h              # 核心接口 (lvui_create / lvui_delete)
├── lvui_conf.h           # 编译时配置
└── README.md             # 项目说明
```

## 🚀 快速开始

### 环境要求

- LVGL 图形库
- C编译器（支持C99标准）
- 嵌入式系统或桌面模拟环境

### 基本使用

1. **初始化LVGL**
```c
lv_init();
```

2. **初始化LVUI框架**
```c
lu_init();
```

3. **创建LVUI实例**
```c
// 创建LVUI实例，指定宽度、高度和屏幕尺寸因子
lvui_t* lvui = lvui_create(320, 480, 3.5);
if (!lvui) {
    LV_LOG_USER("lvui_create failed");
    return -1;
}
```

4. **获取系统UI组件**
```c
// 通过系统UI获取各子模块
lu_launcher_t* launcher = lu_sysui_get_launcher(lvui->sysui);
lu_locker_t*   locker   = lu_sysui_get_locker(lvui->sysui);
lu_ctrcent_t*  ctrcent  = lu_sysui_get_ctrcent(lvui->sysui);
lu_theme_t*    theme    = lu_sysui_get_theme(lvui->sysui);
lu_gesture_t*  gesture  = lu_sysui_get_gesture(lvui->sysui);
```

5. **设置系统时间与时区**
```c
// 设置系统时间（秒级时间戳）
lu_sysui_set_time(lvui->sysui, time(NULL));

// 设置时区（UTC偏移量，例如东八区为 +8）
lu_sysui_set_timezone(lvui->sysui, 8);
```

6. **添加状态栏图标**
```c
// 动态图标（显示在状态栏左边）
lu_sysui_add_stat(lvui->sysui, LV_SYMBOL_WIFI,      LU_SYSUI_STAT_TYPE_DYNAMIC);
lu_sysui_add_stat(lvui->sysui, LV_SYMBOL_BLUETOOTH, LU_SYSUI_STAT_TYPE_DYNAMIC);

// 常量图标（显示在状态栏右边）
lu_sysui_add_stat(lvui->sysui, LV_SYMBOL_CHARGE,    LU_SYSUI_STAT_TYPE_CONST);
lu_sysui_add_stat(lvui->sysui, LV_SYMBOL_BELL,      LU_SYSUI_STAT_TYPE_CONST);
```

7. **添加启动器应用**
```c
lu_app_info_t app_info = {
    .app_contain = app_container,
    .app_pos     = { .x = 0, .y = 0 },
    .app_size    = LU_APP_SIZE_SMALL,
    .app_icon    = LV_SYMBOL_CALL,
    .app_name    = "Phone",
};
lu_app_t* app = lu_launcher_add_app(launcher, &app_info);
lu_launcher_set_app_cb(app, my_app_callback);
```

8. **添加控制中心按钮**
```c
lu_ctrbtn_info_t btn_info = {
    .ctrbtn_pos  = { .x = 0, .y = 0 },
    .ctrbtn_size = LU_CTRBTN_SIZE_SMALL,
    .ctrbtn_icon = LV_SYMBOL_WIFI,
    .ctrbtn_name = "WiFi",
};
lu_ctrbtn_t* btn = lu_ctrcent_add_ctrbtn(ctrcent, &btn_info);
lu_ctrcent_add_ctrbtn_cb(btn, my_btn_callback, NULL);
```

9. **主题与壁纸**
```c
// 设置明暗模式
lu_theme_set_mode(theme, LU_THEME_MODE_BRIGHT);
// 设置主题色
lu_theme_set_color(theme, lv_color_white());

// 设置锁屏和启动器壁纸
lu_locker_set_wallpaper(locker,   "C:/path/to/wallpaper.bmp");
lu_launcher_set_wallpaper(launcher, "C:/path/to/wallpaper.bmp");
```

10. **资源清理**
```c
// lvui_delete 会自动级联释放所有子资源
lvui_delete(&lvui);
```

### 使用示例模块

项目提供了完整示例 [example/lu_example.c](example/lu_example.c)，一键启动所有UI组件：

```c
#include "lvui/lvui.h"
#include "lvui/example/lu_example.h"

// 一行初始化，包含错误检查和资源清理
lvui_t* lvui = lu_example_init();
if (!lvui) return -1;
```

## 📚 核心模块说明

### 1. 框架核心 (frame/)
- **动画系统** (`anim/`)：提供流畅的UI动画效果
- **显示管理** (`disp/`)：处理显示设备配置、字体管理和渲染
- **输入处理** (`input/`)：统一处理触摸等输入事件
- **主题系统** (`theme/`)：管理UI明暗模式和颜色方案
- **基础组件** (`widget/`)：提供基础UI组件和布局系统

### 2. 系统模块 (sys/)
- **系统UI** (`lu_sysui`)：管理整个系统的UI，作为各组件的总入口
- **系统信息** (`lu_sysinfo`)：管理时间、日期、时区信息，支持UTC偏移设置
- **启动器** (`lu_launcher`)：管理应用图标网格，支持小/中/大三种尺寸
- **控制中心** (`lu_ctrcent`)：管理控制按钮网格，支持回调注册

### 3. 视图模块 (view/)
- **桌面** (`lu_desktop`)：提供应用容器，作为启动器和控制中心的父级
- **锁屏** (`lu_locker`)：提供锁屏界面和解锁功能，支持壁纸背景
- **状态栏** (`lu_statbar`)：显示时间、信号、电量、通知图标
- **控制栏** (`lu_ctrbar`)：提供快捷控制切换功能
- **手势识别** (`lu_gesture`)：支持滑动手势导航

### 4. 工具模块
- **日志系统** (`misc/lu_log`)：通过 `LU_LOG_LVGL` 宏输出到 LVGL 日志系统
- **内存管理** (`plat/lu_mem`)：抽象 `lu_malloc` / `lu_calloc` / `lu_free`，支持 `LU_MEM_STDLIB` 配置切换
- **单链表** (`misc/lu_sll`)：通用单链表数据结构

### 面向对象设计特点

LVUI采用面向对象的设计思想，通过以下方式实现：

- **结构体封装**：使用结构体封装数据和行为，例如 `lvui_t`、`lu_sysui_t` 等
- **模块化设计**：每个模块都是一个独立的"类"，有自己的创建、删除和操作方法
- **接口统一**：通过统一的函数接口操作不同的模块，例如 `lu_sysui_create()`、`lu_sysui_delete()` 等
- **组合模式**：通过指针持有实现模块间的组合关系，级联删除保证资源安全
- **封装性**：内部实现细节被封装，只暴露必要的接口

这种设计使得LVUI具有以下优势：
- **代码组织清晰**：模块化的结构使得代码易于理解和维护
- **可扩展性强**：可以通过添加新的模块或扩展现有模块来增加功能
- **复用性高**：相同的设计模式可以在不同的模块中重复使用
- **易于测试**：每个模块可以独立测试，提高代码质量

## 🔧 配置和自定义

### 编译时配置 (`lvui_conf.h`)

| 宏定义 | 默认值 | 说明 |
|--------|--------|------|
| `LU_LOG_LVGL` | 1 | 通过 LVGL 日志系统输出 lvui 日志 |
| `LU_MEM_STDLIB` | 1 | 使用标准库 malloc/free，设为 0 可替换自定义分配器 |

### 主题配置
```c
lu_theme_t* theme = lu_sysui_get_theme(lvui->sysui);
lu_theme_set_mode(theme, LU_THEME_MODE_BRIGHT);   // 明亮模式
lu_theme_set_mode(theme, LU_THEME_MODE_DARK);     // 暗黑模式
lu_theme_set_color(theme, lv_color_hex(0x1E90FF)); // 自定义主题色
```

### 壁纸配置
```c
// 锁屏壁纸
lu_locker_set_wallpaper(lu_sysui_get_locker(lvui->sysui), "path/to/bg.bmp");
// 启动器壁纸
lu_launcher_set_wallpaper(lu_sysui_get_launcher(lvui->sysui), "path/to/bg.bmp");
```

## 📄 许可证

本项目采用 **GNU General Public License v3.0** 许可证。详情请参阅 [LICENSE](../LICENSE) 文件。

## 📞 联系

- GitHub: [Dreams-Possible/OSUI](https://github.com/Dreams-Possible/OSUI)
- 问题反馈: [GitHub Issues](https://github.com/Dreams-Possible/OSUI/issues)

## 🙏 致谢

- [LVGL](https://lvgl.io/) - 强大的嵌入式图形库
- 所有贡献者和用户

---

**LVUI** - 为嵌入式设备带来现代UI体验
