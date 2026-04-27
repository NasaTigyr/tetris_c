#include <stdio.h> 
#include <pthread.h> 
#include <stdlib.h> 
#include <termios.h> 
#include <unistd.h> 
#include <time.h> 

static struct termios term; 

#define H 22
#define W 12

struct Tekushtachast { 

int chast[4][4]; 
int x; 
int y; 

};

struct Tekushtachast tekushtachast;
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

void gametable_init(int gametable[H][W]) { 
  for(int i = 0; i < H; i++) {
    for(int j = 0; j < W; j++) {
      if(i == 0) { 
        gametable[i][j] = 4;
      }else if(i == H-1) { 
        gametable[i][j] = 2;
      } else if(j == 0 || j == W-1) { 
        gametable[i][j] = 3;
      } else { 
        gametable[i][j] = 0;
      }
    }
  }
}
  
  void draw(int *score, int *tick, int gametable[H][W]) {
  for(int i = 0; i < H; i++) {
    for( int j = 0; j < W; j++) {
      switch(gametable[i][j]) {
        case 0: printf(" ");break;
        case 1: printf("#");break;
        case 2: printf("▔");break;
        case 4: printf("▁");break;
        case 3: printf("!");break;
      }
    }
    printf("\n"); 
  }
  printf("Score: %d\n", *score);
  printf("speed: %d", *tick);
}

void iztriichast( int gametable[H][W] ) {
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      if(tekushtachast.chast[i][j] == 1) {
        gametable[tekushtachast.x + i][tekushtachast.y +j] = 0;
      }
    }
  }
}

void delete_line(int *score,int gametable[H][W]) {
  for(int i= 1; i < H -1; i++){
    int full = 1;
    for(int j= 1; j < W -1; j++){
      if(gametable[i][j] == 0) {
        full = 0;
        break;
      }
  
    }
    if(full == 1) {
      *score += 10;
      for(int k = i; k > 1; k--) {
        for( int j = 1; j < W-1; j++) {
          gametable[k][j] = gametable[k-1][j];
        }
      }

      for( int j = 1; j < W-1; j++) {
        gametable[1][j] = 0;
      }
      i--;  
    }
  }
}

void drawpiece(int gametable[H][W]) {
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      if(tekushtachast.chast[i][j] == 1) {
        gametable[tekushtachast.x + i][tekushtachast.y +j] = 1;
      }
    }
  }
}


int check_collision(int nx, int ny,int gametable[H][W]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (tekushtachast.chast[i][j] == 1) {
                int bx = nx + i;
                int by = ny + j;
                if (bx < 0 || bx >= H || by < 0 || by >= W)
                    return 1;
                if (gametable[bx][by] != 0) 
                    return 1;
            }
        }
    }
    return 0;
}

void move_left(int gametable[H][W]) { 
  iztriichast(gametable);
  if(!check_collision(tekushtachast.x, tekushtachast.y -1, gametable))
    tekushtachast.y--;
  drawpiece(gametable);
}

void move_right(int gametable[H][W]) { 
  iztriichast(gametable);
  if(!check_collision(tekushtachast.x, tekushtachast.y + 1, gametable))
    tekushtachast.y++;
  drawpiece(gametable);
}

int falldown(int gametable[H][W]) {
    iztriichast(gametable);                          

    if (check_collision(tekushtachast.x + 1, tekushtachast.y, gametable)) {
        drawpiece(gametable);                       
        return 1;                           
    }

    tekushtachast.x++;
    drawpiece(gametable);
    return 0;
}

void printtekushta( int neshto[4][4]) {  
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      printf("%d", neshto[i][j]);
    }
    printf("\n");
  }
}

 void rotateclck(int gametable[H][W]) {
    iztriichast(gametable);  
    
    int tmp[4][4] = {0};
    
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            tmp[j][i] = tekushtachast.chast[i][j];
    
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 2; j++) {
            int t = tmp[i][j];
            tmp[i][j] = tmp[i][3-j];
            tmp[i][3-j] = t;
        }
    
    int old[4][4];
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            old[i][j] = tekushtachast.chast[i][j];
    
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            tekushtachast.chast[i][j] = tmp[i][j];
    
    if (check_collision(tekushtachast.x, tekushtachast.y, gametable)) {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                tekushtachast.chast[i][j] = old[i][j];
    }
    
    drawpiece(gametable);
}

void rotatecntrclck(int gametable[H][W]) {
    iztriichast(gametable);
    
    int tmp[4][4] = {0};
    
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            tmp[j][i] = tekushtachast.chast[i][j];
    
    for (int j = 0; j < 4; j++)
        for (int i = 0; i < 2; i++) {
            int t = tmp[i][j];
            tmp[i][j] = tmp[3-i][j];
            tmp[3-i][j] = t;
        }
    
    int old[4][4];
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            old[i][j] = tekushtachast.chast[i][j];
    
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            tekushtachast.chast[i][j] = tmp[i][j];
    
    if (check_collision(tekushtachast.x, tekushtachast.y, gametable)) {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                tekushtachast.chast[i][j] = old[i][j];
    }
    
    drawpiece(gametable);
} 

