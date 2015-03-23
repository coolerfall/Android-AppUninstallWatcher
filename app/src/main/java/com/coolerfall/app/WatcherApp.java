package com.coolerfall.app;

import android.app.Application;

import com.coolerfall.watcher.Watcher;

/**
 * Application of this project.
 *
 * @author Vincent Cheung
 * @since  Jan. 23, 2015
 */
public class WatcherApp extends Application {
	private static final String URL = "http://www.baidu.com";

	@Override
	public void onCreate() {
		super.onCreate();

		Watcher.run(this, URL, true);
	}
}
