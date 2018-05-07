/**
 * @file opengles.h Internal API to OpenGLES module
 *
 * Copyright (C) 2010 Creytiv.com
 */

#include <GLES/gl.h>
#include <EGL/egl.h>

struct vidisp_st {
	const struct vidisp *vd;  /* pointer to base-class (inheritance) */
	struct vidframe *vf;

	/* GLES: */
	EGLDisplay display;
	EGLSurface surface;
	GLuint texture_id;
};

int  context_init(struct vidisp_st *st);
void context_destroy(struct vidisp_st *st);
void context_render(struct vidisp_st *st);
