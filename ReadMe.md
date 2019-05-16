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
```


b2 --toolset=msvc-14.2 link=static runtime-link=shared address-model=64 define=BOOST_USE_WINAPI_VERSION=0x0501 --with-chrono --with-date_time --with-filesystem --with-program_options --with-system --with-regex


cmake ../ -G "Visual Studio 16 2019" -A x64