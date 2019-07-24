#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include <linux/fb.h>

#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/mman.h>



int main()
{
	struct fb_var_screeninfo screen_info;
	struct fb_fix_screeninfo fixed_info;

	char *buffer = NULL;
	size_t buflen;
	int fd = -1;
	int r = 1;

	long int loc = 0;

	fd = open("/dev/fb1", O_RDWR);
	
	if (fd >= 0)
	{
		if (	!ioctl(fd, FBIOGET_VSCREENINFO, &screen_info) && 
			!ioctl(fd, FBIOGET_FSCREENINFO, &fixed_info))
		{
			buflen = screen_info.yres_virtual * fixed_info.line_length;
			buffer= mmap(NULL,
					buflen,
					PROT_READ|PROT_WRITE,
					MAP_SHARED,
					fd,0);
			if (buffer != MAP_FAILED)
			{
				for (int y = 100; y < 300; y++)
					for (int x = 100; x < 300; x++)
					{
						loc = (x + screen_info.xoffset) * (screen_info.bits_per_pixel/8) +
							(y+screen_info.yoffset) * fixed_info.line_length;
						*(buffer+loc + 0) = 100;	//blue
						*(buffer+loc + 1) = 100;	//green
						*(buffer+loc + 2) = 100;	//red
						*(buffer+loc + 3) = 0;		//transparency
					}
				r = 0;
			}
			else
				perror("mmap\n");
		}
		else
			perror("ioctl\n");
	}
	else
		perror("open\n");
	
	if (buffer && buffer != MAP_FAILED)
		munmap(buffer, buflen);

	close(fd);

	return r;
}
