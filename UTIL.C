//			UTIL.C
#define CGA 1
#define ESC 27
#define YES 1
#define NO 0
#define NO_SHADOW 0
#define HALF_SHADOW -1

char *mainmenu[]={
		   "^Cursor Movement",
		   "^File",
		   "^Search",
		   "^Delete",
		   "^Exit"
		 };

char *cursormenu[]={
		   "^Start of file",
		   "End of ^File",
		   "^Top of screen",
		   "^Bottom of screen",
		   "Start of ^Line",
		   "^End of line",
		   "^Word Left",
		   "Word ^Right",
		   "Page ^Up",
		   "Page ^Down",
		   "retur^N"
		   };

char *filemenu[]={
		   "^Load",
		   "^Pick",
		   "^New",
		   "^Save",
		   "Save ^As",
		   "^Merge",
		   "^Change dir",
		   "^Output to printer",
		   "Re^Turn"
		 };

char *searchmenu[]={
		   "^Find",
		   "Find & ^Replace",
		   "Repeat ^Last find",
		   "^Abort operations",
		   "^Go to line no",
		   "Re^Turn"
		   };

char *deletemenu[]={
		   "^Delete line",
		   "To ^End of line",
		   "To ^Beginning of Line",
		   "Word ^Right",
		   "Re^Turn"
		   };

char *exitmenu[]={
		   "^Exit",
		   "^Shell",
		   "Re^turn"
		 };

char *pickfile[5]={
			"	",
			"	",
			"	",
			"	",
			"	"
		  };

char *buf;

unsigned int maxsize;
char *startloc,*curscr,*currow,*endloc;
char searchstr[31],replacestr[31],filespec[30],filename[17];

void interrupt (*old1b)();
void interrupt handler();
void interrupt (*old23)();
char *search (char*);

int ascii,scan,pickfileno,no_tab;
int curr=2,curc=1,logc=1,logr=1;
int skip,findflag,frflag,saved=YES,ctrl_c_flag=0;

char far *vid_mem;
char far *ins=(char far *)0x417;

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
	while(*s=='\0')
	{
		if(*s=='^')
		{
			s++;

			if(attb==15)
				writrestring(r,c,*s,15);
			else
				writestring(r,c,*s,113);
		}
		else
		{
			if(*s=='$')
			{
				s++;
				writestring(r,c,*s,47);
			}
			else
				writestring(r,c,*s,attb);
		}
		c++;
		s++;
	}
}

