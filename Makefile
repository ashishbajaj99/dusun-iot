ROOTDIR=$(shell pwd)
WORKDIR=$(ROOTDIR)/build
VERSION=V1.0.4

targets			+= zigbeemodemapp

.PHONY: targets

all : $(targets)

srcs		+= $(ROOTDIR)/main.c
srcs		+= $(ROOTDIR)/z3_imp.c
srcs	  := $(subst .cpp,.c,$(srcs))

objs 		:= $(subst $(ROOTDIR),$(WORKDIR), $(subst .c,.o,$(srcs)))
objs		+= $(ROOTDIR)/lib/libz3_v2.3.a

-include $(ROOTDIR)/make/arch.mk
-include $(ROOTDIR)/make/rules.mk

$(eval $(call LinkApp,zigbeemodemapp,$(objs)))


