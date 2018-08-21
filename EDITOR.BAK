//		      EDITOR.C
#include<math.h>
#include<dos.h>
#include<process.h>
#include<alloc.h>
#include<stdlib.h>
#include<stdio.h>
#include<ctype.h>
#include<conio.h>

#include<util.c>
#include<cursor.c>

main(argc,argv)
int argc;
char *argv[];

{
	int flag;

	if(argc>2)
	{
		printf("\nInvalid number of parameters!");
		printf("\nPress any key....");
		fflush(stdin);
		getch();
		exit(1);
	}

#ifdef CGA
{
	vid_mem=(char far*)0xb8000000L;
	textmode(3);
}
#else
{
	vid_mem=(char far*)0xb0000000L;
	textmode(7);

}
#endif

old23=getvect(0x23);
setvect(0x23,handler);

old1b=getvect(0x1b);
setvect(0x1b,handler);

maxsize=coreleft()-5000;

buf=malloc(maxsize);
if(buf==NULL)
	error_exit();

startloc=endloc=curscr=currow=buf;

*ins |=0x80;

strcpy(filespec,"NONAME");

workscreen();
displaymenuh(mainmenu,5);
about();

if(argc==2)
{
strcpy(filespec,argv[1]);
flag=load();

	if(flag==0)
	{
		strcpy(filespec,"NONMAME");
		write_fname();
	}
}

while(1)
{
	gotoxy(logc+1,logr+2);
	getkey();

	if(*ins&0x80)
		writestring("Insert",24,73,47);
	else
		writestring("	",24,73,47);

	if(ascii==0)
	{
		switch(scan)
		{
		case 59:
			displayhelp(1);
			break;

		case 60:
			save();
			break;

		case 45:
			check_saved();
			exit(0);

		case 46:
			writestring(mainmenu[0],0,2,15);

			cserver();

			writestring(mainmenu[0],0,2,112);

			break;

		case 33:
			writestring(mainmenu[1],0,25,15);

			fserver();

			writestring(mainmenu[1],0,25,112);

			break;

		case 31:
			writestring(mainmenu[2],0,37,15);
			sserver();
			writestring(mainmenu[2],0,37,112);
			break;

		case 32:
			writestring(mainmenu[3],0,51,15);
			dserver();
			writestring(mainmenu[3],0,51,112);
			break;

		case 18:
			writestring(mainmenu[4],0,65,15);
			eserver();
			writestring(mainmenu[4],0,65,112);
			break;

		case 68:
			mm_server();
			break;

		case 93:
			about();
			break;

		case 75:
			left();
			break;

		case 77:
			right();
			break;

		case 72:
			up_line(1);
			break;

		case 80:
			down_line(1);
			break;

		case 73:
			page_up(1);
			break;

		case 81:
			page_down();
			break;

		case 71:
			start_line();
			break;

		case 79:
			end_line();
			break;

		case 132:
			start_file();
			break;

		case 118:
			end_file();
			break;

		case 119:
			top_screen();
			break;

		case 117:
			bottom_screen();
			break;

		case 115:
			word_left();
			break;

		case 116:
			word_right();
			break;

		case 83:
			del_char();
			break;
		}
	}

	else
	{
		switch(ascii)
		{
			case 8:
				backspace();
				break;

			case 20:
				del_word_rt();
				break;

			case 25:
				del_line();
				break;

			case 12:
				repeat_last();
				break;

			default:
				if((ascii>=32&&ascii<=126)||ascii==13||ascii==9)
				displaychar(ascii);
			}
		}
	}
}

mm_server()
{
	int mchoice,esc_flag;

	while(1)
	{
		displaymenuh(mainmenu,5);
		mchoice=getresponseh(mainmenu,"CFSDE",5);

		switch(mchoice)
		{
			case 1:
				esc_flag=cserver();
				break;

			case 2:
				esc_flag=fserver();
				break;


			case 3:
				esc_flag=sserver();
				break;


			case 4:
				esc_flag=dserver();
				break;


			case 5:
				esc_flag=eserver();
				break;

			case ESC:
				esc_flag=ESC;
		}

		if(esc_flag==ESC)
			return(esc_flag);
	}
}

cserver()
{
	int cchoice,esc_flag=0;

	cchoice=popupmenuv(cursormenu,11,1,0,"SFTBLEWRUDN",1);

	switch(cchoice)
	{
		case 1:
			start_file();
			break;

		case 2:
			end_file();
			break;

		case 3:
			top_screen();
			break;

		case 4:
			bottom_screen();
			break;

		case 5:
			start_line();
			break;

		case 6:
			end_line();
			break;

		case 7:
			word_left();
			break;

		case 8:
			word_right();
			break;

		case 9:
			page_up(1);
			break;

		case 10:
			page_down();
			break;

		case 11:
			esc_flag=mm_server();
			break;

		case 75:
			writestring(mainmenu[0],0,2,112);
			writestring(mainmenu[4],0,65,15);
			esc_flag=eserver();
			writestring(mainmenu[4],0,65,112);
			break;

		case 77:
			writestring(mainmenu[0],0,2,112);
			writestring(mainmenu[1],0,25,15);
			esc_flag=fserver();
			writestring(mainmenu[1],0,25,112);
			break;

		case ESC:
			esc_flag=ESC;
	}
	return(esc_flag);
}