int spawnpiece(int gametable[H][W]) { 
        tekushtachast.x = 0;
        tekushtachast.y = W/2 - 2;
    int num = rand()%7;
    switch(num) {
      case 0: 
        for(int i = 0; i < 4; i++) { 
          for(int j = 0; j < 4; j++) { 
           tekushtachast.chast[i][j] = B[i][j]; 
          }
        }
      break;
      case 1: 
        for(int i = 0; i < 4; i++) { 
          for(int j = 0; j < 4; j++) { 
           tekushtachast.chast[i][j] = Line[i][j]; 
          }
        }
      break;
      case 2: 
        for(int i = 0; i < 4; i++) { 
          for(int j = 0; j < 4; j++) { 
           tekushtachast.chast[i][j] = T[i][j]; 
          }
        }
      break;
      case 3: 
        for(int i = 0; i < 4; i++) { 
          for(int j = 0; j < 4; j++) { 
           tekushtachast.chast[i][j] = S[i][j]; 
          }
        }
      break;
      case 4: 
        for(int i = 0; i < 4; i++) { 
          for(int j = 0; j < 4; j++) { 
           tekushtachast.chast[i][j] = Z[i][j]; 
          }
        }
      break;
      case 5: 
        for(int i = 0; i < 4; i++) { 
          for(int j = 0; j < 4; j++) { 
           tekushtachast.chast[i][j] = L[i][j]; 
          }
        }
      break;
      case 6: 
        for(int i = 0; i < 4; i++) { 
          for(int j = 0; j < 4; j++) { 
           tekushtachast.chast[i][j] = J[i][j]; 
          }
        }
      break;
    }
    if(check_collision(tekushtachast.x, tekushtachast.y, gametable)) {
      return -1;
    }
      drawpiece(gametable);
      return 0;
}

void handle_input(char key, int gametable[H][W]) {

    switch(key) { 
      case 'a': move_left(gametable);break;
      case 'e': move_right(gametable);break;
      case 'o': falldown(gametable);break;
      case 'w': rotatecntrclck(gametable);break;
      case 'v': rotateclck(gametable); break; 
    }
}

void gravity_tick(char *flag, int *score, int *tick,int gametable[H][W]) {
  int locked = falldown(gametable);
  if (locked) {
  delete_line(score, gametable);
  if(*score > 0  && *score%100 == 0) *tick-=10;
  *flag = 0;
  }
}

char get_key() { 
  char c = 0; 
  read(STDIN_FILENO, &c, 1); 
  tcflush(STDIN_FILENO, TCIFLUSH); 
  return c; 
}

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
char latest_key = 0;

void *input_thread(void *arg) {
    while (1) {
        char key = get_key();
        if (key != 0) {
            pthread_mutex_lock(&lock);
            latest_key = key;
            pthread_mutex_unlock(&lock);
        }
        usleep(1000); // 1ms — very responsive
    }
    return NULL;
}

int main() {
  int seed =0;
    printf("insert a seed here: ");
    scanf("%d", &seed);
    srand(seed);

    tcgetattr(STDIN_FILENO, &term);
    setvbuf(stdout, NULL, _IONBF, 0);
    enableRawMode();
    printf("\x1b[2J");

    int gametable[H][W]; 
    char fallingpiece = 0;
    int score = 0;
    gametable_init(gametable);

    pthread_t tid;
    pthread_create(&tid, NULL, input_thread, NULL);
    pthread_detach(tid);

    int TICK_MS = 200;
    const int POLL_MS = 10;
    int elapsed = 0;

    while (1) {
        pthread_mutex_lock(&lock);
        char key = latest_key;
        latest_key = 0;
        pthread_mutex_unlock(&lock);

        if (key != 0) {
            handle_input(key, gametable);
            printf("\x1b[2J");
            printf("\033[H");
            draw(&score, &TICK_MS, gametable);
        }

        usleep(POLL_MS * 1000); // 1000 = 1ms, Tova e mongo byrzo, mamka mu... 
        elapsed += POLL_MS;

        if (elapsed >= TICK_MS) {
            elapsed = 0;
            if (fallingpiece == 0) {
                if(spawnpiece(gametable) == -1) {
                  printf("\x1b[2J");
                  printf("\033[H");
                  draw(&score, &TICK_MS,gametable);
                  printf("\nGAME OVER\n");
                  tcsetattr(0, TCSAFLUSH, &term);
                  exit(0);
               }
                fallingpiece = 1; 
            } else {
                gravity_tick(&fallingpiece, &score, &TICK_MS, gametable);
            }

            printf("\x1b[2J");
            printf("\033[H");
            draw(&score, &TICK_MS,gametable);
        }
        
      
    }

    tcsetattr(0, TCSAFLUSH, &term);
    return 0;
}
