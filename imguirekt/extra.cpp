#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_internal.h"
#include "extra.h"
#include <d3d11.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <tchar.h>
#include <stdint.h>
#include <vector>
#include <sstream>
#include <string>

#include <chrono>
#include <future>
#include <algorithm>
#include <fstream>
#include <cstring>


#define get_keycode_name(vk, name)\
switch (vk) {\
case VK_CONTROL: name = _("control"); break;\
case VK_SHIFT: name = _("shift"); break;\
case VK_LSHIFT: name = _("shift"); break;\
case VK_MENU: name = _("alt"); break;\
case VK_TAB: name = _("tab"); break;\
case VK_LBUTTON: name = _("Mouse 1"); break;\
case VK_RBUTTON: name = _("Mouse 2"); break;\
case VK_MBUTTON: name = _("Mouse 3"); break;\
case VK_XBUTTON1: name = _("Mouse 4"); break;\
case VK_XBUTTON2: name = _("Mouse 5"); break;\
case VK_PRIOR: name = _("page up"); break;\
case VK_NEXT: name = _("page down"); break;\
case VK_END: name = _("end"); break;\
case VK_HOME: name = _("home"); break;\
case VK_LEFT: name = _("left arrow"); break;\
case VK_UP: name = _("up arrow"); break;\
case VK_RIGHT: name = _("right arrow"); break;\
case VK_DOWN: name = _("down arrow"); break;\
case VK_INSERT: name = _("insert"); break;\
case VK_DELETE: name = _("delete"); break;\
case 'A': name = _("a"); break;\
case 'B': name = _("b"); break;\
case 'C': name = _("c"); break;\
case 'D': name = _("d"); break;\
case 'E': name = _("e"); break;\
case 'F': name = _("f"); break;\
case 'G': name = _("g"); break;\
case 'H': name = _("h"); break;\
case 'I': name = _("i"); break;\
case 'J': name = _("j"); break;\
case 'K': name = _("k"); break;\
case 'L': name = _("l"); break;\
case 'M': name = _("m"); break;\
case 'N': name = _("n"); break;\
case 'O': name = _("o"); break;\
case 'P': name = _("p"); break;\
case 'Q': name = _("q"); break;\
case 'R': name = _("r"); break;\
case 'S': name = _("s"); break;\
case 'T': name = _("t"); break;\
case 'U': name = _("u"); break;\
case 'V': name = _("v"); break;\
case 'W': name = _("w"); break;\
case 'X': name = _("x"); break;\
case 'Y': name = _("y"); break;\
case 'Z': name = _("z"); break;\
case VK_NUMPAD0: name = _("numpad 0"); break;\
case VK_NUMPAD1: name = _("numpad 1"); break;\
case VK_NUMPAD2: name = _("numpad 2"); break;\
case VK_NUMPAD3: name = _("numpad 3"); break;\
case VK_NUMPAD4: name = _("numpad 4"); break;\
case VK_NUMPAD5: name = _("numpad 5"); break;\
case VK_NUMPAD6: name = _("numpad 6"); break;\
case VK_NUMPAD7: name = _("numpad 7"); break;\
case VK_NUMPAD8: name = _("numpad 8"); break;\
case VK_NUMPAD9: name = _("numpad 9"); break;\
case VK_F1: name = _("F1"); break;\
case VK_F2: name = _("F2"); break;\
case VK_F3: name = _("F3"); break;\
case VK_F4: name = _("F4"); break;\
case VK_F5: name = _("F5"); break;\
case VK_F6: name = _("F6"); break;\
case VK_F7: name = _("F7"); break;\
case VK_F8: name = _("F8"); break;\
case VK_F9: name = _("F9"); break;\
case VK_F10: name = _("F10"); break;\
case VK_F11: name = _("F11"); break;\
case VK_F12: name = _("F12"); break;\
default: name = _("unknown");\
}

ImVec4 Color_to_vec(Color& color)
{
    int r, g, b, a;
    color.GetColor(r, g, b, a);

    return ImVec4((float)r / 255.f, (float)g / 255.f, (float)b / 255.f, (float)a / 255.f);
}

namespace ImGui
{
    static auto vector_getter = [](void* vec, int idx, const char** out_text)
    {
        auto& vector = *static_cast<std::vector<std::string>*>(vec);
        if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
        *out_text = vector.at(idx).c_str();
        return true;
    };

    bool Combo(const char* label, int* currIndex, std::vector<std::string>& values)
    {
        if (values.empty()) { return false; }
        return Combo(label, currIndex, vector_getter,
            static_cast<void*>(&values), values.size());
    }

    bool ListBox(const char* label, int* currIndex, std::vector<std::string>& values, int height_in_items)
    {
        if (values.empty()) { return false; }
        return ListBox(label, currIndex, vector_getter,
            static_cast<void*>(&values), values.size(), height_in_items);
    }

    bool ListBox(const char* label, int* current_item, std::function<const char* (int)> lambda, int items_count, int height_in_items)
    {
        return ImGui::ListBox(label, current_item, [](void* data, int idx, const char** out_text)
        {
            *out_text = (*reinterpret_cast<std::function<const char* (int)>*>(data))(idx);
            return true;
        }, &lambda, items_count, height_in_items);
    }

    bool LabelClick(const char* concatoff, const char* concaton, const char* label, bool* v, const char* unique_id)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        // The concatoff/on thingies were for my weapon config system so if we're going to make that, we still need this aids.
        char Buf[64];
        _snprintf(Buf, 62, "%s%s", ((*v) ? concatoff : concaton), label);

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(unique_id);
        const ImVec2 label_size = CalcTextSize(label, NULL, true);

