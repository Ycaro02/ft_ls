#include <sys/ioctl.h>
#include <stdio.h>

int main(int argc, char**argv)
{
 	struct winsize w;
    	while (1)
	{
		ioctl(1, TIOCGWINSZ, &w); 
		int width = w.ws_col; 
		printf("width terminal = %d\n", width);
	}
	return (0);
}
