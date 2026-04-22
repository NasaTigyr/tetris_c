#include <stdio.h> 
#include <pthread.h> 
#include <stdlib.h> 
#include <termios.h> 
#include <unistd.h> 
#include <time.h> 

static struct termios term; 
static int seed = 0; 

#define H 22
#define W 12

int gametable[H][W]; 
int tekushta[4][4]; 

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
  
void draw(int *score) {
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
  printf("Score: %d", *score);
}

void iztriichast() {
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      if(tekushtachast.chast[i][j] == 1) {
        gametable[tekushtachast.x + i][tekushtachast.y +j] = 0;
      }
    }
  }
}

void delete_line(int *score) {
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

void drawpiece() {
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      if(tekushtachast.chast[i][j] == 1) {
        gametable[tekushtachast.x + i][tekushtachast.y +j] = 1;
      }
    }
  }
}


int check_collision(int nx, int ny) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (tekushtachast.chast[i][j] == 1) {
                int bx = nx + i;
                int by = ny + j;
                /* proverka da ne se izleze ot masiva */
                if (bx < 0 || bx >= H || by < 0 || by >= W)
                    return 1;
                if (gametable[bx][by] != 0) /* proverka da ne bi tazi chast da e neshto drugo?(stena, pod...) */
                    return 1;
            }
        }
    }
    return 0;
}

void move_left() { 
  iztriichast();
  if(!check_collision(tekushtachast.x, tekushtachast.y -1))
    tekushtachast.y--;
  drawpiece();
}

void move_right() { 
  iztriichast();
  if(!check_collision(tekushtachast.x, tekushtachast.y + 1))
    tekushtachast.y++;
  drawpiece();
}

int falldown() {
    iztriichast();                          /* remove from board */

    if (check_collision(tekushtachast.x + 1, tekushtachast.y)) {
        /* Collision below → lock piece in current position */
        drawpiece();                       /* da vyrne chasta tam kydeto beshe predi proverkata*/
        return 1;                           /* promeni flag za da pusne novo parche. */
    }

    /* da se printira na novoto mqsto*/
    tekushtachast.x++;
    drawpiece();
    return 0;
}

void printtekushta( int neshto[4][4]) {  // za debugvane na pravilni parcheta.
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      printf("%d", neshto[i][j]);
    }
    printf("\n");
  }
}

 void rotateclck() {
    iztriichast();  // remove from board first
    
    int tmp[4][4] = {0};
    
    // Transpose
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            tmp[j][i] = tekushtachast.chast[i][j];
    
    // Reverse each row
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 2; j++) {
            int t = tmp[i][j];
            tmp[i][j] = tmp[i][3-j];
            tmp[i][3-j] = t;
        }
    
    // Check collision before committing
    int old[4][4];
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            old[i][j] = tekushtachast.chast[i][j];
    
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            tekushtachast.chast[i][j] = tmp[i][j];
    
    // If collision, undo
    if (check_collision(tekushtachast.x, tekushtachast.y)) {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                tekushtachast.chast[i][j] = old[i][j];
    }
    
    drawpiece();
}

void rotatecntrclck() {
    iztriichast();
    
    int tmp[4][4] = {0};
    
    // Transpose
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            tmp[j][i] = tekushtachast.chast[i][j];
    
    // Reverse each COLUMN instead of row (counter-clockwise)
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
    
    if (check_collision(tekushtachast.x, tekushtachast.y)) {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                tekushtachast.chast[i][j] = old[i][j];
    }
    
    drawpiece();
} 

int spawnpiece() { 
        tekushtachast.x = 0;
        tekushtachast.y = W/2 - 2;
//    srand(time(NULL));
//      srand(seed);
    int num = rand()%7;
    //printf("This is the rand num: %d", num);
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
    if(check_collision(tekushtachast.x, tekushtachast.y)) {
      return -1;
    }
      drawpiece();
      return 0;
}

void handle_input(char key) {

    switch(key) { 
      case 'a': move_left();break;
      case 'e': move_right();break;
      case 'o': falldown();break;
      case 'w': rotatecntrclck();break;
      case 'v': rotateclck(); break; 
    }
}

void gravity_tick(char *flag, int *score ) {
  int locked = falldown();
  if (locked) {
  delete_line(score);
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
    printf("insert a seed here: ");
    scanf("%d", &seed);
    srand(seed);

    tcgetattr(STDIN_FILENO, &term);
    setvbuf(stdout, NULL, _IONBF, 0);
    enableRawMode();
    printf("\x1b[2J");

    char fallingpiece = 0;
    int score = 0;
    gametable_init();

    // start input thread
    pthread_t tid;
    pthread_create(&tid, NULL, input_thread, NULL);
    pthread_detach(tid);

    const int TICK_MS = 200;
    const int POLL_MS = 10;
    int elapsed = 0;

    while (1) {
        // grab and clear the latest key atomically
        pthread_mutex_lock(&lock);
        char key = latest_key;
        latest_key = 0;
        pthread_mutex_unlock(&lock);

        if (key != 0) {
            //otmestvane(key, &fallingpiece);
            handle_input(key);
            printf("\x1b[2J");
            printf("\033[H");
            draw(&score);
        }

        usleep(POLL_MS * 1000); // 1000 = 1ms, Tova e mongo byrzo, mamka mu... 
        elapsed += POLL_MS;

        if (elapsed >= TICK_MS) {
            elapsed = 0;
            if (fallingpiece == 0) {
                //otmestvane(0, &fallingpiece);
                if(spawnpiece() == -1) {
                  printf("\x1b[2J");
                  printf("\033[H");
                  draw(&score);
                  printf("\nGAME OVER\n");
//                  printf("The score is: %d", score);
                  tcsetattr(0, TCSAFLUSH, &term);
                  exit(0);
               }
                fallingpiece = 1; 
            } else {
                gravity_tick(&fallingpiece, &score);
            }

            printf("\x1b[2J");
            printf("\033[H");
            draw(&score);
        }
    }

    tcsetattr(0, TCSAFLUSH, &term);
    return 0;
}