        const ImRect check_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(label_size.y + style.FramePadding.y * 2, label_size.y));
        ItemSize(check_bb, style.FramePadding.y);

        ImRect total_bb = check_bb;
        if (label_size.x > 0)
            SameLine(0, style.ItemInnerSpacing.x);

        const ImRect text_bb(window->DC.CursorPos + ImVec2(0, style.FramePadding.y), window->DC.CursorPos + ImVec2(0, style.FramePadding.y) + label_size);
        if (label_size.x > 0)
        {
            ItemSize(ImVec2(text_bb.GetWidth(), check_bb.GetHeight()), style.FramePadding.y);
            total_bb = ImRect(ImMin(check_bb.Min, text_bb.Min), ImMax(check_bb.Max, text_bb.Max));
        }

        if (!ItemAdd(total_bb, id))
            return false;

        bool hovered, held;
        bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);
        if (pressed)
            *v = !(*v);

        if (label_size.x > 0.0f)
            RenderText(check_bb.GetTL(), Buf);

        return pressed;
    }

    bool ImGui::Hotkey ( const char* label, int* k, const ImVec2& size_arg )
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow ( );
        if ( window->SkipItems )
            return false;

        ImGui::SameLine ( );

        ImGuiContext& g = *GImGui;
        ImGuiIO& io = g.IO;
        const ImGuiStyle& style = g.Style;

        const ImGuiID id = window->GetID ( label );
        const ImVec2 label_size = ImGui::CalcTextSize ( label, NULL, true );
        ImVec2 size = ImGui::CalcItemSize ( size_arg, ImGui::CalcItemWidth ( ), label_size.y + style.FramePadding.y * 2.0f );
        const ImRect frame_bb ( window->DC.CursorPos + ImVec2 { style.ItemInnerSpacing.x, 0.f }, window->DC.CursorPos + size );
        const ImRect total_bb ( window->DC.CursorPos, frame_bb.Max );

        ImGui::ItemSize ( total_bb, style.FramePadding.y );
        if ( !ImGui::ItemAdd ( total_bb, id ) )
            return false;

        const bool focus_requested = ImGui::FocusableItemRegister ( window, g.ActiveId == id );
        //const bool focus_requested_by_code = focus_requested && ( window->FocusIdxAllCounter == window->FocusIdxAllRequestCurrent );
        //const bool focus_requested_by_tab = focus_requested && !focus_requested_by_code;

        const bool hovered = ImGui::ItemHoverable ( frame_bb, id );

        if ( hovered ) {
            ImGui::SetHoveredID ( id );
            g.MouseCursor = ImGuiMouseCursor_TextInput;
        }

        const bool user_clicked = hovered && io.MouseClicked[0];

        if ( focus_requested || user_clicked ) {
            if ( g.ActiveId != id ) {
                // Start edition
                memset ( io.MouseDown, 0, sizeof ( io.MouseDown ) );
                memset ( io.KeysDown, 0, sizeof ( io.KeysDown ) );
                *k = 0;
            }
            ImGui::SetActiveID ( id, window );
            ImGui::FocusWindow ( window );
        }
        else if ( io.MouseClicked[0] ) {
            // Release focus when we click outside
            if ( g.ActiveId == id )
                ImGui::ClearActiveID ( );
        }

        bool value_changed = false;
        int key = *k;

        if ( g.ActiveId == id ) {
            for ( auto i = 0; i <= 6; i++ ) {
                if ( io.MouseDown[i] || GetAsyncKeyState ( VK_XBUTTON1 ) || GetAsyncKeyState ( VK_XBUTTON2 ) ) {
                    switch ( i ) {
                    case 0:
                        key = VK_LBUTTON;
                        break;
                    case 1:
                        key = VK_RBUTTON;
                        break;
                    case 2:
                        key = VK_MBUTTON;
                        break;
                    }
                    if ( GetAsyncKeyState ( VK_XBUTTON2 ) )
                        key = VK_XBUTTON2;
                    if ( GetAsyncKeyState ( VK_XBUTTON1 ) )
                        key = VK_XBUTTON1;

                    value_changed = true;
                    ImGui::ClearActiveID ( );
                }
            }
            if ( !value_changed ) {
                for ( auto i = VK_BACK; i <= VK_RMENU; i++ ) {
                    if ( io.KeysDown[i] ) {
                        key = i;
                        value_changed = true;
                        ImGui::ClearActiveID ( );
                    }
                }
            }

            if ( ImGui::IsKeyPressedMap ( ImGuiKey_Escape ) ) {
                *k = 0;
                ImGui::ClearActiveID ( );
            }
            else {
                *k = key;
            }
        }

        // Render
        // Select which buffer we are going to display. When ImGuiInputTextFlags_NoLiveEdit is Set 'buf' might still be the old value. We Set buf to NULL to prevent accidental usage from now on.

        const char* buf_display = "none";

        // ImGui::RenderFrame ( frame_bb.Min, frame_bb.Max, ImGui::GetColorU32 ( ImVec4 ( 1.f, 1.f, 1.f, 0 ) ), true, style.FrameRounding );

        if ( *k != 0 && g.ActiveId != id ) {
            const char* key_name;
            get_keycode_name ( *k, buf_display );
        }
        else if ( g.ActiveId == id ) {
            buf_display = "...";
            std::this_thread::sleep_for ( std::chrono::milliseconds ( 80 ) );
        }
        ImVec2 render_pos = frame_bb.Min;
        char nigger[48];
        sprintf ( nigger, "[%s]", buf_display );

        ImGui::RenderText ( render_pos, nigger );

        return value_changed;
    }

    void TextContextSelection(const char* concatoff, int* selected, const char* items[], int count)
    {
        if (*selected > count)
            return;

        ImGui::SameLine();

        bool clicked = false;
        std::string text = items[*selected];
        ImGui::LabelClick("| ", "| ", text.c_str(), &clicked, concatoff);
        if (clicked)
            ImGui::OpenPopup(concatoff);

        if (ImGui::BeginPopup(concatoff))
        {
            for (int i = 0; i < count; i++)
                if (ImGui::Selectable(items[i]))
                    *selected = i;
            ImGui::EndPopup();
        }
    }

    void TextContextSelection(const char* concatoff, int* selected, const char* items[], int count, const char* names[], bool* toggles[])
    {
        if (*selected > count)
            return;

        ImGui::SameLine();

        bool clicked = false;
        std::string text = items[*selected];
        ImGui::LabelClick("| ", "| ", text.c_str(), &clicked, concatoff);
        if (clicked)
            ImGui::OpenPopup(concatoff);

        if (ImGui::BeginPopup(concatoff))
        {
            for (int i = 0; i < count; i++)
                if (ImGui::Selectable(items[i]))
                    *selected = i;

            for (int i = 0; i < IM_ARRAYSIZE(names); i++)
                ImGui::MenuItem(names[i], "", toggles[i]);

            ImGui::EndPopup();
        }
    }

    void TextContextSelection(const char* concatoff, bool* selected, const char* items[])
    {
        if ((int)*selected > 1)
            return;

        ImGui::SameLine();

        bool clicked = false;
        std::string text = "";
        if (*selected)
            text = items[1];
        else
            text = items[0];

        ImGui::LabelClick("| ", "| ", text.c_str(), &clicked, concatoff);
        if (clicked)
            ImGui::OpenPopup(concatoff);

        if (ImGui::BeginPopup(concatoff))
        {
            for (int i = 0; i < 2; i++)
                if (ImGui::Selectable(items[i]))
                    *selected = i == 0 ? false : true;
            ImGui::EndPopup();
        }
    }

    void TextContextSelection(const char* concatoff, bool* selected, const char* items[], const char* names[], bool* toggles[])
    {
        if ((int)*selected > 1)
            return;

        ImGui::SameLine();

        bool clicked = false;
        std::string text = "";
        if (*selected)
            text = items[1];
        else
            text = items[0];

        ImGui::LabelClick("| ", "| ", text.c_str(), &clicked, concatoff);
        if (clicked)
            ImGui::OpenPopup(concatoff);

        if (ImGui::BeginPopup(concatoff))
        {
            for (int i = 0; i < 2; i++)
                if (ImGui::Selectable(items[i]))
                    *selected = i == 0 ? false : true;

            for (int i = 0; i < IM_ARRAYSIZE(names); i++)
                ImGui::MenuItem(names[i], "", toggles[i]);

            ImGui::EndPopup();
        }
    }

    // This can be used anywhere, in group boxes etc.
    void SelectTabs(int* selected, const char* items[], int item_count, ImVec2 size)
    {
        ImGui::PushFont(tabfont);
        auto color_grayblue = GetColorU32(ImVec4(0.05, 0.15, 0.45, 0.30));
        auto color_deepblue = GetColorU32(ImVec4(0, 0.25, 0.50, 0.25));
        auto color_shade_hover = GetColorU32(ImVec4(1, 1, 1, 0.05));
        auto color_shade_clicked = GetColorU32(ImVec4(1, 1, 1, 0.1));
        auto color_black_outlines = GetColorU32(ImVec4(0, 0, 0, 1));

        ImGuiStyle& style = GetStyle();
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return;

        std::string names;
        for (int32_t i = 0; i < item_count; i++)
            names += items[i];

        ImGuiContext* g = GImGui;
        const ImGuiID id = window->GetID(names.c_str());
        const ImVec2 label_size = CalcTextSize(names.c_str(), NULL, true);

        ImVec2 Min = window->DC.CursorPos;
        ImVec2 Max = ((size.x <= 0 || size.y <= 0) ? ImVec2(Min.x + GetContentRegionMax().x - style.WindowPadding.x, Min.y + label_size.y * 2) : Min + size);

        ImRect bb(Min, Max);
        ItemSize(bb, style.FramePadding.y);
        if (!ItemAdd(bb, id))
            return;

        PushClipRect(ImVec2(Min.x, Min.y - 1), ImVec2(Max.x, Max.y + 1), false);

        window->DrawList->AddRectFilledMultiColor(Min, Max, color_grayblue, color_grayblue, color_deepblue, color_deepblue); // Main gradient.

        ImVec2 mouse_pos = GetMousePos();
        bool mouse_click = g->IO.MouseClicked[0];

        float TabSize = ceil((Max.x - Min.x) / item_count);

        for (int32_t i = 0; i < item_count; i++)
        {
            ImVec2 Min_cur_label = ImVec2(Min.x + (int)TabSize * i, Min.y);
            ImVec2 Max_cur_label = ImVec2(Min.x + (int)TabSize * i + (int)TabSize, Max.y);

            // Imprecision clamping. gay but works :^)
            Max_cur_label.x = (Max_cur_label.x >= Max.x ? Max.x : Max_cur_label.x);

            if (mouse_pos.x > Min_cur_label.x&& mouse_pos.x < Max_cur_label.x &&
                mouse_pos.y > Min_cur_label.y&& mouse_pos.y < Max_cur_label.y)
            {
                if (mouse_click)
                    *selected = i;
                else if (i != *selected)
                    window->DrawList->AddRectFilled(Min_cur_label, Max_cur_label, color_shade_hover);
            }

            if (i == *selected) {
                window->DrawList->AddRectFilled(Min_cur_label, Max_cur_label, color_shade_clicked);
                window->DrawList->AddRectFilledMultiColor(Min_cur_label, Max_cur_label, color_deepblue, color_deepblue, color_grayblue, color_grayblue);
                window->DrawList->AddLine(ImVec2(Min_cur_label.x - 1.5f, Min_cur_label.y - 1), ImVec2(Max_cur_label.x - 0.5f, Min_cur_label.y - 1), color_black_outlines);
            }
            else
                window->DrawList->AddLine(ImVec2(Min_cur_label.x - 1, Min_cur_label.y), ImVec2(Max_cur_label.x, Min_cur_label.y), color_black_outlines);
            window->DrawList->AddLine(ImVec2(Max_cur_label.x - 1, Max_cur_label.y), ImVec2(Max_cur_label.x - 1, Min_cur_label.y - 0.5f), color_black_outlines);

            const ImVec2 text_size = CalcTextSize(items[i], NULL, true);
            float pad_ = style.FramePadding.x + g->FontSize + style.ItemInnerSpacing.x;
            ImRect tab_rect(Min_cur_label, Max_cur_label);
            RenderTextClipped(Min_cur_label, Max_cur_label, items[i], NULL, &text_size, style.WindowTitleAlign, &tab_rect);
        }

        window->DrawList->AddLine(ImVec2(Min.x, Min.y - 0.5f), ImVec2(Min.x, Max.y), color_black_outlines);
        window->DrawList->AddLine(ImVec2(Min.x, Max.y), Max, color_black_outlines);
        PopClipRect();
        ImGui::PopFont();
    }

    int WarningMessage(std::string Message, std::string Header)
    {
        ImGui::PushFont(tabfont);
        //Custom Warning message modal by jaccko
        int width, height;
        int output = 0;
        auto& style = ImGui::GetStyle();
        //Background
        width = scrrenWidth;
        height = scrrenHeight;
        ImGui::SetNextWindowPos(ImVec2{ 0, 0 }, ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2{ (float)width, (float)height }, ImGuiCond_Always);
        ImGui::SetNextWindowFocus();
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.55f);
        if (ImGui::Begin(XorStr("##gray_bg"), NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove))
        {

        }
        ImGui::End();
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.f);
        //WARNING window
        style.WindowPadding = ImVec2(0, 0);
        static int size = 180; //windows size y	
        auto pos = ImVec2(((float)width / 2.f) - ((float)size * (16.f / 9.f)) / 2.f, (float)height / 2.f - (float)size); //center the window
        ImGui::SetNextWindowSize(ImVec2(float(size) * (16.f / 9.f), float(g_Options.windows_grab_area + size)), ImGuiCond_Once);
        ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
        ImGui::SetNextWindowFocus();
        if (ImGui::Begin(XorStr("##WARNING"), NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings))
        {
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2::Zero);
            {
                style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.9f, 0.f, 0.f, 0.39f);//header colorr
                if (ImGui::BeginChild(XorStr("##headertrasparent"), ImVec2(float(size) * (16.f / 9.f), float(g_Options.windows_grab_area))))
                {
                    auto pos = ImGui::GetCurrentWindow()->Pos;
                    auto wsize = ImGui::GetCurrentWindow()->Size;

                    pos = pos + wsize / 2.0f;
                    //style.Colors[ImGuiCol_Text] = ImVec4(1.f, 1.f, 1.f, 1.f);
                    ImGui::RenderText(pos - ImGui::CalcTextSize(Header.c_str()) / 2.0f, Header.c_str());
                    //style.Colors[ImGuiCol_Text] = text;
                }
                ImGui::EndChild();

                style.Colors[ImGuiCol_ChildWindowBg] = bg_color;
                if (ImGui::BeginChild(XorStr("##body"), ImVec2(float(size) * (16.f / 9.f) /* 16:9 Aspect ratio */, float(size)), false)) //white window
                {
                    style.WindowPadding = ImVec2(5, 5);
                    if (ImGui::BeginChild(XorStr("##body2"), ImVec2(float(size) * (16.f / 9.f) /* 16:9 Aspect ratio */, float(size)), true))//white window with padding
                    {
                        style.Colors[ImGuiCol_ChildWindowBg] = bg_child_color;
                        style.WindowPadding = ImVec2(25, 25);
                        if (ImGui::BeginChild(XorStr("##body3"), ImVec2(float(size) * (16.f / 9.f) /* 16:9 Aspect ratio */ - 10.f /* prev. padding*/, float(size) - 10.f/* prev. padding*/), true)) //grey window
                        {
                            static ImVec2 wsize = ImVec2(0.f, 0.f);
                            style.Colors[ImGuiCol_ChildWindowBg] = bg_color;
                            if (ImGui::BeginChild(XorStr("##body4"), ImVec2(float(size) * (16.f / 9.f) /* 16:9 Aspect ratio */ - 10.f - style.WindowPadding.x * 2, float(size) - 10.f /* prev. padding*/ - style.WindowPadding.y * 2 - 25.f /* Button size */), false)) //Info window (white)
                            {
                                wsize = ImGui::GetCurrentWindow()->Size;
                                ImGui::SpacerText(XorStr("Warning!"));
                                auto pos = ImGui::GetCurrentWindow()->Pos;

                                pos = pos + wsize / 2.0f;

                                ImGui::RenderText(pos - ImGui::CalcTextSize(Message.c_str()) / 2.0f, Message.c_str());
                            }
                            ImGui::EndChild();

                            style.Colors[ImGuiCol_Button] = ImVec4(bg_color.x - 0.05f, bg_color.y - 0.05f, bg_color.z - 0.05f, bg_color.w);
                            style.Colors[ImGuiCol_ButtonHovered] = ImVec4(bg_color.x + 0.05f, bg_color.y + 0.05f, bg_color.z + 0.05f, bg_color.w);
                            style.Colors[ImGuiCol_ButtonActive] = ImVec4(bg_color.x + 0.07f, bg_color.y + 0.07f, bg_color.z + 0.07f, bg_color.w);
                            if (ImGui::Button("Yes", ImVec2(wsize.x / 2.f, 25)))
                            {
                                output = 1;
                            }
                            ImGui::SameLine();
                            if (ImGui::Button("No", ImVec2(wsize.x / 2.f, 25)))
                            {
                                output = 2;
                            }
                            style.Colors[ImGuiCol_Button] = button;
                            style.Colors[ImGuiCol_Text] = text;
                            style.Colors[ImGuiCol_ButtonHovered] = buttonhovered;
                            style.Colors[ImGuiCol_ButtonActive] = buttonActive;
                        }
                        ImGui::EndChild();
                    }
                    ImGui::EndChild();
                }
                ImGui::EndChild();

                style.Colors[ImGuiCol_ChildWindowBg] = bg_color;
            }
            ImGui::PopStyleVar();
        }
        ImGui::End();
        ImGui::PopFont();
        style.WindowPadding = ImVec2(5, 5);
        return output;
        // 0 - waiting // 1 - TRUE  // 2 - False
    }

    std::vector<warnMessage> warning;

    bool Checkbox_warning(const char* label, bool* v, std::string Message, std::string Header)
    {
        int index = -1;
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);
        const ImVec2 label_size = CalcTextSize(label, NULL, true);

        for (unsigned int i = 0; i < warning.size(); i++)
        {
            if (std::strcmp(warning[i].item_label, label) == 0)
            {
                index = i; //get index of current record
                break;
            }
        }
        if (index == -1)
        {
            warning.push_back({ label, true, 0 });
            index = warning.size() - 1;
        }

        if (index < 0)
            return false;

        const ImRect check_bb(window->DC.CursorPos + ImVec2(10.f, 0.f), window->DC.CursorPos + ImVec2(10.f, 0.f) + ImVec2(label_size.y/* + style.FramePadding.y * 2*/, label_size.y /*+ style.FramePadding.y * 2*/)); // We want a square shape to we use Y twice
        ItemSize(check_bb, style.FramePadding.y);

        ImRect total_bb = check_bb;
        if (label_size.x > 0)
            SameLine(0, style.ItemInnerSpacing.x);
        const ImRect text_bb(window->DC.CursorPos /*+ ImVec2(0, style.FramePadding.y)*/, window->DC.CursorPos /*+ ImVec2(0, style.FramePadding.y) */ + label_size);
        if (label_size.x > 0)
        {
            ItemSize(ImVec2(text_bb.GetWidth() + 20.f, check_bb.GetHeight()), style.FramePadding.y);
            total_bb = ImRect(ImMin(check_bb.Min, text_bb.Min), ImMax(check_bb.Max, text_bb.Max));
        }

        if (!ItemAdd(total_bb, id))
            return false;

        bool hovered, held;
        bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);
        if (pressed)
            warning[index].clicked = true;

        if (warning[index].clicked && (warning[index].answer == 0 || warning[index].answer == 2))
        {
            warning[index].answer = WarningMessage(Message, Header);
        }

        if ((pressed || warning[index].clicked) && warning[index].answer == 1)
            *v = !(*v);

        if (warning[index].answer == 1 || warning[index].answer == 2)
            warning[index].clicked = false;

        RenderFrame(check_bb.Min + ImVec2(10.f, 0.f), check_bb.Max + ImVec2(10.f, 0.f), GetColorU32((held && hovered) ? ImGuiCol_FrameBgActive : hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg), true, style.FrameRounding);
        if (*v)
        {
            const float check_sz = ImMin(check_bb.GetWidth(), check_bb.GetHeight());
            const float pad = ImMax(1.0f, (float)(int)(check_sz / 6.0f));
            window->DrawList->AddRectFilled(check_bb.Min + ImVec2(pad, pad) + ImVec2(10.f, 0.f), check_bb.Max - ImVec2(pad, pad) + ImVec2(10.f, 0.f), GetColorU32(ImGuiCol_CheckMark), style.FrameRounding);
        }

        //if (g.LogEnabled)
            //LogRenderedText(&text_bb.Min, *v ? "[x]" : "[ ]");
        if (label_size.x > 0.0f)
            RenderText(text_bb.Min + ImVec2(20.f, 0.f), label);

        return pressed;
    }
}


