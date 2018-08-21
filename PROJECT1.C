#include "stdio.h"
#include "process.h"
#include "string.h"
#include "stdlib.h"
#include "ctype.h"
#include "conio.h"
#include "fcntl.h"
#include "types.h"
#include "stat.h"
#include "dir.h"
#include "dos.h"

char *menu[]= {
		"^file services",
		"^Directory services",
		"^Misc services",
		"e^Xit"
		};

char *fileservices[]= {
		"c^Opy file",
		"^Delete file",
		"^Get file attributes",
		"^Set file attributes",
		"^Encrypt file",
		"decr^Ypt file",
		"^Compress file",
		"deco^Mpress file",
		"ty^Pe file",
		"re^Turn to main menu",
		};

char *directoryservices[]= {
		"^Make directory",
		"^Change directory",
		"^Remove directory",
		"^List directory",
		"^Display directory tree",
		"re^Turn to main menu",
		};

char *miscservices[]= {
		"^Calendar",
		"^Ascii table",
		"^System information",
		"s^Huffle",
		"^Memory size",
		"re^Turn to main menu",
		};

char *messages[]= {
		"Helpmate",
		"	Select using Enter or Hot key	",
		"Performs various file services",
		"Performs various directory services",
		"performs various miscelleneous services",
		"Return to DOS",
		"Copy source file to target file",
		"Delete a file",
		"Display current file attributes",
		"Change existing file attributes",
		"Code a file using offset cypher",
		"Decode a coded file",
		"Compress text file",
		"Decompress a compressed file",
		"Display contents of file",
		"REturn to main menu",
		"Create a new directory",
		"Change default directory",
		"Remove an existing directory",
		"List existing directory contents",
		"Display directory tree",
		"Return to main menu",
		"Display calender of any month & year",
		"Display Ascii values & characters",
		"Display equipment list",
		"A video game",
		"Display base memory size",
		"Return to main menu",
		"	Press any key to continue.....	",
		"	Insufficent space press any key... 	"
		};

char far *vid_mem;
int ascii, scan;


main()
{
int mm_choice;
clrscr();
	#ifdef MA

	vid_mem=(char far*) 0xb0000000L;
	setmode(7);

	#else

	vid_mem=(char far*) 0xb8000000L;
	setmode(3);

	#endif

	size(32,0);

	menubox(0,0,24,79,7,7);
	drawbox(1,0,21,79,7);
	drawbox(1,0,23,79,7);
	logo();


	mainscreen();

	while(1)
	{
	   mm_choice=popupmenu(menu,4,5,5,"FDMX",2);

	   switch(mm_choice)
	   {
		case 1:
			fserver();
			break;
		case 2:
			dserver();
			break;
		case 3:
			mserver();
			break;
		case 4:
		case 27:
			size(6,7);
			clrscr();
			exit(1);
			}
		}
}

setmode(mode)
int mode;
{
	union REGS i,o;

	i.h.ah=0;
	i.h.al=mode;
	int86(16,&i,&o);
}

mainscreen()
{
	int i,j;

	drawbox(1,0,23,79,7);
	drawbox(3,0,21,79,7);

	writechar(3,0,204,7);
	writechar(3,79,185,7);
	writechar(21,0,204,7);
	writechar(21,79,185,7);

	for(i=4;i<=20;i++)
	{
		for(j=1;j<=78;j+=2)
		{
			writechar(i,j,177,7);
			writechar(i,j+1,177,7);
		}
	}

	writestring(messages[0],2,32,112);
	writestring(messages[1],22,14,112);
}

writechar(r,c,ch,attb)
int r,c,attb;
char ch;
{
	char far *v;

	v=vid_mem+r*160+c*2;

	*v=ch;
	v++;
	*v=attb;
}

writestring(s,r,c,attb)
int r,c,attb;
char *s;
{
	while(*s!='\0')
		{
			if(*s=='^')
			{
				s++;
				writechar(r,c,*s,126);
			}
			else
				writechar(r,c,*s,attb);
		s++;
		c++;
		}
}

popupmenu(menu,count,sr,sc,hotkeys,helpnumber)
int count,sr,sc,helpnumber;
char **menu,*hotkeys;
{
	int er,ec,i,l=0,areareqd,areaforhelp,choice;
	char *p,*h;

	er=sr+count+2;

	for(i=0;i<count;i++)
	{
		if(strlen(menu[i])>l)
		  l=strlen(menu[i]);
	}

	ec=sc+l+3;

	areareqd=(er-sr+1)*(ec-sc+1)*2;

	p=malloc(areareqd);

	if(p==NULL)
	{
		writestring(messages[29],22,14,112);
		getch();
		exit(2);
	}

	savevideo(sr,sc,er,ec,p);

	menubox(sr,sc,er,ec,112,66);

	displaymenu(menu,count,sr+1,sc+1);

	areaforhelp=(9-6+1)*(78-35+1)*2;

	h=malloc(areaforhelp);

	if(h==NULL)
	{
		writestring(messages[29],22,14,112);
		getch();
		exit(3);
	}

	savevideo(6,35,9,78,h);
	menubox(6,35,9,78,112,66);

	writestring(messages[helpnumber],7,36,112);

	choice=getresponse(menu,hotkeys,sr,sc,count,helpnumber);

	restorevideo(sr,sc,er,ec,p);
	restorevideo(6,35,9,78,h);

	free(p);
	free(h);

	return(choice);
}

size(ssl,esl)
int ssl,esl;
{
	union REGS i,o;

	i.h.ah=1;
	i.h.ch=ssl;
	i.h.cl=esl;
	i.h.bh=0;

	int86(16,&i,&o);
}

getkey()
{
	union REGS ii,oo;

	while(!kbhit());

	ii.h.ah=0;

	int86(22,&ii,&oo);

	scan=oo.h.ah;
	ascii=oo.h.al;
}

savevideo(sr,sc,er,ec,buffer)
int sr,sc,er,ec;
char *buffer;
{
	char far *v;
	int i,j;

	for(i=sr;i<=er;i++)
	{
		for(j=sc;j<=ec;j++)
		{
		v=vid_mem+i*160+j*2;
		*buffer=*v;
		v++;
		buffer++;
		*buffer=*v;
		buffer++;
		}
	}
}

restorevideo(sr,sc,er,ec,buffer)
int sr,sc,er,ec;
char *buffer;
{
	char far *v;
	int i,j;

	for(i=sr;i<=er;i++)
	{
		for(j=sc;j<=ec;j++)
		{
			v=vid_mem+i*160+j*2;
			*v=*buffer;
			v++;
			buffer++;
			*v=*buffer;
			buffer++;
		}
	}
}

