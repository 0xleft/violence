set -e

# fild all dirs in configs/ folder and use build.sh <dir>
for dir in configs/*/
do
  # get dir name
  dir=${dir%*/}
  dir=${dir##*/}

  # run build.sh <dir>
  ./build.sh $dir $1
done