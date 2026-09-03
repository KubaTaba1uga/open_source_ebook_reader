---
orphan: true
---
# Battery

To charge a battery we use Adafruit Powerboost 1000. Module exposes battery level via Bat and GND pins, because i use Poly-Lipo battery it's max voltage is 4.2V which exceeds maximum reference voltage for ADC (3.3V). To solve this we introduce simple voltage divider which splits 0-4.2V into 0-1.8V range:
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

