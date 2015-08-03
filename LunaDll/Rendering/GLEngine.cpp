#include <windows.h>
#include <gl/glew.h>
#include "GLEngine.h"
#include "../Defines.h"
#include "../Globals.h"
#include "RenderUtils.h"
#include "GLCompat.h"
#include "GLDraw.h"
#include "GLTextureStore.h"
#include "GLContextManager.h"

GLEngine::GLEngine() :
    mInitialized(false), mHadError(false),
    mEnabled(true), mBitwiseCompat(true),
    mFB(0), mColorRB(0), mDepthRB(0),
    mBufTex(NULL, 800, 600) {
}

GLEngine::~GLEngine() {
}

void GLEngine::Init() {
    if (!g_GLContextManager.IsInitialized()) return;

    if (mInitialized || mHadError) return;
    mHadError = true;

#if 1
    // Set up framebuffer object
    glGenFramebuffersANY(1, &mFB);
    GLERRORCHECK();
    glBindFramebufferANY(GL_FRAMEBUFFER_EXT, mFB);
    GLERRORCHECK();

    glGenTextures(1, &mBufTex.name);
    GLERRORCHECK();
    g_GLDraw.BindTexture(&mBufTex);
    GLERRORCHECK();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mBufTex.pw, mBufTex.ph, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    GLERRORCHECK();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    GLERRORCHECK();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    GLERRORCHECK();

    glGenRenderbuffersANY(1, &mDepthRB);
    GLERRORCHECK();
    glBindRenderbufferANY(GL_RENDERBUFFER_EXT, mDepthRB);
    GLERRORCHECK();
    glRenderbufferStorageANY(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, mBufTex.pw, mBufTex.ph);
    GLERRORCHECK();
    glFramebufferRenderbufferANY(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, mDepthRB);
    GLERRORCHECK();
    
    glFramebufferTexture2DANY(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, mBufTex.name, 0);
    GLERRORCHECK();

    GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0_EXT };
    glDrawBuffers(1, DrawBuffers);
    GLERRORCHECK();

    GLenum status = glCheckFramebufferStatusANY(GL_FRAMEBUFFER_EXT);
    GLERRORCHECK();
    if (status != GL_FRAMEBUFFER_COMPLETE_EXT) {
        dbgboxA("error setting up");
    }

    // Bind framebuffer
    glBindFramebufferANY(GL_FRAMEBUFFER_EXT, mFB);
    GLERRORCHECK();
    glClearColor(0.0, 0.0, 0.0, 1.0);
    GLERRORCHECK();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLERRORCHECK();

    // Set projection (test)
    glLoadIdentity();
    GLERRORCHECK();
    glOrtho(0.0f, (float)mBufTex.pw, 0.0f, (float)mBufTex.ph, -1.0f, 1.0f);
    GLERRORCHECK();
    glColor3f(1, 1, 1);
    GLERRORCHECK();
    glDisable(GL_LIGHTING);
    GLERRORCHECK();
    glDisable(GL_DEPTH_TEST);
    GLERRORCHECK();
    glDisable(GL_CULL_FACE);
    GLERRORCHECK();
    glEnable(GL_BLEND);
    GLERRORCHECK();
    glEnable(GL_TEXTURE_2D);
    GLERRORCHECK();
    glEnableClientState(GL_VERTEX_ARRAY);
    GLERRORCHECK();
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    GLERRORCHECK();
#endif
    
    mInitialized = true;
    mHadError = false;
}

void GLEngine::ClearSMBXSprites() {
    g_GLDraw.UnbindTexture();
    g_GLTextureStore.ClearSMBXSprites();
}

void GLEngine::ClearLunaTexture(const BMPBox& bmp) {
    g_GLDraw.UnbindTexture();
    g_GLTextureStore.ClearLunaTexture(bmp);
}

void GLEngine::EmulatedBitBlt(int nXDest, int nYDest, int nWidth, int nHeight, HDC hdcSrc, int nXSrc, int nYSrc, DWORD dwRop)
{
    if (!mInitialized) Init();
    if (!mInitialized) return;

    if (dwRop == BLACKNESS || dwRop == 0x10)
    {
        glColor3f(0.0f, 0.0f, 0.0f);
        g_GLDraw.DrawRectangle(nXDest, nYDest, nWidth, nHeight);
        glColor3f(1, 1, 1);
        return;
    }

    GLDraw::RenderMode mode;
    switch (dwRop) {
    case SRCAND:
        mode = mBitwiseCompat ? GLDraw::RENDER_MODE_AND : GLDraw::RENDER_MODE_MULTIPLY;
        break;
    case SRCPAINT:
        mode = mBitwiseCompat ? GLDraw::RENDER_MODE_OR : GLDraw::RENDER_MODE_MAX;
        break;
    default:
        mode = GLDraw::RENDER_MODE_ALPHA;
        break;
    }

    const GLSprite* sprite = g_GLTextureStore.SpriteFromSMBXBitmap(hdcSrc);
    if (sprite == NULL) {
        return;
    }

    sprite->Draw(nXDest, nYDest, nWidth, nHeight, nXSrc, nYSrc, mode);
}

