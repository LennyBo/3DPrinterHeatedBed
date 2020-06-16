# 3DPrinterHeatedBed
This programm uses an arduino, a DC 6-24V motor controller and a PC power supply to power a 12V heated bed of a 3D printer (XYZ tha vinici mini w).

I use the 5V output of the power supply to power the arduino and the 12V to power the Heated bed. According to my understanding, you need about 150W on the 12V rail to make this happen. The heated bed was bought off of Aliexpress (https://de.aliexpress.com/item/32857294742.html?spm=a2g0s.9042311.0.0.656c4c4dmduQad) and the DC moter controller is a generic PWM controler of an RC boat.

I added 2 switches for some control.

The end result is very nice, I am able to print with much less warping which was my end goal.

I ran into some problems, notably that the printing bed was elevated now since the plate is THICC.
