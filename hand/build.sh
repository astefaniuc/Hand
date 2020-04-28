base_dir="$(pwd)"

build_type="Release"
# Parse shell script argument case insensitive.
if [ "${1,,}" = "debug" ]; then
    build_type="Debug"
fi

mkdir -p build
cd build

cmake -DCMAKE_BUILD_TYPE=$build_type $base_dir
cmake --build .
