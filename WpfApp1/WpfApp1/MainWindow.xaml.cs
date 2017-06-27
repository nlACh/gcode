using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.IO.Ports;
using System.IO;

namespace WpfApp1
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        //variables for universal access
        Boolean nxtLine = false;
        private Int32 _baudRate, lines = 0, count = 0;
        private String _pName, fileName;
        SerialPort SP;

        //Make a list from array
        List<String> gList = new List<String>();
        //this renders the main window
        public MainWindow()
        {
            InitializeComponent();
            //adds the available COM ports to the list
            foreach (String s in SerialPort.GetPortNames())
                portList.Items.Add(s);            
        }

        //Window closing handler...
        private void MainWindow_Closing(object sender, EventArgs e)
        {
            MessageBox.Show("Are You Sure?");
            //SP.Close();
        }

        private void refresh_Click(object sender, RoutedEventArgs e)
        {
            /* *
             * just refreshes the COM port list
             * when a device is plugged in after the
             * program is started
             * */
            foreach (String s in SerialPort.GetPortNames())
                portList.Items.Add(s);
        }

        //button to send data
        private void send_Click(object sender, RoutedEventArgs e)
        {
            
                //@return.Text = count.ToString();
                gcode.Text = gList[lines];
                SP.WriteLine(gcode.Text);
                SP.DataReceived += sp_recieve;
                if (nxtLine)
                    lines++;
        }

        //creating a delegate
        public delegate void OutputResultDelegate(String myString);

        void sp_recieve(Object sender, SerialDataReceivedEventArgs e)
        {
            String data;
            SerialPort _SP = (SerialPort)sender;
            if (SP.IsOpen)
            {
                data = _SP.ReadTo("\n");
                @return.Dispatcher.Invoke(new OutputResultDelegate(setTxt), new Object[] { data }); 
            }
        }

        private void setTxt(String s)
        {
            @return.AppendText(Environment.NewLine + s);
            resTBox.AppendText(s);
            if (s.Contains(">"))
                nxtLine = true;
            else
                nxtLine = false;
        }

        /* *
         * This is a Click handler for Open Button 
         * Creates a file picker dialog for picking
         * the correct files
         * */
        private void fOpen_Click(object sender, RoutedEventArgs e)
        {
            Microsoft.Win32.OpenFileDialog ofd = new Microsoft.Win32.OpenFileDialog();
            ofd.DefaultExt = ".gcode";
            Nullable<bool> result = ofd.ShowDialog();

            if (result == true)
            {
                fileName = ofd.FileName;
                try
                {
                    if (System.IO.File.Exists(fileName) == true)
                    {
                        var Lines = File.ReadAllLines(fileName);
                        foreach(var line in Lines)
                        {
                            gList.Add(line);
                        }
                        //getting a count of the total number of lines
                        count = gList.Count;
                        @return.Text = count.ToString();
                    }
                }
                catch (Exception ex)
                {
                    resTBox.AppendText("ERROR!! " + ex.ToString());
                }
            }
        }

        private void open_Click(object sender, RoutedEventArgs e)
        {
            String bval = baudList.SelectedItem.ToString();
            _baudRate = Int32.Parse(bval);
            _pName = portList.SelectedItem.ToString();
            tB1.Text = bval;
            tB2.Text = _pName;
            SP = new SerialPort(_pName, _baudRate);
            if(!SP.IsOpen)
                SP.Open();
        }

    }
}