fserver()
{
	int fchoice,flag,esc,esc_flag=0;
	char fname[30];
	fchoice=popupmenuv(filename,9,1,23,"LPNSAMCOT",3);

	switch(fchoice)
	{
		case 1:
			check_saved();
			strcpy(fname,filespec);

			esc_flag=ask_name("Enter file name",filespec);
			if(esc_flag==ESC)
				break;

			flag=load();

			if(flag==0)
			{
				strcpy(filespec,fname);
				write_fname();
			}
			break;

		case 2:
			pick();
			break;

		case 3:
			new();
			break;

		case 4:
			save();
			break;

		case 5:
			save_as();
			break;

		case 6:
			merge();
			break;

		case 7:
			change_dir();
			break;

		case 8:
			print();
			break;

		case 9:
			esc_flag=mm_server();
			break;

		case 75:
			writestring(mainmenu[1],0,25,112);
			writestring(mainmenu[0],0,2,15);
			esc_flag=cserver();
			writestring(mainmenu[0],0,2,112);
			break;

		case 77:
			writestring(mainmenu[1],0,25,112);
			writestring(mainmenu[2],0,37,15);
			esc_flag=cserver();
			writestring(mainmenu[2],0,37,112);
			break;

		case ESC:
			esc_flag=ESC;
	}
	return(esc_flag);
}

sserver()
{
	int schoice,esc_flag=0;

	schoice=popupmenuv(searchmenu,6,1,35,"FRLAGT",4);

	switch(schoice)
	{
		case 1:
			findflag=1;
			frflag=0;

			find();
			break;

		case 2:
			findflag=0;
			frflag=1;

			replace();
			break;

		case 3:
			repeat_last();
			break;

		case 4:
			abort_find();
			break;

		case 5:
			gotoline();
			break;

		case 6:
			esc_flag=mm_server();
			break;

		case 75:
			writestring(mainmenu[2],0,37,112);
			writestring(mainmenu[1],0,25,15);
			esc_flag=fserver();
			writestring(mainmenu[1],0,51,112);
			break;

		case 77:
			writestring(mainmenu[2],0,37,112);
			writestring(mainmenu[3],0,51,15);
			esc_flag=dserver();
			writestring(mainmenu[3],0,51,112);
			break;

		case ESC:
			esc_flag=ESC;
	}
	return (esc_flag);
}

dserver()
{
	int dchoice,esc_flag=0;

	dchoice=popupmenuv(deletemenu,5,1,49,"DEBRT",5);

	switch(dchoice)
	{
		case 1:
			del_line();
			break;

		case 2:
			del_line_rt();
			break;

		case 3:
			del_line_lt();
			break;

		case 4:
			del_word_rt();
			break;

		case 5:
			esc_flag=mm_server();
			break;

		case 75:
			writestring(mainmenu[3],0,51,112);
			writestring(mainmenu[2],0,37,15);
			esc_flag=sserver();
			writestring(mainmenu[2],0,37,112);

			break;

		case 77:
			writestring(mainmenu[3],0,51,112);
			writestring(mainmenu[4],0,65,15);
			esc_flag=sserver();
			writestring(mainmenu[4],0,65,112);

			break;
		case ESC:
			esc_flag=ESC;
	}
	return(esc_flag);
}

eserver()
{
	int fchoice,esc_flag;

	fchoice=popupmenuv(exitmenu,3,1,62,"EST",6);

	switch(fchoice)
	{
		case 1:
			check_saved();

			setvect(0x23, old23);
			setvect(0x1b, old1b);

			exit(0);
		case 2:
			shell();
			break;

		case 3:
			esc_flag=mm_server();
			break;

		case 75:
			writestring(mainmenu[4],0,65,112);
			writestring(mainmenu[3],0,51,15);
			esc_flag=dserver();
			writestring(mainmenu[3],0,51,112);
			break;

		case 77:
			writestring(mainmenu[4],0,65,112);
			writestring(mainmenu[0],0,2,15);
			esc_flag=cserver();
			writestring(mainmenu[0],0,2,112);

			break;

		case ESC:
			esc_flag=ESC;
	}
	return(esc_flag);
}

workscreen()
{
	size(32,0);

	menubox(1,0,23,79,27,NO_SHADOW);

	drawbox(1,0,23,79,27);

	write_fname();

	menubox(24,0,24,79,112,NO_SHADOW);

	status_line();

	size(5,7);
}

