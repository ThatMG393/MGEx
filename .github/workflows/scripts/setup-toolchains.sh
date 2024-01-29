echo "Updating..."
sudo apt update


echo "Install essentials..."
sudo apt install bison flex libssl-dev bc build-essential libncurses5-dev g++-multilib clang -y

if [ "$1" -e "armeabi-v7a" ]; then
	echo "Installing 'armeabi-v7a' toolchain.."
	sudo apt install \
		gcc-4.7-arm-linux-gnueabihf \
		gcc-4.7-multilib-arm-linux-gnueabihf \
		binutils-arm-linux-gnueabihf \
		libgcc1-armhf-cross \
		libsfgcc1-armhf-cross \
		libstdc++6-armhf-cross \
		libstdc++6-4.7-dev-armhf-cross -y
else
	echo "Installing 'arm64-v8a' toolchain..."
	sudo apt install libc6-arm64-cross libc6-dev-arm64-cross binutils-arm64-linux-gnueabi -y
fi

echo "Done Setup!"
