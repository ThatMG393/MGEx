# MGEx
Open-source Roblox Executor

NOTE: STILL NOT WORKING BUT OPEN FOR MODIFICATION AND PULL REQUESTS

# Building

## Requires:
- CMake
- Clang (or any c/cpp compiler)
- NDK (optional)

## Steps:
- Clone the repository
- Go to the repository folder
- Run:
  - `mkdir b && cd b`
  - `cmake .. -DANDROID_ABI="arch"`

- Notes:
  - `"arch"` can only be `arm64-v8a` or `armeabi-v7a`
