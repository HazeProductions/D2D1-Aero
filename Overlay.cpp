#include "Overlay.hpp"
#include <codecvt>
using namespace haze;

template< class T >
void SafeRelease( T** ppInterface )
{
    if( ppInterface && *ppInterface ) {
        ( *ppInterface )->Release();
        *ppInterface = nullptr;
    }
}

wstring string_to_wstring( const string& narrow )
{
    wstring_convert< codecvt_utf8_utf16< wchar_t > > converter;
    return converter.from_bytes( narrow );
}

CDirect2DOverlay::CDirect2DSurface::CDirect2DSurface( const CDirect2DOverlay* pDirect2DOverlay )
{
    SetOverlayInstance( pDirect2DOverlay );
}

bool CDirect2DOverlay::CDirect2DSurface::BorderBox( float x, float y, float w, float h, float thickness, const Color& color ) const
{
    if( !m_pDirect2DOverlay ) {
        return false;
    }

    return Rect( x, y, w, thickness, color )     &&
           Rect( x, y, thickness, h, color )     &&
           Rect( x + w, y, thickness, h, color ) &&
           Rect( x, y + h, w + thickness, thickness, color );
}

bool CDirect2DOverlay::CDirect2DSurface::BorderBox( float x, float y, float w, float h, float thickness, float outlined_thickness, const Color& color, const Color& outlined_color ) const
{
    return BorderBox( x, y, w, h, thickness, color ) &&
           BorderBox( x - outlined_thickness, y - outlined_thickness, w + outlined_thickness + thickness, h + outlined_thickness + thickness, outlined_thickness, outlined_color ) &&
           BorderBox( x + thickness, y + thickness, w - outlined_thickness - thickness, h - outlined_thickness - thickness, outlined_thickness, outlined_color );
}

bool CDirect2DOverlay::CDirect2DSurface::Line( float x, float y, float xx, float yy, float thickness, const Color& color ) const
{
    if( !m_pDirect2DOverlay ) {
        return false;
    }

    auto* pDirect2DColorBrush = m_pDirect2DOverlay->GetDirect2DColorBrush();
    if( !pDirect2DColorBrush ) {
        return false;
    }

    auto* pDirect2DHwndRenderTarget = m_pDirect2DOverlay->GetDirect2DHwndRenderTarget();
    if( !pDirect2DHwndRenderTarget ) {
        return false;
    }

    pDirect2DColorBrush->SetColor( D2D1::ColorF( color.hex() ) );
    pDirect2DHwndRenderTarget->DrawLine( { x, y }, { xx, yy }, pDirect2DColorBrush, thickness );

    return true;
}

bool CDirect2DOverlay::CDirect2DSurface::RoundedRect( float x, float y, float w, float h, float x_rad, float y_rad, const Color& color ) const
{
    if( !m_pDirect2DOverlay ) {
        return false;
    }

    auto* pDirect2DColorBrush = m_pDirect2DOverlay->GetDirect2DColorBrush();
    if( !pDirect2DColorBrush ) {
        return false;
    }

    auto* pDirect2DHwndRenderTarget = m_pDirect2DOverlay->GetDirect2DHwndRenderTarget();
    if( !pDirect2DHwndRenderTarget ) {
        return false;
    }

    auto rect = D2D1::RoundedRect( D2D1::RectF( x, y, x + w, y + h ), x_rad, y_rad );
    pDirect2DColorBrush->SetColor( D2D1::ColorF( color.hex() ) );
    pDirect2DHwndRenderTarget->FillRoundedRectangle( &rect, pDirect2DColorBrush );

    return true;
}

bool CDirect2DOverlay::CDirect2DSurface::RoundedRect( float x, float y, float w, float h, float x_rad, float y_rad, float thickness, float x_rad_outlined, float y_rad_outlined, const Color& color, const Color& outlined ) const
{
    return RoundedRect( x - thickness, y - thickness, w + thickness * 2.f, h + thickness * 2.f, x_rad_outlined, y_rad_outlined, outlined ) &&
           RoundedRect( x, y, w, h, x_rad, y_rad, color );
}