//Inspirated by Gladiatorcheatz v2 color picker
void ColorPickerBoxPlayer(const char* picker_idname, Color* team/*ct*/, Color* enemy/*t*/, Color* team_invis/*ct invis*/, Color* enemy_invis/*t invis*/, bool alpha)
{
    ImGui::PushFont(tabfont);
    auto& style = ImGui::GetStyle();

    auto teamf = ImVec4{
        team->r() / 255.0f,
        team->g() / 255.0f,
        team->b() / 255.0f,
        team->a() / 255.0f
    };

    auto team_invisf = ImVec4{
        team_invis->r() / 255.0f,
        team_invis->g() / 255.0f,
        team_invis->b() / 255.0f,
        team_invis->a() / 255.0f
    };

    auto enemyf = ImVec4{
        enemy->r() / 255.0f,
        enemy->g() / 255.0f,
        enemy->b() / 255.0f,
        enemy->a() / 255.0f
    };

    auto enemy_invisf = ImVec4{
        enemy_invis->r() / 255.0f,
        enemy_invis->g() / 255.0f,
        enemy_invis->b() / 255.0f,
        enemy_invis->a() / 255.0f
    };


    ImGui::SameLine();
    static bool switch_entity_teams = false;
    static bool switch_color_vis = false;
    bool open_popup = ImGui::ColorButton(picker_idname, switch_entity_teams ? (switch_color_vis ? teamf : team_invisf) : (switch_color_vis ? enemyf : enemy_invisf), ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoTooltip, ImVec2(20, 0));
    if (open_popup) {
        ImGui::OpenPopup(picker_idname);
        Colorpicker_Close = ImGui::GetID(picker_idname);
    }
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(bg_color.x - 0.05f, bg_color.y - 0.05f, bg_color.z - 0.05f, bg_color.w));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(bg_color.x + 0.05f, bg_color.y + 0.05f, bg_color.z + 0.05f, bg_color.w));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(bg_color.x + 0.07f, bg_color.y + 0.07f, bg_color.z + 0.07f, bg_color.w));
    if (ImGui::BeginPopup(picker_idname))
    {
        color_picker_open = true;
        style.Colors[ImGuiCol_Text] = switch_entity_teams ? (ImVec4(0.1f, 0.1f, 1.f, 1.f)) : (ImVec4(1.f, 0.1f, 0.1f, 1.f));
        const char* button_name0 = switch_entity_teams ? XorStr("Team") : XorStr("Enemy");
        if (ImGui::Button(button_name0, ImVec2(-1, 0)))
            switch_entity_teams = !switch_entity_teams;

        style.Colors[ImGuiCol_Text] = switch_color_vis ? (ImVec4(0.f, 0.f, 0.f, 1.f)) : (ImVec4(0.70f, 0.70f, 0.70f, 1.f));
        const char* button_name1 = switch_color_vis ? XorStr("Visible") : XorStr("Invisible");
        if (ImGui::Button(button_name1, ImVec2(-1, 0)))
            switch_color_vis = !switch_color_vis;

        std::string id_new = picker_idname;
        id_new += "##pickeritself_";
        style.Colors[ImGuiCol_Text] = text;
        if (ImGui::ColorPicker4(id_new.c_str(), switch_entity_teams ? (switch_color_vis ? &teamf.x : &team_invisf.x) : (switch_color_vis ? &enemyf.x : &enemy_invisf.x), ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_PickerHueBar | (alpha ? ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaBar : 0)))
        {
            team->SetColor(teamf.x, teamf.y, teamf.z, teamf.w);
            enemy->SetColor(enemyf.x, enemyf.y, enemyf.z, enemyf.w);
            team_invis->SetColor(team_invisf.x, team_invisf.y, team_invisf.z, team_invisf.w);
            enemy_invis->SetColor(enemy_invisf.x, enemy_invisf.y, enemy_invisf.z, enemy_invisf.w);
        }
        ImGui::EndPopup();
    }
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    style.Colors[ImGuiCol_Text] = text;
    ImGui::PopFont();
}

