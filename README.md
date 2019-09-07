# CVGUI

CVGUI is a project that tries to create a Graphical User Interface for the library OpenCV.

OpenCV is a library for Computer Vision with hundreds of functions. CVGUI is designed to facilitate the use of OpenCV in preliminary stages of a project, where programings skills are not necesary yet.


You can download an AppImage from this [link](https://urjc-my.sharepoint.com/:u:/g/personal/jose_velez_urjc_es/EXZbIpI2fwNEh1EIwumKMC4BBCVZoUd_QorfQHdYa9vtRQ?e=XPnStX) that works in Ubuntu 18.04 and other similar Linux flavours.

To compile the project you need execute this script in Ubuntu 18.04:

```
cd
apt update
apt-get install software-properties-common -y
apt install qt5-default -y
DEBIAN_FRONTEND=noninteractive apt install libopencv-dev -y
apt install git -y

git clone https://github.com/jfvelezserrano/CVGUI.git
mkdir build
cd build
qmake ../CVGUI/sources/CVGUI.pro
make
```

To build the AppImage you have to execute the following line:
```
./linuxdeployqt-6-x86_64.AppImage build-CVGUI-Desktop_Qt_5_12_1_GCC_64bit-Debug/Viewer/Viewer -unsupported-allow-new-glibc -appimage -bundle-non-qt-libs
```
Finally, you hace to add this line to the default.desktop file:
```
Categories=Utility;
```
