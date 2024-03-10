#pragma once
#include <cstdint>
#include <vector>
#include <functional>
#include <string>
#include "imgui.h"

struct ImFont;
struct ImVec4;
struct ImVec2;

#define XorStr(s) s
#define _(s) XorStr(s)
#define OPTION(type, var, val) type var = val

extern int scrrenWidth;
extern int scrrenHeight;

extern ImGuiID Colorpicker_Close;

extern ImFont* menufont;
extern ImFont* menufont_bold;
extern ImFont* tabfont;
extern ImFont* spacerfont;
extern ImFont* weap_icons;
extern ImFont* weap_icons_menu_size;
extern ImFont* espfont;
extern ImFont* logofont;
extern ImFont* infocorrner_text;

////// Colors ////////
extern ImVec4 button;
extern ImVec4 buttonhovered;
extern ImVec4 buttonActive;
extern ImVec4 text;
extern ImVec4 bg_color;
extern ImVec4 themecolor;
extern ImVec4 bg_child_color;
extern ImVec4 separator;
extern ImVec4 slider;

extern bool color_picker_open;

class Color
{
public:
    Color();
    Color(int _r, int _g, int _b);
    Color(int _r, int _g, int _b, int _a);
    Color(float _r, float _g, float _b) : Color(_r, _g, _b, 1.0f) {}
    Color(float _r, float _g, float _b, float _a)
        : Color(
            static_cast<int>(_r * 255.0f),
            static_cast<int>(_g * 255.0f),
            static_cast<int>(_b * 255.0f),
            static_cast<int>(_a * 255.0f))
    {
    }
    explicit Color(float* rgb) : Color(rgb[0], rgb[1], rgb[2], 1.0f) {}
    explicit Color(unsigned long argb)
    {
        _CColor[2] = (unsigned char)((argb & 0x000000FF) >> (0 * 8));
        _CColor[1] = (unsigned char)((argb & 0x0000FF00) >> (1 * 8));
        _CColor[0] = (unsigned char)((argb & 0x00FF0000) >> (2 * 8));
        _CColor[3] = (unsigned char)((argb & 0xFF000000) >> (3 * 8));
    }

    void    SetRawColor(int color32);
    int     GetRawColor() const;
    void    SetColor(int _r, int _g, int _b, int _a = 0);
    void    SetColor(float _r, float _g, float _b, float _a = 0);
    void    GetColor(int &_r, int &_g, int &_b, int &_a) const;

    int r() const { return _CColor[0]; }
    int g() const { return _CColor[1]; }
    int b() const { return _CColor[2]; }
    int a() const { return _CColor[3]; }

    unsigned char &operator[](int index)
    {
        return _CColor[index];
    }
    const unsigned char &operator[](int index) const
    {
        return _CColor[index];
    }

    bool operator==(const Color &rhs) const;
    bool operator!=(const Color &rhs) const;
    Color &operator=(const Color &rhs);

    static Color Black;
    static Color White;
    static Color Red;
    static Color Green;
    static Color Blue;
    static Color Yellow;

private:
    unsigned char _CColor[4];
};

enum LogType : int
{
    LOGTYPE_NONE,
    LOGTYPE_FIX,
    LOGTYPE_ADD,
    LOGTYPE_REMOVE,
    LOGTYPE_CHANGED,
    LOGTYPE_REMADE,
    LOGTYPE_UPDATE
};

