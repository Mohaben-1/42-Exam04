#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

static	pid_t	pid;

void	alarm_handler(int sig)
{
	(void)sig;
}

int	sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
	struct sigaction	sa;
	pid_t				pid;
	int					status;

	pid = fork();
	if (pid == -1)
		return (-1);
	sa.sa_handler = alarm_handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGALRM, &sa, NULL);
	if (pid == 0)
	{
		f();
		exit(0);
	}
	alarm(timeout);
	if (waitpid(pid, &status, 0) == -1)
	{
		if (errno == EINTR)
		{
			kill(pid, SIGKILL);
			waitpid(pid, NULL, 0);
			if (verbose)
				printf("Bad function: timed out after %d seconds\n", timeout);
			return (0);
		}
		return (-1);
	}
	if(WIFEXITED(status))
	{
		if (WEXITSTATUS(status) == 0)
		{
			if (verbose)
				printf("Nice function!\n");
			return (1);
		}
		else
		{
			if (verbose)
				printf("Bad function: exited with code %d\n", WEXITSTATUS(status));
			return (0);
		}
	}
	if (WIFSIGNALED(status))
	{
		int sig = WTERMSIG(status);
		if (verbose)
			printf("Bad function: %s\n", strsignal(sig));
		return (0);
	}
	return (-1);
}

/* Tests:

void nice_function(void)
{
	// This function does nothing and exits normally (exit code 0)
	return;
}

void bad_function_exit_code(void)
{
	// This function exits with code 1 (failure)
	exit(1);
}

void bad_function_segfault(void)
{
	// This function causes a segmentation fault
	int *ptr = NULL;
	*ptr = 42;  // This will cause a segfault
}

void bad_function_timeout(void)
{
	// This function runs indefinitely
	while (1) {}
}

void bad_function_sleep(void)
{
	// This function sleeps for more than the timeout and gets killed by alarm handler
	sleep(5);  // Sleep for 5 seconds (assuming timeout is less than 5 seconds)
}

int main()
{
	int	result;

	printf("Test 1: Normal function (Nice)\n");
	result = sandbox(nice_function, 5, true);
	printf("Result: %d\n", result);  // Expected output: 1 ("Nice function!")

	printf("Test 2: Bad function (Exit code 1)\n");
	result = sandbox(bad_function_exit_code, 5, true);
	printf("Result: %d\n", result);  // Expected output: 0 ("Bad function: exited with code 1")

	printf("Test 3: Bad function (Segfault)\n");
	result = sandbox(bad_function_segfault, 5, true);
	printf("Result: %d\n", result);  // Expected output: 0 ("Bad function: Segmentation fault")

	printf("Test 4: Bad function (Timeout)\n");
	result = sandbox(bad_function_timeout, 2, true);  // Timeout after 2 seconds
	printf("Result: %d\n", result);  // Expected output: 0 ("Bad function: timed out after 2 seconds")
 
	printf("Test 5: Bad function (Killed by SIGKILL)\n");
	result = sandbox(bad_function_sleep, 2, true);  // Timeout after 2 seconds
	printf("Result: %d\n", result);  // Expected output: 0 ("Bad function: timed out after 2 seconds")

	return 0;
}

*/