# Native Libraries for Umajin Editor

[Umajin](https://www.umajin.com/platform/) has an interface that allows integrating native code into your project. This is supported fully for desktop platforms (Windows and Mac), where you can use native libraries in Umajin Editor, Umajin Lite or a published app. We also support mobile platforms (iOS and Android), but due to Apple and Google restrictions, native libraries can only be tested and used in published app builds.

- **Windows:** Dynamic Link Library (DLL) targeting 64 bit x86_64.
- **MacOS:** Dynamic Library (dylib) targeting architecture x86_64.
- **iOS:** A 64 bit framework containing an iOS dylib targeting architecture arm64.
- **Android:** An Android Library (AAR) containing ARM 32 bit and ARM 64 bit Shared Objects (.so) targeting armv7a and armv8a respectively.  

## Development tools

These all have a C-style API.

If you want to use another language, such as .Net (Windows), Objective-C or Swift (Mac and iOS), or Java or Kotlin (Android) then that code must be wrapped in a native C API. 

For Android this will require [installing the NDK and CMake](https://developer.android.com/studio/projects/install-ndk) for Android Studio.
THe library will be built as a .so and then packaged into an .aar file. The library name is defined by the targets in CMakeLists.txt
and then referenced in the build.gradle file. The name of the .aar file does not matter, only the .so files it contains.

## Writing the native library
Your native library must implement several functions and export them to the DLL, Dylib or Shared Object.

```c
// Identifier function
const char* umajinGetIdentifier(void);

// Process function
const char* umajinProcessV2(long long tag, long long timestamp, const char* payload);
// OR
const char* umajinProcessBinary((long long tag, long long timestamp, const char* payload, long long size, unsigned char* buffer);
// OR (for backwards compatibility)
const char* umajinProcess(long long tag, const char* payload);

// Poll function
const char* umajinPoll(long long tag, long long timestamp);
// OR
const char* umajinPollBinary(long long tag, long long timestamp, long long* sizeOut, unsigned char** bufferOut);
// also requires
const char* umajinDestroy(long long size, unsigned char* buffer);
```

**umajinGetIdentifier** just returns an internal name for the library. This is checked for uniqueness within the Umajin Project.

**umajinProcessV2** is the interface for editor/JS to call into the library passing string data.

**umajinProcessBinary** is the interface for editor/JS to call into the library providing both string and binary data (a size and a byte buffer of that size). THe buffer is only valid during the call, so the library must copy data as necessary in its `umajinProcessBinary` implementation.

**umajinProcess** is an older interface for editor/JS to call into the library, maintained for backwards compatibility. New libraries should use `umajinProcessV2` or `umajinProcessBinary`.

**umajinPoll** is called every frame for the library to return string data back to the project JS, if it has any; ie asynchronous events or responses.

**umajinPollBinary** is called every frame for the library to return string and binary data back to the project JS. The library can allocate a buffer via bufferOut and set its size via sizeOut in the callers stack. **NOTE** the library must provide `umajinDestroy` so that the caller can hand the allocated buffer back to the library for safe release/freeing when the caller is done.

**umajinDestroy** is called when Umajin has finished with a buffer allocated by the library and returned by `umajinPollBinary`. The library should free whatever resources were allocated at the address passed to `umajinDestroy`.

### Per platform considerations Umajin Editor 4.0.4 through 4.1.

- All integer types passed to the `umajin...` functions by long long should be 64 bit integers for the compiler and language (C/C++/ObjC/Swift).
- You need to provide for all your own dependencies by dynamic linking.
- Minimum versions of the target OS must not exceed [those provided by the Umajin Editor](https://www.umajin.com/download/download-umajin/).
- For Android, API levels are: minimum 22, target 30.

**Upgrade Note**: In editor 4.0.x the C function names did not have the “umajin” prefix. This was added to avoid clashes with other common functions.

## Installing the native library
Once you have created your DLL/Dylib/framework/AAR, place them in the Umajin Project folder as follows:

- Windows: `/manifest/win/bin/`
- Mac: `/manifest/osx/bin/`
- iOS: `/manifest/ios/frameworks/`
- Android: `/manifest/android/aar/`

You may need to create these folders if they do not already exist.

Note that while you may be able to use other folders, these are the standard folders that will work correctly in all situations, including when the app is “published” as a standalone application. In such standalone apps the files will be moved into appropriate places within the package. 

The **expected filename** is:

- `myLibrary.dll` on Windows
- `libMyLibrary.dylib` on MacOS.  Note the expected `lib` prefix for MacOS.
- `myLibrary.framework/dylib` on iOS. Note, no "lib" prefix and no extension.
- `libMyLibrary.so`, embedded inside any .aar file, on Android. Be aware that CMake target library name does not include the "lib" prefix or ".so" extension but that the resulting binary has both.

## Output to Published Apps ##

On Mac and iOS, they will be placed in the `Contents/Frameworks` folder in the `.app` folder.

On Windows they will be placed in the root of the zip package alongside the main EXE.

On Android the Android Library (aar) will be built into your APK or AAB.

## Loading the library
From Javascript, call `registerExternalFunction()` with the library name. Umajin will work out what to load.

```javascript
libraryId = registerExternalFunction( 'myLibrary', 'onLibData', 'onLibError');
```

The library will be loaded from an appropriate filename and place depending on the type of application and operating system.

- It will return the same name as you pass in, which you should save in a global variable for use later.
- If the library could not be loaded, it will return blank; the `on_error` event will also be called.

Note that one of each type of function (identifier, poll, response) must be present with correct names for loading to succeed.

If you want to load a filename that doesn’t have the standard extension or prefix, you can do so by specifying the extension. You can also supply a path, in which case Umajin will use that directly.
 
## Using the library ##
Once successfully loaded, you can send data to the library using:

```javascript
var result = externalFunctionProcess(libraryId , data);
```
This function can return data immediately.

If the library needs to send data back to the JS at another time (e.g. an asynchronous response to an earlier request), then it can return data when the poll() function is called. This will result in your onLibData function being called (that was registered with registerExternalFunction).

```javascript
function onLibData(data) {
}
```