class Config
{
public:
    OPTION(Color, color_theme, Color(0, 131, 238)); //ImVec4(0.81f,0.09f,0.54f,1.f)
    OPTION(Color, color_bg, Color(240, 240, 240)); //ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
    OPTION(Color, color_bg_child, Color(194, 194, 194));	//ImVec4(0.76f, 0.76f, 0.76f, 1.f);
    OPTION(Color, color_text, Color(0, 0, 0));
    OPTION(int, windows_grab_area, 15);
    OPTION(bool, esp_all_enabled, true);
    OPTION(bool, esp_enemies_only, false);
    OPTION(bool, esp_player_boxes, false);
    OPTION(bool, esp_player_boxes_fill, false);
    OPTION(bool, esp_player_names, false);
    OPTION(bool, esp_player_health, false);
    OPTION(bool, esp_player_skeleton, false);
    OPTION(Color, esp_player_skeleton_color_ally_visible, Color(255, 255, 255, 255));
    OPTION(Color, esp_player_skeleton_color_enemy_visible, Color(255, 255, 255, 255));
    OPTION(Color, esp_player_skeleton_color_ally_occluded, Color(255, 255, 255, 255));
    OPTION(Color, esp_player_skeleton_color_enemy_occluded, Color(255, 255, 255, 255));
    OPTION(int, esp_player_type_health, 3);
    OPTION(bool, esp_backtracked_player_skelet, false);
    OPTION(bool, esp_player_armour, false);
    OPTION(int, esp_player_type_armour, 1);
    OPTION(bool, esp_player_weapons, false);
    OPTION(bool, esp_player_weap_icons, false);
    OPTION(bool, esp_player_snaplines, false);
    OPTION(bool, esp_crosshair, false);
    OPTION(bool, esp_recoil_crosshair, false);
    OPTION(bool, esp_dropped_weapons, false);
    OPTION(bool, esp_dropped_weap_ammo, false);
    OPTION(int, esp_dropped_type_weapons, 0);
    OPTION(int, esp_dropped_style_weapons, 0);
    OPTION(bool, esp_dropped_anim_style, false);
    OPTION(bool, esp_dropped_anim_type, false);

    OPTION(bool, esp_item_esp, false);
    OPTION(bool, esp_defuse_kit, false);
    OPTION(bool, esp_planted_c4, false);
    OPTION(bool, esp_c4_damage, false);
    OPTION(bool, esp_c4_timer, false);
    OPTION(Color, esp_c4_timer_defuse_has_time, Color(20, 245, 245, 255)); //what a long name huh?
    OPTION(Color, esp_c4_timer_defuse_no_time, Color(245, 10, 12, 255)); //lemee sleep dude :'D
    OPTION(Color, esp_c4_timer_defuse_bg, Color(0, 0, 0, 255));
    OPTION(Color, esp_c4_timer_defuse_bg_outline, Color(0, 0, 0, 255));
    OPTION(Color, esp_c4_timer_bomb_slider, Color(10, 245, 20, 255));
    OPTION(Color, esp_c4_timer_bomb_slider_bg, Color(245, 10, 12, 255));
    OPTION(Color, esp_c4_timer_bomb_slider_outline, Color(0, 0, 0, 255));
    OPTION(bool, esp_player_head_pos_box, false);
    OPTION(Color, esp_player_head_pos_box_color_ally_visible, Color(255, 255, 255, 255));
    OPTION(Color, esp_player_head_pos_box_color_enemy_visible, Color(255, 255, 255, 255));
    OPTION(Color, esp_player_head_pos_box_color_ally_occluded, Color(255, 255, 255, 255));
    OPTION(Color, esp_player_head_pos_box_color_enemy_occluded, Color(255, 255, 255, 255));
    OPTION(bool, dmg_indicator, false);
    OPTION(bool, esp_BulletTrace, false);
    OPTION(float, esp_BulletTrace_offset, 0.f);
    OPTION(bool, esp_BulletTrace_showEnemy, false);
    OPTION(bool, esp_player_resolvemode, false);
    OPTION(bool, esp_others_grenade_pred, false);
    OPTION(bool, esp_others_grenade_other_predict, false);
    OPTION(bool, team_esp_others_grenade_other_predict, false);
    OPTION(bool, esp_sound_esp, false);
    OPTION(bool, esp_lagcompensated_showhitbox, false);
    OPTION(int, esp_lagcompensated_hitboxes_type, 0);
    OPTION(bool, esp_play_out_of_view, false);
    OPTION(bool, esp_play_out_of_view_circle, false);
    OPTION(int, esp_play_out_of_view_size, 259);
    OPTION(int, esp_play_out_of_view_arrows_size, 6);
    OPTION(bool, esp_nade, false);
    OPTION(int, esp_type_nade, 0);
    OPTION(bool, esp_nade_timer, false);
    OPTION(Color, esp_nade_color_team, Color(0, 255, 0, 255));
    OPTION(Color, esp_nade_color_enemy, Color(255, 0, 0, 255));
    OPTION(int, esp_spreadCrosshair, 0);
    OPTION(int, esp_cross_type, 0);
    OPTION(Color, esp_text_color, Color(255, 255, 255, 255));
    OPTION(bool, esp_weapon_clip, false);
    OPTION(bool, all_chams_enabled, true);
    OPTION(bool, all_glow_enabled, true);
    OPTION(bool, esp_awp_crosshair, false);
    // 
    // GLOW
    // 
    OPTION(bool, glow_enabled, false);
    OPTION(bool, glow_enemies_only, false);
    OPTION(bool, glow_players, false);
    OPTION(int, glow_players_style, 0);
    OPTION(int, glow_others_style, 0);
    OPTION(bool, glow_chickens, false);
    OPTION(bool, glow_c4_carrier, false);
    OPTION(bool, glow_planted_c4, false);
    OPTION(bool, glow_defuse_kits, false);
    OPTION(bool, glow_weapons, false);

