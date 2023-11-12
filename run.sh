set -e

echo "Running... build at `date`"

if [ "$1" = "" ]; then
  echo "No target selected"
  exit 0
fi

if [ "$3" = "release" ]; then
  echo "Release selected"
  cmake -DCMAKE_BUILD_TYPE=Release -S . -B build
  cmake --build . --target $1 -- -j 4
  exit 0
else
  echo "Debug selected"
  cmake -DCMAKE_BUILD_TYPE=Debug -S . -B build
fi

cmake --build . --target $1 -- -j 4
./$1 $2
exit 0