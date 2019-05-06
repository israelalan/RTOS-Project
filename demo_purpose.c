#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/v4l2-common.h>
#include <linux/v4l2-controls.h>
#include <linux/videodev2.h>
#include <sys/mman.h>
#include <string.h>


int main(void){
    int fd;
    char buf[1024];
    if((fd = open("/dev/video0", O_RDWR)) < 0){
        perror("open");
        exit(1);
    }
 
    struct v4l2_capability cap;
	if(ioctl(fd, VIDIOC_QUERYCAP, &cap) < 0){
	    perror("VIDIOC_QUERYCAP");
	    exit(1);
	}
 	
	if(!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)){
	    fprintf(stderr, "The device does not handle single-planar video capture.\n");
	    exit(1);
	}

	struct v4l2_format format;
	format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	format.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
	format.fmt.pix.width = 512;
	format.fmt.pix.height = 512;
 
	if(ioctl(fd, VIDIOC_S_FMT, &format) < 0){
	    perror("VIDIOC_S_FMT");
	    exit(1);
	}

	struct v4l2_requestbuffers bufrequest;
	bufrequest.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	bufrequest.memory = V4L2_MEMORY_MMAP;
	bufrequest.count = 1;
	 
	if(ioctl(fd, VIDIOC_REQBUFS, &bufrequest) < 0){
	    perror("VIDIOC_REQBUFS");
	    exit(1);
	}

	struct v4l2_buffer bufferinfo;
	memset(&bufferinfo, 0, sizeof(bufferinfo));
	 
	bufferinfo.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	bufferinfo.memory = V4L2_MEMORY_MMAP;
	bufferinfo.index = 0;
	 
	if(ioctl(fd, VIDIOC_QUERYBUF, &bufferinfo) < 0){
	    perror("VIDIOC_QUERYBUF");
	    exit(1);
	}

	void* buffer_start = mmap(
	    NULL,
	    bufferinfo.length,
	    PROT_READ | PROT_WRITE,
	    MAP_SHARED,
	    fd,
	    bufferinfo.m.offset
	);

	if(buffer_start == MAP_FAILED){
	    perror("mmap");
	    exit(1);
	}

	memset(buffer_start, 0, bufferinfo.length);
	
	printf("%ld\n",sizeof(buffer_start) );
	printf("%d\n",bufferinfo.length );
	printf("%ld\n",sizeof(buf) );
	// Activate streaming
	int type = bufferinfo.type;
	if(ioctl(fd, VIDIOC_STREAMON, &type) < 0){
	    perror("VIDIOC_STREAMON");
	    exit(1);
	}
	 
	/* Here is where you typically start two loops:
	 * - One which runs for as long as you want to
	 *   capture frames (shoot the video).
	 * - One which iterates over your buffers everytime. */
	 
	// Put the buffer in the incoming queue.
	if(ioctl(fd, VIDIOC_QBUF, &bufferinfo) < 0){
	    perror("VIDIOC_QBUF");
	    exit(1);
	}
	 
	// The buffer's waiting in the outgoing queue.
	if(ioctl(fd, VIDIOC_DQBUF, &bufferinfo) < 0){
	    perror("VIDIOC_QBUF");
	    exit(1);
	}
	 
	/* Your loops end here. */
	 
	// Deactivate streaming
	if(ioctl(fd, VIDIOC_STREAMOFF, &type) < 0){
	    perror("VIDIOC_STREAMOFF");
	    exit(1);
	}

	
	int jpgfile;
	if((jpgfile = open("myimage.jpeg", O_WRONLY | O_CREAT, 0660)) < 0){
	    perror("open");
	    exit(1);
	}
	 
	write(jpgfile, buffer_start, bufferinfo.length);
	close(jpgfile);
	
    close(fd);
    return EXIT_SUCCESS;
}
