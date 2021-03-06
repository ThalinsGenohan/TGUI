/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2020 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <TGUI/Backend.hpp>
#include <TGUI/RenderTarget.hpp>
#include <TGUI/Timer.hpp>
#include <SFML/Window/Window.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#ifdef TGUI_SYSTEM_WINDOWS
    #include <TGUI/WindowsInclude.hpp>
#endif

#ifdef TGUI_SYSTEM_LINUX
    #include <X11/Xlib.h>
    #include <X11/cursorfont.h>
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    namespace
    {
        std::shared_ptr<BackendBase> globalBackend;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void setBackend(std::shared_ptr<BackendBase> backend)
    {
        globalBackend = backend;

        if (backend == nullptr)
        {
            // If the backend is being destroyed then stop all timers (as they could contain resources that have to be destroyed
            // before the main function exits)
            Timer::clearTimers();
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::shared_ptr<BackendBase> getBackend()
    {
        return globalBackend;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void BackendBase::setDestroyOnLastGuiDetatch(bool destroyOnDetatch)
    {
        m_destroyOnLastGuiDetatch = destroyOnDetatch;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void BackendSFML::attachGui(Gui* gui)
    {
        m_guis[gui] = {};
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void BackendSFML::detatchGui(Gui* gui)
    {
        assert(m_guis.find(gui) != m_guis.end());
        m_guis.erase(gui);

        if (m_destroyOnLastGuiDetatch && m_guis.empty())
            setBackend(nullptr);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void BackendSFML::setMouseCursorStyle(Cursor::Type type, const std::uint8_t* pixels, Vector2u size, Vector2u hotspot)
    {
        // Replace the cursor resource
        auto newCursor = std::make_unique<sf::Cursor>();
        newCursor->loadFromPixels(pixels, size, hotspot);
        updateMouseCursor(type, std::move(newCursor));
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void BackendSFML::resetMouseCursorStyle(Cursor::Type type)
    {
#ifdef TGUI_SYSTEM_LINUX
        // On Linux we use directional resize arrows, but SFML has no support for them
        if ((type == Cursor::Type::SizeLeft) || (type == Cursor::Type::SizeRight)
         || (type == Cursor::Type::SizeTop) || (type == Cursor::Type::SizeBottom)
         || (type == Cursor::Type::SizeBottomRight) || (type == Cursor::Type::SizeTopLeft)
         || (type == Cursor::Type::SizeBottomLeft) || (type == Cursor::Type::SizeTopRight))
        {
            // If the cursor was previously set to a bitmap then release its resources
            m_mouseCursors.erase(type);

            updateMouseCursor(type, nullptr);
            return;
        }
#endif

        updateMouseCursor(type, createSystemCursor(type));
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void BackendSFML::setMouseCursor(Gui* gui, Cursor::Type type)
    {
        assert(m_guis.find(gui) != m_guis.end());
        if (type == m_guis[gui].mouseCursor)
            return;

        m_guis[gui].mouseCursor = type;

        // If the gui has no access to the window then we can't change the mouse cursor
        if (!m_guis[gui].window)
            return;

        updateMouseCursor(m_guis[gui].window, type);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::shared_ptr<RenderTargetSFML> BackendSFML::setGuiTarget(Gui* gui, sf::RenderTarget& target)
    {
        assert(m_guis.find(gui) != m_guis.end());
        m_guis[gui].window = dynamic_cast<sf::Window*>(&target);

        auto renderTarget = std::make_shared<RenderTargetSFML>();
        renderTarget->setTarget(target);
        return renderTarget;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::unique_ptr<sf::Cursor> BackendSFML::createSystemCursor(Cursor::Type type)
    {
        sf::Cursor::Type typeSFML = sf::Cursor::Type::Arrow;
        switch (type)
        {
        case Cursor::Type::Arrow:
            typeSFML = sf::Cursor::Type::Arrow;
            break;
        case Cursor::Type::Text:
            typeSFML = sf::Cursor::Type::Text;
            break;
        case Cursor::Type::Hand:
            typeSFML = sf::Cursor::Type::Hand;
            break;
        case Cursor::Type::SizeLeft:
        case Cursor::Type::SizeRight:
            typeSFML = sf::Cursor::Type::SizeHorizontal;
            break;
        case Cursor::Type::SizeTop:
        case Cursor::Type::SizeBottom:
            typeSFML = sf::Cursor::Type::SizeVertical;
            break;
        case Cursor::Type::SizeBottomRight:
        case Cursor::Type::SizeTopLeft:
            typeSFML = sf::Cursor::Type::SizeTopLeftBottomRight;
            break;
        case Cursor::Type::SizeBottomLeft:
        case Cursor::Type::SizeTopRight:
            typeSFML = sf::Cursor::Type::SizeBottomLeftTopRight;
            break;
        case Cursor::Type::Cross:
            typeSFML = sf::Cursor::Type::Cross;
            break;
        case Cursor::Type::Help:
            typeSFML = sf::Cursor::Type::Help;
            break;
        case Cursor::Type::NotAllowed:
            typeSFML = sf::Cursor::Type::NotAllowed;
            break;
        }

        auto cursor = std::make_unique<sf::Cursor>();
        cursor->loadFromSystem(typeSFML);
        return cursor;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void BackendSFML::updateMouseCursor(Cursor::Type type, std::unique_ptr<sf::Cursor> cursor)
    {
#ifdef TGUI_SYSTEM_WINDOWS
        // Make sure the old cursor isn't still being used before we destroy it
        bool cursorInUse = false;
        for (auto& pair : m_guis)
        {
            if (pair.second.mouseCursor == type)
                cursorInUse = true;
        }
        if (cursorInUse)
            SetCursor(static_cast<HCURSOR>(LoadImage(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED)));
#endif

        m_mouseCursors[type] = std::move(cursor);

        // Update the cursor on the screen if the cursor was in use
        for (auto& pair : m_guis)
        {
            if (pair.second.mouseCursor == type)
            {
                if (pair.second.window)
                    updateMouseCursor(pair.second.window, type);
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void BackendSFML::updateMouseCursor(sf::Window* window, Cursor::Type type)
    {
#ifdef TGUI_SYSTEM_LINUX
        // On Linux we use directional resize arrows, but SFML has no support for them
        if ((type == Cursor::Type::SizeLeft) || (type == Cursor::Type::SizeRight)
            || (type == Cursor::Type::SizeTop) || (type == Cursor::Type::SizeBottom)
            || (type == Cursor::Type::SizeBottomRight) || (type == Cursor::Type::SizeTopLeft)
            || (type == Cursor::Type::SizeBottomLeft) || (type == Cursor::Type::SizeTopRight))
        {
            if (!m_mouseCursors[type]) // Only bypass SFML when system cursors are used
            {
                ::Display* displayX11 = XOpenDisplay(nullptr);
                if (displayX11)
                {
                    unsigned int shapeX11;
                    if (type == Cursor::Type::SizeLeft)
                        shapeX11 = XC_left_side;
                    else if (type == Cursor::Type::SizeRight)
                        shapeX11 = XC_right_side;
                    else if (type == Cursor::Type::SizeTop)
                        shapeX11 = XC_top_side;
                    else if (type == Cursor::Type::SizeBottom)
                        shapeX11 = XC_bottom_side;
                    else if (type == Cursor::Type::SizeBottomRight)
                        shapeX11 = XC_bottom_right_corner;
                    else if (type == Cursor::Type::SizeTopLeft)
                        shapeX11 = XC_top_left_corner;
                    else if (type == Cursor::Type::SizeBottomLeft)
                        shapeX11 = XC_bottom_left_corner;
                    else // if (type == Cursor::Type::SizeTopRight)
                        shapeX11 = XC_top_right_corner;

                    ::Cursor cursorX11 = XCreateFontCursor(displayX11, shapeX11);
                    if (cursorX11 != None)
                    {
                        XDefineCursor(displayX11, window->getSystemHandle(), cursorX11);
                        XFreeCursor(displayX11, cursorX11);
                    }
                    XFlush(displayX11);
                    XCloseDisplay(displayX11);
                }
                return;
            }
        }
#endif

        // If the cursor doesn't exist yet then create it now
        if (!m_mouseCursors[type])
            m_mouseCursors[type] = createSystemCursor(type);

        // Pass the cursor to SFML to set it while the mouse is on top of the window
        window->setMouseCursor(*m_mouseCursors[type]);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

#ifdef TGUI_SYSTEM_LINUX
    // Undefine some annoying generic defines from X.h so that we don't get errors in unity builds
    #undef None
    #undef Bool
    #undef KeyPress
    #undef KeyRelease
    #undef Status
    #undef Always
    #undef Window
    #undef Drawable
    #undef Font
    #undef Cursor
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
