all : flash

TARGET:=main
CH32V003FUN:=./rv003usb/ch32v003fun/ch32v003fun
MINICHLINK?=./rv003usb/ch32v003fun/minichlink

ADDITIONAL_C_FILES+=./rv003usb/rv003usb/rv003usb.S ./rv003usb/rv003usb/rv003usb.c ./matrix.c ./random.c ./touch.c ./usb.c ./ws2812.c
EXTRA_CFLAGS:=-I./rv003usb/lib -I./rv003usb/rv003usb

include ./rv003usb/ch32v003fun/ch32v003fun/ch32v003fun.mk

flash : cv_flash
clean : cv_clean


