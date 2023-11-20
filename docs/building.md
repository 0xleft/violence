# Building from source

### Build

#### Requirements:

```bash
sudo apt install -y build-essential cmake
```

```bash
git clone https://github.com/0xleft/violence.git
cd violence
# if you are not on linux you can use wsl or docker
cat build.sh
chmod +x build.sh
./build.sh <platform> # currently supported: linux
# move the binary to /usr/local/bin or to your PATH
# Enjoy :)
```

### Run

You can run the program in two modes: file and interactive.

#### Modes:
1. File

```bash
violence <file>
```

2. Interactive

```bash
violence
```