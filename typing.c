#include <stdio.h>
#include <getopt.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define VERSION 1.0

#define CLR_RED "\033[31m"
#define CLR_DEF "\033[0m"

void putopt(const char shopt,const char *longopt,const char *msg);
void error(const char *str);
int  notnum(char *str,int len);
void version();
void help();

int main(int argc,char *argv[]){

	if(argc < 2){
		printf("use option -h for get help page\n");
		error("few arguments");
	}

	char fname[64] = "";
	FILE *fd = NULL;

	int optid;
	char opt;
	char sec[8] = "0.1";

	struct option long_opt[] = {
		{"second",1,0,'s'},
		{"file",1,0,'f'},
		{"version",0,0,'v'},
		{"help",0,0,'h'},
		{0,0,0,0}
	};

	while(1){
		if( (opt = getopt_long(argc,argv,"s:f:vh",long_opt,&optid)) == -1 )
			break;

		switch(opt){
			case 's':
				if(strlen(optarg) > 5) error("very long argument 's'");
				snprintf(sec,5,"%s",optarg);
				break;
			case 'f':
				snprintf(fname,63,"%s",optarg);
				break;
			case 'h':
				help();
			case 'v':
				version();
		}
	}

	
	if( !(fd = fopen(fname,"r")) ){
		error("open file");
	}

	char ch;
	char cmd[15] = "sleep ";
	strncat(cmd,sec,8);

	if(notnum(sec,strlen(sec))){
		error("argument 's' not correct");
	}

	while( (ch = fgetc(fd)) != EOF ){
		fputc(ch,stdout);
		fflush(stdout);
		system(cmd);
	}

	fclose(fd);
	return 0;
}

int notnum(char *str,int len){
	for(int i = 0;i < len;i++){
		if( !( ((str[i] >= 0x30) && (str[i] <= 0x39)) || (str[i] == '.')) ){
			return 1;
		}
	}
	return 0;
}

void putopt(const char shopt,const char *longopt,const char *msg){
	printf("  -%c, --%s : %s\n",shopt,longopt,msg);
}

void version(){
	printf("typing; version[%.2f]\n",VERSION);
	exit(0);
}

void help(){
	printf("Usage: typing [options]\n");
	putopt('s',"second <n>","setup count seconds (default 0.1s)");
	putopt('f',"file <file>","cpecified name file");
	putopt('v',"version","print version program");
	putopt('h',"help","print help page");
	exit(0);
}

void error(const char *str){
	printf("[%serror%s]: %s\n",CLR_RED,CLR_DEF,str);
	exit(1);
}