about()
{
	int area;
	char *p;

	size(32,0);

	area=(17-6+1)*(60-19+1)*2;
	p=malloc(area);
	if(p==NULL)
		error_exit();

	savevideo(6,19,17,60,p);
	menubox(6,19,17,60,112,7);
	drawbox(6,19,16,58,112);

	writestring("Easyedit",7,35,112);
	writestring("Version 1.00",9,33,112);
	writestring("Designed and developed at",10,27,112);
	writestring("ICIT,44-A,Gokulpeth",11,28,112);
	writestring("Nagpur, India",12,32,112);

	menubox(14,36,15,43,32,HALF_SHADOW);
	writestring("OK",14,38,47);

	while(1)
	{
		getkey();

		if(ascii==ESC||ascii!='O'||ascii=='o')
			break;
	}

	restorevideo(6,19,17,60,p);
	free(p);

	size(5,7);
}

write_fname()
{
	int len;
	char drive[2],fname[9],ext[5];

	size(32,0);

	drawbox(1,0,23,79,27);

	writecol();
	writerow();

	if(filespec[1]==':')
		drive[0]=filespec[0];
	else
		drive[0]=getdisk()+65;
	drive[1]='\0';

	fnsplit(filespec,"","",fname,ext);

	strcpy(filename," ");
	strcat(filename,drive);
	strcat(filename,":");
	strcat(filename,fname);

	if(ext[0])
		strcat(filename,ext);
		strcat(filename," ");
		strupr(filename);

		len=strlen(filename);
		writestring(filename,1,39-len/2,27);

		size(5,7);
}

writerow()
{
	int i;
	char s[10];

	for(i=0;i<=3;i++)
		writechar(23,60+i,205,27);

	itoa(curr-1,s,10);
	writestring(s,23,64-strlen(s),15);
	writechar(23,64,':',15);

	gotoxy(logc+1,logr+2);
}

writecol()
{
	int i;
	char s[10];

	for(i=0;i<=2;i++)
		writechar(23,65+i,205,27);

	itoa(curc,s,10);
	writestring(s,23,65,15);
	writechar(23,64,':',15);

	gotoxy(logc+1,logr+2);
}

status_line()
{
	menubox(24,0,24,79,112,NO_SHADOW);
	writestring("^F^1-Help  ^F^2-Save  ^S^h-^F^1^0-Product Info ^A^l^t^-^X-Exit",24,1,112);

	if(*ins&0x80)
		writestring("Insert",24,73,47);
}

ask_name(str,name)
char *str,*name;
{
	int area,esc_flag,len;
	char *p,currentdir[31];

	area=(17-7+1)*(62-17+1)*2;
	p=malloc(area);
	if(p==NULL)
		error_exit();

	savevideo(7,17,17,62,p);
	menubox(7,17,17,62,112,7);
	drawbox(7,17,16,60,112);

	len=strlen(str);
	writestring(str,9,39-len/2,112);

	menubox(11,21,12,56,32,HALF_SHADOW);

	if(strcmp(str,"Enter directory name")==0)
	{
		getcwd(currentdir,30);
		writestring(currentdir,11,22,47);
	}

	menubox(14,27,15,51,32,HALF_SHADOW);
	writestring("Press Esc to cancel",11,22,47);

	esc_flag=getname(11,22,name);

	restorevideo(7,17,17,62,p);
	free(p);
	return(esc_flag);
}

getname(row,col,p)
int row,col;
char *p;
{
	int i=0;
	char str[30];

	size(5,7);

	while(1)
	{
		gotoxy(col+i+1,row+1);
		getkey();

		if(ascii==27)
			return(ESC);

		if(i==0)
			menubox(11,21,12,56,32,HALF_SHADOW);

		if(ascii==13||i>30)
			break;

		if(ascii=='\b')
		{
			if(i!=0)
			{
				i--;
				writechar(row,col+i,' ',47);
			}
		}

		if(isascii(ascii)&&!iscntrl(ascii))
		{
			str[i]=ascii;
			writechar(row,col+i,ascii,47);
			i++;
		}
	}
	str[i]='\0';
	strcpy(p,str);
	size(32,0);
	return(0);
}

message(str1,str2)
char *str1,*str2;
{
	int area,len;
	char *p;

	size(32,0);

	area=(17-8+1)*(60-19+1)*2;
	p=malloc(area);
	if(p==NULL)
		error_exit();

	savevideo(8,19,16,60,p);
	menubox(8,19,16,60,112,7);
	drawbox(8,19,15,58,112);

	writestring(filename,10,25,112);

	writestring(str1,10,26+strlen(filename),112);
	len=strlen(str2);
	writestring(str2,11,39-len/2,112);

	menubox(13,24,14,30,32,HALF_SHADOW);
	writestring("$Yes",13,24,32);

	menubox(13,44,14,53,32,HALF_SHADOW);
	writestring("$Cancel",13,44,32);
	menubox(13,34,14,39,32,HALF_SHADOW);
	writestring("$NO",13,34,32);

	while(1)
	{
		getkey();
		ascii=toupper(ascii);
		if(ascii=='Y'||ascii=='N'||ascii=='C')
			break;
	}

	restorevideo(8,19,16,60,p);
	free(p);

	size(5,7);
	return(ascii);
}