void ColorPickerBoxPlayer(const char* picker_idname, Color* team/*ct*/, Color* enemy/*t*/, bool alpha, bool player_or_visible) //player_or_visible = true = player/enemy    false = visible/invisible
{
    ImGui::PushFont(tabfont);
    auto& style = ImGui::GetStyle();

    auto teamf = ImVec4{
        team->r() / 255.0f,
        team->g() / 255.0f,
        team->b() / 255.0f,
        team->a() / 255.0f
    };

    auto enemyf = ImVec4{
        enemy->r() / 255.0f,
        enemy->g() / 255.0f,
        enemy->b() / 255.0f,
        enemy->a() / 255.0f
    };


    ImGui::SameLine();
    static bool switch_entity_teams = false;
    bool open_popup = ImGui::ColorButton(picker_idname, switch_entity_teams ? (teamf) : (enemyf), ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoTooltip, ImVec2(20, 0));
    if (open_popup) {
        ImGui::OpenPopup(picker_idname);
        Colorpicker_Close = ImGui::GetID(picker_idname);
    }
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(bg_color.x - 0.05f, bg_color.y - 0.05f, bg_color.z - 0.05f, bg_color.w));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(bg_color.x + 0.05f, bg_color.y + 0.05f, bg_color.z + 0.05f, bg_color.w));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(bg_color.x + 0.07f, bg_color.y + 0.07f, bg_color.z + 0.07f, bg_color.w));
    if (ImGui::BeginPopup(picker_idname))
    {
        color_picker_open = true;
        style.Colors[ImGuiCol_Text] = player_or_visible ? (switch_entity_teams ? (ImVec4(0.1f, 0.1f, 1.f, 1.f)) : (ImVec4(1.f, 0.1f, 0.1f, 1.f))) : (switch_entity_teams ? (ImVec4(0.f, 0.f, 0.f, 1.f)) : (ImVec4(0.70f, 0.70f, 0.70f, 1.f)));// proè to oboum dava tu druhou barvu ? :D idk ale ty závorky... je to nìjaké divné
        const char* button_name0 = player_or_visible ? (switch_entity_teams ? XorStr("Team") : XorStr("Enemy")) : (switch_entity_teams ? XorStr("Visible") : XorStr("Invisible"));
        if (ImGui::Button(button_name0, ImVec2(-1, 0)))
            switch_entity_teams = !switch_entity_teams;

        std::string id_new = picker_idname;
        id_new += "##pickeritself_";
        style.Colors[ImGuiCol_Text] = text;
        if (ImGui::ColorPicker4(id_new.c_str(), switch_entity_teams ? (&teamf.x) : (&enemyf.x), ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_PickerHueBar | (alpha ? ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaBar : 0)))
        {
            team->SetColor(teamf.x, teamf.y, teamf.z, teamf.w);
            enemy->SetColor(enemyf.x, enemyf.y, enemyf.z, enemyf.w);
        }
        ImGui::EndPopup();
    }
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    style.Colors[ImGuiCol_Text] = text;
    ImGui::PopFont();
}

