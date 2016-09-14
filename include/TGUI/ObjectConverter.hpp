/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2016 Bruno Van de Velde (vdv_b@tgui.eu)
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


#include <TGUI/TextStyle.hpp>
#include <TGUI/Outline.hpp>
#include <TGUI/Texture.hpp>
#include <TGUI/Color.hpp>
#include <TGUI/Font.hpp>
#include <TGUI/Any.hpp>
#include <cassert>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    struct RendererData;

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
            Outline,
            Texture,
            TextStyle,
            RendererData
        };


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Default constructor, to create an empty object
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ObjectConverter() :
            m_type{Type::None}
        {
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Stores a string for later retrieval
        ///
        /// @param string  String to store
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ObjectConverter(const sf::String& string)  :
            m_type      {Type::String},
            m_value     {string},
            m_serialized{true},
            m_string    {string}
        {
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Stores a font object for later retrieval
        ///
        /// @param font  Font to store
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ObjectConverter(Font font) :
            m_type {Type::Font},
            m_value{font}
        {
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Stores a color object for later retrieval
        ///
        /// @param color  Color to store
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ObjectConverter(sf::Color color) :
            ObjectConverter(Color(color))
        {
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Stores a color object for later retrieval
        ///
        /// @param color  Color to store
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ObjectConverter(Color color) :
            m_type {Type::Color},
            m_value{color}
        {
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Stores a number for later retrieval
        ///
        /// @param number  Number to store
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ObjectConverter(float number) :
            m_type {Type::Number},
            m_value{number}
        {
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Stores a outline object for later retrieval
        ///
        /// @param outline  Outline to store
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ObjectConverter(const Outline& outline) :
            m_type {Type::Outline},
            m_value{outline}
        {
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Stores a texture object for later retrieval
        ///
        /// @param texture  Texture to store
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ObjectConverter(const Texture& texture) :
            m_type {Type::Texture},
            m_value{texture}
        {
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Stores a color object for later retrieval
        ///
        /// @param color  Color to store
        ///
        /// To combine multiple text styles, wrap the value inside the TextStyle object before passing it.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ObjectConverter(sf::Text::Style style) :
            ObjectConverter(TextStyle(style))
        {
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Stores a text style for later retrieval
        ///
        /// @param style  Text style to store
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ObjectConverter(TextStyle style) :
            m_type {Type::TextStyle},
            m_value{style}
        {
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Stores render data for later retrieval
        ///
        /// @param data  Renderer data to store
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ObjectConverter(std::shared_ptr<RendererData> data) :
            m_type {Type::RendererData},
            m_value{data}
        {
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Retrieves the saved string
        ///
        /// @return The saved string or a serialized string
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const sf::String& getString();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Retrieves the saved font
        ///
        /// @return The saved font
        ///
        /// This function will assert when something other than a font was saved
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const Font& getFont();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Retrieves the saved color
        ///
        /// @return The saved color
        ///
        /// This function will assert when something other than a color was saved
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const Color& getColor();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Retrieves the saved outline
        ///
        /// @return The saved outline
        ///
        /// This function will assert when something other than a outline was saved
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const Outline& getOutline();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Retrieves the saved number
        ///
        /// @return The saved number
        ///
        /// This function will assert when something other than a number was saved
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        float getNumber();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Retrieves the saved texture
        ///
        /// @return The saved texture
        ///
        /// This function will assert when something other than a texture was saved
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Texture& getTexture();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Retrieves the saved text style
        ///
        /// @return Saved text style
        ///
        /// This function will assert when something other than a text style was saved
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const TextStyle& getTextStyle();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Retrieves the saved renderer data
        ///
        /// @return Saved renderer data
        ///
        /// This function will assert when something other than a renderer data was saved
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const std::shared_ptr<RendererData>& getRenderer();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Retrieves the type of the object that has been stored
        ///
        /// @return The stored object type
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Type getType() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    private:
        Type m_type = Type::None;
        Any  m_value;

        bool m_serialized = false;
        sf::String m_string;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // TGUI_OBJECT_CONVERTER_HPP