bool CDirect2DOverlay::CDirect2DSurface::Rect( float x, float y, float w, float h, const Color& color ) const
{
    if( !m_pDirect2DOverlay ) {
        return false;
    }

    auto* pDirect2DColorBrush = m_pDirect2DOverlay->GetDirect2DColorBrush();
    if( !pDirect2DColorBrush ) {
        return false;
    }

    auto* pDirect2DHwndRenderTarget = m_pDirect2DOverlay->GetDirect2DHwndRenderTarget();
    if( !pDirect2DHwndRenderTarget ) {
        return false;
    }

    auto rect = D2D1::RectF( x, y, x + w, y + h );    
    pDirect2DColorBrush->SetColor( D2D1::ColorF( color.hex() ) );
    pDirect2DHwndRenderTarget->FillRectangle( &rect, pDirect2DColorBrush );

    return true;
}

bool CDirect2DOverlay::CDirect2DSurface::Rect( float x, float y, float w, float h, float thickness, const Color& color, const Color& outlined ) const
{
    return Rect( x, y, w, h, color ) &&
           BorderBox( x - thickness, y - thickness, w + thickness, h + thickness, thickness, outlined );
}

uint64_t CDirect2DOverlay::CDirect2DSurface::GetFramesPerSecond( void ) const
{
    return m_pDirect2DOverlay ? m_pDirect2DOverlay->GetFramesPerSecond() : 0;
}

bool CDirect2DOverlay::CDirect2DSurface::String( float x, float y, const string& font, const Color& color, const char* msg, ... ) const
{
    if( !m_pDirect2DOverlay ) {
        return false;
    }

    auto* pDirect2DColorBrush = m_pDirect2DOverlay->GetDirect2DColorBrush();
    if( !pDirect2DColorBrush ) {
        return false;
    }

    auto* pDirect2DHwndRenderTarget = m_pDirect2DOverlay->GetDirect2DHwndRenderTarget();
    if( !pDirect2DHwndRenderTarget ) {
        return false;
    }

    auto* pDirectWriteTextFormat = m_pDirect2DOverlay->GetFont( font );
    if( !pDirectWriteTextFormat ) {
        return false;
    }

    const auto cSize = m_pDirect2DOverlay->GetSize();

    va_list args;
    va_start( args, msg );
    char buffer[ 0x400 ];
    vsprintf_s( buffer, msg, args );
    va_end( args );

    auto w = string_to_wstring( buffer );
    auto rect = D2D1::RectF( x, y, x + static_cast< float >( cSize[ 0 ] ), y + static_cast< float >( cSize[ 1 ] ) );

    pDirect2DColorBrush->SetColor( D2D1::ColorF( color.hex() ) );
    pDirect2DHwndRenderTarget->DrawText( w.c_str(), w.length(), pDirectWriteTextFormat, &rect, pDirect2DColorBrush );

    return true;
}

void CDirect2DOverlay::CDirect2DSurface::SetOverlayInstance( const CDirect2DOverlay* pDirect2DOverlay )
{
    m_pDirect2DOverlay = pDirect2DOverlay;
}


CDirect2DOverlay::CDirect2DOverlay( void ) :
    m_cPosition( { CW_USEDEFAULT, CW_USEDEFAULT } ),
    m_cSize( { 800, 600 } )
{
    SetWindowClass( "Overlay" );
    SetWindowTitle( "D2DOverlay" );
    m_Direct2DSurface.SetOverlayInstance( this );
}

CDirect2DOverlay::~CDirect2DOverlay( void )
{
    Destroy();
}

array< int32_t, 2 > CDirect2DOverlay::GetSize( void ) const
{
    return m_cSize;
}

bool CDirect2DOverlay::Create( HWND hWindow, WNDPROC wndproc )
{

#ifdef _UNICODE
    auto windowClass = string_to_wstring( m_cWindowData[ 0 ] );
    auto windowTitle = string_to_wstring( m_cWindowData[ 1 ] );
#endif

    WNDCLASSEX wc;
    wc.cbSize = sizeof( WNDCLASSEX );
    wc.style = 0;
    wc.lpfnWndProc = wndproc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = nullptr;
    wc.hIcon = LoadIcon( nullptr, IDI_APPLICATION );
    wc.hCursor = LoadCursor( nullptr, IDC_ARROW );
    wc.hbrBackground = reinterpret_cast< HBRUSH >( COLOR_WINDOW + 1 );
    wc.lpszMenuName = nullptr;
#ifdef _UNICODE
    wc.lpszClassName = windowClass.c_str(),
#else
    wc.lpszClassName = m_cWindowData[ 0 ].c_str();
#endif
    wc.hIconSm = LoadIcon( nullptr, IDI_APPLICATION );

    if( !RegisterClassEx( &wc ) ) {
        return false;
    }

    m_hOvHwnd = CreateWindowEx( WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED,
#ifdef _UNICODE
        windowClass.c_str(),
        windowTitle.c_str(),
#else
        m_cWindowData[ 0 ].c_str(),
        m_cWindowData[ 1 ].c_str(),
#endif
        WS_POPUP,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        800,
        600,
        nullptr, nullptr, nullptr, nullptr );
    if( !m_hOvHwnd ) {
        return false;
    }


    SetLayeredWindowAttributes( m_hOvHwnd, RGB( 0, 0, 0 ), 255, ULW_COLORKEY | LWA_ALPHA );
    if( FAILED( DwmExtendFrameIntoClientArea( m_hOvHwnd, &DWM_MARGINS ) ) ) {
        return false;
    }

    ShowWindow( m_hOvHwnd, SW_SHOWDEFAULT );
    UpdateWindow( m_hOvHwnd );

    Resize( hWindow );
    if( !StartUp( m_hOvHwnd ) ) {
        return false;
    }

    m_hTargetHwnd = hWindow;
    return true;
}

