base_dir="$(pwd)"

mkdir -p build
cd build

echo $base_dir
cmake $base_dir
cmake --build .
