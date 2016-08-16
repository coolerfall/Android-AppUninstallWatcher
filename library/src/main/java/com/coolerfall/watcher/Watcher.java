package com.coolerfall.watcher;

import android.content.Context;
import android.text.TextUtils;
import android.util.Log;

import java.io.File;
import java.io.IOException;

/**
 * App uninstall watcher.
 *
 * @author Vincent Cheung (coolingfall@gmail.com)
 */
public class Watcher {
	private static final String TAG = Watcher.class.getSimpleName();

	private static final String WATCHER_BIN_NAME = "watcher";

	private static void start(Context context, String url, File urlFile,
		boolean shouldOpenBrowser) {
		String cmd = context.getDir(Command.BIN_DIR_NAME, Context.MODE_PRIVATE).getAbsolutePath()
			+ File.separator
			+ WATCHER_BIN_NAME;

		StringBuilder cmdBuilder = new StringBuilder();
		cmdBuilder.append(cmd);
		cmdBuilder.append(" -p ");
		cmdBuilder.append(context.getPackageName());
		cmdBuilder.append(" -b ");
		cmdBuilder.append(shouldOpenBrowser ? 1 : 0);

		if (!TextUtils.isEmpty(url)) {
			cmdBuilder.append(" -u ");
			cmdBuilder.append(url);
		}

		if (urlFile != null) {
			cmdBuilder.append(" -f ");
			cmdBuilder.append(urlFile.getAbsolutePath());
		}

		try {
			Runtime.getRuntime().exec(cmdBuilder.toString()).waitFor();
		} catch (IOException | InterruptedException e) {
			Log.e(TAG, "start daemon error: " + e.getMessage());
		}
	}

	/**
	 * Run uninstallation watcher.
	 *
	 * @param context context
	 * @param url the url to gather uninstallation information
	 * @param shouldOpenBrowser should the wathcer open browser or not
	 */
	private static void run(final Context context, final String url, final File urlFile,
		final boolean shouldOpenBrowser) {
		new Thread(new Runnable() {
			@Override public void run() {
				Command.install(context, WATCHER_BIN_NAME);
				start(context, url, urlFile, shouldOpenBrowser);
			}
		}).start();
	}

	/**
	 * Run uninstall watcher with specified url.
	 *
	 * @param context context
	 * @param url the url to gather uninstallation information
	 * @param shouldOpenBrowser should open url or not
	 */
	public static void run(Context context, String url, boolean shouldOpenBrowser) {
		run(context, url, null, shouldOpenBrowser);
	}

	/**
	 * Run uninstall watcher with specified url file if you saved url in files.
	 *
	 * @param context context
	 * @param urlFile url file
	 * @param shouldOpenBrowser should open url or not
	 */
	public static void run(Context context, File urlFile, boolean shouldOpenBrowser) {
		run(context, null, urlFile, shouldOpenBrowser);
	}

	/**
	 * Run uninstallation watcher.
	 *
	 * @param context context
	 * @param url the url to gather uninstallation information
	 */
	public static void run(Context context, String url) {
		run(context, url, null, false);
	}
}
