#include <stdio.h> 
#include <termios.h> 
#include <unistd.h> 

static struct termios term; 
static char flag = '1'; 

int Block[4][4] = {
  {0,0,0,0},
  {0,1,1,0},
  {0,1,1,0},
  {0,0,0,0}
};

int Line[4][4] = {
  {0,1,0,0},
  {0,1,0,0},
  {0,1,0,0},
  {0,1,0,0}
};

int Spiece[4][4] = {
  {0,1,0,0},
  {0,1,1,0},
  {0,0,1,0},
  {0,0,0,0}
};

int Zpiece[4][4] = {
  {0,0,1,0},
  {0,1,1,0},
  {0,1,0,0},
  {0,0,0,0}
};

int Jpiece[4][4] = {
  {0,0,1,0}, 
  {0,0,1,0},
  {0,1,1,0},
  {0,0,0,0}
};

int Lpiece[4][4] = {
  {0,1,0,0}, 
  {0,1,0,0},
  {0,1,1,0},
  {0,0,0,0}
};

int gametable[21][12] = {
  {1,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1}
}; 

void frame() {
  for(int i = 0; i <=20; i++) {
    for( int j = 0; j <=11; j++) {
      ( gametable[i][j] == 1) ? putchar('#') : putchar(' ') ; 
    }
    printf("\n"); 
  }
}

void enableRawMode() { 
  struct termios raw = term; 
  raw.c_lflag &= ~(ECHO | ICANON); 
  raw.c_cc[VMIN] = 0; 
  raw.c_cc[VTIME] = 0; 

  tcsetattr(0, TCSAFLUSH, &raw); 
} 

  void atexit() { 
    tcsetattr(0, TCSAFLUSH, &term); 
  }
char get_key() { 
  char c = 0; 
  read(STDIN_FILENO, &c, 1); 
  tcflush(STDIN_FILENO, TCIFLUSH); 
  return c; 
}

void update(char key) { 
    switch(key) { 
      case 'a': printf("This is the left button\r\n"); break;
      case 'd': printf("This is the right button\r\n"); break;
      case 's': printf("This is the down button\r\n"); break; 
      case 'q': printf("This is the quit button\r\n"); flag = '0'; break; 
    }
}

int main() { 

  tcgetattr(STDIN_FILENO, &term); 
  atexit(); 
  setvbuf(stdout, NULL, _IONBF, 0); 
  enableRawMode(); 

  printf("\x1b[2J"); 
  while(flag == '1') { 

  printf("\x1b[2J"); 
  printf("\033[H"); 
//  printf("This is the loop\r\n");

    char key = get_key(); 
    update(key);  
    frame(); 

   // printf("this is the key pressed: %c\r\n", key);
    
   
   usleep(100000); 
  }


  tcsetattr(0, TCSAFLUSH, &term); 
  return 0; 
}
