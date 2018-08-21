			//CURSOR.C

start_file()
{
	int display=YES;

	if(curscr==startloc&&skip==0)
		display=NO;

	curr=2;
	curc=1;
	logr=1;
	logc=1;
	skip=0;
	curscr=startloc;
	currow=startloc;

	if(display==YES)
	{
		menubox(2,1,22,78,27,NO_SHADOW);
		displayscreen(curscr);
	}

	writerow();
	writecol();

}

end_file()
{
	char *temp;
	int i,status;

	size(32,0);

	logr=1;
	curr=2;
	temp=startloc;
	while(temp!=endloc)
	{
		if(*temp=='\n')
			curr++;
		temp++;
	}

	curscr=endloc;
	currow=endloc;

	page_up(0);

	for(i=0;i<20;i++)
	{
		status=down_line(0);

		if(status==1)
			break;
	}

	end_line();

	size(5,7);

	writerow();
}

top_screen()
{
	size(32,0);

	while(logr!=1)
		up_line(0);

	writerow();

	size(5,7);
}

bottom_screen()
{
	int status;

	size(32,0);
	while(logr!=21)
	{
		status=down_line(0);

		if(status==1)
			break;
	}

	writerow();
	size(5,7);
}

start_line()
{
	if(skip!=0)
	{
		skip=0;
		menubox(2,1,22,78,27,NO_SHADOW);
		displayscreen(curscr);
	}
	logc=1;
	curc=1;

	writecol();
}

end_line()
{
	char *temp;
	int count, display=YES;

	temp=currow;
	count=1;

	while(*temp!='\n')
	{
		if(temp>=endloc)
			break;

		if(*temp=='\t')
			count+=8;
		else
			count++;

		temp++;
	}
	while(*(temp-1)=='\t'||*(temp-1)=='')
	{
		if(*(temp-1)=='\t')
			count-=8;
		else
			count++;

		temp--;
	}

	if(count<=78)
	{
		if(skip==0)
			display=NO;
		else
			skip=0;

		logc=count;
	}
	else
	{
		skip=count-78;
		logc=78;
	}
	if(display==YES)
	{
		menubox(2,1,22,78,27,NO_SHADOW);
		displayscreen(curscr);
	}

	curc=count;
	writecol();
}

word_left()
{
	char *temp;
	int col,count=0,condition1,condition2,condition3;

	temp=currow;
	for(col=1;col<curc;col++)
	{
		if(temp>=endloc)
			break;

		if(*temp=='\t')
			col+=7;

		if(*temp=='\n')
		{
			end_line();
			break;
		}
		temp++;
	}

	if(temp>=endloc)
		temp--;

	condition1=isalnum(*temp)&&(*(temp-1));

	condition2=isalnum(*temp)&&!isalnum(*(temp-1));

	condition3=!isalnum(*temp);

	if(*temp=='\n')
		temp--;

	if(condition2)
		temp--;

	if(condition1)
	{
		while(isalnum(*temp))
		{
			if(temp==startloc)
				break;

			temp--;
			count++;
		}
	}

	if(condition2||condition3)
	{
	while(!(isalnum(*temp)))
	{
		if(temp<=startloc)
			break;

		if(*temp=='\t')
			count+=7;

		if(*temp=='\n')
		{

			up_line(0);
			end_line();

			return;
		}

		while(isalnum(*temp))
		{
			if(temp==startloc)
				break;

			temp--;
			count++;
		}
	}

	if(temp==startloc)
	{
		logc=1;
		curc=1;
	}
	else
	{
		logc-=count;
		curc-=count;

		if(curc>78)
		{
			logc=78;
			skip=curc-78;
			menubox(2,1,22,78,27,NO_SHADOW);
			displayscreen(curscr);
		}
	}

	writecol();
	writerow();
}

word_right()
{
	char *temp;
	int col,count=0;

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
		while(!(isalnum(*temp)))
		{
			if(temp>=endloc)
				break;

			if(*temp=='\t')
				count+=7;

			if(*temp=='\n')
			{
				down_line(0);

				start_line();

				temp=currow;
				count=0;
				continue;
			}
			temp++;
			count++;
		}
	}
	else
	{
		count=0;

		while(isalnum(*temp))
		{
			if(temp>=endloc)
				break;
			temp++;
			count++;
		}

		while(!(isalnum(*temp)||*temp=='\n'))
		{
			if(temp>=endloc)
				break;

			if(*temp=='\t')
				count+=7;

			temp++;
			count++;
		}
	}

	logc+=count;
	curc+=count;

	if(curc>78)
	{
		logc=78;
		skip=curc-78;
		menubox(2,1,22,78,27,NO_SHADOW);
		displayscreen(curscr);
	}

	writecol();
	writerow();
}

