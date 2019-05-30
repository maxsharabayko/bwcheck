# Building

## Cloning

```
git clone <REPO_URL>
cd bwcheck
git submodule init
git submodule update

mkdir _build && cd _build
cmake ../
cmake --build ./
```

## Boost

Boost 1.70 is used by this app.

### Linux

```
wget https://dl.bintray.com/boostorg/release/1.70.0/source/boost_1_70_0.tar.bz2
tar -C ./ -jxf boost_1_70_0.tar.bz2
cd boost_1_70_0/
./bootstrap.sh --prefix=/usr
./b2 stage threading=multi link=static --with-date_time --with-filesystem --with-system --with-regex
sudo ./b2 install threading=multi link=static --with-date_time --with-filesystem --with-system --with-regex
sudo ln -svf detail/sha1.hpp /usr/include/boost/uuid/sha1.hpp
```

### Windows

```
b2 --toolset=msvc-14.2 link=static runtime-link=shared address-model=64 define=BOOST_USE_WINAPI_VERSION=0x0501 --with-chrono --with-date_time --with-filesystem --with-program_options --with-system --with-regex

cmake ../ -G "Visual Studio 16 2019" -A x64
```

# Running

## Client
```
./bwcheck client host port --num 1000000 --bitrate 1000000000
```

## Server
```
./bwcheck server port
```

# Long-term test

## Clent

Conenct to server, send 474924 UDP packets with payload of 1456 bytes. Repeat 36 times with 1200 seconds between consecutive runs.
```
./bwcheck client 192.168.0.110 4200 --num 474924 --testruns 36 --testruninterval 1200 --msgsize 1456
```

## Server
```
./bwcheck server port
```


