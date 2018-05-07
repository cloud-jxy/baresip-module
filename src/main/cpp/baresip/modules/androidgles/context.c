#include <EGL/egl.h>
#include <GLES/gl.h>
#include <android/native_window_jni.h>

#include <re.h>
#include <rem.h>
#include <baresip.h>

#include "opengles.h"


extern ANativeWindow* egl_window;


GLfloat sVertices[] = {
        0, 0, 0, // bottom left corner
        0, 1, 0, // top left corner
        1, 1, 0, // top right corner
        1, 0, 0}; // bottom right corner
GLfloat sTexcoods[] = {
        0, 1, 0, // top left corner
        0, 0, 0, // bottom left corner
        1, 0, 0, // bottom right corner
        1, 1, 0};// top right corner
GLubyte sIndices[] = {0,1,2,  // first triangle (bottom left - top left - top right)
                     0,2,3}; // second triangle (bottom left - top right - bottom right)

int context_init(struct vidisp_st *st)
{
    EGLint majorVersion;
    EGLint minorVersion;
    EGLConfig config;
    EGLint numConfigs;
    EGLint format;
    EGLint width;
    EGLint height;
    EGLContext context;

    const EGLint attribs[] = {
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_NONE
    };

    if ((st->display = eglGetDisplay(EGL_DEFAULT_DISPLAY)) == EGL_NO_DISPLAY) {
        error_msg("eglGetDisplay() returned error %d", eglGetError());
        return -1;
    }

    if (!eglInitialize(st->display, &majorVersion, &minorVersion)) {
        error_msg("eglInitialize() returned error %d", eglGetError());
        return -1;
    }

    if (!eglChooseConfig(st->display, attribs, &config, 1, &numConfigs)) {
        error_msg("eglChooseConfig() returned error %d", eglGetError());
        return -1;
    }

    if (!eglGetConfigAttrib(st->display, config, EGL_NATIVE_VISUAL_ID, &format)) {
        error_msg("eglGetConfigAttrib() returned error %d", eglGetError());
        return -1;
    }

    info("init androidgles version:%d.%d config:%d/%d format:%d",
         majorVersion, minorVersion, config, numConfigs, format);
    ANativeWindow_setBuffersGeometry(egl_window, 0, 0, format);

    if (!(st->surface = eglCreateWindowSurface(st->display, config, egl_window, 0))) {
        error_msg("eglCreateWindowSurface() returned error %d", eglGetError());
        return -1;
    }

    if (!(context = eglCreateContext(st->display, config, 0, 0))) {
        error_msg("eglCreateContext() returned error %d", eglGetError());
        return -1;
    }

    if (!eglMakeCurrent(st->display, st->surface, st->surface, context)) {
        error_msg("eglMakeCurrent() returned error %d", eglGetError());
        return -1;
    }

    if (!eglQuerySurface(st->display, st->surface, EGL_WIDTH, &width) ||
        !eglQuerySurface(st->display, st->surface, EGL_HEIGHT, &height)) {
        error_msg("eglQuerySurface() returned error %d", eglGetError());
        return -1;
    }

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrthof(0.0, 1.0, 0.0, 1.0, 0.0, 1.0);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    return 0;
}

void init_texture(struct vidisp_st *st) {
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &st->texture_id);
    glBindTexture(GL_TEXTURE_2D, st->texture_id);
    glTexParameterf(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glVertexPointer(3, GL_FLOAT, 0, sVertices);
    glTexCoordPointer(3, GL_FLOAT, 0, sTexcoods);
}

void context_render(struct vidisp_st *st)
{
    if (!st->texture_id) {
       init_texture(st);
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, st->vf->size.w, st->vf->size.h, 0,
                 GL_RGB, GL_UNSIGNED_SHORT_5_6_5, st->vf->data);

    glClearColor(1, 1, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, sIndices);
    eglSwapBuffers(st->display, st->surface);
}

void context_destroy(struct vidisp_st *st)
{
    // TODO:
}
