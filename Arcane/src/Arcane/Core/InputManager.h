#pragma once

#include <glm/glm.hpp>
#include <map>

namespace Arcane {

    enum InputState
    {
        KEY_INITIAL,
        KEY_PRESSED,
        KEY_RELEASED
    };

    enum MouseKeyState
    {
        MOUSE_KEY_INITIAL,
        MOUSE_KEY_PRESSED,
        MOUSE_KEY_RELEASED
    };

    class InputManager
    {
    public:
        static InputManager* GetInstance();

        // Getters
        // -- Keys
        static bool GetKeyPressed(int key);
        static bool GetKeyReleased(int key);

        //-- Mouse
        static std::pair<float, float> GetMouseCoords();
        static bool GetMouseKeyPressed(int mouseKey);
        static bool GetMouseKeyReleased(int mouseKey);
        static glm::vec2 GetScrollOffsets();

        // Setters
        // -- Keys
        static void SetKeyPressed(int key);
        static void SetKeyReleased(int key);

        // -- Mouse
        static void SetMouseCoords(float _x, float _y);
        static void SetMouseKeyPressed(int key);
        static void SetMouseKeyReleased(int key);
        static void SetScrollOffsets(float x, float y);
    private:
        // Non-Static Implementations
        // Getters

        // -- Keys
        bool GetKeyPressedImpl(int key);
        bool GetKeyReleasedImpl(int key);


        //-- Mouse
        std::pair<float, float> GetMouseCoordsImpl();
        bool GetMouseKeyPressedImpl(int mouseKey);
        bool GetMouseKeyReleasedImpl(int mouseKey);
        glm::vec2 GetScrollOffsetsImpl();

        // Setters
        // -- Keys
        void SetKeyPressedImpl(int key);
        void SetKeyReleasedImpl(int key);

        // -- Mouse
        void SetMouseCoordsImpl(float _x, float _y);
        void SetMouseKeyPressedImpl(int key);
        void SetMouseKeyReleasedImpl(int key);
        void SetScrollOffsetsImpl(float x, float y);
    private:
        InputManager();

        static InputManager* s_Instance;
        InputState state[255];
        
        MouseKeyState mouseState[3];
        glm::vec2 m_MouseCoords;
        glm::vec2 m_ScrollOffsets = {0.0f, 0.0f};
    };
}