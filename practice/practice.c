 #include<avr/io.h>
#include<util/delay.h>
#include<avr/eeprom.h>

#define PINI PINA
#define DDRI DDRA

#define PORTM PORTC
#define DDRM DDRC

#define PORTH PORTD
#define DDRH DDRD

#define FORWARD     0b00001001
#define BACKWARD    0b00000110
#define STOP        0b00001111
#define SMOOTHRIGHT 0b00001000
#define SMOOTHLEFT  0b00000001
#define SHARPRIGHT  0b00001010
#define SHARPLEFT   0b00000101

#define HANDOPEN  0b00000010
#define HANDCLOSE 0b00000001
#define HANDUP    0b00001000
#define HANDDOWN  0b00000100

#define MAX 30

int arr[4][4]={
              { 1, 8, 9,16},
			  { 2, 7,10,15},
			  { 3, 6,11,14},
			  { 4, 5,12,13}
			  };
int brr[4][4]={
              { 1, 8, 9,16},
			  { 2, 7,10,15},
			  { 3, 6,11,14},
			  { 4, 5,12,13}
			  };
int crr[4][4]={
              { 1, 8, 9,16},
			  { 2, 7,10,15},
			  { 3, 6,11,14},
			  { 4, 5,12,13}
			  };
int drr[4][4]={
              { 1, 8, 9,16},
			  { 2, 7,10,15},
			  { 3, 6,11,14},
			  { 4, 5,12,13}
			  };

int X=-1,Y=0,k=0,x=0,y=0,xx=3,yy=3,xxx=0,yyy=3;
static int j=2,grab=0;
int dir=3,i=0,reset=0,X1,Y1,X2,Y2;
int xx1,yy1,xx2,yy2;
struct stack
{
     int a[MAX];
     int top;
};
typedef struct stack stack;
void turnleft()
{
	PORTM=FORWARD;
	_delay_ms(100);
    PORTM=STOP;
	_delay_ms(100);
	PORTM=SHARPLEFT;
	_delay_ms(150);
	PORTM=STOP;
    if(dir==4)
	{
	   dir=1;
	}
	else
	{
	   dir=dir+1;
	}
	PORTM=STOP;

}
void turnright()
{
	PORTM=FORWARD;
	_delay_ms(100);
	PORTM=STOP;
    _delay_ms(100);
    PORTM=SHARPRIGHT;
	_delay_ms(150);
	PORTM=STOP;	
	if(dir==1)
	{
	  dir=4;
	}
	else
	{
	  dir=dir-1;
	}
}
void buzzer()
{
      PORTB=0x01;
	  _delay_ms(10);
   	  PORTB=0x00;
	  _delay_ms(50);
}
void aboutturn()
{
    PORTM=FORWARD;
	_delay_ms(100);
	PORTM=STOP;
	_delay_ms(100);
    PORTM=SHARPRIGHT;
	_delay_ms(145);
	PORTM=STOP;	
	if(dir==1)
	{
	  dir=4;
	}
	else
	{
	  dir=dir-1;
	}
	PORTM=STOP;
    PORTM=SHARPRIGHT;
	_delay_ms(150);
	PORTM=STOP;	
	if(dir==1)
	{
	  dir=4;
	}
	else
	{
	  dir=dir-1;
	}
	PORTM=STOP;
}

void backturn()
{
    PORTM=SHARPRIGHT;
	_delay_ms(145);
	PORTM=STOP;	
	if(dir==1)
	{
	  dir=4;
	}
	else
	{
	  dir=dir-1;
	}
	PORTM=STOP;
    PORTM=SHARPRIGHT;
	_delay_ms(150);
	PORTM=STOP;	
	if(dir==1)
	{
	  dir=4;
	}
	else
	{
	  dir=dir-1;
	}
	PORTM=STOP;
}