bool CDirect2DOverlay::Create( const string& targetWindowTitle, WNDPROC wndproc )
{
    if( targetWindowTitle.empty() || !wndproc ) {
        return false;
    }

    m_hTargetHwnd = FindWindowA( nullptr, targetWindowTitle.c_str() );
    if( !m_hTargetHwnd ) {
        return false;
    }

    return Create( m_hTargetHwnd, wndproc );
}

ID2D1Factory* CDirect2DOverlay::GetDirect2DFactory( void ) const
{
    return m_pDirect2DFactory;
}

ID2D1HwndRenderTarget* CDirect2DOverlay::GetDirect2DHwndRenderTarget( void ) const
{
    return m_pDirect2DHwndRenderTarget;
}

IDWriteFactory* CDirect2DOverlay::GetDirectWriteFactory( void ) const
{
    return m_pDirectWriteFactory;
}

ID2D1SolidColorBrush* CDirect2DOverlay::GetDirect2DColorBrush( void ) const
{
    return m_pDiect2DColorBrush;
}

IDWriteTextFormat* CDirect2DOverlay::GetFont( const string& name ) const
{
    if( name.empty() || !m_cCustomFonts.count( name ) ) {
        return nullptr;
    }
    return m_cCustomFonts.at( name );
}

IDWriteTextFormat* CDirect2DOverlay::GetFont( const string& name, const string& fontName, const float size, const string& locale, DWRITE_FONT_WEIGHT weight, DWRITE_FONT_STYLE style, DWRITE_FONT_STRETCH stretch )
{
    if( !m_pDirectWriteFactory || name.empty() || fontName.empty() || locale.empty() ) {
        return nullptr;
    }
    if( !!m_cCustomFonts.count( name ) ) {
        return m_cCustomFonts[ name ];
    }

    IDWriteTextFormat* pDirectWriteTextFormat = nullptr;
    if( FAILED( m_pDirectWriteFactory->CreateTextFormat( string_to_wstring( fontName ).c_str(), nullptr, weight, style, stretch, size, string_to_wstring( locale ).c_str(), &pDirectWriteTextFormat ) ) ) {
        return nullptr;
    }

    m_cCustomFonts.insert( make_pair( name, pDirectWriteTextFormat ) );
    return m_cCustomFonts[ name ];
}

CDirect2DOverlay::CDirect2DSurface CDirect2DOverlay::Surface( void ) const
{
    return m_Direct2DSurface;
}

bool CDirect2DOverlay::Render( void )
{
    MSG msg;
    if( PeekMessage( &msg, m_hOvHwnd, 0, 0, PM_REMOVE ) ) {
        if( msg.message == WM_QUIT ) {
            return false;
        }

        TranslateMessage( &msg );
        DispatchMessage( &msg );
    }

    m_pDirect2DHwndRenderTarget->BeginDraw();
    m_pDirect2DHwndRenderTarget->SetTransform( D2D1::Matrix3x2F::Identity() );
    m_pDirect2DHwndRenderTarget->Clear();

    if( m_hTargetHwnd == GetForegroundWindow() ) {
        CalculateFramesPerSecond( false );

        for( auto& fn : m_cRenderCallbacks ) {
            fn( &m_Direct2DSurface );
        }

        CalculateFramesPerSecond( true );
    }

    m_pDirect2DHwndRenderTarget->EndDraw();
    
    return true;
}

uint64_t CDirect2DOverlay::GetFramesPerSecond( void ) const
{
    return m_nFramesPerSeconds;
}

