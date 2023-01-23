update_format_from_5_to_6: update_format_from_5_to_6.c Makefile
	gcc -Os -Wall -Wextra -g -flto update_format_from_5_to_6.c \
		-lm `pkg-config --cflags --libs glib-2.0 json-glib-1.0` \
		-o update_format_from_5_to_6