void pickup()
{
        buzzer();
		PORTH=HANDOPEN;
		_delay_ms(100);
		PORTH=STOP;
		_delay_ms(100);
		PORTH=HANDDOWN;
		_delay_ms(80);
		PORTH=STOP;
		_delay_ms(100);
        PORTH=HANDCLOSE;
		_delay_ms(200);
		PORTH=STOP;
		_delay_ms(100);
		PORTH=HANDUP;
	    _delay_ms(300);
	    PORTH=STOP;
	 	_delay_ms(50);
		buzzer();
}
void place()
{
		PORTH=HANDDOWN;
		_delay_ms(60);
		PORTH=STOP;
		_delay_ms(70);
        PORTH=HANDOPEN;
		_delay_ms(70);
		PORTH=STOP;
		_delay_ms(70);
		PORTH=HANDUP;
		_delay_ms(250);
		PORTH=STOP;
		_delay_ms(70);
		PORTH=HANDCLOSE;
		_delay_ms(150);
		PORTH=STOP;
}

void node()
{  if(((PINI & 0x04)==0x04)||((PINI & 0x08)==0x08)||((PINI & 0x02)==0x02)) //&&((PINI & 0x20)==0b00000000))
   {
     eeprom_write_byte((uint8_t *)j,((10*X)+Y));
	  if(j==2)
	  buzzer();
	  if(j==3)
	  { buzzer();
	    buzzer();
      }
	  if(j==4)
	  { buzzer();
	    buzzer();
		buzzer();
      }if(j==5)
	  { buzzer();
	    buzzer();
		buzzer();
		buzzer();
      }
	  j++;
	  
   }
}
void gridfollow()
{
   PORTM=FORWARD;
   _delay_ms( 70);
   while(/*((PINI & 0x31)!=0b00000000)||*/((PINI & 0x33)!=0b00000000)||((PINI & 0x39)!=0b00000000)) 
   {
      if((PINI & 0x35)==0b00010001)  
	  {
    	PORTM=FORWARD;
	  }
	  else if((PINI & 0x35)==0b00010000) 
	  {
	    PORTM=SMOOTHRIGHT;
      }
      else if((PINI & 0x35)==0b00010100) 
	  {
	    PORTM=SMOOTHRIGHT;
	  }
	  else if((PINI & 0x35)==0b00000001) 
	  {
	    PORTM=SMOOTHLEFT;
	  }
      else if((PINI & 0x35)==0b00000101) 
	  {
	    PORTM=SMOOTHLEFT;
	  }
	  else if((PINI & 0x35)==0b00010001) 
	  {
	    PORTM=FORWARD;
	  }
	  else if((PINI & 0x35)==0b00010101) 
	  {
	    PORTM=STOP;
	  }
	  else if((PINI & 0x35)==0b00110001)
	  {
	   X1=xx1;Y1=yy1;
	   X2=xx2;Y2=yy2;
	   if(grab==0)
	   {
	    PORTM=STOP;
		_delay_ms(100);
	    pickup();
	    grab=1;
       }
	    xx1=X1;yy1=Y1;
	    xx2=X2;yy2=Y2;       
	  }
   }
   PORTM=STOP;
   _delay_ms(30);
   if(dir==3)
   {
     X=X+1;
   }
   if(dir==2)
   {
     Y=Y+1;
   }
   if(dir==1)
   {
     X=X-1;
   }
   if(dir==4)
   {
     Y=Y-1;
   }
}

void gridfollowfinal()
{
   PORTM=FORWARD;
   _delay_ms( 70);
   while((PINI & 0x35)!=0b00110001) 
   {
      if((PINI & 0x35)==0b00010001)  
	  {
    	PORTM=FORWARD;
	  }
	  else if((PINI & 0x35)==0b00010000) 
	  {
	    PORTM=SMOOTHRIGHT;
      }
      else if((PINI & 0x35)==0b00010100) 
	  {
	    PORTM=SMOOTHRIGHT;
	  }
	  else if((PINI & 0x35)==0b00000001) 
	  {
	    PORTM=SMOOTHLEFT;
	  }
      else if((PINI & 0x35)==0b00000101) 
	  {
	    PORTM=SMOOTHLEFT;
	  }
	  else if((PINI & 0x35)==0b00010001) 
	  {
	    PORTM=FORWARD;
	  }
	  else if((PINI & 0x35)==0b00010101) 
	  {
	    PORTM=STOP;
	  }
   }
}


