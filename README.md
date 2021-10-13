

## Software renderer

<img src="./screens/render.png" style="zoom: 25%;" />

In this project I attempt to explicitely implement what a graphics API hides from the user. The renderer rasterizes the model and via a programmable pipeline lets the user implements more advanced features like SSAO or shadow mapping. Everything runs on the CPU.

The project is intended for educational purposes (and fun).



### Some pictures



Z-buffer and screen space ambient occlusion:

|                           Z-buffer                           |                             SSAO                             |
| :----------------------------------------------------------: | :----------------------------------------------------------: |
| <img src="/home/edoardo/Documents/SoftwareRenderer/screens/zBufferRender.png" style="zoom:33%;" /> | <img src="/home/edoardo/Documents/SoftwareRenderer/screens/coolAO.png" style="zoom: 33%;" /> |



Toon shader:

|                                                              |                                                              |
| :----------------------------------------------------------: | :----------------------------------------------------------: |
| <img src="/home/edoardo/Documents/SoftwareRenderer/screens/gunToon.png" style="zoom: 40%;" /> | <img src="/home/edoardo/Documents/SoftwareRenderer/screens/diabloToon.png" style="zoom:50%;" /> |



Shadows (in green); notice the ugly z-fighting:

|                                                              |                                                              |
| :----------------------------------------------------------: | :----------------------------------------------------------: |
| <img src="/home/edoardo/Documents/SoftwareRenderer/screens/render.png" style="zoom: 20%;" /> | <img src="/home/edoardo/Documents/SoftwareRenderer/screens/shadowsWithSSAO.png" style="zoom:40%;" /> |



Whit and without normal map (and simple shading):

|                             With                             |                           Without                            |
| :----------------------------------------------------------: | :----------------------------------------------------------: |
| <img src="/home/edoardo/Documents/SoftwareRenderer/screens/withoutNmap.png" style="zoom: 33%;" /> | <img src="/home/edoardo/Documents/SoftwareRenderer/screens/withNmap.png" style="zoom: 33%;" /> |



Full models:

|                                                              |                                                              |
| :----------------------------------------------------------: | :----------------------------------------------------------: |
| <img src="/home/edoardo/Documents/SoftwareRenderer/screens/droneFull.png" style="zoom: 27%;" /> | <img src="/home/edoardo/Documents/SoftwareRenderer/screens/diabloColorNM.png" style="zoom:40%;" /> |







