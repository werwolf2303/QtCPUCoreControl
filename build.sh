mkdir -p build
cd build
cmake -DCMAKE_INSTALL_PREFIX=build -DCMAKE_PREFIX_PATH=build -DCMAKE_BUILD_TYPE=Release -GNinja ..
ninja

cd ..

./buildHelper.sh

mkdir -p out
cp build/QtCPUCoreControl out/
cp cpu_toggle_helper out/