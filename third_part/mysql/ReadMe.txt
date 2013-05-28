install process:
 sudo apt-get install bison
 sudo apt-get install libncurses5-dev

 sudo cmake . -DCMAKE_INSTALL_PREFIX=/home/guoliqiang/git-coding/coding/third_part/mysql/

 sudo make

 sudo make install

 NOTE: do not known how to add the dir of bison and curses to mysql in configure process.
       so need to apt-get insall bison and curses to /usr/local/


