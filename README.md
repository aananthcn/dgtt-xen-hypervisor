# "The Definitive Guide to the Xen Hypervisor" examples refreshed.

Great book about Xen was published at the end of 2007, and aim Xen 3.1 and 32bits guest OS-es. The original source code for these examples can be downloaded [from this link]( http://www.informit.com/content/images/9780132349710/sourcecode/9780132349710examples.zip).
The target of this repo is to refresh original examples and make them able to run on new Xen. From version 3.1 a lot of things changed in Xen and IA as well, so some of the code examples can be different from originals.

  - support for upstream Xen hypervisor 4.8 (should be able to run on previous versions)
  - 32 bit and 64 guests OS-es
  - support for xl
  - everything was tested on Xen 4.8 built with debug support
  - 64-bit ports are based on [Mini-os code base](https://github.com/mirage/mini-os)


#### To Do list:
  - Test for charapter4
  - ARM support for 32 and 64 bits OS-es
  - Extend Scheduler from character 12 to don't crash system
  - Provide alternative isXen
  - Extras: Possible detection of Xen, Driver from XenStore
 

#### Extra content:
After reading the book I had a little an unsatisfied. Because most of the examples are small, there are not really functional and don't provide a good base for a real project I decided to extend some of them to learn more about Xen.
If you also feel similar you can find some of my stuff in extras folder


### References:

* [The Definitive Guide to the Xen Hypervisor] - Referred book, a little bit outdated but still great source of knowledge
* [Xen Master] - Github mirror for Xen hypervisor!
* [Mini-OS] - At the time of publication of the book was a part of Xen now is moved as external project


### Build Xen for Debugging

You need to reconfigure xen and install it on your machine:

```sh
$ cd $XEN_SRC
$ ./configure --enable-debug --enable-debugger --enable-verbose
$ make -jN
$ sudo make install
```

### Development

Want to contribute? Great! See TODO list.


License
----
Originally it is not my code, but it comes from the book.

**MIT** -> For everything that do not belongs to the book


**Free Software, Hell Yeah!**

[//]: # (These are reference links used in the body of this note and get stripped out when the markdown processor does its job. There is no need to format nicely because it shouldn't be seen. Thanks SO - http://stackoverflow.com/questions/4823468/store-comments-in-markdown-syntax)


   [The Definitive Guide to the Xen Hypervisor]: <https://www.amazon.com/Definitive-Hypervisor-Prentice-Software-Development/dp/0133582493>
   [Xen Master]: <https://github.com/xen-project/xen>
   [Mini-OS]: <https://github.com/mirage/mini-os>


<br>

### Building Examples on Linux PC *(without errors)*

A simple make command may not work (i.e., throw errors). To overcome do the following
1. Clone https://github.com/xen-project/xen.git in the same folder where you cloned this *(xen-hypervisor-examples)* repo
2. Create a symbolic link name 'xen-master' and point that to 'xen' *(i.e., the repo you just cloned in step 1)*
3. Rename "xen/xen/include/public/features.h" to "xen/xen/include/public/features.renamed.h" to avoid getting make errors.

<br>

### To see messages printed by HYPERVISOR_console_io() functions

Most of the console io messages won't be visible in the console, when do your initial chapters. If you need to view them, then do the following
1. git clone https://github.com/xen-project/xen.git
2. Install the required packages that are listed in https://wiki.xenproject.org/wiki/Compiling_Xen_From_Source#Build_Dependencies
3. sudo apt install python3-dev libudev-dev libsystemd-dev
4. ./configure --enable-systemd --enable-debug --libdir=/usr/local/lib64
5. make dist -j8 debug=y
6. make install debug=y
7. update-grub


The above step will install xen images in /boot, but libraries in /usr/local/lib. This will break some of the tools such as xl. Follow the steps below:
1. sudo vi /etc/sudoers
2. Add "Defaults        env_keep += "LD_LIBRARY_PATH"" to the above file
3. Add "alias sudo='sudo LD_LIBRARY_PATH=/usr/local/lib64:$LD_LIBRARY_PATH'" in the end of ~/.bashrc

Once these are done, then re-boot and do "sudo xl create domain_config" then do "sudo xl dmesg", you should be able to see the messages that you pass through HYPERVISOR_console_io() function.

**Note(s)**:
If you are still not seeing the console io prints, then try these:
* guest_loglvl=all in the command line
* xl set-parameters 'guest_loglvl=all'
* You may need to start the following daemons as below
  * sudo LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH /etc/init.d/xencommons start
   