    //
    // CHAMS
    //
    OPTION(bool, chams_player_enabled, false);
    OPTION(int, esp_player_chams_type, 0);
    OPTION(int, esp_player_chams_type_hands, 0);
    OPTION(int, esp_player_chams_type_weapons, 0);
    OPTION(bool, chams_player_enemies_only, false);
    OPTION(bool, chams_player_ignorez, false);
    OPTION(bool, chams_arms_enabled, false);
    OPTION(bool, chams_arms_wireframe, false);
    OPTION(bool, chams_arms_ignorez, false);
    OPTION(bool, chams_weapon_enabled, false);
    OPTION(bool, chams_weapon_wireframe, false);
    OPTION(bool, chams_weapon_ignorez, false);

    OPTION(bool, chams_glow, false);
    OPTION(float, chams__glow_exp, 10.f);
    OPTION(bool, chams_pearlescent_enabled_hands, false);
    OPTION(float, chams_pearlescent_ammount_hands, 0.f);
    OPTION(bool, chams_pearlescent_enabled_player, false);
    OPTION(float, chams_pearlescent_ammount_player, 0.f);
    OPTION(bool, chams_pearlescent_enabled_weapon, false);
    OPTION(float, chams_pearlescent_ammount_weapon, 0.f);
    OPTION(bool, chams_far_model, false);
    OPTION(bool, chams_hands_glow_enabled, false);
    OPTION(float, chams_hands_glow_exp, 10.f);
    OPTION(Color, color_chams_glow_hands_visible, Color(140, 255, 97));
    OPTION(Color, color_chams_glow_hands_occluded, Color(158, 21, 255));
    OPTION(bool, chams_weapon_glow, false);
    OPTION(float, chams_weap_glow_exp, 10.f);
    OPTION(Color, color_chams_glow_weap, Color(140, 255, 97));
    OPTION(Color, color_chams_glow_player_ally_visible, Color(140, 255, 97));
    OPTION(Color, color_chams_glow_player_ally_occluded, Color(158, 21, 255));
    OPTION(Color, color_chams_glow_player_enemy_visible, Color(255, 97, 97));
    OPTION(Color, color_chams_glow_player_enemy_occluded, Color(76, 252, 255));
    OPTION(Color, sound_esp_color, Color(255, 255, 255, 255));
    OPTION(Color, sound_esp_teamcolor, Color(255, 255, 255, 255));
    OPTION(float, sound_esp_speed, 1.5f);
    OPTION(float, sound_esp_width, 2.f);
    OPTION(float, sound_esp_lenght, 1000.f);
    OPTION(float, sound_esp_distortion, 0.f);
    OPTION(bool, fps_remove_fog, false);
    OPTION(bool, fps_remove_3d_sky, false);
    OPTION(bool, fps_remove_shadows, false);
    OPTION(bool, fps_remove_grass, false);
    OPTION(bool, fps_remove_blur, false);
    OPTION(bool, esp_sound_only, false);
    //
    // Visuals removals
    //
    OPTION(bool, hitmarker, false);
    OPTION(bool, hitmarker_health_shot_effect, false);
    OPTION(bool, hitmarker_health_shot_hs_only, false);
    OPTION(bool, nightmode, false);
    OPTION(bool, watermark, true);
    OPTION(bool, info_corrner, true);
    OPTION(bool, backtrackchams, false);
    OPTION(Color, backtrack_chams_color_A, Color(255, 0, 0, 255));
    OPTION(Color, backtrack_chams_color_B, Color(0, 255, 0, 0));
    OPTION(bool, colored_walls, false);
    OPTION(bool, colored_sky, false);
    OPTION(Color, colored_walls_color, Color(0, 0, 0));
    OPTION(Color, colored_props_color, Color(0, 0, 0));
    OPTION(Color, colored_sky_color, Color(0, 0, 0));
    OPTION(bool, rage_novisual_recoil, false);
    OPTION(bool, misc_no_hands, false);
    OPTION(bool, removals_visualRecoil, false);
    OPTION(bool, removals_smoke, false);
    OPTION(bool, removals_scope, false);
    OPTION(bool, removals_scope_zoom, false);
    OPTION(bool, removals_flash, false);
    OPTION(bool, removals_postprocessing, false);
    OPTION(Color, esp_grenade_predict_color, Color(255, 255, 0, 255));
    OPTION(Color, esp_grenade_predict_hit_color, Color(255, 0, 0, 255));

