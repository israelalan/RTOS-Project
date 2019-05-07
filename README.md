## Real Time Operating Systems Project - Implementation of a video chat system

### Problem Statement
The problem was to retrieve video data from webcam on laptop, USB camera or Picamera on Raspberry Pi along with the audio data and send it to another system through sockets and vice versa for incorporating chat capabilities. There is need for synchronization of audio and video data. On reception of audio and video data on the other system, playback of audio and video needs to be handled.

### Solution
The video data from the camera is captured using the V4L2 (video for linux) API. The audio data is captured using the PulseAudio API. These data streams are sent through sockets from one machine to another.

The video is sent frame by frame through the socket. The frame size sent through socket each time is around 50.7 kb in size. The audio data is sent in buffers of size 1024 bytes through socket. The video and audio data streams are sent through separate ports. The video data is displayed using the SDL library (Simple DirectMedia Layer). 

### Implementation
The video data and audio data is recorded using the video_client.c and audio_client.c respectively. These client programs take data from the camera device and the microphone, and send it to through sockets. 

The video_server.c and audio_server.c are the programs used for receiving the video and audio streams respectively and playing it back. When two systems are connected for the video chat, firstly both systems need to run their server programs. Then both systems need to run the client programs which will connect to the respective server programs.

Basically client programs are used as input programs for the video and audio streams. The server programs are used as the playback programs for displaying the video stream and playing the audio stream through speaker.

### Gaps in Implementation
Initially the idea was to implement this using the Raspberry Pi. Raspberry Pi 3 has a camera module (picamera) and also an audio jack. Transferring images from one Pi to another was tested and worked successfully. But issues were faced with audio. The audio jack in the Pi 3 is an output-only audio jack. And hence microphone input was not able to be sent through this. Hence the audio headsets along with USB adapters were purchased. 

Unfortunately issue were faced with the headsets. The audio quality was very poor. This is due to the microphone's poor sensitivity. Also, one of the headsets was not working. Hence implementation on Raspberry Pi was put on hold. 

### Challenges in Implementation
The video quality when sent on local host through socket to the same machine, was very clear. And when tried across different machines, the video quality was very poor or even not received at times. Even a single frame was not able to be received sometimes. This is probably because of the bandwidth issue. 

Initially the frame size was around 600 kb. No frames were received on the other side os the socket on transmission. Hence the size was reduced by decreasing the pixel height and width to 50 kb. This gave us better quality but still loss in frames. Then the machines were connected to each other through mobile phone hotspot. This reduced to the loss in frames to some extent, but yet we were getting a lot of disturbance in the video data.

When only one client is connected, the video data seemed to be performing better in one-way than the two-way connection in the chat system. This is also due to the bandwidth issue. 

### How to run
To start the video serever.

*gcc video_server.c -o vid_ser -lSDLmain -lSDL -lSDL_image `sdl-config --cflags --libs`*  

*./vid_ser*

To start the audio server:

*gcc audio_server.c -o aud_ser -lpulse -lpulse-simple*

*./aud_ser*

To start the video client:

*gcc video_client.c -o vid_cli*

*./vid_cli*

To start the audio client:

*gcc audio_client.c -o aud_cli -lpulse -lpulse-simple*

*./aud_cli*

### Acknowledgements
Thanks to Prof.Girish Kumar for providing us the support in implementing the project. 

### References:
[1] http://jwhsmith.net/2014/12/capturing-a-webcam-stream-using-v4l2/

[2] https://linuxtv.org/downloads/v4l-dvb-apis/uapi/v4l/common.html

[3] http://0pointer.de/lennart/projects/pulseaudio/doxygen/

[4] https://wiki.libsdl.org/FrontPage
