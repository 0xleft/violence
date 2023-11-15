# check if sudo
if [ "$EUID" -ne 0 ]; then
  echo "Please run as root"
  exit
fi

# check if wget installed
if ! [ -x "$(command -v wget)" ]; then
  # ask to install wget
  echo "wget is not installed. Install? (y/n)"
  read install_wget
  if [ "$install_wget" = "y" ]; then
    apt-get install wget
  else
    echo "wget is required"
    exit 0
  fi
fi

# wget https://github.com/0xleft/violence/releases/download/v1.0.6/violence_linux
wget https://github.com/0xleft/violence/releases/download/v1.0.6/violence_linux -O /usr/bin/violence
chmod +x /usr/bin/violence
echo "Violence installed in /usr/bin/violence"