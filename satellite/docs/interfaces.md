# Interfaces

## Command Input

Text file, one command per line. Lines beginning with `#` are comments.

## Telemetry Output

Plain text frames:

```text
T+0001 mode=NOMINAL  batt= 79% temp= 17C solar= 66% att=lock payload=off downlink=off packets=0 rejected=0
```

## Simulated Drivers

- ADC: battery voltage, board temperature, solar panel voltage
- Radio: link-ready state and sent frame count
- Sensor: radiation count, camera temperature, image-ready flag
