
#include <sys/types.h>         
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include "SDL.h" 
#include "SDL_image.h" 
#include "SDL_timer.h"
#define BUFSIZE 614400

int main(int argc, char*argv[])
{
  int server_desc, client_desc, val;
  size_t  size = 50688;
  struct sockaddr_in server,client;
  //unsigned char message[BUFSIZE];
  //void *message;
  int opt = 1;
  int addrlen = sizeof(client);
  char buf[size];
        ssize_t r;
  void * buff;
  server_desc = socket(AF_INET,SOCK_STREAM,0);
  printf("Server desc = %d\n",server_desc);

          // Initialise everything.
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_JPG);

    // Get the screen's surface.
    SDL_Surface* screen = SDL_SetVideoMode(
        200,
        200,
        32, SDL_HWSURFACE
    );

    SDL_RWops* buffer_stream;
    SDL_Surface* frame;
    SDL_Rect position = {.x = 0, .y = 0};


  if (setsockopt(server_desc, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
  {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }

  server.sin_family=AF_INET;
  server.sin_addr.s_addr=INADDR_ANY;
  server.sin_port=htons(8000);
  memset(server.sin_zero, '\0', sizeof server.sin_zero);

  int bind_check = bind(server_desc, (struct sockaddr *)&server, sizeof(server));
  printf("Bind check = %d\n",bind_check );
  listen(server_desc, 5);
  client_desc = accept(server_desc, (struct sockaddr *)&client, (socklen_t*)&addrlen);
  int i=0;
  while(1)
  {
    
      if ((r = read(client_desc, buf, size)) <= 0) {
            if (r == 0) /* EOF */
                break;
            fprintf(stderr, __FILE__": read() failed: %s\n", strerror(errno));
        }
        //buf = (void*)message;
    buff = (void*)buf;
    // Create a stream based on our buffer.
    buffer_stream = SDL_RWFromMem(buff, size);

    // Create a surface using the data coming out of the above stream.
    frame = IMG_Load_RW(buffer_stream, 0);
     //printf("%d\n",i);
     //i++;
     //printf("%d\n",frame->pitch);
    // Blit the surface and flip the screen.
    SDL_BlitSurface(frame, NULL, screen, &position);
    SDL_Flip(screen);

    // Free everything, and unload SDL & Co.
    SDL_FreeSurface(frame);
    SDL_RWclose(buffer_stream);
  }
}