void linefollow()
{
while((PINI & 0x35)!=0b00110001)
   { 
      if((PINI & 0x3F)==0b00010001)
	  {
    	PORTM=FORWARD;
	  }
	  else if((PINI & 0x3F)==0b00011000)
	  {
	    PORTM=SMOOTHRIGHT;
      }
      else if((PINI & 0x3F)==0b00011100)
	  {
	    PORTM=SMOOTHRIGHT;
	  }
	  else if((PINI & 0x3F)==0b00000011)
	  {
	    PORTM=SMOOTHLEFT;
	  }
      else if((PINI & 0x3F)==0b00000111)
	  {
	    PORTM=SMOOTHLEFT;
	  }
	  else if((PINA & 0x3F)==0b00011011)
	  {
	    PORTM=FORWARD;
	  }
	  else if((PINI & 0x3F)==0b00011111)
	  {
	    PORTM=STOP;
	  }
      else if((PINI & 0x3F)== 0b00010000)
	  {
	    turnright();
	  }
      else if((PINI & 0x3F)== 0b00000001)
	  {
	    turnleft();
		PORTH=HANDUP;
		_delay_ms(250);
		PORTH=STOP;
		_delay_ms(70);
	  }
   }
}
void push(stack *p,int n)
{
 	 if(p->top==MAX-1)
	     return;
     p->top++;
     p->a[p->top]=n;
}
int pop(stack *p)
{
 	 int n;
 	 if(p->top==-1)
 	     return 0;
     n=p->a[p->top];
     p->top--;
     return n;
}
int pushcheck(stack s,int n)
{
 	int i;
 	for(i=s.top;i>=0;i--)
      if(n==s.a[i])
         return 0;
      return 1;
}
int TOP(stack s)
{
 	return s.a[s.top];
}
void left(stack *p,int n,int *d)
{ if(n!=0)
 	 if(pushcheck(*p,n))
 	 {
      push(p,n);*d=*d+1;
     }
}
void below(stack *p,int n,int *d)
{ if(n!=0)
   	 if(pushcheck(*p,n))
     {
	   push(p,n);*d=*d+1;
     }  
}
void right(stack *p,int n,int *d)
{ if(n!=0)
 	 if(pushcheck(*p,n))
 	 {
         push(p,n);*d=*d+1;
     }
}
void up(stack *p,int n,int *d)
{ if(n!=0)
 	 if(pushcheck(*p,n))
 	 {
         push(p,n);*d=*d+1;
     }
}
int checkarray(int n,int p[][4])
{  int i,j;
	 for(i=0;i<4;i++)
	 {		 for(j=0;j<4;j++)
		  	 {    if(n==p[i][j])
  				        { return 10*i+j;}
			 }
	 }
}
void go(int start,int stop)
{
	int i;
	xx1=start/10;
	yy1=start%10;
	xx2=stop/10;
	yy2=stop%10;
	                                         // y-cordinate solving
	 if((yy2-yy1>=1) && (dir==3))
	 {
	 turnright();
	 for(i=0;i<yy2-yy1;i++)
	 {
	 gridfollow();
	 }
	 }
	 else if((yy2-yy1>=1) && (dir==1))
	 {
	 turnleft();	
	 for(i=0;i<yy2-yy1;i++)
	 {
	 gridfollow();
	 }
	 }
	 else if((yy2-yy1>=1) && (dir==4))
	 {
	 aboutturn();	
	 for(i=0;i<yy2-yy1;i++)
	 {
	 gridfollow();
	 }
	 }
	 else if((yy2-yy1>=1) && (dir==2))
	 {	
	 for(i=0;i<yy2-yy1;i++)
	 {
	 gridfollow();
	 }
	 }
	 else if((yy2-yy1<=-1) && (dir==1))
	 {
	 turnright();
	 for(i=0;i<yy1-yy2;i++)
	 {
	 gridfollow();
	 }
	 }
	 else if((yy2-yy1<=-1) && (dir==3))
	 {
	 turnleft();
	 for(i=0;i<yy1-yy2;i++)
	 {
	 gridfollow();
	 } 
	 }
	 else if((yy2-yy1<=-1) && (dir==4))
	 {
	 for(i=0;i<yy1-yy2;i++)
	 {
	 gridfollow();
	 }
	 }
	 else if((yy2-yy1<=-1) && (dir==2))
	 {
	 aboutturn();	
	 for(i=0;i<yy1-yy2;i++)
	 {
	 gridfollow();
	 }
	 }
	                                         //x-cordinate solving...
	 if((xx2-xx1>=1) && (dir==3))
	 {
	 for(i=0;i<xx2-xx1;i++)
	 {
	 gridfollow();
	 }
	 }
	 else if((xx2-xx1>=1) && (dir==1))
	 {
     aboutturn();
	 for(i=0;i<xx2-xx1;i++)
	 {
	 gridfollow();
	 }
	 }
	 else if((xx2-xx1>=1) && (dir==4))
	 {	
	 turnright();
	 for(i=0;i<xx2-xx1;i++)
	 {
	 gridfollow();
	 }
	 }
	 else if((xx2-xx1>=1) && (dir==2))
	 {
	 turnleft();	
	 for(i=0;i<xx2-xx1;i++)
	 {
	 gridfollow();
	 }
	 }
	 else if((xx2-xx1<=-1) && (dir==3))
	 {
	 aboutturn();
	 for(i=0;i<xx1-xx2;i++)
	 {
	 gridfollow();
	 }
	 }
	 else if((xx2-xx1<=-1) && (dir==1))
	 {
	 for(i=0;i<xx1-xx2;i++)
	 {
	 gridfollow();
	 }
	 }
	 else if((xx2-xx1<=-1) && (dir==2))
	 {
	 turnright();	
	 for(i=0;i<xx1-xx2;i++)
	 {
     gridfollow();
	 }
	 }
	 else if((xx2-xx1<=-1) && (dir==4))
	 {
	 turnleft();
	 for(i=0;i<xx1-xx2;i++)
	 {
	 gridfollow();
	 }
	 }
}


