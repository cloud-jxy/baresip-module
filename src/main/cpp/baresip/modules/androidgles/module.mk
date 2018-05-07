#
# module.mk
#
# Copyright (C) 2010 Creytiv.com
#

MOD		:= opengles
$(MOD)_SRCS	+= opengles.c

ifeq ($(OS),darwin)
$(MOD)_SRCS	+= context.c

$(MOD)_LFLAGS	+= -lobjc -framework CoreGraphics -framework CoreFoundation
endif

include mk/mod.mk
