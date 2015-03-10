/*
 * File        : watcher.c
 * Author      : Vincent Cheung
 * Date        : Jan. 21, 2015
 * Description : This is used to watch if the app is uninstall.
 *
 * Copyright (C) Vincent Chueng<coolingfall@gmail.com>
 *
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/inotify.h>
#include <sys/wait.h>

#include "chttp.h"
#include "common.h"

#define PORT            80
#define BUFFER_SIZE     4 * 1024
#define LOG_TAG         "Watcher"

/* child process signal function */
static void sig_child()
{
    int status;
    while(waitpid(-1, &status, WNOHANG) > 0);

    return;
}

int main(int argc, char *argv[])
{
	int i;
	int should_open_browser = 0;
    char *package_name = NULL;
    char *url = NULL;
    char *url_file_path = NULL;

	for (i = 0; i < argc; i ++)
	{
		if (!strcmp("-p", argv[i]))
		{
			package_name = argv[i + 1];
			LOGD(LOG_TAG, "package name: %s", package_name);
		}

		if (!strcmp("-u", argv[i]))
		{
			url = argv[i + 1];
			LOGD(LOG_TAG, "url: %s", url);
		}

		if (!strcmp("-f", argv[i]))
		{
			url_file_path = argv[i + 1];
			LOGD(LOG_TAG, "url file path: %s", url_file_path);
		}

		if (!strcmp("-b", argv[i]))
		{
			should_open_browser = atoi(argv[i + 1]);
            LOGD(LOG_TAG, "should open brwoser: %d", should_open_browser);
		}
	}

	/* get the directory for watcher */
	char *app_dir = str_stitching("/data/data/", package_name);
	char *lib_dir = str_stitching(app_dir, "/lib");
	char *watch_file_path = str_stitching(app_dir, "/uninstall.watch");

	/* the file path should not be null */
	if (watch_file_path == NULL)
	{
		LOGE(LOG_TAG, "watch file path is NULL");
		exit(EXIT_FAILURE);
	}

	/* avoid zombie process */
    signal(SIGCHLD, sig_child);

	/* find pid by name and kill them */
	int pid_list[100];
    int total_num = find_pid_by_name(argv[0], pid_list);
    for (i = 0; i < total_num; i ++)
    {
    	int watcher_pid = pid_list[i];
    	if (watcher_pid > 1 && watcher_pid != getpid())
    	{
    		kill(watcher_pid, SIGKILL);
    	}
    }

	/* get child process */
	pid_t pid = fork();
	if (pid < 0)
	{
		LOGE(LOG_TAG, "fork failed");
	}
	else if (pid == 0)
	{
		/* inotify init */
		int fd = inotify_init();
		if (fd < 0)
		{
			LOGE(LOG_TAG, "inotify_init init failed");
			exit(EXIT_FAILURE);
		}

		int w_fd = open(watch_file_path, O_RDWR | O_CREAT | O_TRUNC,
				S_IRWXU | S_IRWXG | S_IRWXO);
		if (w_fd < 0)
		{
			LOGE(LOG_TAG, "open watch file error");
			exit(EXIT_FAILURE);
		}

		close(w_fd);

		/* add watch in inotify */
		int watch_fd = inotify_add_watch(fd, watch_file_path, IN_DELETE);
		if (watch_fd < 0)
		{
			LOGE(LOG_TAG, "inotify_add_watch failed");
			exit(EXIT_FAILURE);
		}

		void *p_buf = malloc(sizeof(struct inotify_event));
		if (p_buf == NULL)
		{
			LOGD(LOG_TAG, "malloc inotify event failed");
			exit(EXIT_FAILURE);
		}

		LOGD(LOG_TAG, "watcher process fork ok, start to watch");

		while (1)
		{
			/* read will block process */
			size_t read_bytes = read(fd, p_buf, sizeof(struct inotify_event));

			/* delay 200ms */
			usleep(200*1000);

			/* to check if the app has uninstalled, indeed */
			FILE *lib_dir_file = fopen(lib_dir, "r");
			FILE *app_dir_file = fopen(app_dir, "r");
			if (lib_dir_file == NULL || app_dir_file == NULL)
			{
				break;
			}
			else
			{
				/* close app dir file */
				fclose(lib_dir_file);
				fclose(app_dir_file);

				/* add notify watch again */
				int w_fd = open(watch_file_path, O_WRONLY | O_CREAT | O_TRUNC,
						S_IRWXU | S_IRWXG | S_IRWXO);
				close(w_fd);

				int watch_fd = inotify_add_watch(fd, watch_file_path, IN_DELETE);
				if (watch_fd < 0)
				{
					LOGE(LOG_TAG, "inotify_add_watch failed");
					free(p_buf);
					exit(EXIT_FAILURE);
				}
			}
		}

		free(p_buf);
		inotify_rm_watch(fd, IN_DELETE);
		LOGD(LOG_TAG, "the app has been uninstalled, call url");

		/* if the url was saved in file, read out */
		if (url_file_path != NULL)
		{
			int url_fd = open(url_file_path, O_RDONLY);
            if (url_fd < 0)
            {
            	LOGE(LOG_TAG, "url file open error");
            	exit(EXIT_FAILURE);
            }

            char buf[300] = {0};
            if (read(url_fd, buf, 300) > 0)
			{
            	url = buf;
            	LOGD(LOG_TAG, "url from file: %s", url);
			}

            close(url_fd);
		}

		/* call url */
		chttp_get(url);

		/* open browser if needed */
		if (should_open_browser)
		{
			open_browser(url);
		}

		exit(EXIT_SUCCESS);
	}
	else
	{
		/* parent process */
	}
}