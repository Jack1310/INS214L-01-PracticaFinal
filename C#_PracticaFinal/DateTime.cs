using System;
using System.IO;
using System.Globalization;

namespace Final
{
    class DateTime
    {
        public static void Input()
        {
            var lines = File.ReadAllLines("Input.csv");
            try{
            for (var i = 1; i < lines.Length; i += 1)
            {
                var line = lines[i];
                string[] data = new string[3];
                data = line.Split(",");

                var time = data[0];
                string[] newtime = new string[8];

                var sign = "-";
                int tzsign = time.IndexOf(sign, 20);
                // Console.WriteLine(tzsign);

                int min = int.Parse(data[1]), max = int.Parse(data[2]), percent = int.Parse(data[3]);

                Console.WriteLine(TimeSpace(time, newtime, tzsign) + "," + Weather(min, max, percent));
                StreamWriter Archivo = new StreamWriter("Output.csv", true);
                Archivo.Write(TimeSpace(time, newtime, tzsign) + "," + Weather(min, max, percent) + "\n");
                Archivo.Close();
                // Console.WriteLine(newtime[0] + " " + data[1]);
            }
            }

            catch{}
        }

        public static long TimeSpace(string time, string[] newtime, int tzsign)
        {

            if (tzsign < 0) // Positivo
            {
                tzsign = 0;
                // Console.WriteLine(tzsign);
            }

            else // Negativo
            {
                tzsign = 1;
                // Console.WriteLine(tzsign);
            }

            //Borrar caracteres especiales
            time = time.Replace('-', ' ');
            time = time.Replace('T', ' ');
            time = time.Replace(':', ' ');
            time = time.Replace('.', ' ');
            time = time.Replace('+', ' ');
            // Console.WriteLine(time);

            //Crear arreglo de string
            newtime = time.Split(" ");
            // Console.WriteLine(newtime[0]);

            //Convertir a int[] para acceder a cada valor por separado
            int[] numtime = new int[newtime.Length];

            for(int i = 0 ; i < newtime.Length ; i++)
            {
                numtime[i] = int.Parse(newtime[i]);
                // string binary = Convert.ToString(numtime[i], 2);
                // Console.Write(binary);
            }

            // Binario en base 4
            // string binary = Convert.ToString(numtime[1], 2).PadLeft(4, '0');
            // int arrojaba otros resultados
            long binary = numtime[0] << 4;
            binary = (binary | numtime[1]) << 5;
            binary = (binary | numtime[2]) << 5;
            binary = (binary | numtime[3]) << 6;
            binary = (binary | numtime[4]) << 6;
            binary = (binary | numtime[5]) << 10;
            binary = (binary | numtime[6]) << 1;
            binary = (binary | tzsign) << 5; // Para guardar el signo del timezone
            // string binarynuevo = Convert.ToString(binary, 2);
            // Console.WriteLine(binarynuevo);
            binary = (binary | numtime[7]) << 6;
            binary = (binary | numtime[8]);
            // 284355039830501632
            // 11111100100011101101100110111100110011000001101 (1)     0010000000
            // 11111100100011101101100110111100110011000001101(+/-:0/1)00100000000
            // Console.Write(binary);
            return binary;
        }

        public static int Weather(int min, int max, int percent)
        {
            int newbinary = min << 7;
            newbinary = (newbinary | max) << 7;
            newbinary = newbinary | percent;
            return newbinary;
        }
    }
}