menubox(sr,sc,er,ec,fil,shad)
int sr,sc,er,ec;
char fil,shad;
{
int i,j;
	for(i=sr;i<er;i++)
	{
		for(j=sc;j<(ec-1);j++)
			writechar(i,j,' ',fil);
	}

	if (shad==0)
	{
		for(i=sr;i<=er;i++)
		{	writechar(i,ec,' ',fil);
			writechar(i,(ec-1),' ',fil);
		}
		for(j=sc;j<=ec;j++)
			writechar(er,j,' ',fil);
	}
	else
	{
		for(i=sr+1;i<=er;i++)
		{
			writechar(i,ec,' ',shad);
			writechar(i,(ec-1),' ',shad);
		}

		for(j=sc+2;j<=ec;j++)
			writechar(er,j,' ',shad);
	}
}

displaymenu(menu,count,sr,sc)
int count,sr,sc;
char **menu;
{
	int i;

	for(i=0;i<count;i++)
	{
		writestring(menu[i],sr,sc,112);
		sr++;
	}
}

drawbox(sr,sc,er,ec,attr)
int sr,sc,er,ec,attr;
{
	int i;

	for(i=sc+1;i<ec;i++)
	{
		writechar(sr,i,205,attr);
		writechar(er,i,205,attr);
	}

	for(i=sr+1;i<er;i++)
	{
		writechar(i,sc,186,attr);
		writechar(i,ec,186,attr);
	}

	writechar(sr,sc,201,attr);
	writechar(sr,ec,187,attr);
	writechar(er,sc,200,attr);
	writechar(er,ec,188,attr);
}

getresponse(menu,hotkeys,sr,sc,count,helpnumber)
char **menu,*hotkeys;
int sr,sc,count,helpnumber;
{
	int choice=1,len,hotkeychoice;
	len=strlen(hotkeys);

	writestring(menu[choice-1],sr+choice,sc+1,111);

	while(1)
	{
		getkey();

	  if(ascii==0)
	  {
		switch(scan)
		{
		case 80:

		writestring(menu[choice-1],sr+choice,sc+1,112);

		choice++;
		helpnumber++;
		break;
		case 72:

		writestring(menu[choice-1],sr+choice,sc+1,112);

		choice--;
		helpnumber--;
		break;
		}

	     if(choice==0)
	     {
		choice=count;
		helpnumber=helpnumber+count;
	      }

	      if(choice>count)
	      {
		 choice=1;
		 helpnumber=helpnumber-count;
	      }

	      writestring(menu[choice-1],sr+choice,sc+1,111);

	      menubox(6,35,9,78,112,66);

	      writestring(messages[helpnumber],7,36,112);
	  }
	  else
	  {
		if(ascii==13)
			return(choice);
		if(ascii==27)
			return(27);
		ascii=toupper(ascii);
		hotkeychoice=1;

		while(*hotkeys!='\0')
		{
			if(*hotkeys==ascii)
				return(hotkeychoice);
			else
			{
			hotkeys++;
			hotkeychoice++;
			}
		}

		hotkeys=hotkeys-len;
		}
	  }
}

fserver()
{
	int fs_choice;

	while(2)
	{
		fs_choice=popupmenu(fileservices,10,5,5,"ODGSEYCMPT",6);

		switch(fs_choice)
		{
			case 1:
				copyfile();
				break;
			case 2:
				deletefile();
				break;
			case 3:
				getfileattb();
				break;
			case 4:
				setfileattb();
				break;
			case 5:
				encryptfile();
				break;
			case 6:
				decryptfile();
				break;
			case 7:
				compressfile();
				break;
			case 8:
				decompressfile();
				break;
			case 9:
				displayfile();
				break;
			case 10:
				return;
		}
	}
}

dserver()
{
	int ds_choice;

	while(2)
	{
		ds_choice=popupmenu(directoryservices,6,5,5,"MCRLDT",16);

		switch(ds_choice)
		{
			case 1:
				makedir();
				break;
			case 2:
				changedir();
				break;
			case 3:
				removedir();
				break;
			case 4:
				listdir();
				break;
			case 5:
				dirtree();

				break;
			case 6:
				return;
		}
	}
}

mserver()
{
	int ms_choice;

	while(2)
	{
		ms_choice=popupmenu(miscservices,6,5,5,"CASHMT",22);

		switch(ms_choice)
		{
			case 1:
				calendar();
				break;
			case 2:
				asciitable();
				break;
			case 3:
				systeminfo();
				break;
			case 4:
				shuffle();
				break;
			case 5:
				memsize();
				break;
			case 6:
				return;
		}
	}
}


copyfile()
{
	char sfile[20],tfile[20],buffer[512],*p;
	int areareqd,inhandle,outhandle,bytes,flag;

	areareqd=(20-5+1)*(70-5+1)*2;
	p=malloc(areareqd);

	if(p==NULL)
	{
		writestring(messages[29],22,14,112);
		getch();

		writestring(messages[1],22,14,112);
		return;
	}

	savevideo(5,5,20,70,p);
	menubox(6,5,13,60,112,66);

	writestring("	  File copy service	 ",22,14,112);
	writestring("Enter source file name.",7,8,112);
	size(6,7);
	gotoxy(33,8);
	gets(sfile);
	size(32,0);


	inhandle=open(sfile, O_RDONLY | O_BINARY);

	if(inhandle<0)
	{
		writestring("unable to open source file!",9,8,112);
		writestring(messages[28],22,14,112);
		getch();

		writestring(messages[1],22,14,112);
		restorevideo(5,5,20,70,p);
		free(p);
		return;
	}

	writestring("enter target file name",9,8,112);
	size(6,7);
	gotoxy(33,10);
	gets(tfile);
	size(32,0);

	outhandle=open(tfile,O_CREAT | O_WRONLY | O_BINARY,S_IWRITE);

	if(outhandle<0)
	{
		writestring("unable to open target file",11,8,112);
		writestring(messages[28],22,14,112);
		getch();
		close(inhandle);
		writestring(messages[1],22,14,112);
		restorevideo(5,5,20,70,p);
		free(p);
		return;
	}

	while((bytes=read(inhandle,buffer,512))>0)
	{
		flag=write(outhandle,buffer,bytes);
		if(flag==-1)
		break;
	}

	if(flag==-1)
		writestring("Unable To Copy file",11,8,112);
	else
		writestring("File has been succefully copied",11,8,112);

	writestring(messages[28],22,14,112);
	getch();

	close(inhandle);
	close(outhandle);
	writestring(messages[1],22,14,112);
	restorevideo(5,5,20,70,p);
	free(p);
}

