#ifndef GLFramebuffer_hhhh
#define GLFramebuffer_hhhh

#include <windows.h>
#include <gl/glew.h>
#include "GLTextureStore.h"

class GLFramebuffer {
public:
    GLFramebuffer(int w, int h);
    ~GLFramebuffer();

    void Bind();
    void Clear();
    inline GLDraw::Texture& AsTexture() { return mBufTex; }

private:
    // Framebuffer variables
    GLuint mFB;
    GLuint mDepthRB;
    GLDraw::Texture mBufTex;
};

#endif
