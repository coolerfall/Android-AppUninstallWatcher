package com.cooler.watcher;

import android.content.Context;
import android.util.Log;

import java.io.File;
import java.io.IOException;

/**
 * App uninstall watcher.
 *
 * @author Vincent Cheung
 * @since  Jan. 22, 2015
 */
public class Watcher {
	private static final String TAG = Watcher.class.getSimpleName();

	private static final String WATCHER_BIN_NAME = "watcher";

	private static void start(Context context, String url, boolean shouldOpenBrowser) {
		String cmd = context.getDir(Command.BIN_DIR_NAME, Context.MODE_PRIVATE)
				.getAbsolutePath() + File.separator + WATCHER_BIN_NAME;

		StringBuilder cmdBuilder = new StringBuilder();
		cmdBuilder.append(cmd);
		cmdBuilder.append(" -p ");
		cmdBuilder.append(context.getPackageName());
		cmdBuilder.append(" -u ");
		cmdBuilder.append(url);
		cmdBuilder.append(" -b ");
		cmdBuilder.append(shouldOpenBrowser ? 1 : 0);

		try {
			Runtime.getRuntime().exec(cmdBuilder.toString()).waitFor();
		} catch (IOException | InterruptedException e) {
			Log.e(TAG, "start daemon error: " + e.getMessage());
		}
	}

	/**
	 * Run uninstallation watcher.
	 *
	 * @param context           context
	 * @param url               the url to gather uninstallation information
	 * @param shouldOpenBrowser should the wathcer open browser or not
	 */
	public static void run(final Context context, final String url,
	                       final boolean shouldOpenBrowser) {
		new Thread(new Runnable() {
			@Override
			public void run() {
				Command.install(context, WATCHER_BIN_NAME);
				start(context, url, shouldOpenBrowser);
			}
		}).start();
	}

	/**
	 * Run uninstallation watcher.
	 *
	 * @param context context
	 * @param url     the url to gather uninstallation information
	 */
	public static void run(Context context, String url) {
		run(context, url, false);
	}
}
