using System;
using System.IO;
using System.Globalization;

namespace Final
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args[0] == "-I")
            {
                StreamWriter Archivo = new StreamWriter("Output.csv");
                Archivo.WriteLine("DateAndTime,Climate");
                Archivo.Close();

                Console.WriteLine("From Input:");
                DateTime DateAndTime = new DateTime();
                DateTime.Input();
                // FromInput();
                Console.WriteLine("\nSe ha generado la conversión a un archivo Output");
            }
            
            else if (args[0] == "-O")
            {
                if(File.Exists("Output.csv"))
                {
                    Console.WriteLine("From Output:");
                    FromOutput();
                }

                else
                {
                    Console.WriteLine("El archivo Output no existe");
                }
            }
        }

        public static void FromOutput()
        {
            var lines = File.ReadAllLines("Output.csv");
            try{
            for (var i = 1; i < lines.Length; i += 1)
            {
                var line = lines[i];
                string[] data = new string[1];
                data = line.Split(",");

                long datetime = long.Parse(data[0]);
                int climate = int.Parse(data[1]);
                
                // Descodificar binario tiempo
                // 11111100100 0111 01101 10011 011110 011001 1000001101 (+/-:0/1)00100 000000
                long tzmin = datetime & 63, tzhr = (datetime >> 6) & 31, tzsign = (datetime >> 11) & 1, mlls = (datetime >> 12) & 1023;
                string tzminLead = tzmin.ToString("D" + 2), tzhrLead = tzhr.ToString("D" + 2), mllsLead = mlls.ToString("D" + 3);

                long sec = (datetime >> 22) & 63, minute = (datetime >> 28) & 63, hour = (datetime >> 34) & 31;
                string secLead = sec.ToString("D" + 2), minuteLead = minute.ToString("D" + 2), hourLead = hour.ToString("D" + 2); 
                
                long day = (datetime >> 39) & 31, month = (datetime >> 44) & 15, year = (datetime >> 48) & 4095;
                string dayLead = day.ToString("D" + 2), monthLead = month.ToString("D" + 2); 

                // Descodificar binario clima
                // 1100101000100011110
                int precipitation = climate & 127, maxTemp = (climate >> 7) & 127, minTemp = (climate >> 14) & 127;
                
                // Signo del timezone
                string tzsignLead = "";
                if (tzsign == 0)
                {
                    tzsignLead = "+";
                }

                else if (tzsign == 1)
                {
                    tzsignLead = "-";
                }

                Console.WriteLine("{0}-{1}-{2}T{3}:{4}:{5}.{6}{12}{7}:{8},{9},{10},{11}", year, monthLead, dayLead, hourLead, minuteLead, secLead, mllsLead, tzhrLead, tzminLead, minTemp, maxTemp, precipitation, tzsignLead);
            }
            }

            catch{}
        }
    }
}