void ColorPickerBoxCustom(const char* picker_idname, Color* col1/*ct*/, std::string name1, bool alpha, bool Color_or_blackandwhite) //Color_or_blackandwhite = true = BLUE/RED    false = BLACK/GRAY
{
    ImGui::PushFont(tabfont);
    auto& style = ImGui::GetStyle();

    auto teamf = ImVec4{
        col1->r() / 255.0f,
        col1->g() / 255.0f,
        col1->b() / 255.0f,
        col1->a() / 255.0f
    };

    ImGui::SameLine();
    static bool switch_entity_teams = true;
    bool open_popup = ImGui::ColorButton(picker_idname, teamf, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoTooltip, ImVec2(20, 0));
    if (open_popup) {
        ImGui::OpenPopup(picker_idname);
        Colorpicker_Close = ImGui::GetID(picker_idname);
    }
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(bg_color.x - 0.05f, bg_color.y - 0.05f, bg_color.z - 0.05f, bg_color.w));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(bg_color.x + 0.05f, bg_color.y + 0.05f, bg_color.z + 0.05f, bg_color.w));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(bg_color.x + 0.07f, bg_color.y + 0.07f, bg_color.z + 0.07f, bg_color.w));
    if (ImGui::BeginPopup(picker_idname))
    {
        color_picker_open = true;
        style.Colors[ImGuiCol_Text] = Color_or_blackandwhite ? (ImVec4(0.1f, 0.1f, 1.f, 1.f)) : (ImVec4(0.f, 0.f, 0.f, 1.f));
        const char* button_name0 = name1.c_str();
        if (ImGui::Button(button_name0, ImVec2(-1, 0)))
            switch_entity_teams = !switch_entity_teams;

        std::string id_new = picker_idname;
        id_new += "##pickeritself_";
        style.Colors[ImGuiCol_Text] = text;
        if (ImGui::ColorPicker4(id_new.c_str(), &teamf.x, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_PickerHueBar | (alpha ? ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaBar : 0)))
        {
            col1->SetColor(teamf.x, teamf.y, teamf.z, teamf.w);
        }
        ImGui::EndPopup();
    }
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    style.Colors[ImGuiCol_Text] = text;
    ImGui::PopFont();
}