deletefile()
{
	union REGS ii,oo;
	int areareqd;
	char filename[20],*p;

	areareqd=(20-5+1)*(70-5+1)*2;
	p=malloc(areareqd);
	if(p==NULL)
	{
		writestring(messages[29],22,14,112);
		getch();

		writestring(messages[1],22,14,112);
		return;
	}

	savevideo(5,5,20,70,p);
	menubox(6,5,11,60,112,66);

	writestring("	File delete service	",22,14,112);

	writestring("Enter name of file to be deleted,",7,8,112);
	size(6,7);
	gotoxy(43,8);
	gets(filename);
	size(32,0);

	ii.h.ah=65;
	ii.x.dx=(unsigned int)filename;
	intdos(&ii,&oo);

	if(oo.x.cflag==0)
		writestring("File was succesfully deleted",9,8,112);
	else
	{
		switch(oo.x.ax)
		{
			case 2:
				writestring("File not found",9,8,112);
				break;
			case 3:
				writestring("Invalid path",9,8,112);
				break;
			case 5:
				writestring("Access denied",9,8,112);
				break;
			case 0x11:
				writestring("Invalid drive name",9,8,112);
				break;
			default:
				writestring("improper request",9,8,112);
		}
	}

	writestring(messages[28],22,14,112);
	getch();

	writestring(messages[1],22,14,112);
	restorevideo(5,5,20,70,p);
	free(p);
}

getfileattb()
{
	union REGS ii,oo;
	int a,areareqd;
	char filename[20],*p;

	areareqd=(20-5+1)*(70-5+1)*2;
	p=malloc(areareqd);
	if(p==NULL)
	{
		writestring(messages[29],22,14,112);
		getch();

		writestring(messages[1],22,14,112);
		return;
	}

	savevideo(5,5,20,70,p);
	menubox(6,5,16,60,112,66);

	writestring("	Get file attribute services	",22,14,112);

	writestring("Enter name of the file:",7,8,112);
	size(6,7);
	gotoxy(29,8);
	gets(filename);
	size(32,0);

	ii.h.ah=67;
	ii.h.al=0;
	ii.x.dx=(unsigned int)filename;
	intdos(&ii,&oo);

	if(oo.x.cflag==0)
	{
		writestring("ATTRIBUTES",9,27,112);
		writestring("-------",10,27,112);
		a=oo.x.cx;
		writeattr(a,24);
	}
	else
	{
		switch(oo.x.ax)
		{
			case 2:
				writestring("File neot found!",9,8,112);
				break;
			case 3:
				writestring("Invalid path!",9,8,112);
				break;
			case 5:
				writestring("Access denied!",9,8,112);
				break;
			case 0x11:
				writestring("Invalid drive name!",15,8,112);
				break;
			default:
				writestring("improper request!",9,8,112);
		}
	}

	writestring(messages[28],22,14,112);
	getch();

	writestring(messages[1],22,14,112);
	restorevideo(5,5,20,70,p);
	free(p);
}

writeattr(a,col)
int a,col;
{
	writestring("Read only:",11,col,112);
	if((a&1)==0)
		writestring("OFF",11,(col+12),112);
	else
		writestring("ON",11,(col+12),112);

	writestring("Hidden  :",12,col,112);
	if((a&2)==0)
		writestring("OFF",12,(col+12),112);
	else
		writestring("ON",12,(col+12),112);

	writestring("System  :",13,col,112);
	if((a&4)==0)
		writestring("OFF",13,(col+12),112);
	else
		writestring("ON",13,(col+12),112);

	writestring("Archive  :",14,col,112);
	if((a&32)==0)
		writestring("OFF",14,(col+12),112);
	else
		writestring("ON",14,(col+12),112);
}

setfileattb()
{
	union REGS ii,oo;
	int old,new,areareqd;
	char filename[20],*p,ch;

	areareqd=(20-5+1)*(70-5+1)*2;
	p=malloc(areareqd);
	if(p==NULL)
	{
		writestring(messages[29],22,14,112);
		getch();

		writestring(messages[1],22,14,112);
		return;
	}

	savevideo(5,5,20,70,p);
	menubox(6,5,17,60,112,66);

	writestring("	Set file attribute service	",22,14,112);

	writestring("Enter name of file:",7,8,112);
	size(6,7);
	gotoxy(29,8);
	gets(filename);
	size(32,0);

	ii.h.ah=65;
	ii.h.al=0;
	ii.x.dx=(unsigned int)filename;
	intdos(&ii,&oo);

	if(oo.x.cflag==0)
	{
		old=new=oo.x.cx;
		writestring("Existing Attributes",9,8,112);
		writestring("---------------",10,8,112);
		writeattr(old,8);
	}
	else
	{
		switch(oo.x.ax)
		{
			case 2:
				writestring("File not found!",9,8,112);
				break;
			case 3:
				writestring("Invalid path!",9,8,112);
				break;
			case 5:
				writestring("Access denied!",9,8,112);
				break;
			case 0x11:
				writestring("Invalid drive name!",9,8,112);
				break;
			default:
				writestring("Improper request!",9,8,112);
		}

		writestring(messages[28],22,14,112);
		getch();

		restorevideo(5,5,20,70,p);
		free(p);
		return;
	}

	writestring("Change(Y/N):",11,30,112);
	size(6,7);
	gotoxy(45,12);
	fflush(stdin);
	ch=toupper(getch());
	writechar(11,44,ch,112);

	if(ch=='Y')
		new=(new^1);

	writestring("Change(Y/N):",12,30,112);
	gotoxy(45,13);
	fflush(stdin);
	ch=toupper(getch());
	writechar(12,44,ch,112);

	if(ch=='Y')
		new=(new^2);

	writestring("Change(Y/N):",13,30,112);
	gotoxy(45,14);
	fflush(stdin);
	ch=toupper(getch());
	writechar(13,44,ch,112);

	if(ch=='Y')
		new=(new^4);

	writestring("Change(Y/N):",14,30,112);
	gotoxy(45,15);
	fflush(stdin);
	ch=toupper(getch());
	writechar(14,44,ch,112);

	size(32,0);

	if(ch=='Y')
		new=(new^32);

	ii.h.ah=67;
	ii.h.al=1;
	ii.x.cx=new;
	ii.x.dx=(unsigned int)filename;
	intdos(&ii,&oo);

	menubox(6,5,17,60,112,66);
	writestring("File name:",7,8,112);
	writestring(filename,7,19,112);

	if(oo.x.cflag==0)
	{
		writestring("Old Attributes",9,8,112);
		writestring("-----------",10,8,112);
		writeattr(old,8);
		writestring("New Attributes",9,40,112);
		writestring("-----------",10,40,112);
		writeattr(new,40);
	}
	else
		writestring("Error-New attributes not set!",12,8,112);

	writestring(messages[28],22,14,112);
	getch();

	writestring(messages[1],22,14,112);
	restorevideo(5,5,20,70,p);
	free(p);
}

