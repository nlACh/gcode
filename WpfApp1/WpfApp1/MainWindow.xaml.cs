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
        private Int32 _baudRate;
        private String _pName, fileName;
        private Int32 lines = 0, count=0;
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
            //adding a closing window handler
            Closing += MainWindow_Closing;
            //any name will do, just create a new handler method with the same name
        }

        private void MainWindow_Closing(object sender, EventArgs e)
        {
            SP.Close();
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
            
            String bval = baudList.SelectedItem.ToString();
            _baudRate = Int32.Parse(bval);
            _pName = portList.SelectedItem.ToString();
            tB1.Text = bval;
            tB2.Text = _pName;
            SP = new SerialPort(_pName, _baudRate);
            SP.Open();

            @return.Text = count.ToString();
            for (lines = 0; lines < count; lines++)
            {
                gcode.Text = gList[lines];
                SP.WriteLine(gList[lines]);
                SP.DataReceived += sp_recieve;
            }
        }

        //creating a delegate
        public delegate void OutputResultDelegate(String myString);

        void sp_recieve(Object sender, SerialDataReceivedEventArgs e)
        {
            SerialPort _SP = (SerialPort)sender;
            String data = _SP.ReadLine();
            
            @return.Dispatcher.Invoke(new OutputResultDelegate(setTxt), new Object[] { data });
        }

        private void setTxt(String s)
        {
            @return.Text=Environment.NewLine + s;
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
                        FileStream fs = new FileStream(fileName, FileMode.Open, FileAccess.Read); //better way of handling files
                        StreamReader fReader = new StreamReader(fs);
                        while (fReader.ReadLine() != null)
                        {
                            gList.Add(fReader.ReadLine());
                        }
                        fReader.Close();
                        //getting a count of the total number of lines
                        count = gList.Count;
                    }
                }
                catch (Exception ex)
                {
                    resTBox.AppendText("ERROR!! " + ex.ToString());
                }
            }
        }
    }
}