void ColorPickerBoxCustom(const char* picker_idname, Color* col1/*ct*/, Color* col2/*t*/, std::string name1, std::string name2, bool alpha, bool Color_or_blackandwhite) //Color_or_blackandwhite = true = BLUE/RED    false = BLACK/GRAY
{
    ImGui::PushFont(tabfont);
    auto& style = ImGui::GetStyle();

    auto teamf = ImVec4{
        col1->r() / 255.0f,
        col1->g() / 255.0f,
        col1->b() / 255.0f,
        col1->a() / 255.0f
    };

    auto enemyf = ImVec4{
        col2->r() / 255.0f,
        col2->g() / 255.0f,
        col2->b() / 255.0f,
        col2->a() / 255.0f
    };


    ImGui::SameLine();
    static bool switch_entity_teams = true;
    bool open_popup = ImGui::ColorButton(picker_idname, switch_entity_teams ? (teamf) : (enemyf), ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoTooltip, ImVec2(20, 0));
    if (open_popup) {
        ImGui::OpenPopup(picker_idname);
        Colorpicker_Close = ImGui::GetID(picker_idname);
    }
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(bg_color.x - 0.05f, bg_color.y - 0.05f, bg_color.z - 0.05f, bg_color.w));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(bg_color.x + 0.05f, bg_color.y + 0.05f, bg_color.z + 0.05f, bg_color.w));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(bg_color.x + 0.07f, bg_color.y + 0.07f, bg_color.z + 0.07f, bg_color.w));
    if (ImGui::BeginPopup(picker_idname))
    {
        color_picker_open = true;
        style.Colors[ImGuiCol_Text] = Color_or_blackandwhite ? (switch_entity_teams ? (ImVec4(0.1f, 0.1f, 1.f, 1.f)) : (ImVec4(1.f, 0.1f, 0.1f, 1.f))) : (switch_entity_teams ? (ImVec4(0.f, 0.f, 0.f, 1.f)) : (ImVec4(0.70f, 0.70f, 0.70f, 1.f)));
        const char* button_name0 = Color_or_blackandwhite ? (switch_entity_teams ? name1.c_str() : name2.c_str()) : (switch_entity_teams ? name1.c_str() : name2.c_str());
        if (ImGui::Button(button_name0, ImVec2(-1, 0)))
            switch_entity_teams = !switch_entity_teams;

        std::string id_new = picker_idname;
        id_new += "##pickeritself_";
        style.Colors[ImGuiCol_Text] = text;
        if (ImGui::ColorPicker4(id_new.c_str(), switch_entity_teams ? (&teamf.x) : (&enemyf.x), ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_PickerHueBar | (alpha ? ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaBar : 0)))
        {
            col1->SetColor(teamf.x, teamf.y, teamf.z, teamf.w);
            col2->SetColor(enemyf.x, enemyf.y, enemyf.z, enemyf.w);
        }
        ImGui::EndPopup();
    }
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    style.Colors[ImGuiCol_Text] = text;
    ImGui::PopFont();
}