    OPTION(bool, OBS_Proof, false);
    OPTION(bool, shared_esp_master, false);
    OPTION(bool, esp_animated_hp_bar, false);

    OPTION(bool, esp_render_flags, false);
    OPTION(bool, esp_barel_esp, false);
    OPTION(bool, esp_barel_hide_visible, false);
    OPTION(Color, esp_barel_color_visible, Color(0, 255, 0));
    OPTION(Color, esp_barel_color_invisible, Color(255, 0, 0));
    OPTION(int, skybox_texture, 0);
    OPTION(Color, esp_health_bar_color_A, Color(128, 212, 75, 255));
    OPTION(Color, esp_health_bar_color_B, Color(253, 6, 75, 255));
    OPTION(int, viewmodel_fov, 68);
    OPTION(float, viewmodel_view_model_x, 2);
    OPTION(float, viewmodel_view_model_y, 2);
    OPTION(float, viewmodel_view_model_z, -2);
    OPTION(float, player_fov, 86);
    OPTION(float, mat_ambient_light_r, 0.0f);
    OPTION(float, mat_ambient_light_g, 0.0f);
    OPTION(float, mat_ambient_light_b, 0.0f);
    OPTION(float, aspect_ratio, 0.f);
    OPTION(int, hitmarker_sound, 0);
    OPTION(int, hitmarker_size, 5);
    OPTION(Color, color_esp_ally_visible, Color(255, 255, 255, 150));
    OPTION(Color, color_esp_enemy_visible, Color(255, 255, 255, 150));
    OPTION(Color, color_esp_ally_occluded, Color(255, 255, 255, 150));
    OPTION(Color, color_esp_enemy_occluded, Color(255, 255, 255, 150));
    OPTION(Color, color_esp_ally_visible_filled, Color(140, 255, 97, 100));
    OPTION(Color, color_esp_enemy_visible_filled, Color(255, 97, 97, 100));
    OPTION(Color, color_esp_ally_occluded_filled, Color(158, 21, 255, 100));
    OPTION(Color, color_esp_enemy_occluded_filled, Color(76, 252, 255, 100));
    OPTION(Color, color_mapoverview_ally_visible, Color(10, 237, 233));
    OPTION(Color, color_mapoverview_enemy_visible, Color(242, 14, 14));
    OPTION(Color, color_mapoverview_ally_occluded, Color(1, 102, 229));
    OPTION(Color, color_mapoverview_enemy_occluded, Color(165, 0, 0));
    OPTION(Color, color_esp_crosshair, Color(255, 255, 255));
    OPTION(Color, color_spread_crosshair, Color(255, 255, 255));
    OPTION(Color, color_esp_weapons, Color(255, 255, 255));
    OPTION(Color, color_esp_loot_boxes, Color(255, 255, 255));
    OPTION(Color, color_esp_defuse, Color(0, 128, 255));
    OPTION(Color, color_esp_c4, Color(255, 255, 0));