encryptfile()
{
	char sfile[20],tfile[20],*p,ch;
	FILE *fps,*fpt;
	int areareqd,flag;

	areareqd=(20-5+1)*(70-5+1)*2;
	p=malloc(areareqd);
	if(p==NULL)
	{
		writestring(messages[29],22,14,112);
		getch();

		writestring(messages[1],22,14,112);
		return;
	}

	savevideo(5,5,20,70,p);
	menubox(6,5,13,60,112,66);

	writestring("	File coding service	",22,14,112);

	writestring("Enter source file name:",7,8,112);
	size(6,7);
	gotoxy(33,8);
	fflush(stdin);
	gets(sfile);
	size(32,0);

	fps=fopen(sfile,"r");
	if(fps==NULL)
	{
		writestring("Unable to open source file!",9,8,112);
		writestring(messages[28],22,14,112);
		getch();

		writestring(messages[1],22,14,112);
		restorevideo(5,5,20,70,p);
		free(p);
		return;
	}

	writestring("Enter target filename:",9,8,112);
	size(6,7);
	gotoxy(33,10);
	gets(tfile);
	size(32,0);

	fpt=fopen(tfile,"w");
	if(fpt==NULL)
	{
		writestring("Unable to open target file!",11,8,112);
		writestring(messages[28],22,14,112);
		getch();

		fclose(fps);
		writestring(messages[1],22,14,112);
		restorevideo(5,5,20,70,p);
		free(p);
		return;
	}

	while((ch=getc(fps))!=EOF)
	{
		if(ch=='\n')
			flag=putc('\n',fpt);
		else
			flag=putc((ch+127),fpt);

		if(flag==EOF)
		break;
	}

	if(flag==EOF)
		writestring("Unable yo encrypt file!",11,8,112);
	else
		writestring("File is succesfully encrypted!",11,8,112);

	writestring(messages[28],22,14,112);
	getch();

	fclose(fps);
	fclose(fpt);
	writestring(messages[1],22,14,112);
	restorevideo(5,5,20,70,p);
	free(p);
}

decryptfile()
{
	char sfile[20],tfile[20],*p,ch;
	FILE *fps,*fpt;
	int areareqd,flag;

	areareqd=(20-5+1)*(70-5+1)*2;
	p=malloc(areareqd);
	if(p==NULL)
	{
		writestring(messages[29],22,14,112);
		getch();

		writestring(messages[1],22,14,112);
		return;
	}

	savevideo(5,5,20,70,p);
	menubox(6,5,13,60,112,66);

	writestring("	File decode service	",22,14,112);

	writestring("Enter the source file name:",7,8,112);
	size(6,7);
	gotoxy(33,8);
	gets(sfile);
	size(32,0);

	fps=fopen(sfile,"r");
	if(fps==NULL)
	{
		writestring("Unable to open source file!",9,8,112);
		writestring(messages[28],22,14,112);
		getch();

		writestring(messages[1],22,14,112);
		restorevideo(5,5,20,70,p);
		free(p);
		return;
	}

	writestring("Enter target file name:",9,8,112);
	size(6,7);
	gotoxy(33,10);
	gets(tfile);
	size(32,0);

	fpt=fopen(tfile,"w");
	if(fpt==NULL)
	{
		writestring("Unable to open target file!",11,8,112);
		writestring(messages[1],22,14,112);
		getch();

		fclose(fps);
		writestring(messages[1],22,14,112);
		restorevideo(5,5,20,70,p);
		free(p);
		return;
	}

	while((ch=getc(fps))!=EOF)
	{
		if(ch=='\n')
			flag=putc('\n',fpt);
		else
			flag=putc((ch-127),fpt);

			if(flag==EOF)
			break;
	}

	if(flag==EOF)
		writestring("Unable to decrypt file!",11,8,112);
	else
		writestring("File successfully decrypted!",11,8,112);

	writestring(messages[28],22,14,112);
	getch();

	fclose(fps);
	fclose(fpt);
	writestring(messages[1],22,14,112);
	restorevideo(5,5,20,70,p);
	free(p);
}

compressfile()
{
	char *p,sfile[20],tfile[20];
	FILE *fps,*fpt;
	int area,count,ch,flag;

	area=(12-5+1)*(60-5+1)*2;
	p=malloc(area);
	if(p==NULL)
	{
		writestring(messages[29],22,14,112);
		getch();

		writestring(messages[1],22,14,112);
		return;
	}

	savevideo(5,5,12,60,p);
	menubox(5,5,12,60,112,66);

	writestring("	File compress service	",22,14,112);

	writestring("Enter source file name:",6,8,112);
	size(6,7);
	gotoxy(33,7);
	gets(sfile);
	size(32,0);

	fps=fopen(sfile,"r");
	if(fps==NULL)
	{
		writestring("Unable to open sorce file!",8,8,112);
		writestring(messages[28],22,14,112);
		getch();

		writestring(messages[1],22,14,112);
		restorevideo(5,5,12,60,p);
		free(p);
		return;
	}

	writestring("Enter target file name:",8,8,112);
	size(6,7);
	gotoxy(33,9);
	gets(tfile);
	size(32,0);

	fpt=fopen(tfile,"w");
	if(fpt==NULL)
	{
		writestring("Unable to open target file!",10,8,112);
		writestring(messages[28],22,14,112);
		getch();

		fclose(fps);
		writestring(messages[1],22,14,112);
		restorevideo(5,5,12,60,p);
		free(p);
		return;
	}

	while((ch=getc(fps))!=EOF)
	{
		if(ch==' ')
		{
			count=1;

			while((ch=getc(fps))==' ')
				count++;

			flag=putc(count+127,fpt);
			flag=putc(ch,fpt);
		}
		else
			flag=putc(ch,fpt);

		if(flag==EOF)
			break;
	}

	if(flag==EOF)
		writestring("Unable to compress file!",10,8,112);
	else
		writestring("File succesfully compressed!",10,8,112);

	writestring(messages[28],22,14,112);
	getch();

	fclose(fps);
	fclose(fpt);
	writestring(messages[1],22,14,112);
	restorevideo(5,5,12,60,p);
	free(p);
}

