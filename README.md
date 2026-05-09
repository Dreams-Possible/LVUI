# LVUI - 基于LVGL的轻量级操作系统UI框架

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![LVGL](https://img.shields.io/badge/Built%20with-LVGL-green.svg)](https://lvgl.io/)

LVUI是一个基于LVGL的轻量级操作系统UI框架，专为嵌入式系统和资源受限设备设计，提供现代、流畅的用户界面体验。

<img width="400" height="533" alt="202605092121 (1)" src="https://github.com/user-attachments/assets/a12b2990-203a-4691-b34a-5bafa94ddf0d" />

> **⚠️ 重要提示**
>
> 本项目当前处于**开发阶段**，API可能会有变更，请在使用时注意版本兼容性。

## ✨ 特性

- **现代UI设计**：模拟现代移动操作系统的界面风格，提供流畅的用户体验
- **模块化架构**：清晰的模块划分，易于扩展和维护
- **面向对象设计**：采用面向对象的设计思想，使用结构体和函数指针模拟类的概念，提高代码的可维护性和可扩展性
- **完整UI组件**：包含桌面、锁屏、状态栏、控制中心等完整UI组件
- **动画支持**：内置动画系统，提供流畅的UI交互效果
- **主题系统**：支持自定义主题和颜色方案
- **手势支持**：内置手势识别系统，支持多种手势操作
- **系统级API**：提供类系统API，可自定义时间、状态等系统元素
- **跨平台**：基于LVGL，支持多种显示设备和输入设备
- **轻量级**：优化的资源占用，适合资源受限的嵌入式设备

## 📁 项目结构

```
LVUI/
├── frame/             # 框架核心模块
│   ├── anim/          # 动画系统
│   ├── disp/          # 显示管理
│   ├── input/         # 输入处理
│   ├── theme/         # 主题系统
│   └── widget/        # 基础组件
├── misc/              # 杂项功能
│   ├── lu_log.c/h     # 日志系统
│   ├── lu_pdc.c/h     # PDC功能
│   └── lu_sll.c/h     # 单链表实现
├── plat/              # 平台相关
│   ├── lu_lvgl.h      # LVGL接口
│   ├── lu_mem.c/h     # 内存管理
│   └── lu_type.h      # 类型定义
├── sys/               # 系统模块
│   ├── lu_ctrcent.c/h # 控制中心
│   ├── lu_launcher.c/h # 启动器
│   ├── lu_sysinfo.c/h # 系统信息
│   ├── lu_sysui.c/h   # 系统UI
│   └── lu_sysui_stat.c/h # 系统状态
├── view/              # 视图模块
│   ├── lu_ctrbar.c/h  # 控制栏
│   ├── lu_desktop.c/h # 桌面
│   ├── lu_gesture.c/h # 手势识别
│   ├── lu_locker.c/h  # 锁屏
│   └── lu_statbar.c/h # 状态栏
├── lu_init.c/h        # 初始化模块
├── lvui.c/h           # 核心接口
└── README.md          # 项目说明
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

2. **初始化LVUI**
```c
lu_init();
```

3. **创建LVUI实例**
```c
// 创建LVUI实例，指定宽度、高度和尺寸因子
lvui_t* lvui = lvui_create(800, 480, 3.5);
```

4. **获取系统UI**
```c
lu_sysui_t* sysui = lvui->sysui;
```

5. **设置系统时间**
```c
// 设置系统时间（毫秒时间戳）
lu_sysui_set_time(sysui, time(NULL) * 1000);
```

6. **添加系统状态图标**
```c
// 添加常量状态图标
lu_sysui_add_stat(sysui, "wifi", LU_SYSUI_STAT_TYPE_CONST);

// 添加动态状态图标
lu_sysui_add_stat(sysui, "battery", LU_SYSUI_STAT_TYPE_DYNAMIC);
```

## 📚 核心模块说明

### 1. 框架核心 (frame/)
- **动画系统**：提供流畅的UI动画效果
- **显示管理**：处理显示设备配置和管理
- **输入处理**：统一处理触摸等输入事件
- **主题系统**：管理UI主题和颜色方案
- **基础组件**：提供基础UI组件和布局系统

### 面向对象设计特点

LVUI采用面向对象的设计思想，通过以下方式实现：

- **结构体封装**：使用结构体封装数据和行为，例如 `lvui_t`、`lu_sysui_t` 等
- **模块化设计**：每个模块都是一个独立的"类"，有自己的创建、删除和操作方法
- **接口统一**：通过统一的函数接口操作不同的模块，例如 `lu_sysui_create()`、`lu_sysui_delete()` 等
- **继承与组合**：通过结构体嵌套实现类似继承和组合的效果
- **封装性**：内部实现细节被封装，只暴露必要的接口

这种设计使得LVUI具有以下优势：
- **代码组织清晰**：模块化的结构使得代码易于理解和维护
- **可扩展性强**：可以通过添加新的模块或扩展现有模块来增加功能
- **复用性高**：相同的设计模式可以在不同的模块中重复使用
- **易于测试**：每个模块可以独立测试，提高代码质量

### 2. 系统模块 (sys/)
- **控制中心**：提供系统控制功能
- **启动器**：管理应用启动
- **系统信息**：提供系统信息管理
- **系统UI**：管理整个系统的UI
- **系统状态**：管理系统状态图标

### 2. 系统模块 (sys/)
- **控制中心**：提供系统控制功能
- **启动器**：管理应用启动
- **系统信息**：提供系统信息管理
- **系统UI**：管理整个系统的UI
- **系统状态**：管理系统状态图标

### 3. 视图模块 (view/)
- **桌面**：提供应用图标网格、壁纸、时间显示
- **锁屏**：提供锁屏界面和解锁功能
- **状态栏**：显示信号、电量、时间、通知图标
- **控制栏**：提供快捷控制功能
- **手势识别**：支持多种手势操作

### 4. 杂项功能 (misc/)
- **日志系统**：提供调试和日志功能
- **PDC功能**：提供PDC相关功能
- **单链表**：提供单链表数据结构

## 🔧 配置和自定义

### 主题配置
```c
// 获取主题
lu_theme_t* theme = lu_sysui_get_theme(sysui);

// 自定义主题颜色和样式
// ...
```

### 手势配置
```c
// 获取手势系统
lu_gesture_t* gesture = lu_sysui_get_gesture(sysui);

// 配置手势识别
// ...
```

### 锁屏配置
```c
// 获取锁屏
lu_locker_t* locker = lu_sysui_get_locker(sysui);

// 配置锁屏功能
// ...
```

## 📱 应用开发

### 集成到现有项目
1. 将LVUI源代码添加到项目
2. 配置LVGL依赖
3. 包含必要的头文件
4. 调用初始化函数

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