page_up(display)
int display;
{
	int row;

	if(curscr==startloc)
		return;

	for(row=1;row<=20;row++)
	{
		curscr-=2;

		if(curscr<=startloc)
		{
			curscr=startloc;
			currow=startloc;
			logr=1;
			curr=2;
			logc=1;
			curc=1;

			menubox(2,1,22,78,27,NO_SHADOW);
			displayscreen(curscr);

			if(display)
			{
				writecol();
				writerow();
			}

			return;
		}

		while(*curscr!='\n')
		{
			if(curscr<=startloc)
			{
				curscr=startloc;
				break;
			}
			curscr--;
		}
		if(curscr!=startloc&&*(curscr+1)!='\n')
			curscr++;
	}

	menubox(2,1,22,78,27,NO_SHADOW);
	displayscreen(curscr);

	for(row=1;row<=20;row++)
	{
		currow-=2;

		if(currow<startloc)
			currow=startloc;

		while(*currow!='\n')
			currow--;

		if(currow!=startloc||*(currow+1)=='\n')
			currow++;
	}

	curr-=20;

	gotocol();

	if(display)
	{
		writerow();
		writecol();
	}
}

page_down()
{
	char *p;
	int row=1,i,col;

	p=curscr;
	for(row=1;row<=20;row++)
	{
		while(*curscr!='\n')
		{
			if(curscr>=endloc)
			{
				curscr=p;
				return;
			}
		curscr++;
		}
		if(curscr>=endloc)
		{
			curscr=p;
			return;
		}

		curscr++;
	}

	menubox(2,1,22,78,27,NO_SHADOW);
	displayscreen(curscr);

	size(32,0);

	row=1;
	while(currow!=curscr)
	{
		if(*currow=='\n')
		{
			curr++;
			row++;
		}
		currow++;
	}
	logr=1;
	col=curc;

	for(i=row;i<=20;i++)
		down_line(0);

	curc=col;

	gotocol();

	writerow();
	writecol();

	size(5,7);
}

right()
{
	char *temp;
	int col;

	if(curc>=249)
	{
		curc=249;
		printf("\a");
		return;
	}

	temp=currow;
	for(col=1;col<curc;col++)
	{
		if(*temp=='\t')
			col+=7;

		if(*temp=='\n')
			break;

		temp++;
	}

	if(*temp=='\t')
	{
		logc+=7;
		curc+=7;
	}

	curc;

	if(logc>=78)
	{
		skip=curc-78;
		menubox(2,1,22,78,27,NO_SHADOW);
		displayscreen(curscr);
		logc=78;
	}
	else
		logc++;

	writecol();
}

left()
{
	int col=1;
	char *temp;

	if(curc==1)
		return;

	temp=currow;
	for(col=1;col<curc;col++)
	{
		if(*temp=='\t')
			col+=7;

		if(*temp=='\n')
			break;

		temp++;
	}

	if(*(temp-1)=='\t')
	{
		logc-=7;
		curc-=7;

	}

	if(logc<=1&&skip!=0)
	{
		logc=1;
		curc--;
		skip=curc-logc;
		menubox(2,1,22,78,27,NO_SHADOW);
		displayscreen(curscr);
	}
	else
	{
		curc--;
		logc--;
	}
	writecol();
}

up_line(display)
int display;
{
	if(curr==2)
		return;

	del_whitespace();

	logr--;
	curr--;

	currow-=2;
	if(curscr<startloc)
		curscr=startloc;
	while(*currow!='\n')
	{
		if(currow<=startloc)
			break;

		currow--;
	}
	if(currow!=startloc||*(currow+1)=='\n')
		currow++;

	if(logr<1)
	{
		logr=1;
		curscr=currow;
		scrolldown(2,1,22,78);
		displayline(curscr,2);
	}

	gotocol();

	if(display)
	{
		writecol();
		writerow();
	}
}

down_line(display)
int display;
{
	char *p;

	del_whitespace();

	p=currow;
	while(*currow!='\n')
	{
		if(currow>=endloc)
		{
			currow=p;
			return(1);
		}
		currow++;
	}
	if(currow==endloc)
	{
		currow=p;
		return(1);
	}
	currow++;
	logr;
	curr;

	if(logr>=22)
	{
		logr=21;
		scrollup(2,1,22,78);
		displayline(currow,22);

		while(*curscr!='\n')
			curscr++;
		curscr++;
	}

	gotocol();
	{
		writecol();
		writerow();
	}
	return 0;
}

scrolldown(sr,sc,er,ec)
int sr,sc,er,ec;
{

	union REGS ii,oo;

	ii.h.ah=6;
	ii.h.al=1;
	ii.h.ch=sr;
	ii.h.cl=sc;
	ii.h.dh=er;
	ii.h.dl=ec;
	ii.h.bh=27;
	int86(16,&ii,&oo);
}

gotocol()
{
	char *temp;
	int col;

	temp=currow;
	for(col=1;col<curc;col++)
	{
		if(*temp=='\t')
			col+=7;

		if(*temp=='\n')
			break;

		temp++;
	}

	if(col>curc)
	{
		logc+=(col-curc);
		curc=col;

		if(curc>78)
		{
			logc=78;
			skip=curc-78;
			menubox(2,1,22,78,27,NO_SHADOW);
			displayscreen(curscr);
		}
	}