    OPTION(Color, color_glow_ally, Color(140, 255, 97));
    OPTION(Color, color_glow_enemy, Color(255, 97, 97));
    OPTION(Color, color_glow_chickens, Color(108, 52, 52));
    OPTION(Color, color_glow_c4_carrier, Color(255, 255, 0));
    OPTION(Color, color_glow_planted_c4, Color(128, 0, 128));
    OPTION(Color, color_glow_defuse, Color(0, 128, 255));
    OPTION(Color, color_glow_weapons, Color(255, 255, 255));

    OPTION(Color, color_chams_player_ally_visible, Color(140, 255, 97));
    OPTION(Color, color_chams_player_ally_occluded, Color(158, 21, 255));
    OPTION(Color, color_chams_player_enemy_visible, Color(255, 97, 97));
    OPTION(Color, color_chams_player_enemy_occluded, Color(76, 252, 255));
    OPTION(Color, color_chams_arms_visible, Color(0, 128, 255));
    OPTION(Color, color_chams_arms_occluded, Color(255, 221, 10));
    OPTION(Color, color_bullettrace, Color(2, 255, 44, 255));
    OPTION(Color, color_bullettrace_enemy, Color(255, 0, 0, 255));
    OPTION(Color, color_enemy_player_view_localplayer_color, Color(10, 237, 233));
    OPTION(Color, color_esp_pov_arrows, Color(0, 128, 255));
    OPTION(Color, color_chams_weapon, Color(255, 255, 255));
    OPTION(Color, color_chams_player_metalic, Color(255, 255, 255));
};

extern Config g_Options;

ImVec4 Color_to_vec(Color& color);

#pragma warning( push )
#pragma warning( disable : 4806)
namespace ImGuiEx
{
    inline bool ColorEdit4(const char* label, Color* v, bool show_alpha = true)
    {
        auto clr = ImVec4{
            v->r() / 255.0f,
            v->g() / 255.0f,
            v->b() / 255.0f,
            v->a() / 255.0f
        };

        if (ImGui::ColorEdit4(label, &clr.x, show_alpha | ImGuiColorEditFlags_NoInputs)) { //32
            v->SetColor(clr.x, clr.y, clr.z, clr.w);
            return true;
        }
        return false;
    }
    inline bool ColorEdit3(const char* label, Color* v)
    {
        return ColorEdit4(label, v, false);
    }
}
#pragma warning( pop ) 

namespace ImGui
{
    bool Combo(const char* label, int* currIndex, std::vector<std::string>& values);
    
    bool ListBox(const char* label, int* currIndex, std::vector<std::string>& values, int height_in_items = -1);
   
    bool ListBox(const char* label, int* current_item, std::function<const char* (int)> lambda, int items_count, int height_in_items);

    bool LabelClick(const char* concatoff, const char* concaton, const char* label, bool* v, const char* unique_id);
   
    //void KeyBindButton(ButtonCode_t* key);
   
    void TextContextSelection(const char* concatoff, int* selected, const char* items[], int count);

    void TextContextSelection(const char* concatoff, int* selected, const char* items[], int count, const char* names[], bool* toggles[]);

    void TextContextSelection(const char* concatoff, bool* selected, const char* items[]);

    void TextContextSelection(const char* concatoff, bool* selected, const char* items[], const char* names[], bool* toggles[]);
    // This can be used anywhere, in group boxes etc.
    void SelectTabs(int* selected, const char* items[], int item_count, ImVec2 size = ImVec2(0.f, 0.f));

