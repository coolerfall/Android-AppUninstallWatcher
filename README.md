Android-AppUninstallWatcher
===========================
App uninstall watcher. You can monitor uninstall of your app.

Usage
=====
* This wathcer can be added in application or service in your app, use `Watcher.run(context, url, shouldOpenBrowser)` to run the watcher.
* If `shouldOpenBrowser` is true, the wathcer will open default browser with `url`. Don't forget to add *android.permission.INTERNET* permission in manifest.
* If you want to keep your app alive, see also [Android-AppDaemon][1].

Note
====
This library dosen't work on all phones, such as xiaomi phones.

Download
========
If you are building with Gradle, simply add the following line to the dependencies section of your build.gradle file:

    compile 'com.coolerfall:android-app-uninstall-watcher:1.0.3'

License
=======

    Copyright (C) 2015 Vincent Cheung

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

         http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.

[1]: https://github.com/Coolerfall/Android-AppDaemon