decompressfile()
{
	char *p,sfile[20],tfile[20];
	FILE *fps,*fpt;
	int ch,count,area,flag;

	area=(12-5+1)*(60-5+1)*2;
	p=malloc(area);
	if(p==NULL)
	{
		writestring(messages[29],22,14,112);
		getch();

		writestring(messages[1],22,14,112);
		return;
	}

	savevideo(5,5,12,60,p);
	menubox(5,5,12,60,112,66);

	writestring("	File decompress service	",22,14,112);
	writestring("Enter source file name:",6,8,112);
	size(6,7);
	gotoxy(33,7);
	gets(sfile);
	size(32,0);

	fps=fopen(sfile,"r");
	if(fps==NULL)
	{
		writestring("Unable to open source!",8,8,112);
		writestring(messages[28],22,14,112);
		getch();

		writestring(messages[1],22,14,112);
		restorevideo(5,5,12,60,p);
		free(p);
		return;
	}

	writestring("Enter target file name:",8,8,112);
	size(6,7);
	gotoxy(33,9);
	gets(tfile);
	size(32,0);

	fpt=fopen(tfile,"w");
	if(fpt==NULL)
	{
		writestring("Unable to open target file!",10,8,112);
		writestring(messages[28],22,14,112);
		getch();

		fclose(fps);
		writestring(messages[1],22,14,112);
		restorevideo(5,5,12,60,p);
		free(p);
		return;
	}

	while((ch=getc(fps))!=EOF)
	{
		if(ch>127)
		{
			ch=ch-127;

			for(count=1;count<=ch;count++)
				flag=putc(' ',fpt);
		}
		else
			flag=putc(ch,fpt);

		if(flag==EOF)
			break;
	}

	if(flag==EOF)
		writestring("Unable to decompress file!",10,8,112);
	else
		writestring("File successfully decompressed!",10,8,112);

	writestring(messages[28],22,14,112);
	getch();

	fclose(fps);
	fclose(fpt);
	writestring(messages[1],22,14,112);
	restorevideo(5,5,12,60,p);
	free(p);
}

displayfile()
{
	char *p,filename[20],ch,str[5];
	int pg=1,row=2,col=1,area;
	FILE *fp;

	area=(25-0+1)*(80-0+1)*2;
	p=malloc(area);

	if(p==NULL)
	{
		writestring(messages[29],22,14,112);
		getch();

		writestring(messages[1],22,14,112);
		return;
	}

	savevideo(0,0,24,79,p);
	menubox(5,5,10,60,112,66);

	writestring("	File display service	",22,14,112);

	writestring("Enter name of the file:",6,8,112);
	size(6,7);
	gotoxy(29,7);
	gets(filename);
	size(32,0);

	fp=fopen(filename,"r");
	if(fp==NULL)
	{
		writestring("Unable to open source file!",8,8,112);
		writestring(messages[28],22,14,112);
		getch();

		writestring(messages[1],22,14,112);
		restorevideo(0,0,24,79,p);
		free(p);
		return;
	}

	menubox(0,0,1,79,7,7);
	menubox(1,0,24,79,112,32);
	drawbox(1,0,23,78,112);
	drawbox(1,0,21,78,112);
	writechar(21,0,204,112);
	writechar(21,78,185,112);

	writestring("File:-",0,0,7);
	writestring(filename,0,8,7);
	writestring("Page No:-",0,54,7);
	itoa(pg,str,10);
	writestring(str,0,65,7);

	while((ch=getc(fp))!=EOF)
	{
		if((ch!='\n')&&(ch!='\r')&&(ch!='\t'))
			writechar(row,col,ch,112);

			if(ch=='\t')
				col+=4;
			else
				col++;

			if(col>77||ch=='\n')
			{
				col=1;
				row++;

				if(row>20)
				{
					writestring(messages[28],22,17,112);
					getch();

					row=2;
					pg++;

					menubox(2,1,20,77,112,0);

					writestring("File:-",0,0,7);
					writestring(filename,0,8,7);
					writestring("Page No :-",0,54,7);
					itoa(pg,str,10);
					writestring(str,0,65,7);
				}
			}
	}

	writestring("	press any key to return..	",22,14,112);
	getch();

	fclose(fp);
	writestring(messages[1],22,14,112);
	restorevideo(0,0,24,79,p);
	free(p);
}

makedir()
{
	union REGS ii,oo;
	int areareqd;
	char dirname[20],*p;

	areareqd=(20-5+1)*(70-5+1)*2;
	p=malloc(areareqd);
	if(p==NULL)
	{
		writestring(messages[29],22,14,112);
		getch();

		writestring(messages[1],22,14,112);
		return;
	}

	savevideo(5,5,20,70,p);
	menubox(6,5,11,60,112,66);

	writestring("	create directory service	",22,15,112);

	writestring("Enter name of directory:",7,8,112);
	size(6,7);
	gotoxy(34,8);
	gets(dirname);
	size(32,0);

	ii.h.ah=57;
	ii.x.dx=(unsigned int)dirname;
	intdos(&ii,&oo);

	if(oo.x.cflag==0)
		writestring("Directory was successfully created!",9,8,112);
	else
	{
		if(oo.x.ax==5)
			writestring("Improper access!",9,8,112);
		if(oo.x.ax==3)
			writestring("Invalid path!",9,8,112);
	}

	writestring(messages[28],22,14,112);
	getch();

	writestring(messages[1],22,14,112);
	restorevideo(5,5,20,70,p);
	free(p);
}

changedir()
{
	union REGS ii,oo;
	int areareqd;
	char dirname[20],*p;

	areareqd=(20-5+1)*(70-5+1)*2;
	p=malloc(areareqd);
	if(p==NULL)
	{
		writestring(messages[29],22,14,112);
		getch();

		writestring(messages[1],22,14,112);
		return;
	}

	savevideo(5,5,20,70,p);
	menubox(6,5,11,60,112,66);

	writestring("	Change directory Service	",22,14,112);

	writestring("Enter name of directory:",7,8,112);
	size(6,7);
	gotoxy(34,8);
	gets(dirname);
	size(32,0);

	ii.h.ah=59;
	ii.x.dx=(unsigned int)dirname;
	intdos(&ii,&oo);

	if(oo.x.cflag==0)
		writestring("Directory is successfully changed!",9,8,112);
	else
	{
		if(oo.x.ax==3)
			writestring("Invalid path!",9,8,112);
		if(oo.x.ax==5)
			writestring("Improper access!",9,8,112);
	}

	writestring(messages[28],22,14,112);
	getch();

	writestring(messages[1],22,14,112);
	restorevideo(5,5,20,70,p);
	free(p);
}

removedir()
{
	union REGS ii,oo;
	int areareqd;
	char dirname[20],*p;

	areareqd=(20-5+1)*(76-5+1)*2;
	p=malloc(areareqd);
	if(p==NULL)
	{
		writestring(messages[29],22,14,112);
		getch();

		writestring(messages[1],22,14,112);
		return;
	}

	savevideo(5,5,20,76,p);
	menubox(6,5,12,75,112,66);

	writestring("	delete directory service	",22,15,112);

	writestring("Directory name:",7,8,112);
	writestring("(entire path)",8,8,112);
	size(6,7);
	gotoxy(25,8);
	gets(dirname);
	size(32,0);

	ii.h.ah=58;
	ii.x.dx=(unsigned int)dirname;
	intdos(&ii,&oo);

	if(oo.x.cflag==0)
		writestring("Directory was successfully removed!",10,8,112);
	else
	{
		switch(oo.x.ax)
		{
			case 3:
				writestring("Invalid path!",10,8,112);
				break;
			case 5:
				writestring("Improper access!",10,8,112);
				break;
			case 2:
				writestring("Directory does not exist!",10,8,112);
				break;
			case 0x10:
				writestring("Cannot remove current directory!",10,8,112);
		}
	}

	writestring(messages[28],22,14,112);
	getch();

	writestring(messages[1],22,14,112);
	restorevideo(5,5,20,76,p);
	free(p);
}

