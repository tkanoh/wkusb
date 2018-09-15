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

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/ioctl.h>
#include "wkusb.h"

#ifndef TTY_DEV
#define	TTY_DEV	"/dev/ttyU0"
#endif

#define WPM	0

int fd;
struct termios term_def;

void wait_byte(wait_byte)
unsigned char wait_byte;
{
	unsigned char c;

	do {
		read(fd,(char *)&c,1);
	} while(c != wait_byte);
	return;
}

void write_admin_cmd(cmd)
unsigned char cmd;
{
	char buf[3];

	snprintf(buf,sizeof(char)*BUFSIZ,"%c%c",WK_ADMIN_CMD,cmd);
	write(fd,buf,2);

	return;
}
void proc_abort(sig)
int sig;
{
	char buf[BUFSIZ];

	write_admin_cmd(WK_HOST_CLOSE);
	write_admin_cmd(WK_RESET);

	tcflush(fd,TCIOFLUSH);
	tcsetattr(fd, TCSANOW, &term_def);
	close(fd);

	fprintf(stderr,"\nWinkeyer Host Host Mode Closed.\n");

	exit(0);
}

int init_signal()
{
	signal(SIGINT,proc_abort);
	signal(SIGTERM,proc_abort);
	signal(SIGQUIT,proc_abort);
	signal(SIGHUP,proc_abort);

	signal(SIGPIPE,SIG_IGN);

	return(0);
}

int pf_key(buf,len)
char *buf;
int len;
{
	if(len != 6) return(-1);
	if(*buf == 0x1b && *(buf+1) == 0x5b && *(buf+4) == 0x7e && *(buf+5) == 0x0a) {
		if(*(buf+2) == 0x31 && *(buf+3) == 0x31) return(1);
		if(*(buf+2) == 0x31 && *(buf+3) == 0x32) return(2);
		if(*(buf+2) == 0x31 && *(buf+3) == 0x33) return(3);
		if(*(buf+2) == 0x31 && *(buf+3) == 0x34) return(4);
		if(*(buf+2) == 0x31 && *(buf+3) == 0x35) return(5);
		if(*(buf+2) == 0x31 && *(buf+3) == 0x37) return(6);
		if(*(buf+2) == 0x31 && *(buf+3) == 0x38) return(7);
		if(*(buf+2) == 0x31 && *(buf+3) == 0x39) return(8);
		if(*(buf+2) == 0x32 && *(buf+3) == 0x30) return(9);
		if(*(buf+2) == 0x32 && *(buf+3) == 0x31) return(10);
		if(*(buf+2) == 0x32 && *(buf+3) == 0x33) return(11);
		if(*(buf+2) == 0x32 && *(buf+3) == 0x33) return(12);
	}
	return(-1);
}

void usages(buf)
char *buf;
{
	int i;

	printf("usages: %s [-s] [-l tty_dev] \n",buf);

	for(i=1;i<13;i++)
		if(pf_word[i]) printf("  PF%2.2d: %s\n",i,pf_word[i]);

	exit(0);
}