BOOL GLEngine::EmulatedStretchBlt(HDC hdcDest, int nXOriginDest, int nYOriginDest, int nWidthDest, int nHeightDest,
    HDC hdcSrc, int nXOriginSrc, int nYOriginSrc, int nWidthSrc, int nHeightSrc,
    DWORD dwRop)
{
    if (!g_GLContextManager.IsInitialized()) {
        if (!g_GLContextManager.Init(hdcDest)) {
            dbgboxA("Failed to init...");
        } else {
            //dbgboxA("Initialized");
        }
    }
    if (!mInitialized) Init();
    if (!mInitialized) return FALSE;

    g_GLDraw.UnbindTexture();

    // Unbind the texture from the framebuffer
    glBindFramebufferANY(GL_FRAMEBUFFER_EXT, 0);
    GLERRORCHECK();

    // Get window size
    RECT clientRect;
    GetClientRect(WindowFromDC(hdcDest), &clientRect);
    int32_t windowWidth = clientRect.right - clientRect.left;
    int32_t windowHeight = clientRect.bottom - clientRect.top;

    // Implement letterboxing correction
    float scaledWidth = windowWidth / 800.0f;
    float scaledHeight = windowHeight / 600.0f;
    float xScale = 1.0f;
    float yScale = 1.0f;
    if (scaledWidth > scaledHeight) {
        xScale = scaledHeight / scaledWidth;
    } else if (scaledWidth < scaledHeight) {
        yScale = scaledWidth / scaledHeight;
    }
    float xOffset = (windowWidth / xScale - windowWidth) * 0.5f;
    float yOffset = (windowHeight / yScale - windowHeight) * 0.5f;

    // Set viewport for window size
    glViewport(0, 0, windowWidth, windowHeight);
    GLERRORCHECK();
    glLoadIdentity();
    GLERRORCHECK();
    glOrtho(-xOffset, (float)windowWidth + xOffset, (float)windowHeight + yOffset, -yOffset, -1.0f, 1.0f);
    GLERRORCHECK();

    // Draw the buffer, flipped/stretched as appropriate
    g_GLDraw.DrawStretched(nXOriginDest, nYOriginDest, nWidthDest, nHeightDest, &mBufTex, nXOriginSrc, nYOriginSrc, nWidthSrc, nHeightSrc);
    GLERRORCHECK();
    glFlush();
    GLERRORCHECK();

    // Get ready to draw some more
    glBindFramebufferANY(GL_FRAMEBUFFER_EXT, mFB);
    GLERRORCHECK();
    glViewport(0, 0, mBufTex.pw, mBufTex.ph);
    GLERRORCHECK();
    glLoadIdentity();
    GLERRORCHECK();
    glOrtho(0.0f, ((float)mBufTex.pw), 0.0f, ((float)mBufTex.ph), -1.0f, 1.0f);
    GLERRORCHECK();
    glClearColor(0.0, 0.0, 0.0, 1.0);
    GLERRORCHECK();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLERRORCHECK();

    return TRUE;
}

void GLEngine::DrawLunaSprite(int nXOriginDest, int nYOriginDest, int nWidthDest, int nHeightDest,
    const BMPBox& bmp, int nXOriginSrc, int nYOriginSrc, int nWidthSrc, int nHeightSrc)
{
    if (!mInitialized) Init();
    if (!mInitialized) return;

    const GLDraw::Texture* tex = g_GLTextureStore.TextureFromLunaBitmap(bmp);
    if (tex == NULL) {
        return;
    }

    g_GLDraw.DrawStretched(nXOriginDest, nYOriginDest, nWidthDest, nHeightDest, tex, nXOriginSrc, nYOriginSrc, nWidthSrc, nHeightSrc);
}

void GLEngine::EndFrame(HDC hdcDest)
{
    glBindFramebufferANY(GL_FRAMEBUFFER_EXT, 0);
    GLERRORCHECK();
    SwapBuffers(hdcDest);
    GLERRORCHECK();
    glClearColor(0.0, 0.0, 0.0, 1.0);
    GLERRORCHECK();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLERRORCHECK();
    glBindFramebufferANY(GL_FRAMEBUFFER_EXT, mFB);
    GLERRORCHECK();
}

void GLEngine::SetTex(const BMPBox* bmp, uint32_t color) {
    const GLDraw::Texture* tex = NULL;
    if (bmp) {
        tex = g_GLTextureStore.TextureFromLunaBitmap(*bmp);
    }

    glBlendEquationANY(GL_FUNC_ADD);
    GLERRORCHECK();
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    GLERRORCHECK();

    g_GLDraw.BindTexture(tex);

    float r = ((0xFF000000 & color) >> 24) / 255.0f;
    float g = ((0x00FF0000 & color) >> 16) / 255.0f;
    float b = ((0x0000FF00 & color) >> 8) / 255.0f;
    float a = ((0x000000FF & color) >> 0) / 255.0f;
    glColor4f(r*a, g*a, b*a, a);
    GLERRORCHECK();
}

void GLEngine::Draw2DArray(GLuint type, const float* vert, float* tex, uint32_t count) {
    // Convert texel coordinates to what we need for our power-of-two padded textures
    if (g_GLDraw.mLastPwScale != 1.0f) {
        for (unsigned int idx = 0; idx < count * 2; idx += 2) {
            tex[idx] *= g_GLDraw.mLastPwScale;
        }
    }
    if (g_GLDraw.mLastPhScale != 1.0f) {
        for (unsigned int idx = 1; idx < count * 2; idx += 2) {
            tex[idx] *= g_GLDraw.mLastPhScale;
        }
    }

    glVertexPointer(2, GL_FLOAT, 0, vert);
    GLERRORCHECK();
    glTexCoordPointer(2, GL_FLOAT, 0, tex);
    GLERRORCHECK();
    glDrawArrays(type, 0, count);
    GLERRORCHECK();
}
