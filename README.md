# SerialCppRaytracer

Serial CPP Raytracer for CSE701

## Build

```bash
g++ -O2 main.cpp -I include
```

Alternatively using cmake:

```bash
mkdir build/
cd build/
cmake ..
make
```

## Run

`./a.out > out.ppm`

There are config files in config/ that can be passed into the compiled program.

`./a.out high.rt > out.ppm`
