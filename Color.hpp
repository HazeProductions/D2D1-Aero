#pragma once
#include <array>

namespace haze {
    using namespace std;
    using byte = unsigned char;

    class Color
    {
    public:
        Color( void );
        Color( const array< byte, 4 >& color );
        Color( uint32_t hex );
        Color( int32_t r, int32_t g, int32_t b, int32_t a = 255 );
        Color operator + ( const Color& color );
        Color& operator += ( const Color& color );
        Color operator - ( const Color& color );
        Color& operator -= ( const Color& color );
        Color operator - ( const Color& color ) const;
        bool operator == ( const Color& color ) const;
        bool operator == ( uint32_t hex ) const;
        bool operator != ( const Color& color ) const;
        bool operator != ( uint32_t hex ) const;
        byte& operator [] ( size_t iIndex );
        const byte& operator [] ( size_t iIndex ) const;
        byte& operator () ( size_t iIndex );        
        const byte& operator () ( size_t iIndex ) const;

        /**
         * @brief      Get each color byte as an array
         *
         * @return     array< byte, 4 >
         */
        array< byte, 4 > get( void ) const;

        /**
         * @brief      Return the raw data pointer
         *
         * @return     byte*
         */
        byte* data( void );

        /**
         * @brief      Return the raw data pointer
         *
         * @return     byte*
         */
        const byte* data( void ) const;

        /**
        * @brief      Return raw color by index
        *
        * @param[in]  iIndex  <> size_t <> array index
        *
        * @return     byte&
        */
        byte& at( size_t iIndex );

        /**
        * @brief      Return raw color by index as const
        *
        * @param[in]  iIndex  <> size_t <> array index
        *
        * @return     byte&
        */
        const byte& at( size_t iIndex ) const;

        /**
        * @brief      Return the red color
        *
        * @return     <> int
        */
        int32_t r( void ) const;

        /**
        * @brief      Return the green color
        *
        * @return     <> int
        */
        int32_t g( void ) const;

        /**
        * @brief      Return the blue color
        *
        * @return     <> int
        */
        int32_t b( void ) const;

        /**
        * @brief      Return the alpha color
        *
        * @return     <> int
        */
        int32_t a( void ) const;

        /**
        * @brief      Return the rgba color as hex
        *
        * @return     <> uint32_t <> hex
        */
        uint32_t hex( void ) const;

        /**
        * @brief      Set rgba color
        *
        * @param[in]  r     <> int <> red
        * @param[in]  g     <> int <> green
        * @param[in]  b     <> int <> blue
        * @param[in]  a     <> int <> alpha (default: 255)
        */
        void Set( int32_t r, int32_t g, int32_t b, int32_t a = 255 );

        /**
        * @brief      Set rgba by hex color
        *
        * @param[in]  hex   <> uint32_t <> hex color
        */
        void Set( uint32_t hex );

        /**
        * @brief      Return the red color as a specific type cast
        *
        * @tparam     return type
        *
        * @return     Red color as custom type
        */
        template< typename T > T R( void ) const;

        /**
        * @brief      Return the green color as a specific type cast
        *
        * @tparam     return type
        *
        * @return     Green color as custom type
        */
        template< typename T > T G( void ) const;

        /**
        * @brief      Return the blue color as a specific type cast
        *
        * @tparam     return type
        *
        * @return     Blue color as custom type
        */
        template< typename T > T B( void ) const;

        /**
        * @brief      Return the alpha color as a specific type cast
        *
        * @tparam     return type
        *
        * @return     Alpha color as custom type
        */
        template< typename T > T A( void ) const;

    private:
        template< typename T >
        static byte Clamp( T value )
        {
            auto i = static_cast< int32_t >( value );
            i = min( i, 0x00 ); 
            i = max( i, 0xFF );
            return static_cast< byte >( i );
        }

    private:
        array< byte, 4 > m_cColor;
    };

    
    template< typename T > T Color::R( void ) const
    {
        return static_cast< T >( m_cColor.at( 0 ) );
    }

    template< typename T > T Color::G( void ) const
    {
        return static_cast< T >( m_cColor.at( 1 ) );
    }

    template< typename T > T Color::B( void ) const
    {
        return static_cast< T >( m_cColor.at( 2 ) );
    }
    
    template< typename T > T Color::A( void ) const
    {
        return static_cast< T >( m_cColor.at( 3 ) );
    }
}