load()
{
	FILE *fp;
	int i=0,flag=0;
	char ans='N',*temp;

	temp=endloc;
	saved=YES;
	menubox(24,0,24,79,112,NO_SHADOW);
	write_fname();
	writestring("Loading Editor File...",24,1,112);

	endloc=buf;

	fp=fopen(filespec,"r");

	if(fp==NULL)
	{
		menubox(24,0,24,79,112,NO_SHADOW);

		ans=message("does not exist...","Create?");
	}
	else
	{
		while((buf[i]=getc(fp))!=EOF)
		{
			i++;

			if(i==maxsize)
			{
				ans=message("too large!","Truncate?");

				if(ans=='Y')
					break;

				else
				{
					endloc=temp;
					status_line();
					return(0);
				}
			}

			endloc++;
		}
	}

	if(fp!=NULL||ans=='Y')
	{
		curr=2;
		curc=1;
		logr=1;
		logc=1;
		skip=0;
		startloc=curscr=currow=buf;

		writerow();
		writecol();

		menubox(2,1,22,78,27,NO_SHADOW);

		displayscreen(curscr);

		strcpy(pickfile[pickfileno],filespec);

		pickfileno++;

		if(pickfileno>4)
			pickfileno=0;

		flag=1;
		status_line();
	}
	else
	{
		endloc=temp;
		status_line();
		return(0);
	}
	fclose(fp);

	return(flag);
}

check_saved()
{
	char ans;

	if(saved==NO)
	{
		ans=message("is not saved...","Save?");

		if(ans=='Y')
			save();
	}
}

displayline(p,row)
char *p;
int row;
{
	int col,tabflag=0,i,num;

	if(p>=endloc)
		return(0);

	num=skip;

	for(i=1;i<=skip;i++)
	{
		if(*p=='\n')
			return(1);

		if(*p=='\t')
		{
			if(num<=8)
				tabflag=1;
			else
			{
				i+=7;
				num-=8;

				p++;
				if(p>=endloc)
					return(0);
			}
		}
		else
		{
			p++;
			if(p>=endloc)
				return(0);
		}
	}

	for(col=1;col<79;col++)
	{
		if(*p=='\n')
			return(1);

		if(*p=='\t')
		{
			if(tabflag)
			{
				col+=(7-num);
				tabflag=0;
			}
			else
				col+=7;
		}
		else
			writechar(row,col,*p,27);

		p++;
		if(p>=endloc)
			return(0);
	}
	return(1);
}

displayscreen(p)
char *p;
{
	int row,status;

	for(row=2;row<23;row++)
	{
		status=displayline(p,row);

		if(status==0)
			return(0);

		while(*p!='\n')
		{
			p++;
			if(p>=endloc)
				return(0);
		}
		p++;

		if(p>=endloc)
			return(0);
	}
}

pick()
{
	int choice,flag;
	char fname[31];

	if(pickfileno==0)
		return;

	strcpy(fname,filespec);

	choice=popupmenuv(pickfile,pickfileno,1,23,"",7);

	if(choice!=ESC)
	{
		check_saved();
		strcpy(filespec,pickfile[choice-1]);

		flag=load();

		if(flag==0)
		{
			strcpy(filespec,fname);
			write_fname();
		}
	}
}

new()
{
	check_saved();

	strcpy(filespec,"NONAME");
	write_fname();

	curr=2;
	curc=1;
	logr=1;
	logc=1;
	saved=YES;

	startloc=endloc=curscr=currow=buf;

	menubox(2,1,22,78,27,NO_SHADOW);

	writecol();
	writerow();
}

save()
{
	FILE *fp;
	char *p;

	size(32,0);

	if(strcmp(filespec,"NONAME")==0)
	{
		ask_name("Enter file name",filespec);

		write_fname();

		strcpy(pickfile[pickfileno],filespec);
		pickfileno++;
		if(pickfileno>4)
			pickfileno=0;
	}

	fp=fopen(filespec,"w");
	if(fp==NULL)
	{
		message("File creation error","Return?");
		return(0);
	}

	menubox(24,0,24,79,112,NO_SHADOW);
	writestring("Saving Editor File...",24,1,112);

	while(p!=endloc)
	{
		putc(*p,fp);
		p++;
	}

	fclose(fp);
	saved=YES;
	status_line();
	size(5,7);
	return(1);
}

save_as()
{
	int success;

	size(32,0);

	ask_name("Enter new file name",filespec);

	success=save();

	if(success)
	{
		write_fname();
		strcpy(pickfile[pickfileno],filespec);
		pickfileno++;
		if(pickfileno>4)
			pickfileno=0;
	}
	size(5,7);
}

