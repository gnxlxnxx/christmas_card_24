all : flash

TARGET:=main
CH32V003FUN:=./rv003usb/ch32v003fun/ch32v003fun
MINICHLINK?=./rv003usb/ch32v003fun/minichlink

ADDITIONAL_C_FILES+=./rv003usb/rv003usb/rv003usb.S ./rv003usb/rv003usb/rv003usb.c ./matrix/matrix.c ./matrix/text.c ./matrix/animations.c ./matrix/img_scroll.c ./random.c ./touch.c ./usb.c ./ws2812.c
EXTRA_CFLAGS:=-I./rv003usb/lib -I./rv003usb/rv003usb -fexec-charset=iso_8859-15

include ./rv003usb/ch32v003fun/ch32v003fun/ch32v003fun.mk

flash : cv_flash
clean : cv_clean


