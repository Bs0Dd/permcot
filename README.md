# Permcot - UNIX Permissions Converter Tool

Permcot is a simple tool for converting UNIX permissions from `-rwxrwxrwx` format to octal and vice versa.  
`umask` mode is also supported - the program can convert the required value for the new file permission mask.

## Usage
```
permcot [-u] <number or -rwxrwxrwx> ...
  -u     number/result for umask
```

## Examples

### "rwx" format to octal
```
$ permcot 4543 22 1701 532 3666
-r-sr---wx
-----w--w-
-rwx-----t
-r-x-wx-w-
-rw-rwSrwT
```

### Octal format to "rwx"
```
$ permcot -rwxrw-r-T dr-sr--rwx crwsrwsrwt lrwx------ -r--r--r--
1764
4547
7777
0700
0444
```

### Mixed conversion
```
$ permcot 676 7777 -rwxrw-r-T 3444 dr-sr--rwx lrwx------ 1774
-rw-rwxrw-
-rwsrwsrwt
1764
-r--r-Sr-T
4547
0700
-rwxrwxr-T
```

### "umask" mode
```
$ permcot -u 0002 -r--r--r-- -rwx------ 0223 0565
-rw-rw-r--     drwxrwxr-x
0333
0077
-r--r--r--     dr-xr-xr--
--w-----w-     d-w---x-w-
```

## Usage in conjunction with other utilities

### "chmod" (change file permissions)
```
touch myfile
$ ls -l myfile
-rw-rw-r-- 1 bs0dd bs0dd 0 lip 15 16:28 myfile
$ chmod `permcot -rwxrwSr--` myfile
$ ls -l myfile
-rwxrwSr-- 1 bs0dd bs0dd 0 lip 15 16:28 myfile
```

### "ls" and "cut" (get file permissions in octal format)
```
$ ls -l myfile 
-rwxrwSr-- 1 bs0dd bs0dd 0 lip 15 16:28 myfile
$ permcot `ls -l myfile | cut -d' ' -f1`
2764
```

### "umask" (current permissions) 
```
$ umask
0022
$ permcot -u `umask`
-rw-r--r--     drwxr-xr-x
$ mkdir newdir && touch newfile
$ ls -ld newdir newfile
drwxr-xr-x 2 bs0dd bs0dd 4096 lip 15 16:45 newdir
-rw-r--r-- 1 bs0dd bs0dd    0 lip 15 16:45 newfile
```

### "umask" (set new permissions) 
```
$ umask `permcot -u -rwxr--r-x`
$ mkdir newdir && touch newfile
$ ls -ld newfile newdir
drwxr--r-x 2 bs0dd bs0dd 4096 lip 15 16:54 newdir
-rw-r--r-- 1 bs0dd bs0dd    0 lip 15 16:54 newfile
$ umask 
0032
```
