#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
	char outfile[] = "outfile";
	char errfile[] = "errfile";

	int out=open(outfile, O_CREAT|O_TRUNC|O_WRONLY, 0644);
	int err=open(errfile, O_CREAT|O_TRUNC|O_WRONLY, 0644);

	int oldout = dup(1);
	int olderr = dup(2);
	dup2(out, 1);
	dup2(err, 2);

	close(out);
	close(err);

	int status = system("ls");

	dup2(oldout, 1);
	dup2(olderr, 2);

	close(oldout);
	close(olderr);
}