merge()
{
	int col,i;
	unsigned count=0;
	unsigned long totalsize;
	FILE *fp;
	char ans,str[17],*temp;

	size(32,0);

	strcpy(str,filename);

	ask_name("Enter file name",filename);

	fp=fopen(filename,"r");
	if(fp==NULL)
	{
		message("does not exist...","OK?");
		strcpy(filename,str);
		return;
	}

	while(getc(fp)!=EOF)
		count++;

	totalsize=(unsigned)(endloc-startloc);
	totalsize+=count;

	if(totalsize>=maxsize)
	{
		ans=message("too large!""Truncate?");

		if(ans=='Y')
			count=maxsize-(unsigned)(endloc-startloc);
		else
			return;
	}

	temp=currow;
	for(col=1;col<curc;col++)
	{
		if(*temp=='\t')
			col+=7;

		if(*temp=='\n'||temp==endloc)
			break;

		temp++;
	}

	memmove(temp+count,temp,endloc-temp);

	endloc+=count;
	saved=NO;

	rewind(fp);
	for(i=0;i<count;i++)
	{
		*temp=getc(fp);
		temp++;
	}

	menubox(logr+1,1,22,78,27,NO_SHADOW);

	displayscreen(curscr);

	strcpy(filename,str);

	size(5,7);
}

change_dir()
{
	char dirname[31],*p;
	int status,area,esc_flag;

	esc_flag=ask_name("Enter directory name",dirname);
	if(esc_flag)
		return;

	status=chdir(dirname);

	area=(17-8+1)*(60-19+1)*2;
	p=malloc(area);

	if(p==NULL)
		error_exit();

	savevideo(8,19,16,60,p);
	menubox(8,19,16,60,112,7);
	drawbox(8,19,15,58,112);

	menubox(10,21,11,56,32,HALF_SHADOW);
	menubox(13,21,14,56,32,HALF_SHADOW);

	if(status==0)
	{
		writestring("Directory successfully changed",10,22,47);
		write_fname();
	}
	else
		writestring("Error in changing directory",10,22,47);

	writestring("Press any key...",13,22,47);
	fflush(stdin);
	getch();

	restorevideo(8,19,16,60,p);
	free(p);
}

print()
{
	int area,tm,bm,pl,i,row=1,esc_flag,top_of_page=1;
	char *p,ch,topmargin[3],botmargin[3],pagelength[3],fname[31];
	FILE *fs;

	esc_flag=ask_name("Enter file name",fname);

	if(esc_flag)
		return;

	area=(17-8+1)*(60-19+1)*2;
	p=malloc(area);
	if(p==NULL)
		error_exit();

	savevideo(8,19,16,60,p);
	menubox(8,19,15,60,112,7);
	drawbox(8,19,14,58,112);

	fs=fopen(fname,"r");
	if(fs==NULL)
	{
		writestring("Unable to open",10,25,112);
		writestring(fname,10,40,112);
		writestring("Press any key to return...",11,24,112);
		fflush(stdin);
		getch();
		restorevideo(8,19,16,60,p);
		free(p);
		return;
	}

	esc_flag=ask_name("Top Margin",topmargin);
	esc_flag=ask_name("Bottom Margin",botmargin);
	esc_flag=ask_name("Page Length",pagelength);

	tm=atoi(topmargin);
	bm=atoi(botmargin);
	pl=atoi(pagelength);
	writestring("Set up the printer",9,27,112);
	writestring("Press any key when ready...",10,25,112);
	menubox(12,27,13,51,32,HALF_SHADOW);
	writestring("Press ESC to cancel",12,29,47);
	getkey();
	restorevideo(8,19,16,60,p);
	free(p);

	if(ascii==ESC)
		return;

	while((ch=fgetc(fs))!=EOF)
	{
		if(top_of_page)
		{
			for(i=0;i<tm;i++)
				putc('\n',stdprn);

			top_of_page=0;
		}

		putc(ch,stdprn);

		if(ch=='\n')
		{
			row++;

			if(row==pl-tm-bm)
			{
				for(i=0;i<bm;i++)
					putc('/n',stdprn);

				top_of_page=1;
				row=1;
			}
		}
	}
}

find()
{
	int esc_flag;

	esc_flag=ask_name("Enter search string",searchstr);
	if(esc_flag)
		return(esc_flag);

	search(searchstr);
}