    int WarningMessage(std::string Message, std::string Header);
    bool Hotkey ( const char* label, int* k, const ImVec2& size_arg );

    struct warnMessage
    {
        const char* item_label;
        int answer;
        bool clicked;
    };
  
    bool Checkbox_warning(const char* label, bool* v, std::string Message, std::string Header = "WARNING!");
}

ImVec4 Color_to_vec(Color& color);

void ColorPickerBoxPlayer(const char* picker_idname, Color* team/*ct*/, Color* enemy/*t*/, Color* team_invis/*ct invis*/, Color* enemy_invis/*t invis*/, bool alpha = true);

void ColorPickerBoxPlayer(const char* picker_idname, Color* team/*ct*/, Color* enemy/*t*/, bool alpha = true, bool player_or_visible = true);

void ColorPickerBoxCustom(const char* picker_idname, Color* col1/*ct*/, std::string name1, bool alpha = true, bool Color_or_blackandwhite = true);

void ColorPickerBoxCustom(const char* picker_idname, Color* col1/*ct*/, Color* col2/*t*/, std::string name1, std::string name2, bool alpha = true, bool Color_or_blackandwhite = true);

void ColorPickerBox(const char* picker_idname, Color* color, bool alpha = true);

void ColorPickerBox(const char* picker_idname, float color[]);

void AddFixLog(const char* version, const char* _text, LogType type);

void AddChatMessage(int title, int uid, std::string name, std::string message);

