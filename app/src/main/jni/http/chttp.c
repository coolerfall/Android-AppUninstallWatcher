/*
 * File        : chttp.c
 * Author      : Vincent Cheung
 * Date        : Jan. 21, 2015
 * Description : This is file contains some http method.
 *
 * Copyright (C) Vincent Chueng<coolingfall@gmail.com>
 *
 */

#include <curl/curl.h>

#include "chttp.h"
#include "common.h"

#define LOG_TAG     "CHttp"

/** write callback of curl, we can get response body here */
static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
	LOGD(LOG_TAG, "response contents: %s", (char *) contents);

	return 0;
}

/**
 * C http request with get method.
 */
void chttp_get(char *url)
{
	CURL *curl;
	CURLcode res;

	curl = curl_easy_init();
	if(curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, url);
		/* follow redirection */
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

		/* perform the request, res will get the return code */
		res = curl_easy_perform(curl);
		/* Check for errors */
		if(res != CURLE_OK)
		{
			LOGE(LOG_TAG, "curl perform failed: %s", curl_easy_strerror(res));
		}

		/* always cleanup */
		curl_easy_cleanup(curl);

		return;
	}

	LOGD(LOG_TAG, "curl init error");
}