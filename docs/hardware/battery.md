---
orphan: true
---
# Battery

To charge a battery we use Adafruit Powerboost 1000. Module exposes battery level via Bat and GND pins, because i use Poly-Lipo battery it's max voltage is 4.2V which exceeds maximum reference voltage for ADC (3.3V). To solve this we introduce simple voltage divider which splits 0-4.2V into 0-1.8V range. First create voltage divider:
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

To avoid battery leakage frew voltage divider we introduce on off tranistor between R2 and GND:
```txt
+
-------
      |
     ---
	 | | R1 = 47K Ohm
     ---
      |----------- INP16
     ---
	 | | R2 = 33K Ohm
     ---
      |----------- NNP16
	  |
	  |      R3 = 10K Ohm
	  \     ---
	   |----| |--- EN16
	  /     ---
	  V
	  |
-------
-
```

I used IRFZ44N transitor because i had a lot of them, other transistor model will be just as good just remeber to adjust the resistors appropriatly.

| Powerboost Pin | ADC  | Channel | GPIO | PIN | Note                   |
| -------------- | ---- | ------- | ---- | --- | ---------------------- |
| Bat            | ADC1 | INP16   | PA7  | 31  | Battery +              |
| GND            | ADC1 | NNP16   | PA6  | 33  | Battery -              |
|  X             | X    | X       | PE11 | 35  | Enable voltage divider |
