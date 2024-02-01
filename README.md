# MGEx
Open-source Roblox Executor

NOTE: STILL NOT TESTED BUT OPEN FOR MODIFICATION AND PULL REQUESTS

Heavy modification of [rscwn/MobileBloxV2](https://github.com/ironleon888/MobileBloxV2)

# Building
## Requires:
- CMake (required)
- NDK (required)

## Steps:
- Clone the repository
- Go to the repository folder
- Run:
  - `mkdir b && cd b`
  - `cmake .. -DANDROID_ABI="arch"`

- Notes:
  - `"arch"` can only be `arm64-v8a` or `armeabi-v7a`
