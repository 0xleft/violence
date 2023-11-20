set -e

PROJECT_NAME="violence"
TARGET_NAME="$PROJECT_NAME"_"$1"

if [ "$1" = "" ]; then
  echo "No target selected"
  exit 0
fi

./build.sh $1
bin/$TARGET_NAME $2 $3