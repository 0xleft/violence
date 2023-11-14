set -e

PROJECT_NAME="violence"
TARGET_NAME="$PROJECT_NAME"_"$1"

echo "Running... build at `date`"

if [ "$1" = "" ]; then
  echo "No target selected"
  exit 0
fi

# check dir if exists in configs/ folder with name $1
if [ -d "configs/$1" ]; then
  echo "Config $1 selected"
  cd configs/$1
  cmake .
  if [ "$2" = "release" ]; then
    echo "Release selected"
    cmake -DCMAKE_BUILD_TYPE=Release -S . -B build
  else
    echo "Debug selected"
    cmake -DCMAKE_BUILD_TYPE=Debug -S . -B build
  fi
  cmake --build . --target "$TARGET_NAME" -- -j 4
else
  echo "Config $1 not found"
  exit 0
fi