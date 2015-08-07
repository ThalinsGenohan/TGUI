/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus's Graphical User Interface
// Copyright (C) 2012-2015 Bruno Van de Velde (vdv_b@tgui.eu)
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


#ifndef TGUI_OBJECT_CONVERTER_HPP
#define TGUI_OBJECT_CONVERTER_HPP


#include <TGUI/Borders.hpp>
#include <TGUI/Texture.hpp>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Implicit converter for settable properties
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class TGUI_API ObjectConverter
    {
    public:
        enum class Type
        {
            None,
            Font,
            Color,
            String,
            Number,
            Borders,
            Texture
        };


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Default constructor, used to use this class as value in std::map
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ObjectConverter();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Store a font object for later retrieval
        ///
        /// @param font  Font to store
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ObjectConverter(const std::shared_ptr<sf::Font>& font);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Store a color object for later retrieval
        ///
        /// @param color  Color to store
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ObjectConverter(const sf::Color& color);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Store a string for later retrieval
        ///
        /// @param string  String to store
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ObjectConverter(const sf::String& string);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Store a number for later retrieval
        ///
        /// @param number  Number to store
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ObjectConverter(float number);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Store a borders object for later retrieval
        ///
        /// @param borders  Borders to store
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ObjectConverter(const Borders& borders);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Store a texture object for later retrieval
        ///
        /// @param texture  Texture to store
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ObjectConverter(const Texture& texture);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @param Retrieve the saved font
        ///
        /// @return The saved font
        ///
        /// This function will assert when something other than a font was saved
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const std::shared_ptr<sf::Font>& getFont() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @param Retrieve the saved color
        ///
        /// @return The saved color
        ///
        /// This function will assert when something other than a color was saved
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const sf::Color& getColor() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @param Retrieve the saved borders
        ///
        /// @return The saved borders
        ///
        /// This function will assert when something other than a borders was saved
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const Borders& getBorders() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @param Retrieve the saved string
        ///
        /// @return The saved string
        ///
        /// This function will assert when something other than a string was saved
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const sf::String& getString() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @param Retrieve the saved number
        ///
        /// @return The saved number
        ///
        /// This function will assert when something other than a number was saved
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        float getNumber() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @param Retrieve the saved texture
        ///
        /// @return The saved texture
        ///
        /// This function will assert when something other than a texture was saved
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const Texture& getTexture() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @param Retrieve the type of the object that has been stored
        ///
        /// @return The stored object type
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Type getType() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    private:
        Type m_type;

        std::shared_ptr<sf::Font> m_font;
        sf::Color  m_color;
        sf::String m_string;
        float      m_number;
        Borders    m_borders;
        Texture    m_texture;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // TGUI_OBJECT_CONVERTER_HPP