void travel(stack path,int a[][4])
{
    int i,start,stop,t;
 	if(path.top==-1)  return ; 
    for(i=path.top;i>0;i--)
	{
	   start=checkarray(pop(&path),a);
	   t=pop(&path);
	   stop=checkarray(t,a);
	   go(start,stop);
	   push(&path,t); 
	}
}

int checkpoint(int a[][4],int p,int *d,stack t)
{
 	int x=p/10;int y=p%10;
             if(x+1<=3){if(a[x+1][y]!=0 && pushcheck(t,a[x+1][y]))        *d=*d+1;}
             if(y+1<=3){if(a[x][y+1]!=0 && pushcheck(t,a[x][y+1]))        *d=*d+1;}
             if(x-1>=0){if(a[x-1][y]!=0 && pushcheck(t,a[x-1][y]))    	  *d=*d+1;}
		     if(y-1>=0){if(a[x][y-1]!=0 && pushcheck(t,a[x][y-1]))        *d=*d+1;}
 	return *d;
}
void findshortpath_1(int a[][4])
{
	stack s;stack t;stack path;int c[20];int k=0;int link[50]={0};int l;
 	s.top=-1;t.top=-1;path.top=-1;int j=-1;int i,n,u,q,g,w=0,v;
 	while(x!=xx || y!=yy)
 	{if(a[x][y]!=0)
       {     if(pushcheck(s,a[x][y]))
                  push(&s,a[x][y]);
             if(y+1<=3)  right(&s,a[x][y+1],&k);
             if(x-1>=0)  up(&s,a[x-1][y],&k);
             if(y-1>=0)  left(&s,a[x][y-1],&k);
             if(x+1<=3)  below(&s,a[x+1][y],&k);
       }
       j++;
	   link[j]=a[x][y]*10+k;
	   if(link[j]%10==0 )
	   {    while(link[j]%10 <2)
	   	 	{link[j]=0;	 j--;}		
	        l=0;
	        do{c[l]=TOP(s); pop(&s);l++;
			}while(TOP(s)!=link[j]/10);
			if(link[j]%10==2)
			{	int z=checkarray(c[l-2],a);  a[z/10][z%10]=0;  push(&s,c[l-1]);}
		    if(link[j]%10==3)
			{	int z=checkarray(c[l-3],a);  a[z/10][z%10]=0;  push(&s,c[l-1]); push(&s,c[l-2]); }
		    if(link[j]%10==4)
			{	int z=checkarray(c[l-4],a);  a[z/10][z%10]=0;  push(&s,c[l-1]);push(&s,c[l-2]);push(&s,c[l-3]);}
		    link[j]-=1;
		    int p = checkarray(TOP(s),a);
		    x=p/10; y=p%10;
	   }
	   else
	   {    int b=TOP(s);
            int q=checkarray(b,a);
	        x=q/10;y=q%10;
       }
       if(TOP(s)==a[xx][yy]) break;
        k=0;
    }  
   //peep(s);
       g=s.top;
       for(i=0;i<=g;i++)
       {v=pop(&s);    push(&t,v);}
	   g=t.top;
	   for(i=0;i<=g;i++)
       {    
	   		v=pop(&t);
            q=checkarray(v,a);
            push(&s,v);
             u=checkpoint(a,q,&w,s);
            if(TOP(s)==a[xx][yy])      break;
            if(u==2)
            {n=checkarray(pop(&t),a);  a[n/10][n%10]=0; }
	        if(u==3)
            {n=checkarray(pop(&t),a);  a[n/10][n%10]=0;
             n=checkarray(pop(&t),a);  a[n/10][n%10]=0;	}
			if(u==4)
            {n=checkarray(pop(&t),a);  a[n/10][n%10]=0;
			 n=checkarray(pop(&t),a);  a[n/10][n%10]=0;
			 n=checkarray(pop(&t),a);  a[n/10][n%10]=0; }
            w=0;
	   }
	   g=s.top;
	   for(i=0;i<=g;i++)
       {v=pop(&s);    push(&path,v);}
//	   peep(s);
       travel(path,a);
}