char *search(searchstr)
char *searchstr;
{
	char *p,*temp,*t_loc;
	int len,area,col,tr,tc,tlr,tlc;

	t_loc=currow;
	tr=curr;
	tc=curc;
	tlr=logr;
	tlc=logc;

	len=strlen(searchstr);
	temp=currow;
	for(col=1;col<curc;col++)
	{
		if(*temp=='\t')
			col+=7;

		if(*temp=='\n'||temp>=endloc)
			break;

		temp++;
	}

	while(strcmp(searchstr,temp,len)!=0)
	{
		if(temp>=endloc)
		{
			area=(17-8+1)*(60-19+1)*2;
			p=malloc(area);
			if(p==NULL)
				error_exit();

			savevideo(8,19,16,60,p);
			menubox(8,19,16,60,112,7);
			drawbox(8,19,15,58,112);

			menubox(10,21,11,56,32,HALF_SHADOW);
			menubox(13,21,14,56,32,HALF_SHADOW);
			writestring("Search unsuccessful!",10,22,47);
			writestring("Press any key...",13,22,47);
			fflush(stdin);
			getch();

			currow=t_loc;
			curr=tr;
			curc=tc;
			logr=tlr;
			logc=tlc;

			restorevideo(8,19,16,60,p);
			free(p);
			size(5,7);
			return(0);
		}
		else
		{
			if(*temp=='\t')
			{
				curc+=8;
				temp++;
			}
			else
			{
				if(*temp=='\n')
				{
					curr++;
					curc=1;
					temp++;
					currow=temp;
				}
				else
				{
					curc++;
					temp++;
				}
			}
		}
	}
	logr=1;

	curc+=(len-1);

	if(curc>78)
	{
		skip=curc-78;
		logc=78;
	}
	else
	{
		skip=0;
		logc=curc;
	}

	curscr=currow;
	menubox(2,1,22,78,27,NO_SHADOW);
	displayscreen(curscr);
	writecol();
	writerow();

	size(5,7);
	return(temp);
}

replace()
{
	int esc_flag;

	esc_flag=ask_name("Enter search string",searchstr);
	if(esc_flag)
		return;

	esc_flag=ask_name("Replace with",replacestr);
	if(esc_flag)
		return;

	f_and_r(searchstr,replacestr);
}

f_and_r(searchstr,replacestr)
char *searchstr,*replacestr;
{
	int area,ls,lr,i;
	char *p,*temp,*wherefr,ans;

	wherefr=search(searchstr);

	if(wherefr==0)
		return 0;

	area=(17-8+1)*(60-19+1)*2;
	p=malloc(area);
	if(p==NULL)
		error_exit();

	savevideo(8,19,16,60,p);
	menubox(9,19,15,60,112,7);
	drawbox(9,19,14,58,112);

	menubox(11,29,12,48,32,HALF_SHADOW);
	writestring("Replace(Y/N)",11,30,47);
	size(5,7);

	while(!kbhit())
	{
		gotoxy(45,12);
		delay(10);
		gotoxy(logc+1,logr+2);
		delay(10);
	}

	fflush(stdin);
	ans=getch();
	restorevideo(8,19,16,60,p);
	free(p);

	if(!(ans=='y'||ans=='Y'))
		return(0);

	saved=NO;

	ls=strlen(searchstr);
	lr=strlen(replacestr);

	if(exceed_size((unsigned)(endloc-startloc+lr-ls)))
		return(1);

	memmove(wherefr+lr,wherefr+ls,endloc-(wherefr+ls));
	endloc+=(lr-ls);

	temp=wherefr;
	for(i=0;i<lr;i++)
	{
		*temp=replacestr[i];
		temp++;
	}

	curc+=(lr-ls);

	if(curc>78)
	{
		skip=curc-78;
		logc=78;
	}

	else
	{
		skip=0;
		logc=curc;
	}

	curscr=currow;
	menubox(2,1,22,78,27,NO_SHADOW);
	displayscreen(curscr);
	writecol();
}

repeat_last()
{
	if(findflag)
		search(searchstr);

	if(frflag)
		f_and_r(searchstr,replacestr);
}

abort_find()
{
	frflag=0;
	findflag=0;
}

gotoline()
{
	char lineno[31],*temp;
	int number,esc_flag;

	esc_flag=ask_name("Enter line number",lineno);
	if(esc_flag)
		return;

	number=atoi(lineno);
	currow=startloc;
	temp=currow;
	curr=2;
	curc=1;

	while(curr!=(number+1))
	{
		if(temp>=endloc)
			break;

		if(*temp=='\n')
		{
			curr++;
			temp++;
			currow=temp;
		}
		else
			temp++;
	}

	skip=0;
	curscr=currow;
	menubox(2,1,22,78,27,NO_SHADOW);
	displayscreen(curscr);

	logr=1;
	logc=1;
	writerow();
	writecol();

	size(5,7);
}

backspace()
{
	char *temp;
	int col;

	if(curc==1&&curr==2)
		return;

	temp=currow;
	for(col=1;col<curc;col++)
	{
		if(*temp=='\t')
			col+=7;

		if(*temp=='\n')
		{
			left();
			return;
		}

		temp++;
	}

	if(*(temp-1)=='\n')
	{
		up_line(1);

		end_line();

		del_char();
	}
	else
	{
		left();

		del_char();
	}
}

