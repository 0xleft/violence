set -e

# check if sudo
if [ "$EUID" -ne 0 ]; then
  echo "Please run as root"
  exit
fi

# check if wget installed
if ! [ -x "$(command -v wget)" ]; then
  echo "Error: wget is not installed. Please install."
  exit 1
fi

# check if gcc installed
if ! [ -x "$(command -v gcc)" ]; then
  echo "Error: gcc is not installed. Please install."
  exit 1
fi

# wget https://github.com/0xleft/violence/releases/download/v1.0.8/violence_linux
wget https://github.com/0xleft/violence/releases/download/v1.0.8/violence_linux -O /usr/bin/violence
chmod +x /usr/bin/violence
echo "Violence installed in /usr/bin/violence"