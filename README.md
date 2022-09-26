# simple_udp_pub_sub
simple udp publisher and subscriber for transferring best effort data using the ASIO library and google protobuf for serialization/deserialization.


## Windows
install protobuf through vcpkg
make sure to set the environment variable `CMAKE_TOOLCHAIN_FILE=[path to vcpkg]/scripts/buildsystems/vcpkg.cmake`

## Linux
install protobuf from source [link](https://github.com/protocolbuffers/protobuf/blob/main/cmake/README.md#linux-builds)
