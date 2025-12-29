angleFrom = -90
angleTo = 90
requiredPrecisionM = 1.0

radiusM = 6378137
lengthM = 2 * math.pi * radiusM
oneDegreeLengthM = lengthM * 1 / 360
angleRange = math.abs(angleFrom) + math.abs(angleTo)

-- 1 deg = oneDegreeLengthM
-- x deg = requiredPrecisionM

requiredPrecisionDeg = requiredPrecisionM / oneDegreeLengthM
encodedValueMax = angleRange * (1 / requiredPrecisionDeg)
requiredPrecisionBits = math.log(encodedValueMax, 2)

print("requiredPrecisionDeg", string.format("%.10f", requiredPrecisionDeg))
print("encodedValueMax", encodedValueMax)
print("requiredPrecisionBits", requiredPrecisionBits)
