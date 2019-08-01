#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include <linux/fb.h>	//framebuffer

#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

//#include <tslib.h>	//touchscreen library


int main()
{
	struct fb_var_screeninfo screen_info;
	struct fb_fix_screeninfo fixed_info;

	char *buffer = NULL;
	size_t buflen;
	int fd = -1;

	long int loc = 0;

	fd = open("/dev/fb1", O_RDWR);
	
	if (fd < 0)	return 1;
	if (ioctl(fd, FBIOGET_VSCREENINFO, &screen_info) || ioctl(fd, FBIOGET_FSCREENINFO, &fixed_info))
		return 1;
	buflen = screen_info.yres_virtual * fixed_info.line_length;
	buffer= mmap(NULL, 
			buflen,
			PROT_READ|PROT_WRITE,
			MAP_SHARED, 
			fd, 
			0);
	if (buffer == MAP_FAILED)	return 1;
	
	//write to fb
	for (int y = 0; y < 240; y++)
		for (int x = y + 100; x < 319; x++)
		{
			loc = (x + screen_info.xoffset) * (screen_info.bits_per_pixel/8) +
				(y+screen_info.yoffset) * fixed_info.line_length;
			*(buffer+loc + 0) = 255;	//blue
			*(buffer+loc + 1) = 255;	//green
			*(buffer+loc + 2) = 255;	//red
			*(buffer+loc + 3) = 0;		//transparency
		}
	
	//finish writing to fb
	munmap(buffer, buflen);
	close(fd);

	return 0;
}
