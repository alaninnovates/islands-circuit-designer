mkdir build_$1
cd build_$1

APP_NAME=islands-circuit-designer
EXE_NAME=islands_circuit_designer

mkdir win
mkdir $APP_NAME-win
cd win
cmake ../.. -DCMAKE_TOOLCHAIN_FILE=../../windows-cross.cmake -DCMAKE_BUILD_TYPE=Release
make
mv $EXE_NAME.exe ../$APP_NAME-win
mv sprites ../$APP_NAME-win
cd ..

mkdir mac
mkdir $APP_NAME-mac
cd mac
cmake ../.. -DCMAKE_BUILD_TYPE=Release
make
mv $EXE_NAME ../$APP_NAME-mac
mv sprites ../$APP_NAME-mac
cd ..

zip -r $APP_NAME-win.zip $APP_NAME-win
zip -r $APP_NAME-mac.zip $APP_NAME-mac