cmake --build build
adb push build/lib*.so /storage/emulated/0
adb shell am start -n git.artdeell.skymodloader/.elfmod.ModManagerActivity
sleep 1
adb logcat --pid=$(adb shell pidof -s git.artdeell.skymodloader) -v color | grep RobinHood