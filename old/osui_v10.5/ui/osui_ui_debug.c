#include"osui_ui_debug.h"

//控件信息
void osui_ui_debug_info(lv_obj_t* obj);

// //控件信息
// void osui_ui_debug_info(lv_obj_t* obj)
// {
//     osui_log("  Addr: %p", obj);
//     osui_log("  Parent: %p", lv_obj_get_parent(obj));
//     osui_log("  Screen: %p", lv_obj_get_screen(obj));
//     osui_log("  Pos: (%d, %d)", lv_obj_get_x(obj), lv_obj_get_y(obj));
//     osui_log("  Size: %dx%d", lv_obj_get_width(obj), lv_obj_get_height(obj));
//     osui_log("  Hidden: %s", lv_obj_has_flag(obj, LV_OBJ_FLAG_HIDDEN) ? "Yes" : "No");
// }

// 增强的控件调试信息
void osui_ui_debug_info(lv_obj_t* obj)
{
    if (obj == NULL) {
        osui_log("❌ 对象为 NULL");
        return;
    }
    
    
    osui_log("══════════ 控件调试信息 ══════════");
    // 通过检查函数判断对象类型（不依赖内部结构体）
    osui_log("📊 对象类型检查:");
    osui_log("  是按钮: %s", lv_obj_check_type(obj, &lv_button_class) ? "✅" : "❌");
    osui_log("  是标签: %s", lv_obj_check_type(obj, &lv_label_class) ? "✅" : "❌");
    osui_log("  是图片: %s", lv_obj_check_type(obj, &lv_image_class) ? "✅" : "❌");
    osui_log("  是容器: %s", lv_obj_check_type(obj, &lv_obj_class) ? "✅" : "❌");
    
    osui_log("  父对象: 0x%08X", (uint32_t)lv_obj_get_parent(obj));
    osui_log("  屏幕: 0x%08X", (uint32_t)lv_obj_get_screen(obj));
    
    // 位置和大小
    osui_log("📏 几何信息");
    osui_log("  坐标: (%d, %d)", lv_obj_get_x(obj), lv_obj_get_y(obj));
    
    lv_area_t coords;
    lv_obj_get_coords(obj, &coords);
    osui_log("  区域: (%d,%d)-(%d,%d)", 
             coords.x1, coords.y1, coords.x2, coords.y2);
    osui_log("  大小: %dx%d", lv_area_get_width(&coords), lv_area_get_height(&coords));
    
    // 可见性和状态
    osui_log("👀 可见性状态");
    osui_log("  隐藏: %s", lv_obj_has_flag(obj, LV_OBJ_FLAG_HIDDEN) ? "是✅" : "否❌");
    osui_log("  点击: %s", lv_obj_has_flag(obj, LV_OBJ_FLAG_CLICKABLE) ? "是✅" : "否❌");
    osui_log("  滚动: %s", lv_obj_has_flag(obj, LV_OBJ_FLAG_SCROLLABLE) ? "是✅" : "否❌");
    osui_log("  可见: %s", lv_obj_is_visible(obj) ? "是✅" : "否❌");
    
    // 状态
    lv_state_t state = lv_obj_get_state(obj);
    osui_log("🎚️ 状态标志 (0x%04X)", state);
    osui_log("  默认: %s", (state & LV_STATE_DEFAULT) ? "✅" : "❌");
    osui_log("  按下: %s", (state & LV_STATE_PRESSED) ? "✅" : "❌");
    osui_log("  焦点: %s", (state & LV_STATE_FOCUSED) ? "✅" : "❌");
    osui_log("  选中: %s", (state & LV_STATE_CHECKED) ? "✅" : "❌");
    osui_log("  禁用: %s", (state & LV_STATE_DISABLED) ? "✅" : "❌");
    
    // 🌈 颜色信息（最重要的部分）
    osui_log("🎨 颜色信息 (LV_PART_MAIN)");
    
    // 背景色
    lv_color_t bg_color = lv_obj_get_style_bg_color(obj, LV_PART_MAIN);
    lv_opa_t bg_opa = lv_obj_get_style_bg_opa(obj, LV_PART_MAIN);
    osui_log("  背景色: RGB(%3d,%3d,%3d) 透明度: %3d%%", 
             bg_color.red, bg_color.green, bg_color.blue, (bg_opa * 100) / 255);
    
    // 文字色
    lv_color_t text_color = lv_obj_get_style_text_color(obj, LV_PART_MAIN);
    lv_opa_t text_opa = lv_obj_get_style_text_opa(obj, LV_PART_MAIN);
    osui_log("  文字色: RGB(%3d,%3d,%3d) 透明度: %3d%%", 
             text_color.red, text_color.green, text_color.blue, (text_opa * 100) / 255);
    
    // 边框色
    lv_color_t border_color = lv_obj_get_style_border_color(obj, LV_PART_MAIN);
    lv_opa_t border_opa = lv_obj_get_style_border_opa(obj, LV_PART_MAIN);
    lv_coord_t border_width = lv_obj_get_style_border_width(obj, LV_PART_MAIN);
    osui_log("  边框色: RGB(%3d,%3d,%3d) 透明度: %3d%% 宽度: %d", 
             border_color.red, border_color.green, border_color.blue, (border_opa * 100) / 255, border_width);
    
    // 轮廓色
    lv_color_t outline_color = lv_obj_get_style_outline_color(obj, LV_PART_MAIN);
    lv_opa_t outline_opa = lv_obj_get_style_outline_opa(obj, LV_PART_MAIN);
    lv_coord_t outline_width = lv_obj_get_style_outline_width(obj, LV_PART_MAIN);
    osui_log("  轮廓色: RGB(%3d,%3d,%3d) 0x%04X 透明度: %3d%% 宽度: %d", 
             outline_color.red, outline_color.green, outline_color.blue, (outline_opa * 100) / 255, outline_width);
    
    // 阴影色
    lv_color_t shadow_color = lv_obj_get_style_shadow_color(obj, LV_PART_MAIN);
    lv_opa_t shadow_opa = lv_obj_get_style_shadow_opa(obj, LV_PART_MAIN);
    osui_log("  阴影色: RGB(%3d,%3d,%3d) 0x%04X 透明度: %3d%%", 
             shadow_color.red, shadow_color.green, shadow_color.blue, (shadow_opa * 100) / 255);
    
    // 透明度汇总
    osui_log("🎭 透明度汇总");
    osui_log("  整体透明度: %3d%%", (lv_obj_get_style_opa(obj, LV_PART_MAIN) * 100) / 255);
    osui_log("  背景透明度: %3d%%", (bg_opa * 100) / 255);
    osui_log("  边框透明度: %3d%%", (border_opa * 100) / 255);
    osui_log("  轮廓透明度: %3d%%", (outline_opa * 100) / 255);
    osui_log("  阴影透明度: %3d%%", (shadow_opa * 100) / 255);
    osui_log("  文字透明度: %3d%%", (text_opa * 100) / 255);
    
    // 内存和层级
    osui_log("🧠 内存和层级");
    osui_log("  子对象数: %d", lv_obj_get_child_cnt(obj));
    osui_log("  索引位置: %d", lv_obj_get_index(obj));
    
    // 检查父对象透明度
    lv_obj_t *parent = lv_obj_get_parent(obj);
    if (parent) {
        lv_opa_t parent_opa = lv_obj_get_style_opa(parent, LV_PART_MAIN);
        lv_opa_t parent_bg_opa = lv_obj_get_style_bg_opa(parent, LV_PART_MAIN);
        osui_log("👨‍👦 父对象透明度");
        osui_log("  父对象整体: %3d%%", (parent_opa * 100) / 255);
        osui_log("  父对象背景: %3d%%", (parent_bg_opa * 100) / 255);
    }
    
    osui_log("══════════ 调试结束 ══════════");
    
    // // 强制重绘以便立即看到变化
    // lv_obj_invalidate(obj);
}