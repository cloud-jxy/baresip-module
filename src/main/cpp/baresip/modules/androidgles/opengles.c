/**
 * @file opengles.c Video driver for OpenGLES
 *
 * Copyright (C) 2010 - 2015 Creytiv.com
 */

#include <pthread.h>
#include <re.h>
#include <rem.h>
#include <baresip.h>
////#include <OpenGLES/ES1/gl.h>
////#include <OpenGLES/ES1/glext.h>
//#include <GLES/gl.h>
//#include <GLES/glext.h>
#include "opengles.h"


/**
 * @defgroup opengles opengles
 *
 * Video display module for OpenGLES on Android
 */


static struct vidsrc *vidsrc;
static struct vidisp *vid;

static void destructor(void *arg)
{
	struct vidisp_st *st = arg;
	context_destroy(st);
	mem_deref(st->vf);
}

static int opengles_alloc(struct vidisp_st **stp, const struct vidisp *vd,
			  struct vidisp_prm *prm, const char *dev,
			  vidisp_resize_h *resizeh,
			  void *arg)
{
	info("opengles_alloc");
	struct vidisp_st *st;
	int err = 0;

	(void)prm;
	(void)dev;
	(void)resizeh;
	(void)arg;

	st = mem_zalloc(sizeof(*st), destructor);
	if (!st)
		return ENOMEM;

	st->vd = vd;

	err = context_init(st);
	if (err)
		goto out;

 out:
	if (err)
		mem_deref(st);
	else
		*stp = st;

	return err;
}


static int opengles_display(struct vidisp_st *st, const char *title,
			    const struct vidframe *frame)
{
	int err;

	(void)title;

	if (!st->vf) {
		if (frame->size.w & 3) {
			warning("opengles: width must be multiple of 4\n");
			return EINVAL;
		}

		err = vidframe_alloc(&st->vf, VID_FMT_RGB565, &frame->size);
		if (err)
			return err;
	}

	vidconv(st->vf, frame, NULL);

	context_render(st);

	return 0;
}

struct vidsrc_st {
    const struct vidsrc *vs;  /* inheritance */
    struct vidframe *frame;
    pthread_t thread;
    bool run;
    int fps;
    vidsrc_frame_h *frameh;
    void *arg;
};
static struct vidsrc *vidsrc;
static void *read_thread(void *arg)
{
    struct vidsrc_st *st = arg;
    uint64_t ts = tmr_jiffies();

    while (st->run) {

        if (tmr_jiffies() < ts) {
            sys_msleep(4);
            continue;
        }

        st->frameh(st->frame, st->arg);

        ts += (1000/st->fps);
    }

    return NULL;
}
static void src_destructor(void *arg)
{
    struct vidsrc_st *st = arg;

    if (st->run) {
        st->run = false;
        pthread_join(st->thread, NULL);
    }

    mem_deref(st->frame);
}
static int src_alloc(struct vidsrc_st **stp, const struct vidsrc *vs,
                     struct media_ctx **ctx, struct vidsrc_prm *prm,
                     const struct vidsz *size, const char *fmt,
                     const char *dev, vidsrc_frame_h *frameh,
                     vidsrc_error_h *errorh, void *arg)
{
    struct vidsrc_st *st;
    int err;

    (void)ctx;
    (void)fmt;
    (void)dev;
    (void)errorh;

    if (!stp || !prm || !size || !frameh)
        return EINVAL;

    st = mem_zalloc(sizeof(*st), src_destructor);
    if (!st)
        return ENOMEM;

    st->vs     = vs;
    st->fps    = prm->fps;
    st->frameh = frameh;
    st->arg    = arg;

    err = vidframe_alloc(&st->frame, VID_FMT_YUV420P, size);
    if (err)
        goto out;

    st->run = true;
    err = pthread_create(&st->thread, NULL, read_thread, st);
    if (err) {
        st->run = false;
        goto out;
    }

    out:
    if (err)
        mem_deref(st);
    else
        *stp = st;

    return err;
}

static int module_init(void)
{
	int err = 0;
	err |= vidsrc_register(&vidsrc, baresip_vidsrcl(),
						   "fakevideo", src_alloc, NULL);
	err |= vidisp_register(&vid, baresip_vidispl(),
						   "opengles", opengles_alloc, NULL,
						   opengles_display, NULL);
	return err;
}


static int module_close(void)
{
	vid = mem_deref(vid);

	return 0;
}


EXPORT_SYM const struct mod_export DECL_EXPORTS(androidgles) = {
	"androidgles",
	"vidisp",
	module_init,
	module_close,
};
