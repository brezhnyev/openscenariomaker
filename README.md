Briefly steps to compile Carla 0.9.9 and the Examples/CppClient
1. Download rpclib. git clone https://github.com/rpclib/rpclib.git, sudo checkinstall
2. Download recastnavigation. git clone https://github.com/carla-simulator/recastnavigation.git, sudo checkinstall
**Important!** the recastnavigation exists as forks in different repos in internet. **Important to use the one for carla + "recast_builder" branch.**
 Build with -fPIC flag (this may be needed if the carla library is built as shared in p.5).
3. Download boost_1_73_0 as zip package. Start ./bootstrap.sh and then ./b2 to build the libs. Do NOT install, since this may cause conflict with existing boost (1.65 is standard for Ubuntu18)
4. Download sdl: sudo apt install libsdl2-2.0-0
5. Clone the carla: git clone https://github.com/carla-simulator/carla.git, checkout version 0.9.9  
5.1. mkdir Build  
5.2. cd Build  
5.3. touch CMakeLists.txt.in (needed by the CMakeLists.txt)  
5.4. cmake .. -DCMAKE_BUILD_TYPE=Client -DCMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES=/home/kbrezhnyev/BUILDS/boost_1_73_0  
where the DCMAKE_BUILD_TYPE can be Server (not relevant in our case) and
DCMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES the path to the boost_1_73_0
6. To build the Client application:  
6.1. cd Examples/CppClient  
6.2. g++ main.cpp -o clientExample -I ~/BUILDS/carla/LibCarla/source/ -I ~/BUILDS/boost_1_73_0/ -L/home/kbrezhnyev/BUILDS/carla/Build/LibCarla/cmake/client/ -lcarla_client_debug  -L/usr/lib/x86_64-linux-gnu/ -lpng -ljpeg -ltiff -lrt -lpthread -L/usr/local/lib/ -lrpc -lDetour -lDetourCrowd -L/~tools/boost_1_73_0/libs/ -lboost_filesystem -Wl,--unresolved-symbols=ignore-all -g -O3  
7. Start the ./CarlaUE4.sh and then ./clientExample