void ColorPickerBox(const char* picker_idname, Color* color, bool alpha)
{
    ImGui::PushFont(tabfont);
    auto& style = ImGui::GetStyle();

    auto colorf = ImVec4{
        color->r() / 255.0f,
        color->g() / 255.0f,
        color->b() / 255.0f,
        color->a() / 255.0f
    };

    ImGui::SameLine();
    static bool switch_entity_teams = false;
    static bool switch_color_vis = false;
    bool open_popup = ImGui::ColorButton(picker_idname, colorf, ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoTooltip, ImVec2(20, 0));
    if (open_popup) {
        ImGui::OpenPopup(picker_idname);
        Colorpicker_Close = ImGui::GetID(picker_idname);
        color_picker_open = true;
    }
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(bg_color.x - 0.05f, bg_color.y - 0.05f, bg_color.z - 0.05f, bg_color.w));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(bg_color.x + 0.05f, bg_color.y + 0.05f, bg_color.z + 0.05f, bg_color.w));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(bg_color.x + 0.07f, bg_color.y + 0.07f, bg_color.z + 0.07f, bg_color.w));
    if (ImGui::BeginPopup(picker_idname))
    {
        color_picker_open = true;
        std::string id_new = picker_idname;
        id_new += "##pickeritself_";

        if (ImGui::ColorPicker4(id_new.c_str(), &colorf.x, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_PickerHueBar | (alpha ? ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaBar : 0)))
        {
            color->SetColor(colorf.x, colorf.y, colorf.z, colorf.w);
        }
        ImGui::EndPopup();
    }
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopFont();
}

