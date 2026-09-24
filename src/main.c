#include "neslib.h"
#include "nesdoug.h"
#include "types.h"
#include "controls.h"
#include "screens/board.h"
#include "piece.h"
#include "pointer.h"

#define BLACK 0x0f
#define DK_GY 0x00
#define LT_GY 0x10
#define WHITE 0x30
#define LT_YL 0x38
#define GOLD_ 0x27
#define BROWN 0x17
 
#pragma bss-name(push, "ZEROPAGE")

unsigned char i;
val sprid;

val btn_down, btn_new;

const unsigned char palette[]={
    BLACK, DK_GY, LT_GY, WHITE,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0
}; 

const unsigned char spr_palette[]={
    BLACK, DK_GY, BLACK, WHITE, // black pieces
    BLACK, DK_GY, WHITE, LT_GY, // white pieces
    BLACK, LT_YL, GOLD_, BROWN, // selected pieces
    0,0,0,0
};

void main (void) {
    init_pieces();
    init_pointer();

	ppu_off();

	pal_bg(palette);
	pal_spr(spr_palette);
		
	vram_adr(NAMETABLE_A);
    vram_write(board, sizeof(board));
    ppu_on_all();

    set_vram_buffer();
	
	while (1){
        read_inputs();

        // Updates
        update_pointer();

        // Render
        ppu_wait_nmi();
        clear_vram_buffer();
        sprid = 0;
        oam_clear();

        render(Pieces);
        render(Pointer);
	}
}
	
	
