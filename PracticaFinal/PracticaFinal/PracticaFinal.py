# Definir funcion Input
def Input():
    # Lectura de una linea a la vez 
    for line in Lines: 
        data = line.split(',')
    
        # Lectura solamente del tiempo
        time = data[0]

        # Signo del timezone
        sign = "-"
        try:
            tzsign = time.index(sign, 20)
            tzsign = 1      # Negativo
        except:
            tzsign = 0      # Positivo


        time = time.replace('-', ' ')
        time = time.replace('+', ' ')
        time = time.replace('T', ' ')
        time = time.replace(':', ' ')
        time = time.replace('.', ' ')
        newtime = time.split(' ')
    
        # Convertir a int y operaciones binarias
        numtime = [int(i) for i in newtime]
        binary = numtime[0] << 4
        binary = (binary | numtime[1]) << 5
        binary = (binary | numtime[2]) << 5
        binary = (binary | numtime[3]) << 6
        binary = (binary | numtime[4]) << 6
        binary = (binary | numtime[5]) << 10
        binary = (binary | numtime[6]) << 1
        binary = (binary | tzsign) << 5
        binary = (binary | numtime[7]) << 6
        binary = (binary | numtime[8])

        # Lectura del clima y operaciones binarias
        min = int(data[1])
        max = int(data[2])
        percent = int(data[3])
        newbinary = min << 7
        newbinary = (newbinary | max) << 7
        newbinary = (newbinary | percent)

        # Escribir al Output.csv
        file0 = open('Output.csv', 'a')
        file0.write("\n{},{}".format(binary, newbinary))
        file0.close()
        print("{},{}".format(binary, newbinary))

def Output():
    # Lectura de una linea a la vez 
    for line in Lines: 
        data = line.split(',')
    
        # Lectura binaria del tiempo
        datetime = int(data[0])
        tzmin = datetime & 63
        tzminLead = str(tzmin).zfill(2)
        tzhr = (datetime >> 6) & 31
        tzhrLead = str(tzhr).zfill(2)
        tzsign = (datetime >> 11) & 1
        mlls = (datetime >> 12) & 1023
        mllsLead = str(mlls).zfill(3)
        sec = (datetime >> 22) & 63
        secLead = str(sec).zfill(2)
        minute = (datetime >> 28) & 63
        minuteLead = str(minute).zfill(2)
        hour = (datetime >> 34) & 31
        hourLead = str(hour).zfill(2)
        day = (datetime >> 39) & 31
        dayLead = str(day).zfill(2)
        month = (datetime >> 44) & 15
        monthLead = str(month).zfill(2)
        year = (datetime >> 48) & 4095

        # Lectura binaria del clima
        climate = int(data[1])
        precipitation = climate & 127
        maxTemp = (climate >> 7) & 127
        minTemp = (climate >> 14) & 127

        # Signo del timezone
        if(tzsign == 0):
            tzsignLead = "+"
        elif(tzsign == 1):
            tzsignLead = "-"

        print("{}-{}-{}T{}:{}:{}.{}{}{}:{},{},{},{}".format(year, monthLead, dayLead, hourLead, minuteLead, secLead, mllsLead, tzsignLead, tzhrLead, tzminLead, minTemp, maxTemp, precipitation))


# Crear Output.csv
file0 = open('Output.csv', 'w')
file0.write('DateAndTime,Climate')
file0.close()

print("From Input:\n")
# Lectura general de Input.csv sin el header
file1 = open('Input.csv', 'r') 
Lines = file1.readlines()[1:]
Input()
file1.close()

print("\nFrom Output:\n")
# Lectura general de Output.csv sin el header
file0 = open('Output.csv', 'r') 
Lines = file0.readlines()[1:]
Output()
file0.close()