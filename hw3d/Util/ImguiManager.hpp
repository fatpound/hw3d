#pragma once

namespace fatpound::util
{
    class ImguiManager final
    {
    public:
        ImguiManager();
        ImguiManager(const ImguiManager& src) = delete;
        ImguiManager& operator = (const ImguiManager& src) = delete;
        ImguiManager(ImguiManager&& src) = delete;
        ImguiManager& operator = (ImguiManager&& src) = delete;
        ~ImguiManager();


    protected:


    private:
    };
}