listdir()
{
	int areareqd;
	char *p,filetosearch[20];
	char sz[10],dd[10],mm[10],yy[10],hr[10],m[10],temp[3]="0";
	struct ffblk file;
	unsigned int done,row,col,a,year,month,day,hour,min;

	areareqd=(20-3+1)*(70-3+1)*2;
	p=malloc(areareqd);
	if(p==NULL)
	{
		writestring(messages[29],22,14,112);
		getch();

		writestring(messages[1],22,14,112);
		return;
	}

	savevideo(5,5,20,70,p);
	menubox(6,5,11,60,112,66);

	writestring("	list directory service	",22,15,112);

	writestring("Enter skeleton for searching:",7,8,112);
	size(6,7);
	gotoxy(39,8);
	gets(filetosearch);
	size(32,0);

	done=findfirst(filetosearch,&file,FA_DIREC);

	if(done==0)
	{
		menubox(5,5,20,70,112,66);
		row=8;
		col=8;

		writestring("Directory listing",6,28,112);
		writestring("-------------",7,28,112);

		while(done==0)
		{
			row++;
			writestring(file.ff_name,row,col,112);

			if((file.ff_attrib&16)==0)
			{
				itoa(file.ff_fsize,sz,10);
				writestring(sz,row,col+19,112);

				a=file.ff_fdate;
				year=80+(a>>9);
				month=(a<<7)>>12;
				day=(a<<11)>>11;

				itoa(day,dd,10);

				if(strlen(dd)==1)
					strcat(temp,dd);
				else
				strcpy(temp,dd);
				writestring(temp,row,col+30,112);
				writechar(row,col+32,'/',112);

				strcpy(temp,"0");

				itoa(month,mm,10);

				if(strlen(mm)==1)
					strcat(temp,mm);
				else
					strcpy(temp,mm);

				writestring(temp,row,col+33,112);
				writechar(row,col+35,'/',112);

				strcpy(temp,"0");

				itoa(year,yy,10);
				if(strlen(yy)==1)
					strcat(temp,yy);
				else
					strcpy(temp,yy);

				writestring(temp,row,col+36,112);

				strcpy(temp,"0");

				a=file.ff_ftime;
				hour=(a>>11);
				min=(a<<5)>>10;

				if(hour==0)
					hour=12;
				strcpy(temp,"0");

				itoa(hour,hr,10);
				if(strlen(hr)==1)
					strcat(temp,hr);
				else
					strcpy(temp,hr);

				writestring(temp,row,col,col+45,112);
				writechar(row,col+47,':',112);

				strcpy(temp,"0");

				itoa(min,m,10);
				if(strlen(m)==1)
					strcat(temp,m);
				else
					strcpy(temp,m);

				writestring(temp,row,col+48,112);

				strcpy(temp,"0");
			}
			else
				writestring("<DIR>",row,col+15,112);

			done=findnext(&file);

			if(row==18)
			{
				row=8;
				writestring(messages[28],22,14,112);
				getch();
				menubox(5,5,20,70,112,66);
				writestring("Directory listing",6,28,112);
				writestring("-------------",7,28,112);
			}
		}
	}
	else
		writestring("File not found!",9,8,112);

	writestring(messages[28],22,14,112);
	getch();

	writestring(messages[1],22,14,112);
	restorevideo(5,5,20,70,p);
	free(p);
}

char dirname[40],dir[32],name[32],attb,entirepath[40];
int dirtree_row,in;

dirtree()
{
	char *p,current_dir[32];
	int area;

	area=(25-0+1)*(80-0+1)*2;
	p=malloc(area);
	if(p==NULL)
	{
		writestring(messages[29],22,14,112);
		getch();

		writestring(messages[1],22,14,112);
		return;
	}
	savevideo(0,0,24,79,p);

	menubox(0,0,1,79,7,7);
	menubox(0,0,24,79,112,32);
	drawbox(0,0,23,78,112);
	drawbox(0,0,21,78,112);
	writechar(21,0,204,112);
	writechar(21,78,185,112);

	writestring("Directory tree",1,20,112);
	writestring("=================",2,20,112);

	getcwd(current_dir,32);
	chdir("\\");

	name[0]='\0';
	dir[0]='\0';

	entirepath[0]='\0';

	dirtree_row=3;

	in=0;

	if(strlen(current_dir)==3)
		attb=159;
	else
	{
		attb=112;

		fnsplit(current_dir,0,dir,name,0);

		strcpy(entirepath,dir);
		strcat(entirepath,name);
	}

	writestring("R",3,2,attb);
	writestring("O",4,2,attb);
	writestring("O",5,2,attb);
	writestring("T",6,2,attb);

	tree("*.*");
	printf(" hai i am here");
	sleep(5);
	chdir(current_dir);

	writestring("	Presss any key to return	",22,14,112);
	getch();
	restorevideo(0,0,24,79,p);
	free(p);
}

tree(ptr)
char *ptr;
{
	struct ffblk file;
	int flag,i,len;
	static char path[40]="";
	char str1[9]="     ",str2[9]="     ",str3[9]="     ";
	char str4[9]="     ",str5[4]="  ";

	strnset(str1,196,8);
	strnset(str1,195,1);

	strnset(str2,32,8);
	strnset(str2,179,1);

	strnset(str3,196,8);
	strnset(str3,192,1);

	strnset(str5,196,3);
	str5[2]=197;

	flag=findfirst(ptr,&file,FA_DIREC);

	while(flag==0)
	{
		if(((file.ff_attrib & 16)==16)&&file.ff_name[0]!='.')
		{
			if(in==0)
			{
				strcpy(path,"\\");
				strcat(path,file.ff_name);
			}
			else
			{
				strcat(path,"\\");
				strcat(path,file.ff_name);
			}

			if(dirtree_row>=20)
			{
				writestring(messages[28],22,17,112);
				getch();
				menubox(2,1,20,77,112,0);

				writestring("Directory tree",1,20,112);
				writestring("================",2,20,112);
				dirtree_row=3;

				writestring("R",3,2,attb);
				writestring("O",4,2,attb);
				writestring("O",5,2,attb);
				writestring("T",6,2,attb);
			}

			in++;
			dirname[0]='\0';

			if(in==1)
				strcat(dirname,str1);
			else
			{
				strcat(dirname,str2);
				i=2;
				while(i<in)
				{
					strcat(dirname,str4);
					i++;
				}
				strcat(dirname,str3);
			}

			if(strcmp(path,entirepath)==0)
			{
				writestring(dirname,dirtree_row,5,112);

				writestring(file.ff_name,dirtree_row,5+strlen(dirname),159);
			}
			else
			{
				strcat(dirname,file.ff_name);
				writestring(dirname,dirtree_row,5,112);
			}

			if(dirtree_row==3)
				writestring(str5,3,3,112);

			dirtree_row++;

			chdir(file.ff_name);
			file.ff_name[0]='\0';

			tree(ptr);
		}

		flag=findnext(&file);
	}

	if(in-->0)
	{
		chdir("..");

		len=strlen(path);
		if(in>=1)
		{
			while(path[len-1]!='\\')
				len--;
			path[len-1]='\0';
		}
	}
printf("i am here\n");
sleep(5);
}

