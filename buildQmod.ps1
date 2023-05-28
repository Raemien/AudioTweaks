# Builds a .zip file for loading with BMBF
& ./build.ps1
Compress-Archive -Path  "./extern/libs/libbeatsaber-hook_3_13_0.so",`
                        "./build/libaudiotweaks.so",`
                        "./mod.json" -DestinationPath "./AudioTweaks.zip" -Update
& copy-item -Force "./AudioTweaks.zip" "./AudioTweaks.qmod"
