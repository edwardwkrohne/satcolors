from numpy import *

def heatIntensity(m, x):
    keys = sorted(m.keys())
    for i in range(len(keys)-1):
        if keys[i+1] > x:
            lb = keys[i]
            ub = keys[i+1]
            break
    else:
        lb=keys[-2]
        ub=keys[-1]
        

    return (x-lb)*(m[lb]-m[ub])/(lb-ub)+m[lb]


colorMap = {
    0: matrix([0,0,0]),
    1.5: matrix([64, 0, 96]),
    2: matrix([0,0,192]),
    3.0: matrix([0,255,255]),
    3.3: matrix([255,255,0]),
    4.2: matrix([255,0,0]),
    4.5: matrix([255,128,128]),
    5.5: matrix([255, 255, 255]),
    6: matrix([255,255,255])
}

def idx(color, i):
    return hex(int(color[0,i]))[2:].zfill(2)

for x in range(0,70):
    topKey = sorted(colorMap.keys())[-1]
    color = heatIntensity(colorMap, x/69.*(topKey))
    print x, "0x" + idx(color, 0) + idx(color, 1) + idx(color, 2)

    
