# Native Libraries
> April 30, 2021
>  
> Aaron Lawrence

Umajin has an interface that allows integrating native code into your project. This is supported for desktop platforms (Windows and Mac) and mobile platforms (iOS and Android). 

- **Windows** Dynamic Link Library (DLL) targeting 64 bit x86_64.
- **Mac** Dynamic Library (dylib) targeting architecture x86_64.
- **iOS** A 64 bit framework containing an iOS dylib targeting architecture arm64.
- **Android (future release)** An Android Library (AAR) containing arm 32 bit and arm 64 bit Shared Objects (.so) targeting armv7a and armv8a respectively.  

These all have a C-style API.

## Writing the native library
Your native library must implement the following C functions and export them to the DLL, Dylib or Shared Object.

```
const char* umajinGetIdentifier(void);
const char* umajinProcessV2(long long tag, long long timestamp, const char* payload);
const char* umajinProcessBinary((long long tag, long long timestamp, const char* payload, long long size, unsigned char* buffer);
const char* umajinProcess(long long tag, const char* payload); // Old interface, kept for backwards compatibility.
const char* umajinPoll(long long tag, long long timestamp);
const char* umajinPollBinary(long long tag, long long timestamp, long long* sizeOut, unsigned char** bufferOut);
const char* umajinDestroy(long long size, unsigned char* buffer);
```

**umajinGetIdentifier** just returns an internal name for the library.

**umajinProcessV2** is the interface for editor/JS to call into the library passing string data.

**umajinProcessBinary** (future release) is the interface for editor/JS to call into the library providing both string and binary data (a size and a byte buffer of that size). THe buffer is only valid during the call, so the library must copy data as necessary in its `umajinProcessBinary` implementation.

**umajinProcess** is an older interface for editor/JS to call into the library, maintained for backwards compatibility. New libraries should use `umajinProcessV2` or `umajinProcessBinary`.

**umajinPoll** is called every frame for the library to return string data back to the project JS, if it has any; ie asynchronous events or responses.

**umajinPollBinary** (future release) is called every frame for the library to return string and binary data back to the project JS. The library can allocate a buffer via bufferOut and set its size via sizeOut in the callers stack. **NOTE** the library must provide `umajinDestroy` so that the caller can hand the allocated buffer back to the library for safe release/freeing when the caller is done.

**umajinDestory** (future release) is called when Umajin has finished with a buffer allocated by the library and returned by umajinPollBinary. The library should free whatever resources were allocated at the address passed to `umajinDestroy`.

### Per platform considerations Umajin Editor 4.0.4 through 4.1.

- All integer types passed to the `umajin...` functions by long long should be 64 bit integers for the compiler and language (C/C++/ObjC/Swift).
- You need to provide for all your own dependencies by dynamic linking.
- Minimum versions of the target OS must not exceed those provided by the Umajin Editor. What those minima are will depend upon the version of the Umajin Editor.

#### Windows.

- Minimum: Windows 10.
- Architectures: x64.

#### MacOS.

- Minimum: MacOS 10.13. 
- Architectures: x86_64. arm64 tbd.

#### iOS.

- Minimum: iOS 13.
- Architectures: arm64.

#### Android (future release).

- Minimum: Android API level 22.
- Target: Android API level 30.
- Architectures: armv7a, armv8a.

## Installing the native library
Once you have created your DLL/Dylib/framework/AAR, place them in the Umajin Project folder as follows:

- Windows: `/manifest/win/bin`
- Mac: `/manifest/osx/bin`
- iOS: `/manifest/ios/frameworks`
- Android (future release): `/manifest/android/aar`

You may need to create these folders if they do not already exist.

Note that while you may be able to use other folders, these are the standard folders that will work correctly in all situations, including when the app is “published” as a standalone application. In such standalone apps the files will be moved into appropriate places within the package. 

On Mac and iOS, they will be placed in the `Contents/Frameworks` folder in the `.app` folder.

On Android (future release) the Android Library containing the Shared Objects will be added to your Android project as a dependency.

## Loading the library
From Javascript, call `registerExternalFunction()` with the library name. Umajin will work out what to load.

    libraryId = registerExternalFunction( 'myLibrary', 'onLibData', 'onLibError');

The library will be loaded from an appropriate filename and place depending on the type of application and operating system.

It will return the same name as you pass in, which you should save in a global variable for use later.

If the library could not be loaded, it will return blank; the on_error event will also be called.

Note that all the 3 functions must be present with correct names for loading to succeed.

The expected filename is:

- `myLibrary.dll` on Windows
- `libMyLibrary.dylib` on MacOS.  Note the expected `lib` prefix for MacOS.
- `myLibrary.framework/dylib` on iOS. Note, no "lib" prefix and no extension.
- `libMyLibrary.so` on Android (future release). Be aware that CMake target library name does not include the "lib" prefix or ".so" extension but that the resulting binary has both.

If you want to load a filename that doesn’t have the standard extension or prefix, you can do so by specifying the extension. You can also supply a path, in which case Umajin will use that directly.
 
## Using the library ##
Once successfully loaded, you can send data to the library using:

```
var result = externalFunctionProcess(libraryId , data);
```
This function can return data immediately.

If the library needs to send data back to the JS at another time (e.g. an asynchronous response to an earlier request), then it can return data when the poll() function is called. This will result in your onLibData function being called (that was registered with registerExternalFunction).

```
function onLibData(data) {
}
```
