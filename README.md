# duson-iot api document  
	This is test Application for rbsdk library

## Build
Two ways are supported:
1. By using setting cross tool enviornment
  1. edit the make/arch.mk, enter the correct corsstool env
  2. execute make
  3. transfer the /build/zigbeemodemapp to board and run it

2. By using cross compiler
  1. $(CROSS)gcc main.c z3_imp.c libz3_v2.3.a -lpthread -lm -lreadline -lncurses -o ./zigbeemodemapp
  2. where $(CROSS) is the cross gcc
  3. copy the zigbeemodemapp to board and run it
  
```
description : 
1. Application uses command line arguments to perform following operations:-
   a) Discover and Add Device
   b) Remove Device
   c) Read Attribute 

```