calendar()
{
	char *months[]={
			"January",  "february",  "March",
			"April",    "may",      "June",
			"july",     "August",   "September",
			"October",  "November", "December"
			};

	int days[]={31,28,31,30,31,30,31,31,30,31,30,31};
	int m,y,leapyears,row,col,x,i,areareqd,firstday,thisyrdays;
	long int totaldays;
	char *p,str1[5],str2[3];

	areareqd=(20-3+1)*(70-3+1)*2;
	p=malloc(areareqd);
	if(p==NULL)
	{
		writestring(messages[29],22,14,112);
		getch();

		writestring(messages[1],22,14,112);
		return;
	}

	savevideo(5,5,20,70,p);
	menubox(6,5,11,50,112,66);

	writestring("	Display calendar services	",22,14,112);
	writestring("Enter month(1-12):",7,8,112);
	size(6,7);
	gotoxy(33,8);
	scanf("%d",&m);
	size(32,0);

	writestring("Enter year:",9,8,112);
	size(6,7);
	gotoxy(21,10);
	scanf("%d",&y);
	size(32,0);

	while(1)
	{
		days[1]=28;
		thisyrdays=0;

		leapyears=(y-1)/4-(y-1)/100+(y-1)/400;

		if(y%400==0||y%100!=0&&y%4==0)
			days[1]=29;
		else
			days[1]=28;

		totaldays=leapyears+(y-1)*365L;

		for(i=0;i<=m-2;i++)
			thisyrdays=thisyrdays+days[i];

		firstday=(int)((totaldays+thisyrdays)%7);

		menubox(5,5,20,70,112,66);
		writestring(months[m-1],6,19,112);
		itoa(y,str1,10);
		writestring(str1,6,29,112);

		writestring("Sun   Mon   Tue   Wed   Thu   Fri   Sat",8,7,112);

		col=7+firstday*6;

		row=10;

		for(x=1;x<=days[m-1];x++)
		{
			itoa(x,str2,10);
			writestring(str2,row,col,112);
			col=col+6;

			if(y==1752&&m==9&&x==2)
				x=13;

			if(col>43)
			{
				row=row+2;
				col=7;
			}

			if(row>18&&col==7)
				row=10;
		}

		writestring("Change using arrow keys",22,28,112);
		writestring("Next year   Up",10,49,112);
		writestring("Previous year   Dn",12,49,112);
		writestring("Next month   Rt",14,49,112);
		writestring("Previous month   Lt",16,49,112);
		writestring("Esc for exit",18,49,112);
		getkey();

		switch(scan)
		{
			case 72:
				y++;
				break;

			case 80:
				y--;
				break;

			case 77:
				if(m==12)
				{
					y=y+1;
					m=1;
				}
				else
					m=m+1;
				break;

			case 75:
				if(m==1)
				{
					y=y-1;
					m=12;
				}
				else
					m=m-1;
				break;

			case 1:
				writestring(messages[1],22,14,112);
				restorevideo(5,5,20,70,p);
				free(p);
				return;
		}
	}
}

asciitable()
{
	char *p,str[4];
	int areareqd,j,row,col;

	areareqd=(20-5+1)*(70-5+1)*2;
	p=malloc(areareqd);
	if(p==NULL)
	{
		writestring(messages[29],22,14,112);
		getch();

		writestring(messages[1],22,14,112);
		return;
	}

	savevideo(5,5,20,70,p);
	menubox(5,5,20,70,112,66);

	writestring("	Display ascii table service	",22,14,112);

	row=8;
	col=10;
	writestring("Value",row-2,col-2,112);
	writestring("Character",row-2,col+5,112);

	for(j=0;j<=225;j++)
	{
		writechar(row,(col+9),j,112);

		itoa(j,str,10);
		writestring(str,row,col,112);

		row++;

		if(row>=18)
		{
			row=8;
			col+=19;

			if(col>48)
			{
				writestring(messages[28],22,14,112);
				getch();
				row=8;
				col=10;
				menubox(5,5,20,70,112);
			}
			writestring("Value",row-2,col-2,112);
			writestring("Charcter",row-2,col+5,112);
		       //	getch();
	       //	}
	//}
		}
	writestring(messages[28],22,14,112);
       getch(); //}
	writestring(messages[1],22,14,112);
	    }
	restorevideo(5,5,20,70,p);
	free(p);



}

systeminfo()
{
	union REGS ii,oo;
	int areareqd,a,n;
	char *p,str[3];

	areareqd=(20-5+1)*(70-5+1)*2;
	p=malloc(areareqd);
	if(p==NULL)
	{
		writestring(messages[29],22,14,112);
		getch();

		writestring(messages[1],22,14,112);
		return;
	}

	savevideo(5,5,20,70,p);
	menubox(5,5,18,65,112,66);

	writestring("	Display equipment list service	",22,14,112);

	writestring("Equipment list",6,16,112);
	writestring("------------",7,16,112);

	int86(17,&ii,&oo);
	a=oo.x.ax;

	if((a&1)==0)
		writestring("Disk drive:Absent",9,8,112);
	else
		writestring("Disk drive:Present",9,8,112);

	if((a&2)==0)
		writestring("Math co-processor:Absent",10,8,112);
	else
		writestring("Math co-processor:Present",10,8,112);

	writestring("Initial video mode:",11,8,112);
	n=(a&48);
	switch(n)
	{
		case 48:
			writestring("80x25 BW with mono card",11,28,112);
			break;

		case 32:
			writestring("80x25 BW with color card",11,28,112);
			break;

		case 16:
			writestring("40x25 BW with color card",11,28,112);
	}

	if((a&1)==1)
	{
		n=(a&0x00C0)>>6;
		writestring("No. of disk drives:",12,8,112);
		writestring(itoa(n+1,str,10),12,28,112);
	}

	n=(a&0x100);
	if(n==0x100)
		writestring("DMA:Absent",16,8,112);
	else
		writestring("DMA:Present",16,8,112);

	n=(a<<4)>>13;
	writestring("No. of serial ports present:",13,8,112);
	writestring(itoa(n,str,10),13,37,112);

	if((a & 0x1000)==0)
		writestring("Game adapter:Absent",14,8,112);
	else
		writestring("Game adapter: Present:",14,8,112);

	n=a>>14;
	writestring("No. of parallel ports present:",15,8,112);
	writestring(itoa(n,str,10),15,39,112);

	writestring(messages[28],22,14,112);
	getch();

	restorevideo(5,5,20,70,p);
	free(p);
}

