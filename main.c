#include <stdio.h> 
#include <stdlib.h> 
#include <termios.h> 
#include <unistd.h> 
#include <time.h> 

static struct termios term; 
static char flag = '1'; 

#define H 22
#define W 12

int gametable[H][W]; 
int tekushta[4][4]; 

int B[4][4] = {
  {0,0,0,0},
  {0,1,1,0},
  {0,1,1,0},
  {0,0,0,0}
};

int Line[4][4] = {
  {0,0,0,0},
  {1,1,1,1},
  {0,0,0,0},
  {0,0,0,0}
};

int S[4][4] = {
  {0,0,0,0},
  {0,1,1,0},
  {1,1,0,0},
  {0,0,0,0}
};

int Z[4][4] = {
  {0,0,0,0},
  {0,1,1,0},
  {0,0,1,1},
  {0,0,0,0}
};

int J[4][4] = {
  {0,0,0,0},
  {1,1,1,0},
  {0,0,1,0},
  {0,0,0,0}
};

int L[4][4] = {
  {0,0,0,0},
  {0,1,1,1},
  {0,1,0,0},
  {0,0,0,0}
};

int T[4][4] = {
  {0,0,0,0},
  {0,1,1,1},
  {0,0,1,0},
  {0,0,0,0}
};


void enableRawMode() { 
  struct termios raw = term; 
  raw.c_lflag &= ~(ECHO | ICANON); 
  raw.c_cc[VMIN] = 0; 
  raw.c_cc[VTIME] = 0; 

  tcsetattr(0, TCSAFLUSH, &raw); 
} 

void gametable_init() { 
  for(int i = 0; i < H; i++) {
    for(int j = 0; j < W; j++) {
      if(i == 0 || i == H-1) { 
        gametable[i][j] = 2;
      } else if(j == 0 || j == W-1) { 
        gametable[i][j] = 3;
      } else { 
        gametable[i][j] = 0;
      }
    }
  }
}
  
void draw() {
  for(int i = 0; i < H; i++) {
    for( int j = 0; j < W; j++) {
//      printf("%d", gametable[i][j]);
      switch(gametable[i][j]) {
        case 0: printf(" ");break;
        case 1: printf("#");break;
        case 2: printf("-");break;
        case 3: printf("!");break;
      }
    }
    printf("\n"); 
  }
}

void printtekushta( int neshto[4][4]) { 
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      printf("%d", neshto[i][j]);
    }
    printf("\n");
  }
}

void otmestvane(char key, char flag) {
  if(flag == 0) {
//    int num = rand()%7;
    srand(time(NULL));
    int num = rand()%7;
    printf("This is the rand num: %d", num);
    switch(num) {
      case 0: 
        for(int i = 0; i < 4; i++) { 
          for(int j = 0; j < 4; j++) { 
           tekushta[i][j] = B[i][j]; 
          }
        }
        printf("This is case0\n");
      break;
      case 1: 
        for(int i = 0; i < 4; i++) { 
          for(int j = 0; j < 4; j++) { 
           tekushta[i][j] = Line[i][j]; 
          }
        }
        printf("This is case1\n");
      break;
      case 2: 
        for(int i = 0; i < 4; i++) { 
          for(int j = 0; j < 4; j++) { 
           tekushta[i][j] = T[i][j]; 
          }
        }
        printf("This is case2\n");
      break;
      case 3: 
        for(int i = 0; i < 4; i++) { 
          for(int j = 0; j < 4; j++) { 
           tekushta[i][j] = S[i][j]; 
          }
        }
        printf("This is case3\n");
      break;
      case 4: 
        for(int i = 0; i < 4; i++) { 
          for(int j = 0; j < 4; j++) { 
           tekushta[i][j] = Z[i][j]; 
          }
        }
        printf("This is case4\n");
      break;
      case 5: 
        for(int i = 0; i < 4; i++) { 
          for(int j = 0; j < 4; j++) { 
           tekushta[i][j] = L[i][j]; 
          }
        }
        printf("This is case5\n");
      break;
      case 6: 
        for(int i = 0; i < 4; i++) { 
          for(int j = 0; j < 4; j++) { 
           tekushta[i][j] = Z[i][j]; 
          }
        }
        printf("This is case6\n");
      break;
    }
      
//     printtekushta(tekushta);
  
    int x = W/2 - 1;
      for(int i = 0; i < 4; i++) { 
        for(int j = 0; j < 4; j++) { 
//          printf("This is i: %d and this is j: %d\n", i, j);
         gametable[i+1][x + j] = tekushta[i][j]; 
        }
      }
  }
}

char get_key() { 
  char c = 0; 
  read(STDIN_FILENO, &c, 1); 
  tcflush(STDIN_FILENO, TCIFLUSH); 
  return c; 
}

int main() { 

  tcgetattr(STDIN_FILENO, &term); 
  setvbuf(stdout, NULL, _IONBF, 0); 
  enableRawMode(); 
  printf("\x1b[2J"); 

  char flag = 0;
  char run = 1;
  gametable_init(); 

  while(run == 1) { 

  printf("\x1b[2J"); 
  printf("\033[H"); 
//  printf("This is the loop\r\n");

    char key = get_key(); 
    otmestvane(key, flag);
    draw(); 

   // printf("this is the key pressed: %c\r\n", key);
    
     
   usleep(100000); 
//   run = 0;
  }


  tcsetattr(0, TCSAFLUSH, &term); 
  return 0; 
}
