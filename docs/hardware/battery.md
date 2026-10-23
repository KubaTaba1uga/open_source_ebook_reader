---
orphan: true
---
# Battery

The EPD draws substantial current when refreshing full screen and all pixels changes. Using lab PSU we discovered that min 3A at 5V are required to repeatably 
display such pictures. To solve power issue we propose using 18650 power cell rated at 10A such as `https://www.tme.eu/pl/details/xtar-18650-3.5ah10/akumulatory/xtar/18650-350pcm/`,
to hold battery we use this holder `https://www.tme.eu/pl/details/bhc-18650-1p/baterie-pojemniki-i-uchwyty/comf/`. Charging the battery is done externally after ejecting 
battery from the device. To supply high amps from the battery at stable 5V we use Pololulu 5V Step-Up Voltage Regulator U3V70F5 `https://www.pololu.com/product/2891`.

To connect the Pololulu booster with the 

## ADC

To charge a battery we use Adafruit Powerboost 1000. Module exposes battery level via Bat and GND pins, because i use Poly-Lipo battery it's max voltage is 4.1V which exceeds maximum reference voltage for ADC (3.3V). To solve this we introduce simple voltage divider which splits 0-4.1V into 0-1.8V range:
```txt
+
-------
      |
     ---
	 | | R1 = 47K Ohm
     ---
      |--------- INP16
     ---
	 | | R2 = 33K Ohm
     ---
      |--------- NNP16
-------
-
```

| Powerboost Pin | ADC  | Channel | GPIO | PIN | Note                   |
| -------------- | ---- | ------- | ---- | --- | ---------------------- |
| Bat            | ADC1 | INP16   | PA7  | 31  | Battery +              |
| GND            | ADC1 | NNP16   | PA6  | 33  | Battery -              |

## Battery Testing

```
read_adc /sys/bus/iio/devices/iio:device0/in_voltage16-voltage16_raw \
          /sys/bus/iio/devices/iio:device0/in_voltage-voltage_scale \
		  /sys/bus/iio/devices/iio:device0/in_voltage-voltage_offset
```