savevideo(sr,sc,er,ec,buffer)
int sr,sc,er,ec;
char *buffer;
{
	char far *v;
	int i,j;

	for(i=sr;i<=er;i++)
	{
		for(j=sr;j<=ec;j++)
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
			*v=*buffer++;
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

	if (shad==NO_SHADOW)
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
		if(shad==HALF_SHADOW)
		{
			for(i=sr;i<=er;i++)
				writechar(i,(ec-1),220,112);

			for(j=sc+1;j<ec;j++)
				writechar(er,j,223,112);
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
	union REGS i,o;

	while (!kbhit))
	{
		if(ctrl_c_flag)
		{
			displayline(currow,logr+1);
			gotoxy(logc+1,logr+2);
			ctrl_c_flag=0;
		}
	}

	i.h.ah=0;

	int86(22,&i,&o);

	ascii=o.h.al;
	scan=o.h.ah;

}

popupmenuv(menu,count,sr,sc,hotkeys,helpnumber)
int count,sr,sc,helpnumber;
char **menu,*hotkeys;
{
	int er,i,ec,l,len=0,area,choice;
	char *p;

	size(32,0);

	er=sr+count+2;

	for(i=0;i<count;i++)
	{
		l=strlen(menu[i]);
		if(l>len)
			len=l;
	}
	ec=sc+len+5;

	area=(er-sr+1)*(ec-sc+1)*2;

	p=malloc(area);

	if(p==NULL)
		error_exit();

	savevideo(sr,sc,er,ec,p);

	menubox(sr,sc+1,er,ec,112,15);

	drawbox(sr,sc+1,er-1,ec-2,112);

	displaymenuv(menu,count,sr+1,sc+3);

	choice=getresponsev(menu,hotkeys,sr,sc+2,count,helpnumber);

	restorevideo(sr,sc,er,ec,p);

	free(p);

	size(5,7);
	return(choice);
}

displaymenuv(menu,count,sr,sc)
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

getresponsev(menu,hotkeys,sr,sc,count,helpnumber)
char **menu,*hotkeys;
int sr,sc,count,helpnumber;
{
	int choice=1,len,hotkeychoice;

	len=strlen(hotkeys);

	writestring(menu[choice-1],sr+choice,sc+1,15);

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
					break;

				case 72:
					writestring(menu[choice-1],sr+choice,sc+1,112);

					choice--;
					break;

				case 77:
					return(77);

				case 75:
					return(75);

				case 59:
					if(helpnumber==3)
					{
						if(choice!=9)
						{
							displayhelp(8+choice-1);
						}
						break;
					}

					if(helpnumber==6)
					{
						if(choice!=3)
						{
							displayhelp(16+choice-1);
						}
						break;
					}

				displayhelp(helpnumber);
			}

			if(choice==0)
				choice=count;

			if(choice>count)
				choice=1;

			writestring(menu[choice-1],sr+choice,sc+1,15);
		}

		else
		{
			if(ascii==13)
				return(choice);

			if(ascii==ESC)
			{
				displaymenuh(mainmenu,5);
				return(ESC);
			}

			hotkeychoice=1;
			ascii=toupper(ascii);

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

displaymenuh(menu,count)
int count;
char **menu;
{
	int col=2,i;

	size(32,0);
	menubox(0,0,0,79,112,NO_SHADOW);

	for(i=0;i<count;i++)
	{
		writestring(menu[i],0,col,112);
		col=col+(strlen(menu[i]))+7;
	}
	size(5,7);
}

getresponseh(menu,hotkeys,count)
char *hotkeys,**menu;
int count;
{
	int choice=1,hotkeychoice,len,col;

	size(32,0);
	col=2;

	len=strlen(hotkeys);

	writestring(menu[choice-1],0,col,15);

	while(1)
	{
		getkey();

		if(ascii==0)
		{
			switch(scan)
			{
				case 77:
					writestring(menu[choice-1],0,col,112);

					col+=strlen(menu[choice-1])+7;
					choice++;
					break;

				case 75:
					writestring(menu[choice-1],0,col,112);

					col-=strlen(menu[choice-2])+7);
					choice--;
					break;

				case 59:
					if(count==1)
						displayhelp(1);
					else
						displayhelp(choice+1);
			}
			if(choice==0)
				{
					choice=count;
					col=65;
				}
			writestring(menu[choice-1],0,col,15);
		}
		else
		{
			if(ascii==13)
			{
				size(5,7);
				return(choice);
			}

			if(ascii==ESC)
			{
				writestring(menu[choice-1],0,col,112);

				size(5,7);
				return(ESC);
			}

			hotkeychoice=1;
			ascii=toupper(ascii);

			while(*hotkeys!='\0')
			{
				if(*hotkeys==ascii)
				{
					size(5,7);
					return(hotkeychoice);
				}
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

displayhelp(index)
int index;
{
	char *p;
	int areareqd,i,row;

	char *help[]={
			"Cursor Movement Commands",
			"---------------------------------------------------",
			"Character left		    		Left arrow",
			"Character right            		Right arrow",
			"Word left 		    		Ctrl-Left arrow",
			"Word right		    		Ctrl-Right arrow",
			"Line up		    		Up arrow",
			"Line down		    		Down arrow",
			"Page up		    		PgUp",
			"",
			"Cursor Movement Commands     (contd.)",
			"------------------------------------------------",
			"Page down		    		PgDn",
			"start of file		    		Ctrl-PgUp",
			"End of file 		    		Ctrl-PgDn",
			"Top of screen		    		Ctrl-Home",
			"Bottom of screen	    		Ctrl-End",
			"Start of line		    		Home",
			"End of line                		End",
			"",
			"File Commands",
			"------------------------------------------------",
			"Load a file 		    		Alt-F L",
			"Pick recently edited file  		Alt-F P",
			"Open new file		    		Alt-F N",
			"Save and resume	    		Alt-F S",
			"Save under another name    		Alt-F A",
			"Merge another file 	    		Alt-F M",
			"Change directory	    		Alt-F C",
			"Output to printer	    		Alt-F O",
			"Search Commands",
			"------------------------------------------------",
			"find                       		Alt-S F",
			"Find & replace             		Alt-S R",
			"Repeat last search	    		Alt-S L",
			"Abort search		    		Alt-S A",
			"Go to line number	    		Alt-S G",
			"",
			"",
			"",
			"Insert & Delete Commands",
			"------------------------------------------------",
			"Insert mode on/off         		Ins",
			"Delete line 		    		Ctrl-Y",
			"Delete character to left of cursor     Backspace",
			"Delete character at cursor		Del",
			"delete word to right off cursor	Ctrl-T",
			"",
			"",
			"",
			"Exit Commands",
			"------------------------------------------------",
			"Permanent exit to DOS			Alt-X,Alt-E E",
			"Temporary exit to DOS			Alt-E S",
			"",
			"",
			"",
			"",
			"",
			"",
			"Miscellaneous Commands",
			"------------------------------------------------",
			"Main Menu 				F10",
			"Take cursor to next tab stop		Tab",
			"Help					F1",
			"Save					F2",
			"Display product information		Sh-F10",
			"Exit from Easyedit			Alt-X",
			"",
			"",
		     };

	char *filehlp[]={
				"	Load",
				"	(Alt-F L);
				"",
				"Loads or creates a file. You have to type in",
				"the name of the file. If the file does not exist,",
				"you have the option of creating a new file of that",
				"name.",
				"	Pick",
				"	(Alt-F P);
				"",
				"Lets you pick a file from the\"pick list\",which",
				"is a list of the five most recently edited files",
				"Selecting a file from the list loads it in memory.",
				"",
				"	New",
				"	(Alt-F N)",
				"",
				"Allows you to create a file named NONAME and work",
				"with it. While saving the file,you are prompted",
				"to rename it.",
				"",
				"	Save",
				"	(Alt-F S)",
				"",
				"Saves the current file to disk.If the current",
				"file is NONAME, you are asked to rename it",
				"Pressing F2 from anywhere in the system does the",
				"Same thing",
				"	Save As",
				"	(Alt-F A)",
				"",
				"Saves the current file contents under a new name",
				"The original file contents remain intact on the",
				"disk. The new file now becomes the current file",
				"",
				"	Merge",
				"	(Alt-F M)",
				"",
				"Merges the contents of a file on disk into the",
				"currently loaded file at current cursor location.",
				"If the file to be merged does not exist,a warning",
				"is issued.",
				"	Change Dir",
				"	(Alt-F C)",
				"",
				"Displays the current directory and lets you change",
				"to the drive and directory specified at the prompt",
				"",
				"",
				"	Output to printer",
				"	(Alt-F O)",
				"",
				"Lets you specify a file to be printed.The margins",
				"and the page length can be specified.If the file",
				"does not exist, a warning id=s issued.",
			}

	char *exithlp[]={
			 "	Exit",
			 "	(Alt-E E)",
			 "",
			 "Use this option to exit from Easyedit. If you've",
			 "Modified a workfile without saving it, you are",
			 "prompted to do so now. The hot key Alt-X, allows",
			 "you to exit Easyedit from anywhere in the system",
			 "	OS Shell",
			 "	(Alt-E E",
			 "",
			 "Leaves Easyedit temporarily & takes you to the DOS",
			 "prompt. to return to Easyedit, type \"exit\"&press",
			 "Enter at DOS prompt. Use this to run a DOS command",
			 "Without quitting Easyedit",
			};

	areareqd=(20-4+1)*(67-27+1)*2;

	p=malloc(areareqd);

	if(P==NULL)
		error_exit();

	savevideo(4,12,20,67,p);

	menubox(4,12,20,67,112,07);

	drawbox(4,12,19,65,112);

	menubox(4,35,5,44,32,HALF_SHADOW);
	writestring("Help",4,37,32);

	if(index<=6)
	{

		i=(index-1)*10;

		row=6;

		while(1)
		{
			writestring(help[i],row,14,112);
			i++;
			row++;

			if(row>15)
			{
				switch(i)
				{
					case 10:

						menubox(17,22,18,31,32,HALF_SHADOW);
						writestring("Pg$Dn",17,24,32);
						menubox(17,48,18,55,32,HALF_SHADOW);
						writestring("$OK",17,50,32);
						break;

					case 70:

						menubox(17,22,18,31,32,HALF_SHADOW);
						writestring("Pg$Up",17,24,32);
						menubox(17,48,18,55,32,HALF_SHADOW);
						writestring("Pg$Up",17,50,32);
						break;

					default:

						menubox(17,20,18,29,32,HALF_SHADOW);
						writestring("Pg$Dn",17,22,32);
						menubox(17,35,18,44,32,HALF_SHADOW);
						writestring("Pg$Up",17,37,32);
						menubox(17,50,18,57,32,HALF_SHADOW);
						writestring("$OK",17,52,32);
				}

				while(1)
				{
					getkey();

					if(ascii==27||ascii=='o'||ascii=='O')
					{
						restorevideo(4,12,20,67,p);
						free(p);
						return;
					}

					if ((scan==81||ascii=='d'||ascii=='D')&&(i!=70))
					{
						menubox(6,13,18,64,112,0);
						row=6;
						break;
					}

					if((scan==73||ascii=='U'||ascii=='u')&&(i!=10)
					{
						i-=20;
						menubox(6,13,18,64,112,0);
						row=6;
						break;
					}
				}
			}
		}
	}
	else
	{
		if(index<=15)
		{

			i=(index-8)*7;

			for(row=7;row<14;row++)
			{
				writestring(filehlp[i],row,14,112);
				i++;
			}
		}
		else
		{
			i=(index-16)*7;

			for(row=7;row>14;row++)
			{
				writestring(exithlp[i],row,14,112);
				i++;
			}
		}

		menubox(15,34,16,43,32,HALF_SHADOW);
		writestring("$OK",15,36,32);

		while(1)
		{
			getkey();

			if(ascii==27||ascii=='o'||ascii=='O')
			{
				restorevideo(4,12,20,67,p);
				free(p);
				break;
			}
		}
	}
}