void CDirect2DOverlay::AddToRenderFrame( RenderCallbackFn fn )
{
    if( fn ) {
        m_cRenderCallbacks.push_back( fn );
    }
}

void CDirect2DOverlay::Destroy( void )
{
    if( !m_hOvHwnd ) {
        return;
    }

    // Unregister the overlay window class and destroy the window
    UnregisterClassA( m_cWindowData[ 0 ].c_str(), nullptr );
    DestroyWindow( m_hOvHwnd );
    m_hOvHwnd = nullptr;

    // Release each interface pointer
    SafeRelease( &m_pDirect2DFactory );
    SafeRelease( &m_pDirect2DHwndRenderTarget );
    SafeRelease( &m_pDirectWriteFactory );
    SafeRelease( &m_pDiect2DColorBrush );

    // and also the font interface pointers
    for( auto& _pair : m_cCustomFonts ) {
        SafeRelease( &_pair.second );
    }
}

void CDirect2DOverlay::Resize( HWND hWindow )
{
    RECT rc_client;
    GetClientRect( hWindow, &rc_client );

    m_cSize[ 0 ] = static_cast< int32_t >( rc_client.right );
    m_cSize[ 1 ] = static_cast< int32_t >( rc_client.bottom );

    RECT rc_window;
    GetWindowRect( hWindow, &rc_window );

    POINT ptDifference;
    ptDifference.x = ptDifference.y = 0;
    ClientToScreen( hWindow, &ptDifference );

    m_cPosition[ 0 ] = static_cast< int32_t >( rc_window.left + ( ptDifference.x - rc_window.left ) );
    m_cPosition[ 1 ] = static_cast< int32_t >( rc_window.top + ( ptDifference.y - rc_window.top ) );
    if( !m_cPosition[ 0 ] ) {
        --m_cPosition[ 0 ];
        ++m_cSize[ 0 ];
    }
    if( !m_cPosition[ 1 ] ) {
        --m_cPosition[ 1 ];
        ++m_cSize[ 1 ];
    }

    MoveWindow( m_hOvHwnd, m_cPosition[ 0 ], m_cPosition[ 1 ], m_cSize[ 0 ], m_cSize[ 1 ], TRUE );
}

void CDirect2DOverlay::SetWindowClass( const string& windowClass )
{
    m_cWindowData[ 0 ] = windowClass;
}

void CDirect2DOverlay::SetWindowTitle( const string& windowTitle )
{
    m_cWindowData[ 1 ] = windowTitle;
}

bool CDirect2DOverlay::StartUp( HWND hWindow )
{
    auto hr = hWindow ? S_OK : E_FAIL;
    if( SUCCEEDED( hr ) ) {
        hr = D2D1CreateFactory( D2D1_FACTORY_TYPE_MULTI_THREADED, __uuidof( ID2D1Factory ), nullptr, reinterpret_cast< void** >( &m_pDirect2DFactory ) );
    }
    if( SUCCEEDED( hr ) ) {
        RECT rect;
        GetClientRect( hWindow, &rect );

        auto size = D2D1::SizeU( rect.right - rect.left, rect.bottom - rect.top );
        hr = m_pDirect2DFactory->CreateHwndRenderTarget( D2D1::RenderTargetProperties( D2D1_RENDER_TARGET_TYPE_HARDWARE, D2D1::PixelFormat( DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED ) ), D2D1::HwndRenderTargetProperties( hWindow, size, D2D1_PRESENT_OPTIONS_IMMEDIATELY ), &m_pDirect2DHwndRenderTarget );
    }
    if( SUCCEEDED( hr ) ) {
        hr = DWriteCreateFactory( DWRITE_FACTORY_TYPE_SHARED, __uuidof( IDWriteFactory ), reinterpret_cast< IUnknown** >( &m_pDirectWriteFactory ) );
    }
    if( SUCCEEDED( hr ) ) {
        hr = m_pDirect2DHwndRenderTarget->CreateSolidColorBrush( D2D1::ColorF( 0xFFFFFFFF ), &m_pDiect2DColorBrush );
    }
    return SUCCEEDED( hr );
}

void CDirect2DOverlay::CalculateFramesPerSecond( bool finished )
{
    static uint64_t last = 0, count = 0;
    if( !finished ) {
        if( GetTickCount64() - last > static_cast< uint32_t >( 333 ) ) {
            m_nFramesPerSeconds = static_cast< uint64_t >( static_cast< float >( count ) * 3.333333333f - 3.f );
            last = GetTickCount64();
            count = 0;
        }
    }
    else {
        ++count;
    }
}