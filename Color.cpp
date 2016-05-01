#include "Color.hpp"
using namespace haze;

Color::Color( void )
{
    m_cColor.fill( 255 );
}

Color::Color( const array< byte, 4 >& color ) :
    m_cColor( color )
{
}

Color::Color( uint32_t hex )
{
    Set( hex );
}

Color::Color( int32_t r, int32_t g, int32_t b, int32_t a )
{
    Set( r, g, b, a );
}

Color Color::operator - ( const Color& color ) const
{
    return Color( r() - color.r(), g() - color.g(), b() - color.b(), a() - color.a() );
}

bool Color::operator == ( const Color& color ) const
{
    for( size_t i = 0; i < 4; ++i ) {
        if( at( i ) != color( i ) ) {
            return false;
        }
    }
    return true;
}

bool Color::operator == ( uint32_t Hex ) const
{
    return hex() == Hex;
}

bool Color::operator != ( const Color& color ) const
{
    return !( *this == color );
}

bool Color::operator != ( uint32_t hex ) const
{
    return !( *this == hex );
}

byte& Color::operator [] ( size_t iIndex )
{
    return at( iIndex );
}

const byte& Color::operator [] ( size_t iIndex ) const
{
    return at( iIndex );
}

byte& Color::operator () ( size_t iIndex )
{
    return at( iIndex );
}

const byte& Color::operator () ( size_t iIndex ) const
{
    return at( iIndex );
}

array< byte, 4 > Color::get( void ) const
{
    return m_cColor;
}

byte* Color::data( void )
{
    return m_cColor.data();
}

const byte* Color::data( void ) const
{
    return m_cColor.data();
}

byte& Color::at( size_t iIndex )
{
    if( iIndex > 3 ) {
        iIndex = 3;
    }
    return m_cColor.at( iIndex );
}

const byte& Color::at( size_t iIndex ) const
{
    if( iIndex > 3 ) {
        iIndex = 3;
    }
    return m_cColor.at( iIndex );
}

Color Color::operator + ( const Color& color )
{
    array< byte, 4 > cColor;
    for( size_t i = 0; i < 4; ++i ) {
        cColor.at( i ) = Clamp( at( i ) + color( i ) );
    }
    return Color( cColor );
}

Color& Color::operator += ( const Color& color )
{
    for( size_t i = 0; i < 4; ++i ) {
        at( i ) = Clamp( at( i ) + color( i ) );
    }
    return *this;
}

Color Color::operator - ( const Color& color )
{
    array< byte, 4 > cColor;
    for( size_t i = 0; i < 4; ++i ) {
        cColor.at( i ) = Clamp( at( i ) - color( i ) );
    }
    return Color( cColor );
}

Color& Color::operator -= ( const Color& color )
{
    for( size_t i = 0; i < 4; ++i ) {
        at( i ) = Clamp( at( i ) - color( i ) );
    }
    return *this;
}

int32_t Color::r( void ) const
{
    return static_cast< int32_t >( at( 0 ) );
}

int32_t Color::g( void ) const
{
    return static_cast< int32_t >( at( 1 ) );
}

int32_t Color::b( void ) const
{
    return static_cast< int32_t >( at( 2 ) );
}

int32_t Color::a( void ) const
{
    return static_cast< int32_t >( at( 3 ) );
}

uint32_t Color::hex( void ) const
{
    return static_cast< uint32_t >( ( at( 3 ) << 24 ) | ( at( 0 ) << 16 ) | ( at( 1 ) << 8 ) | at( 2 ) );
}

void Color::Set( int32_t r, int32_t g, int32_t b, int32_t a )
{
    at( 0 ) = Clamp( r );
    at( 1 ) = Clamp( g );
    at( 2 ) = Clamp( b );
    at( 3 ) = Clamp( a );
}

void Color::Set( uint32_t hex )
{
    at( 3 ) = static_cast< byte >( ( hex >> 24 ) & 0xFF );
    at( 0 ) = static_cast< byte >( ( hex >> 16 ) & 0xFF );
    at( 1 ) = static_cast< byte >( ( hex >> 8 ) & 0xFF );
    at( 2 ) = static_cast< byte >( hex & 0xFF );
}