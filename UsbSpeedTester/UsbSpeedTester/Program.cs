using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace UsbSpeedTester
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length < 3)
            {
                Console.WriteLine("Bad use, usage: name, onname, offname");
            }
            
            Program program = new Program()
            {
                fileName = args[0],
                argToTest1 = args[1],
                argToTest2 = args[2],
            };
        }


       public string fileName = "set-led.exe";
       public string argToTest1 = "on";
       public string argToTest2 = "off";

        private void Measure()
        {
            var SetLed = new Process
            {
                StartInfo = new ProcessStartInfo()
                {
                    FileName = fileName,
                    Arguments = argToTest1,
                    UseShellExecute = false,
                    RedirectStandardOutput = true,
                    CreateNoWindow = true
                }
            };
            var StopLed = new Process
            {
                StartInfo = new ProcessStartInfo()
                {
                    FileName = fileName,
                    Arguments = argToTest2,
                    UseShellExecute = false,
                    RedirectStandardOutput = true,
                    CreateNoWindow = true
                }

            };

            Thread thread = new Thread(() =>
            {
                Stopwatch sw = new Stopwatch();

                sw.Start();
                for (int i = 0; i < 100; i++)
                {
                    SetLed.Start();
                    SetLed.WaitForExit();
                    StopLed.Start();
                    StopLed.WaitForExit();
                }
                sw.Stop();

                double seconds = sw.ElapsedMilliseconds / 1000.0;
                Console.WriteLine($"Resolution={200.0 / seconds}");
            });
            thread.Start();
        }

    }
}
