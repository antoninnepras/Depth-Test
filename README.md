# Depth Test
- test depth of c files

## Instalation
```
make
make install
```

## Uninstalation
```
make uninstall
```

remove PATH=$PATH:~/.bin from yours .bahsrc


## Usage
```
cdepth_test [options] [files]
```

to test all .c files in directory:
```
cdepth_test $(find ${dir} -name "*.c")
```

## Options
```
-h     | print help
-t [n] | treshold
```
