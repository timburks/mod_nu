sudo apxs -c -i -a -Wc,"-ObjC -arch x86_64" -Wl,"-arch x86_64 -framework Foundation -framework Nu" mod_nu.c