template<size_t N>
void render_tabs(const char* (&names)[N], int& activetab, float w, float h, bool sameline)
{
    ImGui::PushFont(tabfont);
    auto& style = ImGui::GetStyle();
    bool values[N] = { false };

    values[activetab] = true;
    style.Colors[ImGuiCol_Button] = ImVec4(0.21f, 0.21f, 0.21f, 1.f);
    for (auto i = 0; i < N; ++i) {
        if (activetab == i)
        {
            style.Colors[ImGuiCol_Text] = ImVec4(themecolor.x, themecolor.y, themecolor.z, themecolor.w);
            //style.Colors[ImGuiCol_Button] = buttonActive;
        }
        else
        {
            style.Colors[ImGuiCol_Text] = ImVec4(1.f, 1.f, 1.f, 1.f);
            //style.Colors[ImGuiCol_Button] = button;
        }
        if (ImGui::Button(names[i], ImVec2{ w, h })) {
            activetab = i;
        }
        if (sameline && i < N - 1)
            ImGui::SameLine();
    }
    style.Colors[ImGuiCol_Separator] = separator;
    style.Colors[ImGuiCol_Text] = text;
    ImGui::PopFont();
}
;
template<size_t N>
void renderweap_tabs(char* (&names)[N], int& activetab, float w, float h, bool sameline, ImVec2 dummysize)
{
    auto& style = ImGui::GetStyle();
    bool values[N] = { false };

    values[activetab] = true;
    style.Colors[ImGuiCol_Button] = ImVec4(bg_color.x - 0.05f, bg_color.y - 0.05f, bg_color.z - 0.05f, bg_color.w);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(bg_color.x + 0.05f, bg_color.y + 0.05f, bg_color.z + 0.05f, bg_color.w);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(bg_color.x + 0.07f, bg_color.y + 0.07f, bg_color.z + 0.07f, bg_color.w);
    ImGui::PushFont(weap_icons);
    for (auto i = 0; i < N; ++i) {
        if (dummysize.x > 0.f || dummysize.y > 0.f)
        {
            ImGui::Dummy(dummysize);
            ImGui::SameLine();
        }

        if (activetab == i)
        {
            style.Colors[ImGuiCol_Text] = ImVec4(themecolor.x, themecolor.y, themecolor.z, themecolor.w);
            style.Colors[ImGuiCol_Separator] = themecolor;
            //style.Colors[ImGuiCol_Button] = buttonActive;
        }
        else
        {
            style.Colors[ImGuiCol_Separator] = separator;
            style.Colors[ImGuiCol_Text] = ImVec4(0.70f, 0.70f, 0.70f, 1.f);
            //style.Colors[ImGuiCol_Button] = button;
        }
        if (ImGui::ButtonMenu(names[i], ImVec2{ w, h })) {
            activetab = i;
        }
        if (sameline && i < N - 1)
            ImGui::SameLine();
    }
    style.Colors[ImGuiCol_Separator] = separator;
    style.Colors[ImGuiCol_Button] = button;
    style.Colors[ImGuiCol_Text] = text;
    style.Colors[ImGuiCol_ButtonHovered] = buttonhovered;
    style.Colors[ImGuiCol_ButtonActive] = buttonActive;
    ImGui::PopFont();
}
;
template<size_t N>
void renderMENU_tabs(const char* (&names)[N], int& activetab, float w, float h, bool sameline)
{
    ImGui::PushFont(tabfont);
    auto& style = ImGui::GetStyle();
    bool values[N] = { false };

    values[activetab] = true;
    style.Colors[ImGuiCol_Button] = ImVec4(0.21f, 0.21f, 0.21f, 1.f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.12f + 0.05f, 0.12f + 0.05f, 0.12f + 0.05f, 1.f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.12f + 0.07f, 0.12 + 0.07f, 0.12f + 0.07f, 1.f);
    for (auto i = 0; i < N; ++i) {
        if (activetab == i)
        {
            style.Colors[ImGuiCol_Text] = ImVec4(themecolor.x, themecolor.y, themecolor.z, themecolor.w);
            style.Colors[ImGuiCol_Separator] = themecolor;
            //style.Colors[ImGuiCol_Button] = buttonActive;
        }
        else
        {
            style.Colors[ImGuiCol_Separator] = separator;
            style.Colors[ImGuiCol_Text] = ImVec4(1.f, 1.f, 1.f, 1.f);
            //style.Colors[ImGuiCol_Button] = button;
        }
        if (ImGui::ButtonMenu(names[i], ImVec2{ w, h })) {
            activetab = i;
        }
        if (sameline && i < N - 1)
            ImGui::SameLine();
    }
    style.Colors[ImGuiCol_Separator] = separator;
    style.Colors[ImGuiCol_Text] = text;
    ImGui::PopFont();
}
;
template<size_t N>
void renderOther_tabs(const char* (&names)[N], int& activetab, float w, float h, bool sameline)
{
    ImGui::PushFont(tabfont);
    auto& style = ImGui::GetStyle();
    bool values[N] = { false };

    values[activetab] = true;
    style.Colors[ImGuiCol_Button] = ImVec4(bg_color.x - 0.05f, bg_color.y - 0.05f, bg_color.z - 0.05f, bg_color.w);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(bg_color.x + 0.05f, bg_color.y + 0.05f, bg_color.z + 0.05f, bg_color.w);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(bg_color.x + 0.07f, bg_color.y + 0.07f, bg_color.z + 0.07f, bg_color.w);
    for (auto i = 0; i < N; ++i) {
        if (activetab == i)
        {
            style.Colors[ImGuiCol_Text] = ImVec4(themecolor.x, themecolor.y, themecolor.z, themecolor.w);
            style.Colors[ImGuiCol_Separator] = themecolor;
            //style.Colors[ImGuiCol_Button] = buttonActive;
        }
        else
        {
            style.Colors[ImGuiCol_Separator] = separator;
            style.Colors[ImGuiCol_Text] = ImVec4(0.70f, 0.70f, 0.70f, 1.f);
            //style.Colors[ImGuiCol_Button] = button;
        }
        if (ImGui::ButtonMenu(names[i], ImVec2{ w, h })) {
            activetab = i;
        }
        if (sameline && i < N - 1)
            ImGui::SameLine();
    }
    style.Colors[ImGuiCol_Separator] = separator;
    style.Colors[ImGuiCol_Button] = button;
    style.Colors[ImGuiCol_Text] = text;
    style.Colors[ImGuiCol_ButtonHovered] = buttonhovered;
    style.Colors[ImGuiCol_ButtonActive] = buttonActive;
    ImGui::PopFont();
}
