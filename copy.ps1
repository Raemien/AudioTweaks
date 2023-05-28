& ./build.ps1

& adb push build/libaudiotweaks.so /sdcard/Android/data/com.beatgames.beatsaber/files/mods/libaudiotweaks.so
& adb shell am force-stop com.beatgames.beatsaber
& adb shell am start com.beatgames.beatsaber/com.unity3d.player.UnityPlayerActivity