del_char()
{
	char *temp;
	int col,row,count=0;

	if(currow>=endloc)
		return;

	temp=currow;
	for(col=1;col<curc;col++)
	{
		if(temp>=endloc)
			return;

		if(*temp=='\t')
			col+=7;

		if(*temp=='\n')
			break;

		temp++;
	}

	if(temp>=endloc)
		return;

	if(*temp=='\n')
	{
		count=curc-col;

		memmove(temp+count,temp+1,endloc-temp);

		memset(temp,32,count);

		endloc+=(count-1);
		saved=NO;

		menubox(logr+1,1,logr+1,78,27,NO_SHADOW);
		displayline(currow,logr+1);

		scrollup(logr+2,1,22,78);

		temp=currow;
		for(row=logr+1;row<22;row++)
		{
			while(*temp!='\n')
			{
				if(temp>=endloc)
					return;
				temp++;
			}
			temp++;

			if(temp>=endloc)
				return;
		}
		displayline(temp,row);
	}
	else
	{
		memmove(temp,temp+1,endloc-temp);

		endloc--;
		saved=NO;

		menubox(logr+1,1,logr+1,78,27,NO_SHADOW);
		displayline(currow,logr+1);
	}
}

del_line()
{
	char *temp;
	int count=1,row;

	if(currow==endloc)
		return(0);

	temp=currow;
	while(*temp!='\n')
	{
		if(temp>=endloc)
			break;

		count++;
		temp++;
	}

	if(temp>=endloc)
	{
		endloc-=count;

		menubox(logr+1,1,logr+1,78,27,NO_SHADOW);

		up_line(1);
		start_line();

		return(0);
	}

	temp++;

	memmove(currow,temp,endloc-temp);

	endloc-=count;
	saved=NO;

	scrollup(logr+1,1,22,78);

	temp=currow;
	for(row=logr+1;row<22;row++)
	{
		while(*temp!='\n')
		{
			if(temp>=endloc)
				return(0);
			temp++;
		}
		temp++;

		if(temp>=endloc)
			return(0);
	}
	displayline(temp,row);
}

del_line_lt()
{
	char *temp;
	int count,col;

	if(currow>=endloc)
		return;

	temp=currow;
	count=0;
	for(col=1;col<curc;col++);
	{
		if(*temp=='\t')
			col+=7;

		if(*temp=='\n')
		{
			del_line();
			return;
		}
		temp++;
		count++;
	}

	memmove(currow,temp,endloc-temp);

	endloc-=count;
	saved=NO;

	menubox(logr+1,1,logr+1,78,27,NO_SHADOW);
	displayline(currow,logr+1);

	start_line();
}

del_line_rt()
{
	char *temp,*temp1;
	int col,count=0;

	if(currow>=endloc)
		return;

	temp=currow;
	for(col=1;col<curc;col++)
	{
		if(temp>=endloc)
			return;

		if(*temp=='\t')
			col+=7;

		if(*temp=='\n')
			return;

		temp++;
	}

	if(*temp=='\n')
		return;

	temp1=temp;
	count=0;
	while(*temp1!='\n')
	{
		if(*temp1>=endloc)
			break;

		temp1++;
		count++;
	}

	memmove(temp,temp1,endloc-temp1);

	endloc-=count;
	saved=NO;

	menubox(logr+1,1,logr+1,78,27,NO_SHADOW);
	displayline(currow,logr+1);
}

del_word_rt()
{
	char *temp,*temp1;
	int col,row,count=0;

	if(currow>=endloc)
		return;

	temp=currow;
	for(col=1;col<curc;col++)
	{
		if(temp>=endloc)
			return;

		if(*temp=='\t')
			col+=7;

		if(*temp=='\n')
			break;

		temp++;
	}

	if(temp>=endloc)
		return;

	if(*temp=='\n')
	{
		count=curc-col;

		memmove(temp+count,temp+1,endloc-temp);

		memset(temp,32,count);

		endloc+=(count-1);
		saved=NO;

		menubox(logr+1,1,logr+1,78,27,NO_SHADOW);
		displayline(currow,logr=1);

		scrollup(logr+2,1,22,78);

		temp=currow;
		for(row=logr+1;row<22;row++)
		{
			while(*temp!='\n')
			{
				if(temp>=endloc)
					return;
				temp++;
			}
			temp++;
			if(temp>=endloc)
				return;
		}
		displayline(temp,row);
	   }
	   else
	   {
		temp1=temp;

		if(isalnum(*temp1))
		{
			while(isalnum(*temp1))
			{
				if(temp1==endloc)
					break;

				temp1++;
				count++;
			}
		}
		else
		{
			temp1++;
			count++;
		}
		while(*temp1==' ')
		{
			if(temp1==endloc)
				break;

			temp1++;
			count++;
		}
		memmove(temp,temp1,endloc-temp1);
		endloc-=count;

		menubox(logr+1,1,logr+1,78,27,NO_SHADOW);
		displayline(currow,logr+1);
	}
}

