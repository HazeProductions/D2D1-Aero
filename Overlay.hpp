/**
* @author       Haze-Productions, ReactiioN
* @last update  05.01.2016
*/
#pragma once
#include <Windows.h>
#include <unordered_map>
#include <dwmapi.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <dwmapi.h>
#include "Color.hpp"

#pragma comment( lib, "d2d1.lib" )
#pragma comment( lib, "dwrite.lib" )
#pragma comment( lib, "dwmapi.lib" )

namespace haze {
    
    class CDirect2DOverlay
    {
    public:
        
        /**
         * @brief      CDirect2DSurface is a render class for Direct2D.
         *             Every function does return a boolean which is only
         *             false when an interface wasn't initialized
         */
        class CDirect2DSurface
        {
        public:
            CDirect2DSurface( void ) = default;
            explicit CDirect2DSurface( const CDirect2DOverlay* pDirect2DOverlay );
            
            /**
             * @brief      Render a bordered box
             *
             * @param[in]  x          x-position
             * @param[in]  y          y-position
             * @param[in]  w          width
             * @param[in]  h          height
             * @param[in]  thickness  thickness
             * @param[in]  color      color
             *
             * @return     bool
             */
            bool BorderBox( float x, float y, float w, float h, float thickness, const Color& color ) const;
            
            /**
             * @brief      Render an outlined bordered box
             *
             * @param[in]  x                   x-position
             * @param[in]  y                   y-position
             * @param[in]  w                   width
             * @param[in]  h                   height
             * @param[in]  thickness           thickness
             * @param[in]  outlined_thickness  outlined thickness
             * @param[in]  color               color
             * @param[in]  outlined_color      outlined color
             *
             * @return     bool
             */
            bool BorderBox( float x, float y, float w, float h, float thickness, float outlined_thickness, const Color& color, const Color& outlined_color ) const;
            
            /**
             * @brief      Render a line
             *
             * @param[in]  x          x-initial-position
             * @param[in]  y          y-initial-position
             * @param[in]  xx         x-final-position
             * @param[in]  yy         y-final-posiiton
             * @param[in]  thickness  tickness
             * @param[in]  color      color
             *
             * @return     biik
             */
            bool Line( float x, float y, float xx, float yy, float thickness, const Color& color ) const;
            
            /**
             * @brief      Render a rounded rectangle
             *
             * @param[in]  x      x-position
             * @param[in]  y      y-position
             * @param[in]  w      width
             * @param[in]  h      height
             * @param[in]  x_rad  x-radius
             * @param[in]  y_rad  y-radius
             * @param[in]  color  color
             *
             * @return     bool
             */
            bool RoundedRect( float x, float y, float w, float h, float x_rad, float y_rad, const Color& color ) const;
            
            /**
             * @brief      Render an outlined rounded rectangle
             *
             * @param[in]  x               x-position
             * @param[in]  y               y-position
             * @param[in]  w               width
             * @param[in]  h               height
             * @param[in]  x_rad           x-radius
             * @param[in]  y_rad           y-radius
             * @param[in]  thickness       thickness
             * @param[in]  x_rad_outlined  outlined x-radius (whenever -1 was passed, the outlined radius will have the same value as the x_rad)
             * @param[in]  y_rad_outlined  outlined y-radius (whenever -1 was passed, the outlined radius will have the same value as the y_rad)
             * @param[in]  color           color
             * @param[in]  outlined        outlined color
             *
             * @return     bool
             */
            bool RoundedRect( float x, float y, float w, float h, float x_rad, float y_rad, float thickness, float x_rad_outlined, float y_rad_outlined, const Color& color, const Color& outlined ) const;
            
            /**
             * @brief      Render a rectangle
             *
             * @param[in]  x      x-position
             * @param[in]  y      y-position
             * @param[in]  w      width
             * @param[in]  h      height
             * @param[in]  color  color
             *
             * @return     bool
             */
            bool Rect( float x, float y, float w, float h, const Color& color ) const;
            
            /**
             * @brief      Reder an outlined rectangle
             *
             * @param[in]  x          x-position
             * @param[in]  y          y-position
             * @param[in]  w          width
             * @param[in]  h          height
             * @param[in]  thickness  thickness
             * @param[in]  color      color
             * @param[in]  outlined   outlined color
             *
             * @return     bool
             */
            bool Rect( float x, float y, float w, float h, float thickness, const Color& color, const Color& outlined ) const;
            
            /**
             * @brief      Get the frames per second.
             *
             * @return     uint64_t
             */
            uint64_t GetFramesPerSecond( void ) const;
            
            /**
             * @brief      Render a string
             *
             * @param[in]  x          x-position
             * @param[in]  y          y-position
             * @param[in]  font       buffer name
             * @param[in]  color      color
             * @param[in]  msg        string
             * @param[in]  <unnamed>  optional args
             *
             * @return     bool
             */
            bool String( float x, float y, const string& font, const Color& color, const char* msg, ... ) const;
            
            /**
             * @brief      Set the overlay instance.
             *
             * @param[in]  pDirect2DOverlay  The direct2 d overlay
             */
            void SetOverlayInstance( const CDirect2DOverlay* pDirect2DOverlay );

        private:
            const CDirect2DOverlay* m_pDirect2DOverlay = nullptr;
        };

