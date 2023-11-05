set -e

echo "Running... build at `date`"

if [ "$1" = "" ]; then
  echo "No target selected"
  exit 0
fi

cmake --build . --target $1 -- -j 4
./$1
exit 0