shell()
{
	int area,status;
	char *p;

	area=(24-0+1)*(79-0+1)*2;
	p=malloc(area);
	if(p==NULL)
		error_exit();

	savevideo(0,0,24,79,p);
	menubox(0,0,24,79,7,NO_SHADOW);
	menubox(8,21,16,60,127,47);

	drawbox(9,23,14,56,127);

	writestring("Quitting temporarily to DOS",11,25,127);
	writestring("Type EXIT to return...",13,25,127);

	gotoxy(7,1);
	status=system("C:\\COMMaND.COM");

	if(status==-1)
	{
		writestring("Oops! cannot load COMMAND.COM!",11,25,127);
		writestring("Press any key..",13,25,127);
		fflush(stdin);
		getch();
	}

	restorevideo(0,0,24,79,p);
	free(p);
}

void interrupt handler()
{
ctrl_c_flag=1;
}

displaychar(ch)
char ch;
{
	char *temp;
	int col=1,insert;

	if(curc>=249)
	{
		printf("\a");
		return;
	}

	if(*ins&0x80)
		insert=YES;
	else
		insert=NO;

	if(ch=='\r')
		ch='\n';

	temp=currow;
	for(col=1;col<curc;col++)
	{
		if(*temp=='\n'||temp>=endloc)
		{
			if(ch==' ')
			{
				right();
				return;
			}

			if(ch=='\n')
				break;

			if(exceed_size((unsigned)(endloc-startloc+curc-col+1)))
				return;

			memmove(temp+curc-col,temp,endloc-temp);

			memset(temp,32,curc-col);

			temp+=curc-col;

			endloc+=curc-col;
			saved=NO;

			memmove(temp+1,temp,endloc-temp);

			*temp=ch;

			endloc++;

			writechar(logr+1,logc,ch,27);

			if(ch=='\t')
			{
				curc+=8;
				logc+=8;

				gotocol();
			}
			else
				right();

			return;
		}
		if(*temp=='\t')
			col+=7;

		temp++;
	}

	if(ch=='/n')
	{
		if(temp>=endloc)
		{
			if(exceed_size((unsigned)(endloc-startloc+1)))
				return;

			*temp=ch;

			endloc++;
			saved=NO;

			menubox(logr+1,logc,logr+1,78,27,NO_SHADOW);

			displayline(currow,logr+1);

			down_line(1);
			start_line();
			return;
		}

		if(insert==NO)
		{
			down_line(1);
			start_line();

			temp=currow;
			while(*temp==' '||*temp=='\t')
			{
				if(*temp=='\t')
					curc+=7;

				temp++;
				curc++;
			}

			if(curc>78)
			{
				logc=78;
				skip=curc-78;
				menubox(2,1,22,78,27,NO_SHADOW);
				displayscreen(curscr);
			}
			else
				logc=curc;

			writecol();
			return;
		}
	}
	if(insert==YES||temp==endloc||*temp=='\n')
	{
		if(exceed_size((unsigned)(endloc-startloc+1)))
			return;

		memmove(temp+1,temp,endloc-temp);

		endloc++;
	}

	*temp=ch;

	saved=NO;

	if(ch=='\n')
	{
		del_whitespace();

		menubox(logr+1,logc,logr+1,78,27,NO_SHADOW);

		scrolldown(logr+2,1,22,78);

		down_line(1);
		start_line();

		displayline(currow,logr+1);
	}
	else
	{
		menubox(logr+1,logc,logr+1,78,27,NO_SHADOW);

		displayline(currow,logr+1);

		if(ch=='\t')
		{
			curc+=8;
			logc+=8;

			gotocol();
		}
		else
			right();
	}
}

error_exit()
{
	writestring("Memeory Allocation Error! Press any key...",22,14,112);
	fflush(stdin);
	getch();
	exit(2);
}

del_whitespace()
{
	char *temp;

	temp=currow;
	while(*temp!='\n')
	{
		if(temp>=endloc)
			return;

		temp++;
	}

	while(*(temp-1)=='\t'||*(temp-1)==' ')
	{
		memmove(temp-1,temp,endloc-temp);
		temp--;
		endloc--;
	}
}

exceed_size(size)
unsigned int size;
{
	int area;
	void *p;

	if(size>=maxsize)
	{
		area=(14-11+1)*(64-15+1)*2;
		p=malloc(area);
		if(p==NULL)
			error_exit();

		savevideo(9,15,15,65,p);
		menubox(9,15,15,65,112,7);
		drawbox(9,15,14,63,112);

		writestring("Filee size too large! Delete some characters!!",11,17,112);
		writestring("Press any key...",12,17,112);
		getch();

		restorevideo(9,15,15,65,p);
		return(1);
	}
	return(0);
}
//-------------------------------------------------------------------------------------------------