        using RenderCallbackFn = void( *)( const CDirect2DSurface* );

    public:
        CDirect2DOverlay( void );
        ~CDirect2DOverlay( void );
        
        /**
         * @brief      Get the overlay resolution
         *
         * @return     array< int32_t, 2 >
         */
        array< int32_t, 2 >    GetSize( void ) const;

        /**
         * @brief      Create the aero overlay
         *
         * @param[in]  hWindow  pointer to the target window
         * @param[in]  wndproc  overlay window procedure
         *
         * @return     bool
         */
        bool                   Create( HWND hWindow, WNDPROC wndproc );
        
        /**
         * @brief      Creates the aero overlay
         *
         * @param[in]  targetWindowTitle  target window title
         * @param[in]  wndproc            overlay window procedure
         *
         * @return     bool
         */
        bool                   Create( const string& targetWindowTitle, WNDPROC wndproc );
        
        /**
         * @brief      Render frame
         *
         * @return     bool
         */
        bool                   Render( void );

        /**
         * @brief      Returns the initialized Direct2D render surface
         *
         * @return     CDirect2DSurface
         */
        CDirect2DSurface       Surface( void ) const;
        /**
         * @brief      Get the pointer to the D2D1 Factory
         *
         * @return     ID2D1Factory*
         */
        ID2D1Factory*          GetDirect2DFactory( void ) const;

        /**
         * @brief      Get the pointer to the window render target
         *
         * @return     ID2D1HwndRenderTarget*
         */
        ID2D1HwndRenderTarget* GetDirect2DHwndRenderTarget( void ) const;
        
        /**
         * @brief      Get the pointer to the direct write factory
         *
         * @return     IDWriteFactory*
         */
        IDWriteFactory*        GetDirectWriteFactory( void ) const;
        
        /**
         * @brief      Get the pointer to the solid color brush
         *
         * @return     ID2D1SolidColorBrush*
         */
        ID2D1SolidColorBrush*  GetDirect2DColorBrush( void ) const;
        
        /**
         * @brief      Get a pointer to a registered font interface
         *
         * @param[in]  name  buffer name
         *
         * @return     IDWriteTextFormat*
         */
        IDWriteTextFormat*     GetFont( const string& name ) const;
        
        /**
         * @brief      Create a new pointer to an IDWriteTextFormat interface
         *
         * @param[in]  name      buffer name
         * @param[in]  fontName  font name
         * @param[in]  size      font size
         * @param[in]  locale    locale
         * @param[in]  weight    font weight
         * @param[in]  style     font style
         * @param[in]  stretch   font stretch
         *
         * @return     Font.
         */
        IDWriteTextFormat*     GetFont( const string& name, const string& fontName, const float size = 12.f, const string& locale = "en-US",DWRITE_FONT_WEIGHT weight = DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE style = DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH stretch = DWRITE_FONT_STRETCH_NORMAL );
        
        /**
         * @brief      Get the frames per second.
         *
         * @return     uint64_t
         */
        uint64_t               GetFramesPerSecond( void ) const;
        
        /**
         * @brief      Add a render function which get executed inside the Render frame
         *
         * @param[in]  fn    function or function as lambda
         */
        void                   AddToRenderFrame( RenderCallbackFn fn );
        
        /**
         * @brief      Destroy the aero overlay
         */
        void                   Destroy( void );
        
        /**
         * @brief      Resizies the overlay equal to the passwed window
         *
         * @param[in]  hWindow  pointer to a window
         */
        void                   Resize( HWND hWindow );
        
        /**
         * @brief      Set the window class.
         *
         * @param[in]  <unnamed>  classname of the win32 window
         */
        void                   SetWindowClass( const string& );
        
        /**
         * @brief      Set the window title.
         *
         * @param[in]  <unnamed>  name of the win32 window
         */
        void                   SetWindowTitle( const string& );

    private:
        
        /**
         * @brief      Initialize all D2D1 and Direct Write interfaces
         *
         * @param[in]  hWindow  pointer to the aero overlay
         *
         * @return     bool
         */
        bool                   StartUp( HWND hWindow );
        
        /**
         * @brief      Calculate the frames per second.
         *
         * @param[in]  finished  has finished the render frame?
         */
        void                   CalculateFramesPerSecond( bool finished );

    private:
        static constexpr MARGINS   DWM_MARGINS = { -1, -1, -1, -1 };
        CDirect2DSurface           m_Direct2DSurface;
        array< int32_t, 2 >        m_cPosition;
        array< int32_t, 2 >        m_cSize;
        array< string, 2 >         m_cWindowData;
        unordered_map< string,
            IDWriteTextFormat* >   m_cCustomFonts;
        vector< RenderCallbackFn > m_cRenderCallbacks;
        HWND                       m_hOvHwnd = nullptr;
        HWND                       m_hTargetHwnd = nullptr;
        uint64_t                   m_nFramesPerSeconds = 0;
        ID2D1Factory*              m_pDirect2DFactory = nullptr;
        ID2D1HwndRenderTarget*     m_pDirect2DHwndRenderTarget = nullptr;
        IDWriteFactory*            m_pDirectWriteFactory = nullptr;
        ID2D1SolidColorBrush*      m_pDiect2DColorBrush = nullptr;
    };
}