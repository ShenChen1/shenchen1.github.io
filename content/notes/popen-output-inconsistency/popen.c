#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	FILE *pipein_fp;
	char readbuf[80];

	/* Create one way pipe line with call to popen() */
	if (( pipein_fp = popen("ls", "r")) == NULL)
	{
		perror("popen");
		exit(1);
	}

	/* Processing loop */
	while(fgets(readbuf, 80, pipein_fp))
		fputs(readbuf, stdout);

	/* Close the pipes */
	pclose(pipein_fp);

	return(0);
}