int main(argc, argv)
int argc;
char **argv;
{
	int i, j, wd, rc, last_rc, iFlags;
	static pid_t pid;
	struct termios term;
	struct timeval timeout;
	fd_set rd, mask;
	char c, *tty_dev, buf[BUFSIZ], *buf_p, str[BUFSIZ], *str_p, *pf_word_p;
	unsigned char side_tone;

	last_rc = 0;
	side_tone = 0x85;
	tty_dev = TTY_DEV;

	while ((i = getopt(argc, argv, "l:?")) != -1) {
		switch (i) {
			case 'l':
				tty_dev = optarg;
				break;
			case 's':
				side_tone = 0x05;
				break;
			case 'h':
			case '?':
				default:
				usages(argv[0]);
		}
	}

	if((fd = open(tty_dev, O_RDWR | O_NONBLOCK)) < 0) {
		perror("open");
		exit(0);
	}

	init_signal();

	setbuf(stdin,NULL);
	setbuf(stdout,NULL);

	FD_ZERO(&mask);
	FD_SET(STDIN_FILENO,&mask);
	FD_SET(fd,&mask);
	wd=fd+1;

	tcgetattr(fd,&term_def);
	bzero(&term, sizeof(term)); 

	term.c_cflag = CS8 | CREAD | CLOCAL | MDMBUF;
	term.c_iflag = IGNBRK | IGNPAR;
	term.c_oflag = 0;
	term.c_lflag = 0;

	term.c_cc[VTIME]    = 1;
	term.c_cc[VMIN]     = 0;

	cfsetispeed(&term,B1200);
	cfsetospeed(&term,B1200);

	tcflush(fd,TCIOFLUSH);
	tcsetattr(fd, TCSANOW, &term);

	// turn on DTR
	iFlags = TIOCM_DTR | TIOCM_DSR;
	ioctl(fd, TIOCMBIS, &iFlags);

	write_admin_cmd(WK_RESET);
	write_admin_cmd(WK_HOST_OPEN);
	wait_byte(WK_VERSION);

	//write_admin_cmd(WK_BAUD_9600);
	//cfsetispeed(&term,B9600);
	//cfsetospeed(&term,B9600);

	snprintf(buf,sizeof(char)*BUFSIZ,"%c%c",WK_SET_WINKEYER,0x42);
	//snprintf(buf,sizeof(char)*BUFSIZ,"%c%c",WK_SET_WINKEYER,0x17);
	write(fd,buf,2);

	write_admin_cmd(WK_SET_WK2_MODE);

	snprintf(buf,sizeof(char)*BUFSIZ,"%c%c",WK_SET_PINCONF,0xae);
	//snprintf(buf,sizeof(char)*BUFSIZ,"%c%c",WK_SET_PINCONF,0x05);

	write(fd,buf,2);

	snprintf(buf,sizeof(char)*BUFSIZ,"%c%c",WK_SET_WPM,WPM);
	write(fd,buf,2);

	snprintf(buf,sizeof(char)*BUFSIZ,"%c%c",WK_SET_SIDETONE,side_tone);
	write(fd,buf,2);

	snprintf(buf,sizeof(char)*BUFSIZ,"%c%c%c",WK_SET_PTT,0x01,0x00);
	write(fd,buf,3);

/*
	sleep(3);
	snprintf(buf,sizeof(char)*BUFSIZ,"JF2DSI/1%c%c%c%c%c%c%c",WK_NOP,WK_NOP,WK_NOP,WK_NOP,WK_NOP,WK_NOP,WK_NOP);
	write(fd,buf,15);
	sleep(10);
*/

	while (1) {
		timeout.tv_sec=10;
		timeout.tv_usec=0;
		rd=mask;
		switch(select(wd,&rd,NULL,NULL,&timeout)){
			case -1:
				if(errno!=EINTR){
					perror("select");
					void proc_abort(SIGTERM);
					;
				}
				break;
			case  0:
				break;
			default:
				if(FD_ISSET(fd,&rd)){
					rc=read(fd,&c,1);
					if((c & 0xc0) == 0xc0) {
					}
					else if((c & 0xc0) == 0x80) {
					}
					else {
						fputc(c,stdout);
					}
				}
				if(FD_ISSET(STDIN_FILENO,&rd)){
					buf_p=buf;
					str_p=str;
					rc=read(STDIN_FILENO,buf,255);
					if(rc==1) 
						write(fd,str,last_rc);
					else if(*buf == 0x08) 		/* DEL for abort */
						write(fd,buf,rc);
					else if(*buf == '~') {
						last_rc = strlen(pf_word[KEY_CALL]);
						strcpy(str,pf_word[KEY_CALL]);
						write(fd,str,last_rc);
					}
					else if(*buf == '`')
						write(fd,pf_word[KEY_RST],strlen(pf_word[KEY_RST]));
					else if ((i  = pf_key(buf,rc)) > 0) {
						pf_word_p=pf_word[i];
						last_rc=strlen(pf_word[i]);
						for(j=0;j<last_rc;j++) *str_p++ = *pf_word_p++;
						write(fd,pf_word[i],last_rc);
						printf("%s\n",pf_word[i]);
					}
					else {
						for(i=0;i<rc;i++) *str_p++ = toupper(*buf_p++); 
						write(fd,str,rc-1);
						last_rc = rc - 1;
					}
				}
				break;
		}
	}
}
