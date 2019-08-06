# CVGUI

Puedes descargar una AppImage desde [aquí](https://urjc-my.sharepoint.com/:u:/g/personal/jose_velez_urjc_es/EXZbIpI2fwNEh1EIwumKMC4BBCVZoUd_QorfQHdYa9vtRQ?e=XPnStX)

Para compilarlo debes ejecutar en un Ubuntu 18.04:

```
cd
apt update
apt-get install software-properties-common -y
apt install qt5-default -y
apt install libopencv-dev -y
apt install git

git clone https://github.com/jfvelezserrano/CVGUI.git
mkdir build
cd build
qmake ../CVGUI/sources/CVGUI.pro
make
```

Para construir la AppImage debes ejecutar el siguiente comando:
```
./linuxdeployqt-6-x86_64.AppImage build-CVGUI-Desktop_Qt_5_12_1_GCC_64bit-Debug/Viewer/Viewer -unsupported-allow-new-glibc -appimage -bundle-non-qt-libs
```
Debes añadir esta categoría al fichero default.desktop:
```
Categories=Utility;
```
