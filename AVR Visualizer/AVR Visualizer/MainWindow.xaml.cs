using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace AVR_Visualizer
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    /// 
    /// 
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }
        Process externalCProcess = new Process
        {
            StartInfo = new ProcessStartInfo()
            {
                FileName = "hidtool.exe",
                Arguments = "read",
                UseShellExecute = false,
                RedirectStandardOutput = true,
                CreateNoWindow = true,
                
            },
        };

        private string str = "";

        private async void Start_Click(object sender, RoutedEventArgs e)
        {
            int badResponse = 0;
            int allResponses = 0;
            int longestResponseTime = 0;
            int minTime = 1000;
            while (Switch.IsChecked)
            {
                allResponses++;
                Console.Clear();
                Console.WriteLine($"Bad Responses: {badResponse}, All: {allResponses}, Precentage: {(double)badResponse /(double) allResponses}, Longest time: {longestResponseTime}, ShortestTime: {minTime}");

                Stopwatch stopwatch = new Stopwatch();
                stopwatch.Start();
                externalCProcess.Start();
                await Task.Delay(20);
                if (!externalCProcess.WaitForExit(20))
                {
                    externalCProcess.Close();
                    badResponse++;
                    continue;

                }
                str = await externalCProcess.StandardOutput.ReadToEndAsync();
                

                var time = stopwatch.ElapsedMilliseconds;

                var separated = str.Replace(Environment.NewLine, " ").Split(' ');

                textBox1.Text = str;

                List<int> integers = new List<int>();

                foreach (string s in separated)
                {
                    if (s != "")
                    {
                        integers.Add(Convert.ToInt32(s, 16));
                    }
                }

                if (integers.Count == 0)
                {
                    badResponse++;
                    continue;
                }

                if (time > longestResponseTime)
                {
                    longestResponseTime = (int)time;
                }

                if (time < minTime)
                {
                    minTime = (int)time;
                }

                A0.Text = AvrHelper.ReturnString(integers, 0, 0);
                A1.Text = AvrHelper.ReturnString(integers, 1, 0);
                A2.Text = AvrHelper.ReturnString(integers, 2, 0);
                A3.Text = AvrHelper.ReturnString(integers, 3, 0);
                A4.Text = AvrHelper.ReturnString(integers, 4, 0);
                A5.Text = AvrHelper.ReturnString(integers, 5, 0);
                A6.Text = AvrHelper.ReturnString(integers, 6, 0);
                A7.Text = AvrHelper.ReturnString(integers, 7, 0);

                B0.Text = AvrHelper.ReturnString(integers, 0, 1);
                B1.Text = AvrHelper.ReturnString(integers, 1, 1);
                B2.Text = AvrHelper.ReturnString(integers, 2, 1);
                B3.Text = AvrHelper.ReturnString(integers, 3, 1);
                B4.Text = AvrHelper.ReturnString(integers, 4, 1);
                B5.Text = AvrHelper.ReturnString(integers, 5, 1);
                B6.Text = AvrHelper.ReturnString(integers, 6, 1);
                B7.Text = AvrHelper.ReturnString(integers, 7, 1);

                C0.Text = AvrHelper.ReturnString(integers, 0, 2);
                C1.Text = AvrHelper.ReturnString(integers, 1, 2);
                C2.Text = AvrHelper.ReturnString(integers, 2, 2);
                C3.Text = AvrHelper.ReturnString(integers, 3, 2);
                C4.Text = AvrHelper.ReturnString(integers, 4, 2);
                C5.Text = AvrHelper.ReturnString(integers, 5, 2);
                C6.Text = AvrHelper.ReturnString(integers, 6, 2);
                C7.Text = AvrHelper.ReturnString(integers, 7, 2);

                D0.Text = AvrHelper.ReturnString(integers, 0, 3);
                D1.Text = AvrHelper.ReturnString(integers, 1, 3);
                D2.Text = AvrHelper.ReturnString(integers, 2, 3);
                D3.Text = AvrHelper.ReturnString(integers, 3, 3);
                D4.Text = AvrHelper.ReturnString(integers, 4, 3);
                D5.Text = AvrHelper.ReturnString(integers, 5, 3);
                D6.Text = AvrHelper.ReturnString(integers, 6, 3);
                D7.Text = AvrHelper.ReturnString(integers, 7, 3);

                textBox.Text = time.ToString();
            }
        }

        private void SendButton_Click(object sender, RoutedEventArgs e)
        {
            Process setProcess = new Process
            {
                StartInfo = new ProcessStartInfo()
                {
                    FileName = "hidtool.exe",
                    Arguments = $"write {PortA.Text} {DdrA.Text} {PortB.Text} {DdrB.Text} {PortC.Text} {DdrC.Text} {PortD.Text} {DdrD.Text}",
                    UseShellExecute = false,
                    RedirectStandardOutput = true,
                    CreateNoWindow = false,
                },
            };
            setProcess.Start();
        }
    }
}