int num[4][4]={
		2,11,15,1,
		6,4,3,14,
		8,7,9,10,
		5,12,13,0
	      };


shuffle()
{
	int r=3,c=3,t,flag,areareqd;

	int no_moves=0,x;
	char *p,str[5];

	areareqd=(25-0+1)*(80-0+1)*2;
	p=malloc(areareqd);
	if(p==NULL)
	{
		writestring(messages[29],22,14,112);
		getch();

		writestring(messages[1],22,14,112);
		return;
	}

	savevideo(0,0,24,79,p);
	menubox(5,14,19,65,112,66);

	writestring("	Video game service	",22,20,112);
	sleep(1);
	shufflebox();

	writestring("Hit arrow keys to shift numbers",16,23,112);
	writestring("Press Esc to abort",17,23,112);

      while(1)
	{
		display();
		sleep(1);
		getkey();

		switch(scan)
		{
			case 80:
				if(r!=0)
				{
					t=num[r][c];
					num[r][c]=num[r-1][c];
					num[r-1][c]=t;
					r--;

					no_moves++;
				}
				break;

			case 72:
				if(r!=3)
				{
					t=num[r][c];
					num[r][c]=num[r+1][c];
					num[r+1][c]=t;
					r++;

					no_moves++;
				}
				break;

			case 75:
				if(c!=3)
				{
					t=num[r][c];
					num[r][c]=num[r][c+1];
					num[r][c+1]=t;

					c++;

					no_moves++;
				}
				break;

			case 77:
				if(c!=0)
				{
					t=num[r][c];
					num[c][r]=num[r][c-1];
					num[r][c-1]=t;
					c--;

					no_moves++;
				}
				break;

			case 1:
				writestring("	Goodbye!      ",16,23,112);
				writestring("	Better luck next time	",17,23,112);
				writestring(messages[28],22,14,112);
				getch();

				writestring(messages[1],22,14,112);
				restorevideo(0,0,24,79,p);
				free(p);
				return;
		}
		flag=check();

		if(flag==0)
		{
			display();
			writestring("Success! You have done it!!  ",16,23,112);
			writestring("No. of moves-  ",17,13,112);
			itoa(no_moves,str,10);
			writestring(str,17,38,112);
			writestring(messages[28],22,14,112);
			getch();

			restorevideo(0,0,24,79,p);
			free(p);
			return;
		}
	}
}

shufflebox()
{
	int row,col;

	for(col=32;col<=44;col++)
	{
		for(row=6;row<=14;row+=2)
			writechar(row,col,196,112);
	}


     for(row=7;row<=13;row+=2)
	{
		for(col=32;col<=44;col+=3)
			writechar(row,col,179,112);
	}

	for(row=8;row<=12;row+=2)
	{
		for(col=35;col<=41;col+=3)
			writechar(row,col,197,112);
	}

	for(row=8;row<=12;row+=2)
	{
		writechar(row,32,195,112);
		writechar(row,44,180,112);
	}

	for(col=35;col<=41;col+=3)
	{
		writechar(6,col,194,112);
		writechar(14,col,193,112);
	}

	writechar(6,32,218,112);
	writechar(6,44,191,112);
	writechar(14,32,192,112);
	writechar(14,44,217,112);

}

display()
{
	int i,j,row=7,col=33;
	char str[5];

	for(i=0;i<=3;i++)
	{
		for(j=0;j<=3;j++)
		{
			if(num[i][j]==0)
				writestring(" ",row,col,112);
			else
			{
				itoa(num[i][j],str,10);
				writestring(str,row,col,112);
			}

			col+=3;
			if(col>42)
			{
				col=33;
				row+=2;
			}
		}
	}
}

check()
{
	int row,col;

	int result[4][4]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0};

	for(row=0;row<=3;row++)
	{
		for(col=0;col<=3;col++)
		{
			if(num[row][col]!=result[row][col])
				return(1);
		}
	}
	return (0);
}

memsize()
{
	union REGS ii,oo;
	int areareqd,n;
	char *p,str[6];

	areareqd=(20-5+1)*(70-5+1)*2;
	p=malloc(areareqd);
	if(p==NULL)
	{
		writestring(messages[29],22,14,112);
		getch();

		writestring(messages[1],22,14,112);
		return;
	}

	savevideo(5,5,20,70,p);
	menubox(9,15,12,47,112,66);

	int86(18,&ii,&oo);
	n=oo.x.ax;

	writestring("Base memory size:",10,18,112);
	itoa(n,str,10);
	writestring(str,10,36,112);
	writestring("KB",10,40,112);

	writestring(messages[28],22,14,112);
	getch();

	writestring(messages[1],22,14,112);
	restorevideo(5,5,20,70,p);
	free(p);
}

logo()
{
	int i,j;

	for(i=2;i<=20;i++)
	{
		for(j=2;j<=77;j++)
			writechar(i,j,176,7);
	}

	writestring("	Designed & Written	",13,26,112);
	writestring("	By	",14,26,112);
	writestring("	Govamsh Sai	",15,26,112);
	writestring("	At	",16,26,112);
	writestring("	vidyanagar	",17,26,112);
	writestring("	1st line ext	",18,26,112);
	writestring("	ph 7306707652	",19,26,112);
	writestring("	press any key	",22,26,112);

	writestring("				",3,5,77);
	writestring("** ** ****** **  ****** *** *** ****** ******** *****",4,5,77);
	writestring("** ** **  ** ** ** * * ** **  **  **  **",5,5,77);
	writestring("** ** **  ** ** ** *** ** **  **  **",6,5,77);
	writestring("****** ***** **   ***** ** * ** ***** ** ****",7,5,77);
	writestring("** ** **  **  ** ** * ** ** * ** **  **",8,5,77);
	writestring("** ** ** ** **   *** ***  *** ** * *  ***",9,5,77);
	writestring("** ** ****** ****** ***   **  **** ***  ***",10,5,77);
	writestring("					       ",11,5,77);
	getch();
}
