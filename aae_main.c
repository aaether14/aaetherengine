#include "aae_memory_allocator.h"
#include "aae_allocators.h"
#include "string/aae_string.h"
#include "ctype/aae_ctype.h"
#include "os/linux/aae_os_functions.h" 
#include <pthread.h>


#include <X11/Xlib.h>
#include <GL/glx.h>



extern aae_allocator    aae_mallocator;
static Display          *m_display;
static GLint            m_visual_attributes[] = {
    GLX_X_RENDERABLE,   True, 
    GLX_DRAWABLE_TYPE,  GLX_WINDOW_BIT,
    GLX_RENDER_TYPE,    GLX_RGBA_BIT,
    GLX_X_VISUAL_TYPE,  GLX_TRUE_COLOR,
    GLX_RED_SIZE,       8,
    GLX_GREEN_SIZE,     8,
    GLX_BLUE_SIZE,      8,
    GLX_ALPHA_SIZE,     8,
    GLX_DEPTH_SIZE,     24,
    GLX_STENCIL_SIZE,   8,
    GLX_DOUBLEBUFFER,   True,
    None
};
static i32          glx_major, glx_minor;



AAE_EXTERN_TOOL i32 aae_main()
{
    
    byte buffer[1000];
    aae_sprintf(buffer, "%ilu - %c - should also work with %4F ,,,,flo%5fats :))) --- %B", 534, 'Z', -0.052, 15.132, 25);
    aae_write(stderr, buffer, aae_strlen(buffer));

    m_display = XOpenDisplay(NULL);
    if (!m_display)
    {
        aae_write(stderr, "Could not connect to X server!\n", 31);
        aae_exit(0);
    }
    if (!glXQueryVersion(m_display, &glx_major, &glx_minor) || glx_major < 1 || glx_minor < 3)
    {
        aae_write(stderr, "Invalid GLX version!\n", 21);
        aae_exit(0);
    }
    i32 m_framebuffer_count;
    GLXFBConfig* m_framebuffer_config = glXChooseFBConfig(m_display, XDefaultScreen(m_display), m_visual_attributes, &m_framebuffer_count);
    if (!m_framebuffer_config)
    {
        aae_write(stderr, "Failed to retrieve a framebuffer configuarion!\n", 47);
        aae_exit(0);
    }
    for (i32 it = 0; it < m_framebuffer_count; ++it)
    {

    }
    XFree(m_framebuffer_config);
    return 0;
}