int main()
{
   DDRI=0x00;
   DDRM=0xFF;
   DDRH=0xFF;
   DDRB=0xFF;
   int anti=1;
  uint8_t a = eeprom_read_byte(0);
  uint8_t b,c;
 	
 if(a==0)
{
    buzzer();
	 while((X!=0) || (Y!=3))
	 {
	    gridfollow();
		_delay_ms(100);
		node();
	//	buzzer();
		PORTM=STOP;
		if(X==3)     //&&(x!=3))
		{
		  turnright();
		}
		if((X==0)&&(Y==1))
		{
		  turnleft();
		}
		if((X==0)&&(Y==2))
		{
		  turnleft();
		}
      }
	  PORTB=0x01;
	  _delay_ms(80);
   	  PORTB=0x00;
	  _delay_ms(50);
        eeprom_write_byte(0,5);
		eeprom_write_byte((uint8_t*)1,j);

	
}
else
  {   
      buzzer();
	  buzzer();
	  c=eeprom_read_byte((uint8_t *)1);
	  gridfollow();
      for(anti=2;anti<c;anti++)
      {
	    b=eeprom_read_byte((uint8_t *)anti);
		arr[b/10][b%10]=0;
		brr[b/10][b%10]=0;
		crr[b/10][b%10]=0;
		drr[b/10][b%10]=0;
	  }
	  findshortpath_1(arr);
	  if(arr[3][0]!=0)
	  {
	   x=xx;y=yy;xx=3;yy=0;
	   findshortpath_1(brr);
	   x=3;y=0;xx=0;yy=3;
	   findshortpath_1(crr);
      }
	  else
	  {
	    x=xx;y=yy;xx=xxx;yy=yyy;
		findshortpath_1(brr);
      }
   if(dir==2)
     {
      turnright();
      PORTM=FORWARD;
	  _delay_ms(100);
	  PORTM=STOP;
      _delay_ms(200);
      place();
	 }
   else
     {
      PORTM=FORWARD;
	  _delay_ms(150);
	  PORTM=STOP;
      _delay_ms(200);
      place();
	 }
	backturn();
	PORTM=STOP;
    _delay_ms(100);
	PORTM=BACKWARD;
	_delay_ms(80);
	gridfollow();
	turnleft();
	gridfollow();
	turnleft();
	PORTM=FORWARD;
	_delay_ms(150);
	while((PINI & 0x35)!=0b00110001)
	{
	 PORTM=STOP;
    }
	if((PINI & 0x35)==0b00110001)
	{
	  PORTH=STOP;
	  _delay_ms(200);
	  pickup();
     }
	backturn();
	PORTM=STOP;
    _delay_ms(100);
	PORTM=BACKWARD;
	_delay_ms(80);
	gridfollow();
	turnright();
	gridfollow();
    turnleft();
	gridfollow();
	turnright();
	PORTM=FORWARD;
	_delay_ms(150);
	PORTM=STOP;
    _delay_ms(100);
	place();
	backturn();
	PORTM=STOP;
    _delay_ms(100);
	PORTM=BACKWARD;
	_delay_ms(80);
	gridfollow();
  if(arr[2][3]!=0)
	 {
	  turnright();
	  gridfollow();
	  gridfollow();
      PORTM=FORWARD;
	  _delay_ms(80);
     }
  else
     {
	  x=1;y=3;xx=3;yy=3;
	  findshortpath_1(drr);
	  turnleft();
     }
	 PORTH=HANDDOWN;
     _delay_ms(60);
   	 PORTH=STOP;
	 _delay_ms(70);
	 linefollow();
	 PORTM=STOP;
     _delay_ms(200);
	 pickup();
	 PORTM=BACKWARD;
      _delay_ms(80);
     gridfollow();
	 turnleft();
     gridfollow();
	 gridfollow();
	 gridfollow();
	 turnright();
	 PORTM=FORWARD;
	 _delay_ms(100);
	 PORTM=STOP;
     _delay_ms(200);
     place();
	 backturn();
	 PORTM=STOP;
     _delay_ms(100);
	 PORTM=BACKWARD;
	 _delay_ms(80);
	 gridfollow();
	 turnleft();
	 gridfollow();
	 gridfollow();
	 turnleft();
	 gridfollow();
	 gridfollow();
	 turnleft();
     gridfollowfinal();
	 PORTH=HANDDOWN;
	 _delay_ms(40);
	 PORTH=STOP;
	 _delay_ms(50);
     PORTH=HANDUP;
	 _delay_ms(40);
	 PORTH=STOP;
	 _delay_ms(50);
     PORTH=HANDDOWN;
	 _delay_ms(40);
	 PORTH=STOP;
	 _delay_ms(50);
     PORTH=HANDUP;
	 _delay_ms(40);
	 PORTH=STOP;
	 _delay_ms(50);
	 backturn();
	 gridfollow();
	 turnleft();
     PORTM=FORWARD;
	 _delay_ms(100);
	 PORTM=STOP;
	 buzzer();
   for(reset=0;reset<10;reset++)
	  {
	     eeprom_write_byte((uint8_t *)reset,0);
	  }
  }
 return 0;
 
}
