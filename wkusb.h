/*
 * Copyright (c) 2016
 *      Tamotsu Kanoh <kanoh@kanoh.org>. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither my name nor the names of its contributors may be used to
 *    endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#define WK_ADMIN_CMD	0x00
#define WK_SET_SIDETONE	0x01
#define WK_SET_WPM	0x02
#define WK_SET_WEIT	0x03
#define WK_SET_PTT	0x04
#define WK_SET_POT	0x05
#define WK_SET_PAUSE	0x06
#define WK_GET_POT	0x07
#define WK_BACKSPACE	0x08
#define WK_SET_PINCONF	0x09
#define WK_CLEAR_BUFF	0x0a
#define WK_KEY_IMM	0x0b
#define WK_HSCW		0x0c
#define WK_SET_F_WPM	0x0d
#define WK_SET_WINKEYER	0x0e
#define WK_LOAD_DEF	0x0f
#define WK_SET_1ST_EXT	0x10
#define WK_SET_KEY_COMP	0x11
#define WK_SET_PAD_SW	0x12
#define WK_NOP		0x13
#define WK_SOFT_PAD	0x14
#define WK_REQ_STATUS	0x15 
#define WK_POINTER_CMD	0x16 
#define WK_SET_DD_RATIO	0x17 
#define WK_PTT_ON_OFF	0x18 
#define WK_KEY_BUFF	0x19 
#define WK_WAIT_SEC	0x1a 
#define WK_MERGE_LETTER	0x1b 
#define WK_BUFF_SPEED	0x1c 
#define WK_HSCW_SPEED	0x1d 
#define WK_CANCEL_SPEED	0x1e 
#define WK_BUFF_NOP	0x1f 


/* Winkeyer Version */
#define	WK_VERSION	0x17

/* Admin Command */
#define	WK_RESET	0x01
#define	WK_HOST_OPEN	0x02
#define	WK_HOST_CLOSE	0x03
#define	WK_ECHO_TEST	0x04
#define	WK_GET_VAL	0x07
#define	WK_SET_WK1_MODE	0x10
#define	WK_SET_WK2_MODE	0x11
#define	WK_DUMP_EEPROM	0x12
#define	WK_LOAD_EEPROM	0x13
#define	WK_SEND_MSG	0x14
#define	WK_LOAD_XMODEM	0x15
#define	WK_BAUD_9600	0x17
#define	WK_BAUD_1200	0x18

#define KEY_CALL	1
#define KEY_RST		3

static char *pf_word[] = {
	NULL,
	"JF2DSI/1",
	"DE JF2DSI/1",
	"BK 5NN TU",
	"BK JF2DSI/1 5NN TU",
	"QSL UR 5NN TU",
	"73 > E E",
	"E E",
	"BK 5NN 25",
	"JF2DSI/1 5NN 25",
	NULL,
	NULL,
	NULL,
	NULL
};