void ColorPickerBox(const char* picker_idname, float color[])
{
    ImGui::PushFont(tabfont);
    auto& style = ImGui::GetStyle();

    auto colorf = ImVec4{
        color[0],
        color[1],
        color[2],
        1.f
    };

    ImGui::SameLine();
    static bool switch_entity_teams = false;
    static bool switch_color_vis = false;
    bool open_popup = ImGui::ColorButton(picker_idname, colorf, ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoTooltip, ImVec2(20, 0));
    if (open_popup) {
        ImGui::OpenPopup(picker_idname);
        Colorpicker_Close = ImGui::GetID(picker_idname);
        color_picker_open = true;
    }
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(bg_color.x - 0.05f, bg_color.y - 0.05f, bg_color.z - 0.05f, bg_color.w));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(bg_color.x + 0.05f, bg_color.y + 0.05f, bg_color.z + 0.05f, bg_color.w));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(bg_color.x + 0.07f, bg_color.y + 0.07f, bg_color.z + 0.07f, bg_color.w));
    if (ImGui::BeginPopup(picker_idname))
    {
        color_picker_open = true;
        std::string id_new = picker_idname;
        id_new += "##pickeritself_";

        if (ImGui::ColorPicker4(id_new.c_str(), &colorf.x, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_PickerHueBar))
        {
            color[0] = colorf.x;
            color[1] = colorf.y;
            color[2] = colorf.z;
        }
        ImGui::EndPopup();
    }
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopFont();
}

void AddFixLog(const char* version, const char* _text, LogType type)
{
    auto& style = ImGui::GetStyle();
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2::Zero);
    {
        ImGui::Columns(1);
        ImGui::Columns(3, NULL, false);
        {
            ImGui::SetColumnWidth(-1, 75.f);
            ImGui::Dummy(ImVec2(0.f, 2.f));
            ImGui::Dummy(ImVec2(5.f, 0.f));
            ImGui::SameLine();
            ImGui::TextColored(style.Colors[ImGuiCol_Text], version);
        }
        ImGui::NextColumn();
        {
            ImGui::SetColumnWidth(-1, 65.f);
            if (type != LOGTYPE_NONE)
            {
                ImVec4 FixColor(0.f, 0.f, 0.f, 1.f);
                auto tx = "";
                switch (type)
                {
                case LOGTYPE_FIX:
                    FixColor = ImVec4(0.1f, 0.4f, 0.9f, 1.f);
                    tx = XorStr("FIX");
                    break;
                case LOGTYPE_ADD:
                    FixColor = ImVec4(0.14f, 0.44f, 0.12f, 1.f);
                    tx = XorStr("ADD");
                    break;
                case LOGTYPE_REMOVE:
                    FixColor = ImVec4(0.44f, 0.12f, 0.12f, 1.f);
                    tx = XorStr("REMOVE");
                    break;
                case LOGTYPE_CHANGED:
                    FixColor = ImVec4(1.f, 0.49f, 0.31f, 1.f);
                    tx = XorStr("CHANGED");
                    break;
                case LOGTYPE_REMADE:
                    FixColor = ImVec4(0.54f, 0.16f, 0.88f, 1.f);
                    tx = XorStr("REMADE");
                    break;
                case LOGTYPE_UPDATE:
                    FixColor = ImVec4(0.54f, 0.16f, 0.88f, 1.f);
                    tx = XorStr("UPDATE");
                    break;
                }

                ImGui::PushStyleColor(ImGuiCol_Button, FixColor);
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, FixColor);
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, FixColor);
                ImGui::PushStyleColor(ImGuiCol_Separator, FixColor);
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 1.f, 1.f, text.w));
                ImGui::Button(tx, ImVec2(-1.f, 0.f));
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
            }
        }
        ImGui::NextColumn();
        {
            ImGui::SetColumnWidth(-1, 280.f);
            ImGui::Dummy(ImVec2(0.f, 2.f));
            ImGui::TextColored(style.Colors[ImGuiCol_Text], _text);
        }
    }
    ImGui::PopStyleVar();
    ImGui::Dummy(ImVec2(1.f, 3.f));
}

void AddChatMessage(int title, int uid, std::string name, std::string message)
{
    auto& style = ImGui::GetStyle();
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2::Zero);
    {
        ImGui::Columns(1);
        ImGui::Columns(2, NULL, false);
        {
            ImGui::SetColumnWidth(-1, 85.f);
            ImGui::Dummy(ImVec2(0.f, 2.f));
            ImGui::Dummy(ImVec2(5.f, 0.f));
            ImGui::SameLine();
            if (title > 0)
                ImGui::PushFont(menufont_bold);
            ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + 85.f);
            ImGui::TextColored(title > 0 ? ImVec4(themecolor.x, themecolor.y, themecolor.z, themecolor.w) : style.Colors[ImGuiCol_Text], (std::string(name) + " (" + std::to_string(uid) + "):").c_str());
            ImGui::PopTextWrapPos();
            if (title > 0)
                ImGui::PopFont();
        }
        ImGui::NextColumn();
        {
            ImGui::SetColumnWidth(-1, 320.f);
            ImGui::Dummy(ImVec2(0.f, 2.f));
            ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + 305.f);
            ImGui::TextColored(style.Colors[ImGuiCol_Text], std::string(message).c_str());
            ImGui::PopTextWrapPos();
        }
    }
    ImGui::PopStyleVar();
    ImGui::Dummy(ImVec2(1.f, 3.f));
}
