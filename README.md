# cyber_rt_standalone_r6.0.0
1, Based on Cyber-RT framework from apollo

2, CMakelists.txt is from the https://blog.csdn.net/qq_35066464

3, Add glog gflag uuid into this repositry

4, Most of the Cyber-RT standalone project generates static library,but this CMakelists.txt make different, it generates libcyber.so

Apollo cyber is a small but strong framework aims at IPC & RPC.

1, CO-Routin in userspace

2, Thread allocate to arrange the priority

3, Qos in FAST-RTPS

4, Still need to study the detail inside the Cyber-RT

...

Hope this could be a middle-ware to do better than AutoSar standard

Build and Install 

1, Prepare install fast-rtps , gflag , glog , uuid , protobuf ...

   (1) Config the glog in shared library mode

   (2) uuid and poco can be installed by apt-get by 

         apt-get install uuid-dev

         apt-get install libpoco-dev

2, If not Ubuntu 1804,some of the *.cc or *.h need to modify (C++11/C++14)

mkdir build

cd build

cmake ..

make -j32
