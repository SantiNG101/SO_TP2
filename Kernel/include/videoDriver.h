//
// Created by ptorl on 5/12/2023.
//

#ifndef KERNEL_VIDEODRIVER_H
#define KERNEL_VIDEODRIVER_H

#include <stdint.h>

#define BLACK 0x000000
#define BLUE 0x0000AA
#define GREEN 0x00AA00
#define CYAN 0x00AAAA
#define RED 0xAA0000
#define PURPLE 0xAA00AA
#define BROWN 0xAA5500
#define GRAY 0xAAAAAA
#define DARK_GRAY 0x555555
#define LIGHT_BLUE 0x5555FF
#define LIGHT_GREEN 0x55FF55
#define LIGHT_CYAN 0x55FFFF
#define LIGHT_RED 0xFF5555
#define LIGHT_PURPLE 0xFF55FF
#define YELLOW	0xFFFF55
#define WHITE 0xFFFFFF

struct vbe_mode_info_structure {
	uint16_t attributes;		// deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
	uint8_t window_a;			// deprecated
	uint8_t window_b;			// deprecated
	uint16_t granularity;		// deprecated; used while calculating bank numbers
	uint16_t window_size;
	uint16_t segment_a;
	uint16_t segment_b;
	uint32_t win_func_ptr;		// deprecated; used to switch banks from protected mode without returning to real mode
	uint16_t pitch;			// number of bytes per horizontal line
	uint16_t width;			// width in pixels
	uint16_t height;			// height in pixels
	uint8_t w_char;			// unused...
	uint8_t y_char;			// ...
	uint8_t planes;
	uint8_t bpp;			// bits per pixel in this mode
	uint8_t banks;			// deprecated; total number of banks in this mode
	uint8_t memory_model;
	uint8_t bank_size;		// deprecated; size of a bank, almost always 64 KB but may be 16 KB...
	uint8_t image_pages;
	uint8_t reserved0;
 
	uint8_t red_mask;
	uint8_t red_position;
	uint8_t green_mask;
	uint8_t green_position;
	uint8_t blue_mask;
	uint8_t blue_position;
	uint8_t reserved_mask;
	uint8_t reserved_position;
	uint8_t direct_color_attributes;
 
	uint32_t framebuffer;		// physical address of the linear frame buffer; write here to draw to the screen
	uint32_t off_screen_mem_off;
	uint16_t off_screen_mem_size;	// size of memory in the framebuffer but not being displayed on the screen
	uint8_t reserved1[206];
} __attribute__ ((packed));

void putPixel(uint32_t hexColor, uint32_t x, uint32_t y);

#endif //KERNEL_VIDEODRIVER_H
