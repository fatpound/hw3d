#pragma once

namespace fatpound::util
{
    class ImguiManager final
    {
    public:
        ImguiManager();
        ImguiManager(const ImguiManager& src) = delete;
        ImguiManager(ImguiManager&& src) = delete;

        auto operator = (const ImguiManager& src) -> ImguiManager& = delete;
        auto operator = (ImguiManager&& src)      -> ImguiManager& = delete;
        ~ImguiManager() noexcept;


    protected:


    private:
    };
}